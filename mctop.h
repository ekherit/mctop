#ifndef IBN_MCTOP_H
#define IBN_MCTOP_H
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

#endif
