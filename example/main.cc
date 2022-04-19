
//#include <mpi.h>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/graphviz.hpp>      // write_graphviz
#include <boost/property_map/property_map.hpp>
#include <fstream>
#include <hwloc.h>
#include <unordered_map>

// Todo create combined header
//#include <yloc.h>
#include "graph_type.h"
#include "init.h"
#include "modules.h"
#include "query_graph.h"

int sample_usage_myloq();

int main(int arc, char *argv[])
{

    // MPI_Init(&argc, &argv);
    yloc_init(YLOC_FULL | YLOC_ONGOING);

    sample_usage_myloq();

    yloc_finalize();
    // MPI_Finalize();
}

typedef boost::graph_traits<graph_t>::vertex_descriptor VD;
extern std::unordered_map<VD, hwloc_obj_t> hwloc_property_map;

// Adopted from myloq by Jakub Orlowski (jakubo87)
// https://github.com/jakubo87/myLoq
int sample_usage_myloq()
{
    graph_t g;
    main_module()->init_graph(&g);

    std::string dot_file_name{"graph.dot"};
    std::ofstream ofs{dot_file_name};

    // write graph with labels to file
    // get interal property map for Vertex::type to write labels to graph vertices
    // boost::write_graphviz(ofs, g, boost::make_label_writer(get(&Vertex::type, g)));

    // example of using an external property map:
    {
        std::unordered_map<VD, std::string> hwloc_string_property_map;

        // build new string property map from hwloc objects to write labels of hwloc object attributes:
        for (auto kv : hwloc_property_map) {
            hwloc_obj_t hwloc_object = kv.second;
            std::stringstream ss;

            ss << hwloc_obj_type_string(kv.second->type);

#define USE_LONG_ATTRIBUTE_DESCRIPTION 1
#if USE_LONG_ATTRIBUTE_DESCRIPTION
            ss << std::endl;
            char buf[1024];
            hwloc_obj_attr_snprintf(buf, 1024, hwloc_object, "\n", 1);
            ss << buf;
#else
            if (hwloc_object->attr != NULL) {
                // attr points to a union hwloc_obj_attr_u of type-specific attribute structures
                ss << std::endl;

                if (hwloc_obj_type_is_cache(kv.second->type)) {
                    ss << kv.second->attr->cache.size / 1024 << "kB"
                       << " "
                       << kv.second->attr->cache.associativity << "-way";
                }

                if (hwloc_obj_type_is_memory(kv.second->type)) {
                    ss << kv.second->attr->numanode.local_memory / 1024 / 1024 << "MB";
                }

                if (hwloc_obj_type_is_io(kv.second->type)) {
                    if (hwloc_compare_types(kv.second->type, HWLOC_OBJ_OS_DEVICE) == 0) {
                        // TODO: convert osdev type to meaningful string
                        ss << kv.second->attr->osdev.type;

                        /*

HWLOC_OBJ_OSDEV_BLOCK Operating system block device, or non-volatile memory device. For
instance "sda" or "dax2.0" on Linux.

HWLOC_OBJ_OSDEV_GPU Operating system GPU device. For instance ":0.0" for a GL display,
"card0" for a Linux DRM device.

HWLOC_OBJ_OSDEV_NETWORK Operating system network device. For instance the "eth0" interface
on Linux.

HWLOC_OBJ_OSDEV_OPENFABRICS Operating system openfabrics device. For instance the "mlx4_0"
InfiniBand HCA, or "hfi1_0" Omni-Path interface on Linux.

HWLOC_OBJ_OSDEV_DMA Operating system dma engine device. For instance the
"dma0chan0" DMA channel on Linux.

HWLOC_OBJ_OSDEV_COPROC Operating system co-processor device. For instance "opencl0d0"
for a OpenCL device, "cuda0" for a CUDA device.

                        */
                    }
                }
            }

            if (hwloc_object->subtype != NULL) {
                ss << std::endl
                   << hwloc_object->subtype;
            }

            if (hwloc_object->name != NULL) {
                ss << std::endl
                   << hwloc_object->name;
            }
#endif

            hwloc_string_property_map[kv.first] = ss.str();
        }

        auto pm = boost::make_assoc_property_map(hwloc_string_property_map);

        write_graphviz(ofs, g, boost::make_label_writer(pm));
    }

#if 0
  make_dotfile(g, "new.dot");

  //###################################################################
  //TESTS:

  //###############################   FILTER GRAPH   ################################################
  auto fgv = filtered_graph(g, &Vertex::index, Index(1));
  std::cout << "show only vertices with index = 1" << std::endl; 
  boost::print_graph(fgv);
  make_dotfile(fgv, "filtered_vertex_graph.dot");

  auto fge = filtered_graph(g, &Edge::label, EType("child"));
  std::cout << "show only edges with label child" << std::endl; 
  boost::print_graph(fge);
  make_dotfile(fge, "filtered_edge_graph.dot");

  //##################################    MATHS     #################################################
  //combinatorics (to be integrated into finding best solution)
  auto vec = comb(4,std::vector<int> {2,4,6,8,10});
  std::cout << "combining  [2,4,6,8,10] into sets of 4:" << std::endl;
  for(auto co : vec){
    for(auto el : co)
      std::cout << el << " ";
    std::cout << std::endl;
  }
  std::cout << vec.size() << " possible combinations." << std::endl;

  //################################     BASICS     ##################################################
  //find vd 
  auto co0 = get_vds(
      g,                //the graph
      std::make_pair(&Vertex::type,VType("HWLOC_OBJ_CORE")), //the type
      std::make_pair(&Vertex::index, Index(0)));               //the index
  std::cout << "vd of core 0: " << co0.front() << std::endl;
//
//  //###############################      GET/SET     #################################################
//  const auto e1 = get_ed(g,2,1,"child").front();
//  std::cout << "setting the weight of child edge between vertices 2 and 1. Original value: " << get(&Edge::weight, g, e1) << std::endl;
//  put(&Edge::weight, g, e1, 1000);
//  std::cout << "new value is: " << get(&Edge::weight, g, e1) << std::endl;
//
//
//
//  //generic vd query TODO does this also work when queries are generated at runtime...???
//  vds = get_vds(g, VType("HWLOC_OBJ_CORE"));
//  std::cout << "testing generic querying for vds... CORES have vd: " << std::endl;
//  for (auto& v : vds)
//    std::cout << v << " ";
//  std::cout << std::endl;
//  std::cout << "VDs with rubbish type: " << std::endl;
//  vds = get_vds(g, VType("xxx"));
//  for (auto& v : vds)
//    std::cout << v << " ";
//  std::cout << std::endl;
//  //  vds = get_vds(g, 1); <- does not compile you have to write the explicit type
//  std::cout << "searching for VD with type: CORE and index: 1 (testing matching in reverse order): " << std::endl;
//  vds = get_vds(g, Index(1), VType("HWLOC_OBJ_CORE"));
//  for (auto& v : vds)
//    std::cout << v << " ";
//  std::cout << std::endl;
//
//
//  
//  //find edge properties
//  std::cout << "ed label from vd 1 to 2: " << get_edge_label(g, get_ed(g,1,2,"parent").at(0)) << std::endl;
//  //find edges by their property
//  std::cout << "List all edges with the parent property:" << std::endl;
//  auto eds = get_eds(g, "parent");
//  for (const auto& ed : eds){
//    std::cout << boost::source(ed,g) << " to " << boost::target(ed,g) << std::endl;
//  }
//
//
//  //GROUPS
//  //make group - at first arbitrary
//  std::vector<VD> vs = {10,12};
//  auto i = make_group("Group1", vs, g);
//  std::cout << " Group1 has vd: " << i << std::endl; 
//
//
  //#################################      VISITORS       TODO###########################################
  count_obj(g,1); //a bit obsolete due to filtering
  accumulate(g,&Vertex::index, 1);
//
//  //###### PATHS/patterns #########
//  //return the group members of group 
//  //std::cout << "The members of Group1 are the following:" << std::endl;
//  //TODO make path queries like "is connected to group", or "is 'child' of cache" 
//
//
//  //find subgraphs
//  find_pattern(g); //TODO
//
//  ///NOTE!!!! in order to calculate properties simply make a subgraph and use std::accumlate... for instance...
//
//
//
  //#####################################       DISTANCES          #######################################
  //calc custom distance
  //define a distance function:
  //NOTE: it may be benefivial defining it in a separate header and making it a template to be able to use iton filtered graphs
  //though it may not be useful, as non-modifying distance calculations will take place on the original graph
  //... or maybe on a filtered graph..?
  std::function<double(VD,VD, const graph_t&)> dist1 =  [&](auto va, auto vb, const graph_t& g)
    {
      //the result, if the graph has no direct edge in any allowed category defined by this function
      double res = NOPATH;  //default 
      auto range = boost::out_edges(va, g);
      //check all edges for label "child"
       std::for_each (range.first, range.second,[&](const auto & ei){
        if (g[ei].label=="child" && vb==target(ei,g))
          res = 10.0;       //case of rising in the hierarchy
      });
      //check all edges for label "parent"
      std::for_each (range.first, range.second,[&](const auto & ei){
        if (g[ei].label=="parent" && va==source(ei,g) && vb==target(ei,g))
          res = 0.0;        //case when descending in hierarchy
      });

      res+= get(&Vertex::index, g, vb); 
      return res;
    };


  std::cout << "path from PU#0 to PU#3:" << std::endl; 
  auto pu0 = get_vds(g, std::make_pair(&Vertex::type, "HWLOC_OBJ_PU"), std::make_pair(&Vertex::index, Index(0))).front();
  auto pu3 = get_vds(g, std::make_pair(&Vertex::type, "HWLOC_OBJ_PU"), std::make_pair(&Vertex::index, Index(3))).front();
  auto r1 = shortest_path(g, pu0, pu3, dist1); 
  for (auto vd : r1)
    std::cout << vd << " ";
  std::cout << std::endl;

  //ancestry iterator
  auto l3c0 = get_vds(g, std::make_pair(&Vertex::type, "HWLOC_OBJ_L3CACHE"), std::make_pair(&Vertex::index, Index(0))).front();
  std::cout << "is LVL3 cache#0 an ancestor of PU#3?: " << is_ancestor(l3c0,pu3,g) << std::endl;
  std::cout << "vice versa?: " << is_ancestor(pu3,l3c0,g) << std::endl;
//  //return clostest vertices of specified type sorted by distance
//  auto cl_pus = find_closest_to(g, dist1, "HWLOC_OBJ_PU", 11);
//  std::cout << "closest PUs relative to vd(11) with respect to user defined function dist1:" << std::endl;
//  for (auto a : cl_pus){
//    std::cout << a.first << " " ;
//  }
//  std::cout << std::endl;
//  std::cout << "with distances (respectively):" << std::endl;
//  for (auto a : cl_pus){
//    std::cout << a.second << " " ;
//  }
//  std::cout << std::endl;
//
//  //dijstra
//  auto r2 = dijkstra_spaths(g, 5, dist1);
//  for(VD vd = 0; vd<r2.size(); ++vd)
//    std::cout << "from vd 5 to "<< vd << ", shortest distance: " << r2[vd] << std::endl; 
//
//
//  //dijstrap on a filtered graph
//  //auto r3 = dijkstra_spaths(fgv, 5, dist1);
//  //for(VD vd = 0; vd<r3.size(); ++vd)
//  //  std::cout << "from vd 5 to "<< vd << ", shortest distance: " << r3[vd] << std::endl; 
//
//
//  //TODO find partitioning
//
//
//
//
//
//  //isolate a subgraph and reduce to hwloc relationships
//  auto ctree2 = make_can_tree(g,14);
//  make_dotfile_nolabel(ctree2,"hwloc.dot");

  make_dotfile_nolabel(g,"totalnl.dot");

  //copy tests with copy graph
  graph_t cfg; //deep copy of filtered graph (is this even possible...?, what about the vertex indices...?
  copy_graph(g, cfg);
  make_dotfile_nolabel(cfg, "copied_nolabel.dot");


  //####################################   K-PARTITIONS CU   #################################################
  //make k partitions
  k_partitions(g,2,lca_dist);


  //#####################################   REACHABILITY  #######################################
  //forming a disjoined graph 
  auto part = filtered_graph(g, &Edge::label, EType("partition"));
  is_reachable(part, 10, 11);
  is_reachable(part, 11, 10); //against the direction....
  is_reachable(part, 10, 13);



  //#################### ADJACENCY FILTER #########################
  auto vic = vicinity(g, l3c0);
  make_dotfile(vic, "vicinity.dot");

  


  //######################################## LEADER TEST  #####################################################
  
  //partition into 2 cluster
  //
  //Measurement happens here
  //for (a : N)
  //  for (b : M)
  //     (<a,b>,t_ab) = measured time
  //
  
  auto leadpart = isolate_cat(g, EType("partition"));
  make_dotfile(leadpart, "part.dot");

  std::function<double(VD,VD, const graph_t&)> d_meas =  [&](auto va, auto vb, const graph_t& g)
    {
      //return your measured results here
      //we have no results so we cheat...
      return lca(g, va,vb) + get(&Vertex::index, g, va) + get(&Vertex::index, g, vb);
    };
  
  //would have been nicer with something like accumulate_if
  auto range = vertices(leadpart);
  std::vector<VD> N;
  auto t = *range.first; //choose which goes where
  std::vector<VD> M;

  //populate
  std::for_each(range.first, range.second, [&](const auto & v)
    {
      if (is_reachable(leadpart, t, v))
        N.push_back(v);
      else
        M.push_back(v);
    }
  );

  VD n_ = 0;
  VD m_ = 0;
  double dn = NOPATH;
  double dm = NOPATH;

  //accumulate and find min sum
  for(auto n : N){
    double sum=0;      //(re)set
    for (auto m : M)
      sum+=d_meas(n,m,g);
    if (sum < dn){
      dn = sum;
      n_ = n;
    }
  }

  //accumulate and find min sum
  for(auto m : M){
    double sum=0;
    for (auto n : N)
      sum+=d_meas(m,n,g);
    if (sum < dm){
      dm = sum;
      m_ = m;
    }
  }

  std::vector<VD> leet;
  leet.push_back(n_);
  leet.push_back(m_);

  make_group(std::string("leader"), leet, g);



  make_dotfile_nolabel(g,"totalnl.dot");
  make_dotfile(g,"total.dot");
#endif
    return EXIT_SUCCESS;
}
