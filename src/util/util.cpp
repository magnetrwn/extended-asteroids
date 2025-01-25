#include "util.hpp"

/*std::string util::abs_dir() {
    char buffer[PATH_BUFFER_SIZE];
    ssize_t pathLength = readlink("/proc/self/exe", buffer, PATH_BUFFER_SIZE - 1);

    if (pathLength == -1)
        throw std::runtime_error("Error with util::abs_dir().");

    std::string path(buffer, pathLength);

    return path.substr(0, path.rfind('/') + 1);
}*/

std::string util::cfg_string(const std::string& section, const std::string& key) {
    inipp::Ini<char> ini;
    std::ifstream is(SETTINGS_FILE);
    ini.parse(is);

    std::string value;
    inipp::get_value(ini.sections[section], key, value);

    if (value.empty())
        throw std::runtime_error("util::cfg_string cannot parse section \"" + section + "\", key \"" + key + "\": value is empty or key absent.");

    return value;
}

usize util::cfg_usize(const std::string& section, const std::string& key) {
    std::string cfgStr = cfg_string(section, key);

    if (cfgStr[0] == '-')
        throw std::runtime_error("util::cfg_usize cannot parse section \"" + section + "\", key \"" + key + "\": value is negative.");

    try {
        return std::stoul(cfgStr);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("util::cfg_usize cannot parse section \"" + section + "\", key \"" + key + "\": value is not a number.");
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("util::cfg_usize cannot parse section \"" + section + "\", key \"" + key + "\": value is out of range.");
    }
}

f32 util::cfg_f32(const std::string& section, const std::string& key) {
    try {
        return std::stof(cfg_string(section, key));
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("util::cfg_f32 cannot parse section \"" + section + "\", key \"" + key + "\": value is not a number.");
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("util::cfg_f32 cannot parse section \"" + section + "\", key \"" + key + "\": value is out of range.");
    }
}

bool util::cfg_bool(const std::string& section, const std::string& key) {
    std::string value = cfg_string(section, key);
    if (value == "true")
        return true;
    if (value == "false")
        return false;
    
    throw std::runtime_error("util::cfg_bool cannot parse section \"" + section + "\", key \"" + key + "\": value is neither of \"true\", \"false\".");
}