/*
 * This allows to hash a string at compile time.
 * DJB2 algorithm is used for hashing.
 *
 * Inspired by Jerry Coffin's post
 * (https://stackoverflow.com/a/2112111/22294352)
*/

#pragma once

#include <cstdint>

namespace util
{
namespace str
{
    
    constexpr uint64_t HashString(const char *input)
    {
        uint64_t hash = 5381;
        while (*input)
        {
            hash = static_cast<uint64_t>(*input++) + 33 * hash;
        }
        return hash;
    }
}
}
