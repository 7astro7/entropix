#include "file_reader.hpp"
#include "entropy_calculator.hpp"
#include "block_entropy_scanner.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <utils.hpp>
#include <nlohmann/json.hpp>
#include <chrono> 

namespace fs = std::filesystem;
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 2 || std::string(argv[1]) == "--help") {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <filename> [options...]\n";
            return 1; 
        }
        std::cout << "Usage: " << argv[0]
          << " <filename> "
          << "[--entropy-threshold <value>] "
          << "[--verbose -v] "
          << "[--recursive -r] "
          << "[--output <file>] "
          << "[--block-scan <size>]\n";
        return 0;
    }
    fs::path input_path = argv[1];
    std::vector<fs::path> files;
    double entropy_threshold = -1.0;
    int block_size = 0;
    bool verbose = false;
    bool recursive = false;
    std::string out_path = utils::make_report_filename();

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--entropy-threshold" && i + 1 < argc) {
            entropy_threshold = std::stod(argv[++i]);
        } else if (arg == "--block-scan" || arg == "-b" && i + 1 < argc) {
            block_size = std::stoi(argv[++i]);
        } else if (arg == "--verbose" || arg == "-v") {
            verbose = true; 
        } else if (arg == "--recursive" || arg == "-r") {
            recursive = true; 
        } else if (arg == "--output" || arg == "-o" && i + 1 < argc) {
            out_path = argv[++i];
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return 1;
        }
    }

    if (entropy_threshold < 0.0 || entropy_threshold > 8.0) {
        std::cerr << "Error: Entropy threshold must be in range [0.0, 8.0].\n";
        exit(1);
    }
    if (block_size < 0) {
        std::cerr << "Error: --block-scan must be >= 0.\n";
        return 1;
    }
    
    std::ofstream report(out_path);
    if (!report) {
        std::cerr << "Error: cannot open " << out_path << "\n";
        return 1;
    }

    // gather needed files, recursive or not
    try {
        if (!fs::exists(input_path)) {
            throw std::invalid_argument("File or directory does not exist.");
        }
        else {
            files = utils::collect_files(input_path, recursive);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(1);
    }

    // collect files (honoring --recursive)
    files = utils::collect_files(input_path, recursive);    
    json jresults = json::array();
    // for each file, either block scan or global scan 
    for (const fs::path& path : files) {
        FileReader reader(path.string());
        if (!reader.read_file()) {
            std::cerr << "Error reading file: " << reader.get_error_message() << "\n";
            continue;
        }
    
        const std::vector<uint8_t>& data = reader.get_data();

        json entry; 
        entry["path"] = path.string();
        entry["threshold"] = entropy_threshold;
        
        // block scan mode 
        if (block_size > 0) {
            std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(
                data, 
                block_size, 
                entropy_threshold
            );
            if (results.empty()) continue;
            
            json blocks = json::array();
            for (const auto& [offset, entropy] : results) {
                json block;
                block["offset"] = offset;
                block["entropy"] = entropy;
                blocks.push_back(block);
            }
            entry["type"] = "block";
            entry["blocks"] = std::move(blocks);
            EntropyCalculator calc(data);
            entry["file_entropy"] = calc.get_entropy();        
        }
        else {
            // global scan mode
            double entropy = EntropyCalculator(data).get_entropy();
            if (entropy < entropy_threshold) continue;
            entry["type"] = "global";
            entry["entropy"] = entropy;
        }
        jresults.push_back(entry);
    }
    utils::write_json_output(report, jresults);
    std::cout << "Report written to " << out_path << "\n";

    return 0;
}
