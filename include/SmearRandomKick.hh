#include "fastjet/PseudoJet.hh"
//#include <TLorentzVector.h>
#include <iostream>
#include <TRandom2.h>

std::vector<fastjet::PseudoJet> SmearRandomKick(std::vector<fastjet::PseudoJet> &input, double gaussian_sigma=0)
{
   std::vector<fastjet::PseudoJet> output;
   TRandom *r2 = new TRandom2();
   
   for (int i=0; i < (int)input.size(); i++) {
      double kickMag=fabs(r2->Gaus(0,gaussian_sigma));
      double kx,ky,kz;
      r2->Sphere(kx,ky,kz,kickMag);
      
      double px,py,pz,E;
      px=input[i].px()+kx;
      py=input[i].py()+ky;
      pz=input[i].pz()+kz;
      E=sqrt(input[i].m()*input[i].m()+px*px+py*py+pz*pz);
      int pdgid = input[i].user_info<PU14>().pdg_id();
      int vtxnum = input[i].user_info<PU14>().vertex_number();
      
      fastjet::PseudoJet p4;
      p4.reset_momentum(px,py,pz,E);
      p4.set_user_info(new PU14(pdgid, output.size(), vtxnum));
      output.push_back(p4);
   }
   
   return output;
}
