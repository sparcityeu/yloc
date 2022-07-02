#pragma once

/** TODO: Documentation, ie.:
 * Add ASCII Art of type hiearchie here
 * Add description to classes and there purpose
 */

/** TODO: CPU-type that combines CPUCore and L1,2,3... Caches? */
/** TODO: Unified Cache, subclasses that inherit from L1Data und L1Instruction */
/** TODO: Distinguish between Memory-DIMMs and Memory in general */
/** TODO: Add more IO-Devices and Link-types (PCIe, USB, InfiniBand ?) */

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

    YLOC_DECLARE_TYPE(UnknownComponentType, Component)

    YLOC_DECLARE_TYPE(Compute, Component)
    YLOC_DECLARE_TYPE(CPUCore, Compute)

    YLOC_DECLARE_TYPE(Storage, Component)
    YLOC_DECLARE_TYPE(Memory, Storage)
    YLOC_DECLARE_TYPE(Cache, Storage)
    YLOC_DECLARE_TYPE(Disk, Storage)

    YLOC_DECLARE_TYPE(Misc, Component)

    YLOC_DECLARE_TYPE(MultipleInheritance, Storage, Compute)

#if DO_NOT_USE_MACRO
    /***********************************
     * Compute-Components
     ***********************************/

    class Compute : public Component
    {
    public:
        virtual ~Compute() = default;
    };

    class CPUCore : public Compute
    {
    public:
        virtual ~CPUCore() = default;
    };

    class LogicalCore : public CPUCore
    {
    public:
        virtual ~LogicalCore() = default;
    };

    class PhysicalCore : public CPUCore
    {
    public:
        virtual ~PhysicalCore() = default;
    };

    /***********************************
     * Storage-Components
     ***********************************/

    class Storage : public Component
    {
    public:
        virtual ~Storage() = default;
    };

    class Memory : public Storage
    {
    public:
        virtual ~Memory() = default;
    };

    /** All Memory is implicitly Volatile if not specified otherwise */
    /* class VolatileMemory : public Memory
    {
    public:
        virtual ~VolatileMemory() = default;
    }; */

    class NonVolatileMemory : public Memory
    {
    public:
        virtual ~NonVolatileMemory() = default;
    };

    class PersistentStorage : public Storage
    {
    public:
        virtual ~PersistentStorage() = default;
    };

    class HardDrive : public PersistentStorage
    {
    public:
        virtual ~HardDrive() = default;
    };

    class SolidStateDrive : public PersistentStorage
    {
    public:
        virtual ~SolidStateDrive() = default;
    };

    class Cache : public Storage
    {
    public:
        virtual ~Cache() = default;
    };

    class DataCache : public Cache
    {
    public:
        virtual ~DataCache() = default;
    };

    class InstructionCache : public Cache
    {
    public:
        virtual ~InstructionCache() = default;
    };

    class L1Cache : public Cache
    {
    public:
        virtual ~L1Cache() = default;
    };

    class L2Cache : public Cache
    {
    public:
        virtual ~L2Cache() = default;
    };

    class L3Cache : public Cache
    {
    public:
        virtual ~L3Cache() = default;
    };

    class L4Cache : public Cache
    {
    public:
        virtual ~L4Cache() = default;
    };

    class L1DataCache : public L1Cache, public DataCache
    {
    public:
        virtual ~L1DataCache() = default;
    };

    class L2DataCache : public L2Cache, public DataCache
    {
    public:
        virtual ~L2DataCache() = default;
    };

    class L3DataCache : public L3Cache, public DataCache
    {
    public:
        virtual ~L3DataCache() = default;
    };

    class L4DataCache : public L4Cache, public DataCache
    {
    public:
        virtual ~L4DataCache() = default;
    };

    class L1InstructionCache : public L1Cache, public InstructionCache
    {
    public:
        virtual ~L1InstructionCache() = default;
    };

    class L2InstructionCache : public L2Cache, public InstructionCache
    {
    public:
        virtual ~L2InstructionCache() = default;
    };

    class L3InstructionCache : public L3Cache, public InstructionCache
    {
    public:
        virtual ~L3InstructionCache() = default;
    };

    class L4InstructionCache : public L4Cache, public InstructionCache
    {
    public:
        virtual ~L4InstructionCache() = default;
    };

    /***********************************
     * Accelerator-Components
     ***********************************/

    class Accelerator : public Component
    {
    public:
        virtual ~Accelerator() = default;
    };

    class GPU : public Accelerator
    {
    public:
        virtual ~GPU() = default;
    };

    class GPUCore : public GPU, public Compute
    {
    public:
        virtual ~GPUCore() = default;
    };

    class GPUMemory : public GPU, public Memory
    {
    public:
        virtual ~GPUMemory() = default;
    };

    /***********************************
     * IO-Components
     ***********************************/

    class IO : public Component
    {
    public:
        virtual ~IO() = default;
    };

    class NetworkDevice : public IO
    {
    public:
        virtual ~NetworkDevice() = default;
    };

    /***********************************
     * Link-Components
     ***********************************/

    class Link : public Component
    {
    public:
        virtual ~Link() = default;
    };

    /***********************************
     * Miscellaneous
     ***********************************/

    /**
     * @brief Type for Miscellaneous components, filler class
     *
     */
    class Misc : public Component
    {
    public:
        virtual ~Misc() = default;
    };

    /**
     * @brief Edge type that enables ordering within the graph
     */
    class Parent : public Component
    {
    public:
        virtual ~Parent() = default;
    };

    /**
     * @brief Edge type that enables ordering within the graph
     */
    class Child : public Component
    {
    public:
        virtual ~Child() = default;
    };
#endif
}
