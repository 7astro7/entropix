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
    std::string help_str = R"(
    Usage:
        entropix_cli <path> [options]
    
    Arguments:
        <path>                     File or directory to scan
    
    Options:
        --entropy-threshold, -et <value>
                                   Flag files with entropy above this value (default: 7.9)
        --block-scan, -b <size>    Perform block-wise entropy scan (e.g. 512 for 512B chunks)
        --recursive, -r            Recursively scan subdirectories
        --extension, -e <.ext>     Only include files with the given extension (e.g. .bin)
        --output, -o <file>        Write JSON report to file
        --verbose, -v              Print per-file entropy to stdout
        --help                     Show this message
    )";  
    if (argc < 2 || std::string(argv[1]) == "--help") {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <filename> [options...]\n";
            return 1; 
        }
        std::cout << help_str << std::endl; 
        return 0;
    }
    fs::path input_path = argv[1];
    std::vector<fs::path> files;
    double entropy_threshold = -1.0;
    int block_size = 0;
    bool verbose = false;
    std::string extension;
    bool recursive = false;
    std::string out_path = utils::make_report_filename();

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--entropy-threshold" || arg == "-et") {
            if (i + 1 < argc) { 
                entropy_threshold = std::stod(argv[++i]); 
            }
            else { 
                std::cerr 
                << "Error: --entropy-threshold requires a value.\n"; 
                exit(1); 
            }
        }
        else if (arg == "--block-scan" || arg == "-b") {
            if (i + 1 < argc) {
                block_size = std::stoi(argv[++i]);
            }
            else { 
                std::cerr << "Error: --block-scan requires a value.\n"; 
                exit(1); 
            }
        } else if (arg == "--verbose" || arg == "-v") {
            verbose = true; 
        } else if (arg == "--recursive" || arg == "-r") {
            recursive = true; 
        } else if (arg == "--output" || arg == "-o") {
            if (i + 1 < argc) { 
                out_path = argv[++i];
            }
            else { 
                std::cerr << "Error: --output requires a value.\n"; 
                exit(1); 
            }
        } else if (arg == "--extension" || arg == "-e") {
            if (i + 1 < argc) {
                extension = argv[++i];
            }
        } else if (arg == "--help") { 
            std::cout << help_str << std::endl;
            return 0;
        }
        else {
            std::cerr << "Unknown option: " << arg << "\n";
            exit(1);
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
    if (!extension.empty() && extension[0] != '.')
        extension = "." + extension;

    
    std::ofstream report(out_path);
    if (!report) {
        std::cerr << "Error: cannot open " << out_path << "\n";
        return 1;
    }

    // collect files (honoring --recursive)
    try {
        if (!fs::exists(input_path)) {
            throw std::invalid_argument("File or directory does not exist.");
        }
        else {
            files = utils::collect_files(input_path, recursive, extension);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(1);
    }

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
