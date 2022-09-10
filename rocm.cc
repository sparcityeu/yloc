
#include <cstring>
// #include <hwloc/opencl.h>
#include "rocm_smi/rocm_smi.h"


    // #include <stdint.h>
  rsmi_status_t ret;
  uint32_t num_devices;
  uint16_t dev_id;
  uint32_t numa_node;

  // We will skip return code checks for this example, but it
  // is recommended to always check this as some calls may not
  // apply for some devices or ROCm releases

int MAX_SKU_LEN = 2048;
char sku[MAX_SKU_LEN];
  ret = rsmi_init(0);
  ret = rsmi_num_monitor_devices(&num_devices);

  for (int i=0; i < num_devices; ++i) {
    ret = rsmi_dev_id_get(i, &dev_id);
    std::cout << "device id: " << dev_id << std::endl;
//   rsmi_dev_sku_get(i, sku);
  rsmi_dev_vendor_name_get (i, sku, MAX_SKU_LEN);
    std::cout << sku << std::endl;
  rsmi_dev_vram_vendor_get (i, sku, MAX_SKU_LEN);
  std::cout << sku << std::endl;

  rsmi_topo_numa_affinity_get (
dev_id,
&numa_node );
std::cout << "numa node: " << numa_node << std::endl;
    // dev_id holds the device ID of device i, upon a
    // successful call
  }
  ret = rsmi_shut_down();

  /*
  The sysfs topology is arranged hierarchically as following. The root directory of the topology is
/sys/devices/virtual/kfd/kfd/topology/nodes/

Application software should
always use Thunk library API (libhsakmt) to access topology information. Please refer to Thunk API for more infor-
mation.
*/
