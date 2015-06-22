//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 22 04:37:25 2015 by ROOT version 6.04/01
// from TTree mctopoKK/mcTopo KK events
// found on file: data12-Mrec90MeV.root
//////////////////////////////////////////////////////////

#ifndef McTopoBase_h
#define McTopoBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>

// Header file for the classes stored in the TTree if any.

class McTopoBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           indexmc;
   Int_t           pdgid[100];   //[indexmc]
   Int_t           motheridx[100];   //[indexmc]
   Int_t           idx[100];   //[indexmc]
   UInt_t          hash;

   // List of branches
   TBranch        *b_indexmc;   //!
   TBranch        *b_pdgid;   //!
   TBranch        *b_motheridx;   //!
   TBranch        *b_idx;   //!
   TBranch        *b_hash;   //!

   McTopoBase(TTree *tree=0);
   virtual ~McTopoBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef McTopoBase_cxx
McTopoBase::McTopoBase(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("data12-Mrec90MeV.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("data12-Mrec90MeV.root");
      }
      f->GetObject("mctopoKK",tree);

   }
   Init(tree);
}

McTopoBase::~McTopoBase()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t McTopoBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t McTopoBase::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void McTopoBase::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("indexmc", &indexmc, &b_indexmc);
   fChain->SetBranchAddress("pdgid", pdgid, &b_pdgid);
   fChain->SetBranchAddress("motheridx", motheridx, &b_motheridx);
	 fChain->SetBranchAddress("idx", idx, &b_idx);
	 fChain->SetBranchAddress("hash", &hash, &b_hash);
	 //std::cout << "SetBranchAddress for idx: ";
   //std::cout << fChain->SetBranchAddress("idx", idx, &b_idx) << std::endl;
	 //std::cout << "SetBranchAddress for hash: ";
   ////std::cout << fChain->SetBranchAddress("hash", &hash, &b_hash) << std::endl;
   Notify();
}

Bool_t McTopoBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void McTopoBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t McTopoBase::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef McTopoBase_cxx
