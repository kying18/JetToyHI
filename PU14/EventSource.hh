#ifndef __EVENTSOURCE_HH__
#define __EVENTSOURCE_HH__

#include <string>
#include <vector>
#include <istream>
#include <memory>
#include <map>
#include "fastjet/PseudoJet.hh"
#include "fastjet/SharedPtr.hh"
#include "PU14.hh"

class EventHepMC3;
class EventHepMC2;
class EventList;
class EventSource;

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
   int VCount;
   int PCount;
public:
   EventHepMC3()
   {
      EventCount = 0;
      PCount = 0;
      VCount = 0;
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
      PCount = 0;
      VCount = 0;
   }
   void CopyParticles(std::vector<fastjet::PseudoJet> & particles, double &event_weight)
   {
      event_weight = W;
      particles.clear();
      for(int i = 0; i < E[2]; i++)
      {
         bool Tag = false;

         int vertex = 999;
         if(P[8][i] < 3.1 && P[8][i] > 2.9)   // 3
            vertex = -1;
         if(P[8][i] < 7.1 && P[8][i] > 6.9)   // 7 - hybrid negative wake
            vertex = -1, Tag = true;
         if(P[8][i] < 6.1 && P[8][i] > 5.9)   // 6 - hybrid positive wake
            vertex = 0;
         if(P[8][i] < 1.1 && P[8][i] > 0.9)   // 1
            vertex = 0;
         if(P[8][i] < 4.1 && P[8][i] > 3.9)   // 4
            vertex = 0;
         if(vertex == 999)
            continue;

         fastjet::PseudoJet particle = fastjet::PseudoJet(P[3][i], P[4][i], P[5][i], P[6][i]);
         particle.set_user_info(new PU14(P[2][i], i, vertex));
         
         if(particle.perp() < 1e-5 && fabs(particle.pz()) > 2000)
            continue;
         particles.push_back(particle);

         if(Tag == true)
         {
            fastjet::PseudoJet particle2 = fastjet::PseudoJet(P[3][i] * 1e-10, P[4][i] * 1e-10, P[5][i] * 1e-10, P[6][i] * 1e-10);
            particle2.set_user_info(new PU14(P[2][i], i, 0));
            particles.push_back(particle2);
         }
      }
      EventCount = EventCount + 1;
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
   double V[9][10000];
   int VCount;
   int PCount;
   double P[13][10000];   // index 12 is mother vertex
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
      VCount = 0;
      for(int i = 0; i < 9; i++)
         for(int j = 0; j < 10000; j++)
            V[i][j] = 0;
      PCount = 0;
      for(int i = 0; i < 13; i++)
         for(int j = 0; j < 10000; j++)
            P[i][j] = 0;
   }
   void CopyParticles(std::vector<fastjet::PseudoJet> & particles, double &event_weight)
   {
      event_weight = EWeight;
      particles.clear();
      for(int i = 0; i < PCount; i++)
      {
         bool Tag = false;

         int vertex = 999;
         if(P[7][i] < 3.1 && P[7][i] > 2.9)   // 3
            vertex = -1;
         if(P[7][i] < 1.1 && P[7][i] > 0.9)   // 1
            vertex = 0;
         if(P[7][i] < 4.1 && P[7][i] > 3.9)   // 4
            vertex = 0;
         if(P[7][i] < 7.1 && P[7][i] > 6.9)   // 7 - hybrid negative wake
            vertex = -1, Tag = true;
         if(P[7][i] < 6.1 && P[7][i] > 5.9)   // 6 - hybrid positive wake
            vertex = 0;
         if(vertex == 999)
            continue;

         fastjet::PseudoJet particle = fastjet::PseudoJet(P[2][i], P[3][i], P[4][i], P[5][i]);
         particle.set_user_info(new PU14(P[1][i], i, vertex));

         if(particle.perp() < 1e-5 && fabs(particle.pz()) > 2000)
            continue;
         particles.push_back(particle);

         if(Tag == true)
         {
            fastjet::PseudoJet particle2 = fastjet::PseudoJet(P[2][i] * 1e-10, P[3][i] * 1e-10, P[4][i] * 1e-10, P[5][i] * 1e-10);
            particle2.set_user_info(new PU14(P[1][i], i, 0));
            particles.push_back(particle2);
         }
      }
      EventCount = EventCount + 1;
   }
};

class EventList
{
public:
   std::map<int, int> Index;
   std::vector<fastjet::PseudoJet> P;
   std::vector<std::vector<int>> Mother;
   std::vector<std::vector<int>> Daughter;
   std::vector<int> ID;
   std::vector<int> Status;
public:
   EventList() { Clear(); }
   ~EventList() { }
   void Clear()
   {
      Index.clear();
      P.clear();
      Mother.clear();
      Daughter.clear();
      ID.clear();
      Status.clear();
   }
   void Initialize(const EventHepMC2 &E)
   {
      Clear();
      
      std::map<int, std::vector<int>> VMother;
      for(int i = 0; i < E.PCount; i++)
         VMother[E.P[10][i]].push_back(E.P[0][i]);
      
      for(int i = 0; i < E.PCount; i++)
      {
         fastjet::PseudoJet particle = fastjet::PseudoJet(E.P[2][i], E.P[3][i], E.P[4][i], E.P[5][i]);

         P.push_back(particle);
         ID.push_back(E.P[1][i]);
         Status.push_back(E.P[7][i]);
         Daughter.push_back(std::vector<int>());
         if(E.P[10][i] != E.P[12][i])   // not an orphaned particle
            Mother.push_back(VMother[E.P[12][i]]);
         else
            Mother.push_back(std::vector<int>());

         Index[E.P[0][i]] = i;
      }
      Initialize();
   }
   void Initialize(const EventHepMC3 &E)
   {
      Clear();

      // WARNING!  NOT TESTED YET
      
      std::map<int, std::vector<int>> VMother;
      for(int i = 0; i < E.PCount; i++)
         if(E.P[1][i] < 0)   // vertex mother!
            VMother[E.P[1][i]].push_back(E.P[0][i]);
      
      for(int i = 0; i < E.PCount; i++)
      {
         fastjet::PseudoJet particle = fastjet::PseudoJet(E.P[3][i], E.P[4][i], E.P[5][i], E.P[6][i]);

         P.push_back(particle);
         ID.push_back(E.P[2][i]);
         Status.push_back(E.P[8][i]);
         Daughter.push_back(std::vector<int>());
         
         if(E.P[1][i] < 0)   // vertex mother
            Mother.push_back(VMother[E.P[1][i]]);
         else
         {
            std::vector<int> X;
            X.push_back(E.P[1][i]);
            Mother.push_back(X);
         }

         Index[E.P[0][i]] = i;
      }
      Initialize();
   }
   void Initialize()
   {
      for(int i = 0; i < (int)P.size(); i++)
      {
         if(Mother[i].size() == 0)
            continue;

         for(int j = 0; j < (int)Mother[i].size(); j++)
         {
            if(Index.find(Mother[i][j]) == Index.end())
               Mother[i][j] = -1;
            else
               Mother[i][j] = Index[Mother[i][j]];
         }
         
         for(int j = 0; j < (int)Mother[i].size(); j++)
         {
            if(Mother[i][j] == -1)
               continue;
            Daughter[Mother[i][j]].push_back(i);
         }
      }
   }
   std::vector<int> GetListByStatus(int status) const
   {
      std::vector<int> Result;
      for(int i = 0; i < (int)P.size(); i++)
         if(Status[i] == status)
            Result.push_back(i);
      return Result;
   }
   std::vector<int> TraceShower(int start) const
   {
      std::vector<int> Result;

      while(start < (int)P.size() && Daughter[start].size() > 0)
      {
         if(Daughter[start].size() == 1)
            start = Daughter[start][0];
         else
         {
            Result.push_back(start);

            int Best = 0;
            for(int i = 0; i < (int)Daughter[start].size(); i++)
               if(P[Daughter[start][Best]].perp() < P[Daughter[start][i]].perp())
                  Best = i;

            start = Daughter[start][Best];
         }
      }

      return Result;
   }
   std::vector<int> KeepParton(std::vector<int> &List) const
   {
      std::vector<int> Result;
      for(int i = 0; i < (int)List.size(); i++)
      {
         bool IsParton = false;
         if(ID[List[i]] <= 6 && ID[List[i]] >= -6)
            IsParton = true;
         if(ID[List[i]] == 21)
            IsParton = true;

         if(IsParton == true)
            Result.push_back(List[i]);
      }
      return Result;
   }
   std::vector<double> GetZGs(std::vector<int> &List) const
   {
      std::vector<double> Result;
      for(int i = 0; i < (int)List.size(); i++)
      {
         if(List[i] >= (int)P.size())
            Result.push_back(-1);
         else
         {
            if(Daughter[List[i]].size() <= 1)
               Result.push_back(-1);
            else
            {
               double PTSum = 0;
               double PTMin = -1;
               for(int j = 0; j < (int)Daughter[List[i]].size(); j++)
               {
                  double PT = P[Daughter[List[i]][j]].perp();
                  PTSum = PTSum + PT;
                  if(PTMin < 0 || PTMin > PT)
                     PTMin = PT;
               }

               Result.push_back(PTMin / PTSum);
            }
         }
      }
      return Result;
   }
   std::vector<double> GetDRs(std::vector<int> &List) const
   {
      std::vector<double> Result;
      for(int i = 0; i < (int)List.size(); i++)
      {
         if(List[i] >= (int)P.size())
            Result.push_back(-1);
         else
         {
            if(Daughter[List[i]].size() != 2)
               Result.push_back(-1);
            else
            {
               const fastjet::PseudoJet &P1 = P[Daughter[List[i]][0]];
               const fastjet::PseudoJet &P2 = P[Daughter[List[i]][1]];
               Result.push_back(P1.delta_R(P2));
            }
         }
      }
      return Result;
   }
   std::vector<fastjet::PseudoJet> GetSJ1s(std::vector<int> &List) const
   {
      std::vector<fastjet::PseudoJet> Result;
      for(int i = 0; i < (int)List.size(); i++)
      {
         if(List[i] >= (int)P.size())
            Result.push_back(fastjet::PseudoJet());
         else
         {
            if(Daughter[List[i]].size() != 2)
               Result.push_back(fastjet::PseudoJet());
            else
            {
               const fastjet::PseudoJet &P1 = P[Daughter[List[i]][0]];
               const fastjet::PseudoJet &P2 = P[Daughter[List[i]][1]];
               if(P1.perp() > P2.perp())
                  Result.push_back(P1);
               else
                  Result.push_back(P2);
            }
         }
      }
      return Result;
   }
   std::vector<fastjet::PseudoJet> GetSJ2s(std::vector<int> &List) const
   {
      std::vector<fastjet::PseudoJet> Result;
      for(int i = 0; i < (int)List.size(); i++)
      {
         if(List[i] >= (int)P.size())
            Result.push_back(fastjet::PseudoJet());
         else
         {
            if(Daughter[List[i]].size() != 2)
               Result.push_back(fastjet::PseudoJet());
            else
            {
               const fastjet::PseudoJet &P1 = P[Daughter[List[i]][0]];
               const fastjet::PseudoJet &P2 = P[Daughter[List[i]][1]];
               if(P1.perp() > P2.perp())
                  Result.push_back(P2);
               else
                  Result.push_back(P1);
            }
         }
      }
      return Result;
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

   bool Recycle;

   EventSource(const std::string & filename, const std::string &type)
   {
      open_stream(filename);

      if(type == "PU14")
         Type = FileType::Pu14;
      else if(type == "HepMC2")
         Type = FileType::HepMC2;
      else if(type == "HepMC3")
         Type = FileType::HepMC3;
      else
         Type = FileType::Unknown;

      Recycle = false;
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

public:
   EventList List;
};

#endif // __EVENTSOURCE_HH__
