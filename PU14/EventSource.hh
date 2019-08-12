#ifndef __EVENTSOURCE_HH__
#define __EVENTSOURCE_HH__

#include <string>
#include <vector>
#include <istream>
#include <memory>
#include "fastjet/PseudoJet.hh"
#include "fastjet/SharedPtr.hh"

class EventHepMC3
{
public:
   int E[3];
   double ELocation[4];
   double W;
   double *P[9];
   double *V[6];
   double AHeavyIon[14];
   double AEventScale;
   double AProcessID;
   double AProcessVertex;
   int EventCount;
public:
   EventHepMC3()
   {
      EventCount = 0;
      for(int i = 0; i < 9; i++)
         P[i] = new double[100000];
      for(int i = 0; i < 6; i++)
         V[i] = new double[100000];
   }
   ~EventHepMC3()
   {
      for(int i = 0; i < 9; i++)
         delete[] P[i];
      for(int i = 0; i < 6; i++)
         delete[] V[i];
   }
   void Clean()
   {
      for(int i = 0; i < 3; i++)
         E[i] = 0;
      for(int i = 0; i < 4; i++)
         ELocation[i] = 0;
      W = 0;
      for(int i = 0; i < 9; i++)
         for(int j = 0; j < 100000; j++)
            P[i][j] = 0;
      for(int i = 0; i < 6; i++)
         for(int j = 0; j < 100000; j++)
            V[i][j] = 0;
      for(int i = 0; i < 14; i++)
         AHeavyIon[i] = 0;
      AEventScale = 0;
      AProcessID = 0;
      AProcessVertex = 0;
   }
};

class EventHepMC2
{
public:
   double E[12];
   int ERand;
   double EWeight;
   std::string N[2];
   std::string U[2];
   double C[2];
   double H[13];
   double F[9];
   double V[9];
   int PCount;
   double P[12][10000];
   double EventCount;
public:
   EventHepMC2() {}
   ~EventHepMC2() {}
   void Clean()
   {
      for(int i = 0; i < 12; i++)   E[i] = 0;
      for(int i = 0; i < 2; i++)    N[i] = "";
      for(int i = 0; i < 2; i++)    U[i] = "";
      for(int i = 0; i < 2; i++)    C[i] = 0;
      for(int i = 0; i < 13; i++)   H[i] = 0;
      for(int i = 0; i < 9; i++)    F[i] = 0;
      for(int i = 0; i < 9; i++)    V[i] = 0;
      PCount = 0;
      for(int i = 0; i < 12; i++)
         for(int j = 0; j < 10000; j++)
            P[i][j] = 0;
   }
};

//----------------------------------------------------------------------
/// \class EventSource
///
/// Class for reading events from a file (or stdin) in some simple format
class EventSource
{
public:

   enum FileType {Pu14, HepMC2, HepMC3, Unknown};

   EventSource(const std::string & filename, const std::string &type) {
      open_stream(filename);

      if(type == "PU14")
         Type = FileType::Pu14;
      else if(type == "HepMC2")
         Type = FileType::HepMC2;
      else if(type == "HepMC3")
         Type = FileType::HepMC3;
      else
         Type = FileType::Unknown;
   }

   /// set up an event stream from the corresponding file (in the PU14 format) 
   void open_stream(const std::string & filename);

   /// appends the particles from the next event that is read onto the 
   /// particles vector.
   bool append_next_event(std::vector<fastjet::PseudoJet> & particles,
         double &event_weight, int vertex_number = 0);
   bool append_next_event_pu14(std::vector<fastjet::PseudoJet> & particles,
         double &event_weight, int vertex_number = 0);
   bool append_next_event_hepmc2(std::vector<fastjet::PseudoJet> & particles,
         double &event_weight, int vertex_number = 0);
   bool append_next_event_hepmc3(std::vector<fastjet::PseudoJet> & particles,
         double &event_weight, int vertex_number = 0);

private:
   std::istream * _stream;
   fastjet::SharedPtr<std::istream> _stream_auto;
   FileType Type;

   EventHepMC2 Event2;
   EventHepMC3 Event3;
};

#endif // __EVENTSOURCE_HH__
