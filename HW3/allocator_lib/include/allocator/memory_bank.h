#pragma once

#include <cinttypes>

enum class MemoryBank : std::uint8_t
{
    General,
    Audio,
    Video
};