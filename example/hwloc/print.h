/**
 * print.h
 */

// TODO: move print functionality to hwloc module

#pragma once

#include <hwloc.h>
#include <iostream>

static std::string hwloc_attribute_string(hwloc_obj_t obj)
{
    std::stringstream ss;

#if 0
    for (int i = 0; i < obj->infos_count; i++) {
        ss << std::endl;
        ss << obj->infos->name << "=";
        ss << obj->infos->value;
    }
#endif

    if (obj->attr != NULL) {
        // attr points to a union hwloc_obj_attr_u of type-specific attribute structures
        ss << std::endl;

        if (hwloc_obj_type_is_cache(obj->type)) {
            ss << obj->attr->cache.size / 1024 << "kB"
               << " "
               << obj->attr->cache.associativity << "-way";
        }

        if (hwloc_obj_type_is_memory(obj->type)) {
            ss << obj->attr->numanode.local_memory / 1024 / 1024 << "MB";
        }

        if (hwloc_obj_type_is_io(obj->type)) {
            if (hwloc_compare_types(obj->type, HWLOC_OBJ_OS_DEVICE) == 0) {
                // ss << obj->attr->osdev.type;
                //
                // TODO: convert osdev type to meaningful string according to the following list
                // there must be a hwloc function ...
                /* from hwloc doc:
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
    } /* obj->attr != NULL */

    if (obj->subtype != NULL) {
        ss << "subtype: " << obj->subtype
           << std::endl;
    }

    if (obj->name != NULL) {
        ss << "name: " << obj->name
           << std::endl;
    }

    return ss.str();
}

std::string hwloc_string(hwloc_obj_t obj)
{
    std::stringstream ss;
    ss << hwloc_obj_type_string(obj->type); /*<< std::endl*/
#if VERBOSE_HWLOC_ATTRIBUTES
    int verbose = 1;
    const char *sep = "\n";
    const size_t attribute_len_max = 4096;
    char attributes[attribute_len_max];
    hwloc_obj_attr_snprintf(attributes, attribute_len_max, obj, sep, verbose);
    ss << attributes;
#else
    ss << hwloc_attribute_string(obj);
#endif
    return ss.str();
}