#ifndef LTMATH_HPP_
#define LTMATH_HPP_

#include <cmath>
#include <array>
#include <mutex>

/**
 * @brief Utility namespace for trigonometric functions using a lookup table.
 */
namespace LookupTableMath {
    static constexpr int TABLE_SIZE = 40;

    static std::array<float, TABLE_SIZE> sin_table;
    static std::array<float, TABLE_SIZE> cos_table;
    static std::once_flag init_flag;

    /**
     * @brief Initialize the lookup table.
     */
    static void ltinit() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            sin_table[i] = std::sinf(i * 2 * M_PI / TABLE_SIZE);
            cos_table[i] = std::cosf(i * 2 * M_PI / TABLE_SIZE);
        }
    }

    /**
     * @brief Get the sine of an angle in radians.
     * @param rad The angle in radians.
     * @return The sine of the angle.
     */
    static float ltsinf(float rad) {
        std::call_once(init_flag, ltinit);
        rad = std::fmod(rad, 2 * M_PI);
        int index = static_cast<int>(rad * TABLE_SIZE / (2 * M_PI)) % TABLE_SIZE;
        if (index < 0)
            index += TABLE_SIZE;
        return sin_table[index];
    }

    /**
     * @brief Get the sine of an angle in radians, with a range of [0, 2 * PI).
     * @param rad The angle in radians.
     * @return The sine of the angle.
     * @warning This function will return wrong values for angles outside the range [0, 2 * PI).
     */
    static float ltsinf_q(float rad) {
        std::call_once(init_flag, ltinit);
        return sin_table[static_cast<int>(rad * TABLE_SIZE / (2 * M_PI))];
    }

    /**
     * @brief Get the cosine of an angle in radians.
     * @param rad The angle in radians.
     * @return The cosine of the angle.
     */
    static float ltcosf(float rad) {
        std::call_once(init_flag, ltinit);
        rad = std::fmod(rad, 2 * M_PI);
        int index = static_cast<int>(rad * TABLE_SIZE / (2 * M_PI)) % TABLE_SIZE;
        if (index < 0)
            index += TABLE_SIZE;
        return cos_table[index];
    }

    /**
     * @brief Get the cosine of an angle in radians, with a range of [0, 2 * PI).
     * @param rad The angle in radians.
     * @return The cosine of the angle.
     * @warning This function will return wrong values for angles outside the range [0, 2 * PI).
     */
    static float ltcosf_q(float rad) {
        std::call_once(init_flag, ltinit);
        return cos_table[static_cast<int>(rad * TABLE_SIZE / (2 * M_PI))];
    }
};

#endif
