#include "Memory.hpp"

#include "SystemSettings.hpp"

#include <iostream>

namespace FORGE
{
    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Memory::Memory() : currently_allocated_bytes(0)
    {

    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Memory::~Memory()
    {

    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    bool Memory::is_id_mapped(std::string id)
    {
        if(memory_map.find(id) != memory_map.end())
        {
            return true;
        }
        return false;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Memory::MemAlloc Memory::get_mem_info(std::string id)
    {
        if(memory_map.find(id) != memory_map.end())
        {
            return memory_map[id];
        }

        return MemAlloc();
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    uint64_t Memory::get_currently_allocated_bytes_amnt() const
    {
        return currently_allocated_bytes;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Memory::reset()
    {
        memory_map.clear();
        currently_allocated_bytes = 0;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Memory::remove_item(std::string id)
    {
        if(is_id_mapped(id))
        {
            memory_map.erase(memory_map.find(id));
        }
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    bool Memory::alloc_mem(std::string id)
    {
        if((currently_allocated_bytes + SETTINGS::SYSTEM_WORD_SIZE_BYTES) > SETTINGS::MAX_GLOBAL_MEMORY )
        {
            return false;
        }

        MemAlloc allocated; 

        allocated.start_pos = currently_allocated_bytes;

        currently_allocated_bytes += SETTINGS::SYSTEM_WORD_SIZE_BYTES;

        memory_map[id] = allocated;

        return true;
    }


}