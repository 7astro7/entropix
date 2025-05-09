#pragma once
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
namespace utils {

    /**
     * @brief Collects files from a directory, or the file itself, optionally
     *       recursively.
     *
     * @param root      the file or directory to scan
     * @param recursive if true, descend into subdirs
     * @throws std::invalid_argument if `root` doesn’t exist or isn’t a file/dir
     * @returns a flat vector of all regular files found
     */
    std::vector<fs::path> collect_files(const fs::path& root, bool recursive, const std::string& extension = ""); 
    
    /*
    @brief Creates a JSON output report filename from prefix, with timestamp
    @param prefix the prefix for the report filename
    @returns a string: the report filename 
    */
    std::string make_report_filename(const std::string& prefix = "entropix-report");
    
    /*
    @brief Writes JSON output to a stream
    @param out the output stream to write to
    @param data the JSON data to write
    @returns void
    */
    void write_json_output(std::ostream& out, const nlohmann::json& data);
}