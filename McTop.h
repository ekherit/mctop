//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Nov 27 17:58:23 2014 by ROOT version 5.34/05
// from TTree mctopo/Monte Carlo truth information topology
// found on file: MCJPKK-0009023.root
//////////////////////////////////////////////////////////

#ifndef McTop_h
#define McTop_h

//#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <stdexcept>


struct mctop_t
{
  std::vector<int> pdgid;
  std::vector<int> mother;
  size_t size(void) const
  {
    if(pdgid.size()!=mother.size())
    {
      throw std::runtime_error("Wrong topology size: pdgid.size != mother.size");
    }
    return pdgid.size();
  }
};

inline bool operator==(const mctop_t & top1, const mctop_t & top2)
{
  if(top1.size() != top2.size()) return false;
  for(int i=0;i<top1.size();i++)
  {
    if(top1.pdgid[i]!=top2.pdgid[i]) return false;
    if(top1.mother[i]!=top2.mother[i]) return false;
  }
  return true;
}

inline bool operator<(const mctop_t & top1, const mctop_t & top2)
{
  bool less = top1.size() < top2.size();
  for(int i=0;i<top1.size();i++)
  {
    less = less && top1.pdgid[i] < top2.pdgid[i];
    less = less && top1.mother[i] < top2.mother[i];
  }
  return less
}

class McTop
{
  std::string     fName; //tree name
public :
   TChain          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           indexmc;
   Int_t           pdgid[100];   //[indexmc]
   Int_t           motheridx[100];   //[indexmc]

   // List of branches
   TBranch        *b_indexmc;   //!
   TBranch        *b_pdgid;   //!
   TBranch        *b_motheridx;   //!

   McTop(const char * tree_name);
   virtual ~McTop();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TChain *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void AddFile(const char * file_name);
   virtual std::map<mctop_t,Long64_t> Count();
};

#endif

#ifdef McTop_cxx

McTop::McTop(const char * tree_name) : fChain(0) 
{
  fName = tree_name;
  fChain= new TChain(tree_name, tree_name);
  Init(fChain);
}

void McTop::AddFile(const char * file_name)
{
  if(fChain)
  {
    fChain->AddFile(file_name);
  }
}

McTop::~McTop()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t McTop::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t McTop::LoadTree(Long64_t entry)
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

void McTop::Init(TChain *tree)
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
   Notify();
}

Bool_t McTop::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void McTop::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t McTop::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef McTop_cxx
