#pragma once
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
namespace utils {
    std::vector<fs::path> collect_files(const fs::path& root, bool recursive); 
    
    std::string make_report_filename(const std::string& prefix = "entropix-report");
    
    void write_json_output(std::ostream& out, const nlohmann::json& data);
}