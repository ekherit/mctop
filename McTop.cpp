#define McTop_cxx
#include "McTop.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdexcept>
#include <iostream>

std::map<mctop_t,Long64_t> McTop::Count()
{
   if (fChain == 0) throw std::runtime_error("No chain");

   Long64_t nentries = fChain->GetEntriesFast();
   std::cout << "Total number of entries: " <<  nentries << std::endl;
   std::map<mctop_t, Long64_t> TopMap;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      mctop_t top;
      top.pdgid.resize(indexmc);
      top.mother.resize(indexmc);
      for(int i=0;i<indexmc; i++)
      {
        top.pdgid[i]=pdgid[i];
        top.mother[i]=motheridx[i];
      }
      TopMap[top]++;
   }
   std::cout << "Found " << TopMap.size() << " topologies" << std::endl;
   return TopMap;
}
