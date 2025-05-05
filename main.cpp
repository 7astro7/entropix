#include "file_reader.hpp"
#include "entropy_calculator.hpp"
#include "block_entropy_scanner.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <utils.hpp>

namespace fs = std::filesystem;

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
          << "[--block-scan <size>]\n";
        return 0;
    }
    fs::path input_path = argv[1];
    std::vector<fs::path> files;
    double entropy_threshold = -1.0;
    int block_size = 0;
    bool verbose = false;
    bool recursive = false;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--entropy-threshold" && i + 1 < argc) {
            entropy_threshold = std::stod(argv[++i]);
        } else if (arg == "--block-scan" && i + 1 < argc) {
            block_size = std::stoi(argv[++i]);
        } else if (arg == "--verbose" || arg == "-v") {
            verbose = true; 
        } else if (arg == "--recursive" || arg == "-r") {
            recursive = true; 
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return 1;
        }
    }

    if (entropy_threshold < 0.0 || entropy_threshold > 8.0) {
        std::cerr << "Error: Entropy threshold must be in range [0.0, 8.0].\n";
        exit(1);
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
    
    size_t count_high_entropy = 0;
    // for each file, either block scan or global scan 
    for (const fs::path& path : files) {
        FileReader reader(path.string());
        if (!reader.read_file()) {
            std::cerr << "Error reading file: " << reader.get_error_message() << "\n";
            continue;
        }
    
        const std::vector<uint8_t>& data = reader.get_data();
        
        // block scan mode 
        if (block_size > 0) {
            std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(
                data, 
                block_size, 
                entropy_threshold
            );
            for (const std::pair<size_t, double>& pair : results) {
                std::cout
                << path 
                << "  Offset 0x"
                << std::hex << std::setw(8) << std::setfill('0') << pair.first
                << std::dec    // back to decimal
                << ": "
                << std::fixed << std::setprecision(4) << pair.second
                << " bits/byte\n";    
            }
            continue; // Skip global entropy
        }
        
        // global scan mode
        EntropyCalculator calculator(data);
        double entropy = calculator.get_entropy();

        if (entropy_threshold > 0.0 && entropy >= entropy_threshold) {
            std::cout << path << " [!] High entropy detected: " << entropy 
                << " bits/byte — possible encryption or packing\n";
            count_high_entropy++;
        } else if (verbose) {
            std::cout << path << "Entropy: " << entropy << " bits/byte";
            if (entropy_threshold > 0.0) {
                std::cout << " (below threshold " << entropy_threshold << ")";
            }
            std::cout << "\n";
        }
    }

    if (count_high_entropy > 0) {
        std::cout << "Found " << count_high_entropy 
            << " files with entropy above threshold.\n";
    } else if (verbose) {
        std::cout << "No files found with entropy above threshold.\n";
    }

    return 0;
}
