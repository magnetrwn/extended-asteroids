#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <cstddef>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <unistd.h>
#include <raylib.h>
#include <inipp.h>

#include "typedef.hpp"

class util {
public:
    //static std::string abs_dir();
    static std::string cfg_string(const std::string& section, const std::string& key);
    static usize cfg_usize(const std::string& section, const std::string& key);
    static f32 cfg_f32(const std::string& section, const std::string& key);
    static bool cfg_bool(const std::string& section, const std::string& key);

    static inline f32 clamp_lh(f32& value, const f32 lowLimit, const f32 highLimit) {
        f32 original = value;
        value = std::max(lowLimit, std::min(value, highLimit));
        return original - value;
    }

    static inline f32 clamp_l(f32& value, const f32 lowLimit) {
        f32 original = value;
        value = std::max(value, lowLimit);
        return original - value;
    }

    static inline f32 clamp_h(f32& value, const f32 highLimit) {
        f32 original = value;
        value = std::min(value, highLimit);
        return original - value;
    }

    static inline f32 randf() {
        return static_cast<f32>(GetRandomValue(0, RAND_MAX)) / static_cast<f32>(RAND_MAX);
    }

    static inline int randi(const int low, const int high) {
        return GetRandomValue(low, high);
    }
};

#endif