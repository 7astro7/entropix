#pragma once
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;
namespace utils {
    std::vector<fs::path> collect_files(const fs::path& root, bool recursive); 
}