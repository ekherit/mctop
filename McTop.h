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
#include <cmath>
#include <iostream>
#include <algorithm>

#include <boost/crc.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

struct particle_t
{
  int pdgid;
  std::string name;
};

struct topology_property_t
{
  Long64_t hash;
};

typedef boost::adjacency_list <boost::vecS,boost::vecS,boost::bidirectionalS,particle_t, boost::no_property, topology_property_t > decay_topology_t;

inline bool operator<(const decay_topology_t & top1, const decay_topology_t & top2)
{
  return top1[boost::graph_bundle].hash < top2[boost::graph_bundle].hash;
};

//find the root of the graph
inline void find_root(const  decay_topology_t &  top, std::list<int> & root_list,  int idx)
{
  decay_topology_t::in_edge_iterator in_begin, in_end;
  boost::tie(in_begin, in_end) = in_edges(idx,top);
  if(in_begin == in_end) //если нет родителей, тогда заполним массив
  {
    root_list.push_back(idx);
    return;
  }
  else//а если есть, то поёдем по родителям
  {
    for (; in_begin != in_end; ++in_begin)
    {   
      find_root(top,root_list, source(*in_begin,top));
    }
  }
}

inline std::string to_string(const  decay_topology_t & top)
{
    std::list<int> root_list;
    find_root(top,root_list,0);
    std::function<void(int)> loop;
    std::string s;
    loop = [&](int idx)
    {
      decay_topology_t::adjacency_iterator begin, end;
      tie(begin, end) = adjacent_vertices(idx, top);
      std::string name = top[idx].name;
      if(name == "") name = "["+std::to_string(top[idx].pdgid)+"]";
      if(begin != end)
      {
        s+="(" + name + " -> ";
        for (; begin != end; ++begin)
        {   
          loop(*begin);
        }
        s+=")";
      }
      else 
      {
        s+=name;
      }
    };
    for(auto root : root_list)
    {
      loop(root);
      s+= ",";
    }
    return s;
}




inline Long64_t hash(const decay_topology_t & top) 
{
  boost::crc_32_type crc;
  //1. Find list of roots (vertices which has no parrents)
  std::list<int> roots;
  find_root(top,roots,2);
  std::function<void(int)> loop;
  loop = [&](int idx)
  {
    crc(top[idx].pdgid);
    decay_topology_t::adjacency_iterator begin, end;
    tie(begin, end) = adjacent_vertices(idx, top);
    if(begin == end) return; //the end of this branch
    else
    {
      for (; begin != end; ++begin)
      {   
        loop(*begin);
        //std::cout << source(*in_begin,top) << std::endl;
      }
    }
  };
  for(int root: roots)
  {
    loop(root);
  }
  return crc.checksum();
};


struct mctop_t
{
  std::vector<int> pdgid;
  std::vector<int> mother;
  std::vector<int> idx;
  size_t size(void) const
  {
    if(pdgid.size()!=mother.size())
    {
      throw std::runtime_error("Wrong topology size: pdgid.size != mother.size");
    }
    return pdgid.size();
  }

  void resize(size_t N)
  {
    pdgid.resize(N);
    mother.resize(N);
    idx.resize(N);
  }

  Long64_t hash(void) const
  {
    boost::crc_32_type crc;
    crc = std::for_each(pdgid.begin(),pdgid.end(),crc);
    crc = std::for_each(idx.begin(),idx.end(),crc);
    crc = std::for_each(mother.begin(),mother.end(),crc);
    return crc.checksum();
  };

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
  return top1.hash() < top2.hash();
};


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
   Int_t           idx[100];   //[indexmc]

   // List of branches
   TBranch        *b_indexmc;   //!
   TBranch        *b_pdgid;   //!
   TBranch        *b_motheridx;   //!
   TBranch        *b_idx;   //!

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
   virtual std::map<decay_topology_t, Long64_t> Count2(void);
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
   fChain->SetBranchAddress("idx", idx, &b_idx);
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
