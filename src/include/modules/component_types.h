#pragma once

/** TODO: Documentation, ie.:
 * Add ASCII Art of type hierarchy here
 * Add description to classes and their purpose
 */

/** TODO: CPU-type that combines CPUCore and L1,2,3... Caches? */
/** TODO: Unified Cache, subclasses that inherit from L1Data und L1Instruction */

/** TODO: Distinguish between Memory-DIMMs and Memory in general 
 * @see e.g. SMBIOS specification for better memory component types
 * like e.g. memory controller, memory channel, memory dimm (module) 
 */

/** TODO: Add more IO-Devices and Link-types (PCIe, USB, InfiniBand ?) */

/** @brief Helper macro for type declaration. */
#define YLOC_DECLARE_TYPE(type_name, ...)    \
    class type_name : public __VA_ARGS__     \
    {                                        \
    public:                                  \
        virtual ~type_name() = default;      \
        inline static const type_name *ptr() \
        {                                    \
            static const type_name s;        \
            return &s;                       \
        }                                    \
    };

namespace yloc
{
    /** @brief Base class for all hardware component types */
    class Component
    {
    public:
        virtual ~Component() = default;

        template <class ComponentType>
        bool is_a() const
        {
            return nullptr != dynamic_cast<const ComponentType *>(this);
        }
    };

    /***********************************
     * List of component types
     ***********************************/

    /** @brief Null-type. */
    YLOC_DECLARE_TYPE(UnknownComponentType, Component)

    /***********************************
     * Compute-Components
     ***********************************/

    YLOC_DECLARE_TYPE(Compute, Component)

    YLOC_DECLARE_TYPE(CPUCore, Compute)
    YLOC_DECLARE_TYPE(LogicalCore, Compute)
    YLOC_DECLARE_TYPE(PhysicalCore, Compute)

    /***********************************
     * Storage-Components
     ***********************************/

    YLOC_DECLARE_TYPE(Storage, Component)
    YLOC_DECLARE_TYPE(Memory, Storage)
    YLOC_DECLARE_TYPE(Cache, Storage)
    YLOC_DECLARE_TYPE(PersistentStorage, Storage)

    YLOC_DECLARE_TYPE(VolatileMemory, Memory)
    YLOC_DECLARE_TYPE(NonVolatileMemory, Memory)

    YLOC_DECLARE_TYPE(DataCache, Cache)
    YLOC_DECLARE_TYPE(InstructionCache, Cache)
    YLOC_DECLARE_TYPE(UnifiedCache, DataCache, InstructionCache)

    YLOC_DECLARE_TYPE(L1Cache, Cache)
    YLOC_DECLARE_TYPE(L2Cache, Cache)
    YLOC_DECLARE_TYPE(L3Cache, Cache)
    YLOC_DECLARE_TYPE(L4Cache, Cache)

    YLOC_DECLARE_TYPE(L1DataCache, L1Cache, DataCache)
    YLOC_DECLARE_TYPE(L2DataCache, L2Cache, DataCache)
    YLOC_DECLARE_TYPE(L3DataCache, L3Cache, DataCache)
    YLOC_DECLARE_TYPE(L4DataCache, L4Cache, DataCache)

    YLOC_DECLARE_TYPE(L1InstructionCache, L1Cache, InstructionCache)
    YLOC_DECLARE_TYPE(L2InstructionCache, L2Cache, InstructionCache)
    YLOC_DECLARE_TYPE(L3InstructionCache, L3Cache, InstructionCache)
    YLOC_DECLARE_TYPE(L4InstructionCache, L4Cache, InstructionCache)

    YLOC_DECLARE_TYPE(L1UnifiedCache, L1Cache, UnifiedCache)
    YLOC_DECLARE_TYPE(L2UnifiedCache, L2Cache, UnifiedCache)
    YLOC_DECLARE_TYPE(L3UnifiedCache, L3Cache, UnifiedCache)
    YLOC_DECLARE_TYPE(L4UnifiedCache, L4Cache, UnifiedCache)

    YLOC_DECLARE_TYPE(SolidStateDrive, PersistentStorage)
    YLOC_DECLARE_TYPE(HardDiskDrive, PersistentStorage)

    /***********************************
     * Accelerator-Components
     ***********************************/

    YLOC_DECLARE_TYPE(Accelerator, Component)
    YLOC_DECLARE_TYPE(GPU, Accelerator)
    YLOC_DECLARE_TYPE(FPGA, Accelerator)

    YLOC_DECLARE_TYPE(GPUCore, GPU, Compute)
    YLOC_DECLARE_TYPE(GPUMemory, GPU, Memory)

    /***********************************
     * IO-Components
     ***********************************/

    YLOC_DECLARE_TYPE(InputOutput, Component)
    YLOC_DECLARE_TYPE(NetworkDevice, InputOutput)

    /** TODO: isn't HDD also I/O? */
    // YLOC_DECLARE_TYPE(PersistentStorage, InputOutput)

    /***********************************
     * Link-Components
     ***********************************/

    YLOC_DECLARE_TYPE(Link, Component)
    YLOC_DECLARE_TYPE(Bus, Link)

    /***********************************
     * Miscellaneous
     ***********************************/

    /**
     * @brief Type for Miscellaneous components, filler class.
     *
     */
    YLOC_DECLARE_TYPE(Misc, Component)

/** TODO: do we really want to use dynamic_cast for logical edge types parent & child? */
    /**
     * @brief Edge type that enables ordering within the graph
     */
    YLOC_DECLARE_TYPE(LogicalEdgeType, Component)
    YLOC_DECLARE_TYPE(Parent, LogicalEdgeType)
    YLOC_DECLARE_TYPE(Child, LogicalEdgeType)
}
