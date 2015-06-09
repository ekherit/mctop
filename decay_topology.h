#ifndef IBN_MCTOP_DECAY_TOPOLOGY_H
#define IBN_MCTOP_DECAY_TOPOLOGY_H

#include <iostream>
#include <string>

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


#endif
