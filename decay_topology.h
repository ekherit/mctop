#ifndef IBN_MCTOP_DECAY_TOPOLOGY_H
#define IBN_MCTOP_DECAY_TOPOLOGY_H

#include <iostream>
#include <string>

#include <boost/crc.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>


#include "pdg_table.h"

struct particle_t
{
  int pdgid;
  std::string name;
  particle_t(int id=0)
  {
    pdgid=id;
    name = PdgTable[pdgid];
  }
};

struct topology_property_t
{
  //std::list<Long64_t> hash_list;
  //Long64_t hash(void) { return hash_list.front(); };
  long hash;
};

typedef boost::adjacency_list <boost::vecS,boost::vecS,boost::bidirectionalS,particle_t, boost::no_property, topology_property_t > decay_topology_t;

typedef boost::graph_traits<decay_topology_t>::vertex_descriptor vertex_t;

inline bool operator<(const decay_topology_t & top1, const decay_topology_t & top2)
{
  return top1[boost::graph_bundle].hash < top2[boost::graph_bundle].hash;
};


//inline bool operator==(const decay_topology_t & top1, const decay_topology_t & top2)
//{
//  return top1[boost::graph_bundle].hash == top2[boost::graph_bundle].hash;
//};



struct PdgComp //lower |pdgid| go first positive charged go first, 
{
  const decay_topology_t & top;
  PdgComp(const decay_topology_t & t) : top(t) {}
  //bool operator()(int i1, int i2)
  bool operator()(auto i1, auto i2)
  {
    if(abs(top[i1].pdgid) == abs(top[i2].pdgid))
    {
      return top[i1].pdgid > top[i2].pdgid;  //positive charged go first
    }
    return abs(top[i1].pdgid) <  abs(top[i2].pdgid); //lower |pdgid| go first
  }
};

//find the root of the graph
inline void find_root(const  decay_topology_t &  top, std::list<vertex_t> & root_list,  vertex_t idx)
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

//find the root of the graph
inline std::list<vertex_t> find_root(const  decay_topology_t &  top)
{
  std::list<vertex_t>  root_list;
  boost::graph_traits <decay_topology_t>::vertex_iterator it, end;
  //loop over all particles
  for(tie(it, end) = boost::vertices(top); it!=end; ++it)
  {
    decay_topology_t::in_edge_iterator in_begin, in_end;
    boost::tie(in_begin, in_end) = boost::in_edges(*it,top);
    if(in_begin == in_end)  
    {
      //if(top[*it].pdgid!=100443)
      //std::cout << *it << " " << top[*it].pdgid << " " << top[*it].name << std::endl;
      root_list.push_back(*it);
    }
  }
  root_list.sort();
  root_list.unique();
  return root_list;
}


inline std::string to_string(const  decay_topology_t & top)
{
    std::list<vertex_t> root_list = find_root(top);
    std::function<void(vertex_t)> loop;
    std::string s;
    bool first_entry=true;
    loop = [&](vertex_t idx)
    {
      std::string name = top[idx].name;
      if(name == "") name = "["+std::to_string(top[idx].pdgid)+"]";
      bool isroot = first_entry;
      if(first_entry) first_entry=false;
      decay_topology_t::adjacency_iterator begin, end;
      tie(begin, end) = adjacent_vertices(idx, top);
      if(begin != end)
      {
        if(!isroot) s+="(";
        s+= name + " → ";

        std::list<vertex_t> lst(begin,end);
        //for (auto i=begin; i != end; ++i) lst.push_back(*i);
        lst.sort(PdgComp(top));
        for(auto k : lst) loop(k);

        //for (; begin != end; ++begin)
        //{   
        //  loop(*begin);
        //}
        if(!isroot) s+=")";
      }
      else 
      {
        s+=name;
      }
    };
    for(auto  it = begin(root_list); it!=end(root_list); it++)
    {
      loop(*it);
      //if(it != rbegin(root_list)) 
      if(next(it)!=end(root_list)) s+=",";
    }
    return s;
}

inline std::string final_state(const  decay_topology_t & top)
{
  typedef boost::graph_traits <decay_topology_t>::vertex_iterator  VI_t ;
  VI_t it, end;
  std::list<vertex_t> lst;
  for(tie(it, end) = boost::vertices(top); it!=end; ++it)
  {
    decay_topology_t::adjacency_iterator begin, end;
    tie(begin, end) = adjacent_vertices(*it, top);
    if(begin==end) 
    {
      lst.push_back(*it);
    }
  }
  lst.sort(PdgComp(top));
  std::string s;
  for(auto i : lst)
  {
      std::string name = top[i].name;
      if(name == "") name = "["+std::to_string(top[i].pdgid)+"]";
      s+=name;
  }
  return s;
}




inline long hash(const decay_topology_t & top) 
{
  boost::crc_32_type crc;
  auto roots = find_root(top);
  std::function<void(vertex_t)> loop;
  loop = [&](vertex_t idx)
  {
    crc(top[idx].pdgid);
    boost::graph_traits<decay_topology_t>::adjacency_iterator i,begin, end;
    tie(begin, end) = adjacent_vertices(idx, top);
    if(begin == end) return; //the end of this branch
    else
    {
      std::list<vertex_t> lst(begin,end);
      lst.sort(PdgComp(top));
      for(auto & b : lst) loop(b);
    }
  };
  for(vertex_t r: roots) loop(r);
  return crc.checksum();
};

inline decay_topology_t conj(const decay_topology_t  & top)
{
  auto atop = top; //antiparticle topology
  boost::graph_traits <decay_topology_t>::vertex_iterator it, end;
  for(tie(it, end) = boost::vertices(atop); it!=end; ++it)
  {
    int & pdgid = atop[*it].pdgid;
    auto & name = atop[*it].name;
    //check neutral 
    auto aname = PdgTable[-pdgid];
    if( aname == name) continue; 
    pdgid = - pdgid;
    name = aname;
  }
  atop[boost::graph_bundle].hash = hash(atop); //recalculate hash
  return atop;
}

inline void remove_particle(int pdgid, decay_topology_t & top)
{
  boost::graph_traits <decay_topology_t>::vertex_iterator it, end;
  //плохо работало в цикле очистка вершин и затем одновременное удаление.
  //вначале надо было очистить по списку а затем вторым проходом удалять вершину.
  //for(tie(it, end) = boost::vertices(top); it!=end; ++it)
  //{
  //  if(top[*it].pdgid == pdgid)
  //  {
  //    clear_vertex(*it,top);
  //  }
  //}
  //for(tie(it, end) = boost::vertices(top); it!=end; ++it)
  //{
  //  if(top[*it].pdgid == pdgid)
  //  {
  //    remove_vertex(*it,top);
  //  }
  //}

  boost::graph_traits<decay_topology_t>::vertex_iterator vi, vi_end, next;
  tie(vi, vi_end) = vertices(top);
  for (next = vi; vi != vi_end; vi = next) 
  {
    ++next;
    if(top[*vi].pdgid == pdgid) 
    {
      clear_vertex(*vi,top);
      remove_vertex(*vi, top);
    }
  }
}


#endif
