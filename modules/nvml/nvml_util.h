#pragma once
#include <nvml.h>

#define VERBOSE_NVML 1

#if (VERBOSE_NVML > 0)
#define CHECK_NVML(x) CHECK_NVML_MSG(x)
#else
#define CHECK_NVML(x) ((x) == NVML_SUCCESS)
#endif

#define CHECK_NVML_MSG(x)                                                                                    \
    ({                                                                                                       \
        bool success;                                                                                        \
        nvmlReturn_t ret = (x);                                                                              \
        success = (ret == NVML_SUCCESS);                                                                     \
        if (!success) {                                                                                      \
            const char *err_msg = nvmlErrorString(ret);                                                      \
            std::cerr << "[yloc nvml module] " #x " failed in file " __FILE__ " line " << __LINE__ << ":\n"; \
            std::cerr << err_msg << '\n';                                                                    \
        }                                                                                                    \
        success;                                                                                             \
    })

#define EXIT_ERR_NVML(x)                            \
    do {                                            \
        if (!CHECK_NVML_MSG(x)) {                   \
            std::cerr << "shutting nvml down...\n"; \
            nvmlShutdown();                         \
        }                                           \
    } while (0)


static uint64_t bdfid_get(nvmlPciInfo_t &pci)
{
    return ((static_cast<uint64_t>(pci.domain) & 0xffffffff) << 32) | ((static_cast<uint64_t>(pci.bus) & 0xff) << 8) |
           ((static_cast<uint64_t>(pci.device) & 0x1f) << 3) /* | ((static_cast<uint64_t>(pci.busId[31])) & 0x7) */;
           // or pci.busId[0] ? not clear from NVML documentation -- assume zero function.
}
