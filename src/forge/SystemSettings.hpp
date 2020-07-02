#ifndef FORGE_SYSTEM_SETTINGS_HPP
#define FORGE_SYSTEM_SETTINGS_HPP

#include <libnabla/VSysSettings.hpp>

namespace FORGE
{
namespace SETTINGS
{

    //! \brief Maximum memory dictated by the VSYS
    static constexpr int MAX_GLOBAL_MEMORY = NABLA::VSYS::NABLA_VSYS_SETTINGS_GLOBAL_MEMORY_BYTES;

    //  Settings for reserved spaces in the global stack
    //
    //
    static constexpr int GS_FRAME_OFFSET_RESERVE  = 1;
    static constexpr int GS_FUNC_PARAM_RESERVE    = 10;
    static constexpr int GS_RETURN_RESERVE        = 1;

    //  To avoid magic numbers, we say why use 8
    //
    static constexpr int SYSTEM_WORD_SIZE_BYTES   = 8;

    //  The index into GS where program data will start 
    //
    static constexpr int GS_INDEX_PROGRAM_SPACE   = SYSTEM_WORD_SIZE_BYTES * (GS_FRAME_OFFSET_RESERVE + GS_FUNC_PARAM_RESERVE + GS_RETURN_RESERVE );

    static constexpr int GS_INDEX_RETURN_SPACE    = SYSTEM_WORD_SIZE_BYTES * (GS_FRAME_OFFSET_RESERVE + GS_FUNC_PARAM_RESERVE); 
}
}

#endif