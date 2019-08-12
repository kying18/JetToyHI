#include "EventSource.hh"
#include "FastIStringStream.hh"
#include "PU14.hh"
#include "zfstream.h"
#include <cassert>
#include <fstream>
#include <sstream>

using namespace std;
using namespace fastjet;


//----------------------------------------------------------------------
void EventSource::open_stream(const std::string & filename) {
   if (filename == "-") {
      _stream  = & cin;
   } else if (filename.length() > 3 && 
         filename.find(std::string(".gz")) +3 == filename.length()) {
      _stream = new gzifstream(filename.c_str());
      _stream_auto.reset(_stream);
   } else {
      _stream = new ifstream(filename.c_str());
      _stream_auto.reset(_stream);
   }
   if (! _stream->good()) {
      cerr << "ERROR: could not access file " << filename << endl;
      exit(-1);
   }
}


//----------------------------------------------------------------------
bool EventSource::append_next_event(std::vector<fastjet::PseudoJet> & particles,
      double &event_weight, int vertex_number)
{
   if(Type == FileType::Pu14)
      return append_next_event_pu14(particles, event_weight, vertex_number);
   else if(Type == FileType::HepMC2)
      return append_next_event_hepmc2(particles, event_weight, vertex_number);
   else if(Type == FileType::HepMC3)
      return append_next_event_hepmc3(particles, event_weight, vertex_number);

   std::cerr << "Error!  File type not found!" << std::endl;

   return false;
}


//----------------------------------------------------------------------
bool EventSource::append_next_event_pu14(std::vector<fastjet::PseudoJet> & particles,
      double &event_weight, int vertex_number)
{
   PseudoJet particle;
   string line;
   double px, py, pz, m, E;
   int pdgid, vertex;

   unsigned original_size = particles.size();
   event_weight = 1;

   // read in particles
   while (getline(*_stream, line)) {
      // ignore blank lines and comment lines
      if (line.length() == 0 || line[0] == '#') continue;

      // if the line says "end" then assume we've found the end of the
      // event (try to make the check as efficient as possible).
      if (line[0] == 'e' && line.substr(0,3) == "end") break;

      // if the line says "weight", we multiply current weight by the number that follows
      if(line[0] == 'w' && line.substr(0, 6) == "weight")
      {
         string dummy;
         double temp = 1;
         FastIStringStream readline(line.c_str());
         readline >> dummy >> temp;
         if(temp > 0)
            event_weight = event_weight * temp;
         continue;
      }

      // FastIStringStream is not a proper stream, but it's a lot faster
      // than standard istringstream.
      FastIStringStream readline(line.c_str());
      readline >> px >> py >> pz >> m >> pdgid >> vertex;
      assert(!readline.error());

      E = sqrt(px*px + py*py + pz*pz + m*m);
      particle = PseudoJet(px,py,pz,E);

      // now set the user info
      int barcode = particles.size();
      particle.set_user_info(new PU14(pdgid, barcode, vertex));

      // and add the particle to our final output
      particles.push_back(particle);
   }

   // if there were no new particles, then we assume the event has an error
   return (particles.size() != original_size);
}



//----------------------------------------------------------------------
bool EventSource::append_next_event_hepmc3(std::vector<fastjet::PseudoJet> & particles,
      double &event_weight, int vertex_number)
{
   EventHepMC3 &E = Event3;

   static bool BeforeFirstEvent = true;
   static string EventString = "";
   string line;

   int PCount = 0;
   int VCount = 0;

   while(getline(*_stream, line))
   {
      if(line.length() == 0 || line[0] == '#')
         continue;

      stringstream str(line);
      string Type;
      str >> Type;

      if(Type == "E")
      {
         if(BeforeFirstEvent == true)
         {
            BeforeFirstEvent = false;
            EventString = line;
            continue;
         }

         str.clear();
         str.str(EventString);

         string Dummy = "";
         str >> Dummy;   // should be "E"

         for(int i = 0; i < 3; i++)
            str >> E.E[i];
         if(E.E[1] >= 100000)   E.E[1] = 100000;
         if(E.E[2] >= 100000)   E.E[2] = 100000;

         str >> Dummy;

         if(Dummy == "@")
            for(int i = 0; i < 4; i++)
               str >> E.ELocation[i];
         
         E.CopyParticles(particles, event_weight);
         EventString = line;

         E.Clean();
         PCount = 0;
         VCount = 0;

         break;
      }
      if(Type == "W")
      {
         double Weight = -1;
         str >> Weight;
         if(Weight > 0)
            E.W = Weight;
         else
            E.W = 1;
      }
      if(Type == "A")
      {
         string AType;
         str >> AType >> AType;

         if(AType == "GenHeavyIon")             for(int i = 0; i < 14; i++) str >> E.AHeavyIon[i];
         if(AType == "signal_process_vertex")   str >> E.AProcessVertex;
         if(AType == "signal_process_id")       str >> E.AProcessID;
         if(AType == "event_scale")             str >> E.AEventScale;
      }
      if(Type == "V")
      {
         str >> E.V[0][VCount] >> E.V[1][VCount];

         // Do something here later if needed
         string ParticleIn = "";
         str >> ParticleIn;

         string Dummy = "";
         str >> Dummy;
         if(Dummy == "@")
            str >> E.V[2][VCount] >> E.V[3][VCount] >> E.V[4][VCount] >> E.V[5][VCount];

         VCount = VCount + 1;
      }
      if(Type == "P")
      {
         for(int i = 0; i < 9; i++)
            str >> E.P[i][PCount];
         PCount = PCount + 1;
      }
   }

   // TODO: Get last event

   return true;
}


//----------------------------------------------------------------------
bool EventSource::append_next_event_hepmc2(std::vector<fastjet::PseudoJet> & particles,
      double &event_weight, int vertex_number)
{
   EventHepMC2 &E = Event2;

   static bool BeforeFirstEvent = true;
   static string EventString = "";
   string line;

   while(getline(*_stream, line))
   {
      if(line.length() == 0 || line[0] == '#')
         continue;

      stringstream str(line);
      string Type;
      str >> Type;

      if(Type == "E")
      {
         if(BeforeFirstEvent == true)
         {
            BeforeFirstEvent = false;
            EventString = line;
            continue;
         }

         str.clear();
         str.str(EventString);

         string Dummy = "";
         str >> Dummy;   // should be "E"
         
         for(int i = 0; i < 10; i++)
            str >> E.E[i];

         str >> E.E[10];
         if(E.E[10] != 0)
            for(int i = 0; i < E.E[10]; i++)
               str >> E.ERand;
         str >> E.E[11];
         if(E.E[11] != 0)
         {
            E.EWeight = 1;
            double Weight = 1;
            for(int i = 0; i < E.E[11]; i++)
            {
               str >> Weight;
               E.EWeight = E.EWeight * Weight;
            }
         }
         else
            E.EWeight = 1;
         
         E.CopyParticles(particles, event_weight);
         EventString = line;

         E.Clean();

         break;
      }
      if(Type == "N")   for(int i = 0; i < 2; i++)    str >> E.N[i];
      if(Type == "U")   for(int i = 0; i < 2; i++)    str >> E.U[i];
      if(Type == "C")   for(int i = 0; i < 2; i++)    str >> E.C[i];
      if(Type == "H")   for(int i = 0; i < 13; i++)   str >> E.H[i];
      if(Type == "F")   for(int i = 0; i < 9; i++)    str >> E.F[i];
      if(Type == "V")   for(int i = 0; i < 9; i++)    str >> E.V[i];
      if(Type == "P")
      {
         for(int i = 0; i < 12; i++)
            str >> E.P[i][E.PCount];
         E.PCount = E.PCount + 1;
      }
   }

   // TODO: Get last event
   
   return true;
}

