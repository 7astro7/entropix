#include "file_reader.hpp"
#include "entropy_calculator.hpp"
#include "block_entropy_scanner.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc < 2 || std::string(argv[1]) == "--help") {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <filename> [options...]\n";
            return 1; 
        }
        std::cout << "Usage: " << argv[0]
          << " <filename> "
          << "[--entropy-threshold <value>] "
          << "[--block-scan <size>]\n";
        return 0;
    }

    std::string filename = argv[1];
    double entropy_threshold = -1.0;
    int block_size = 0;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--entropy-threshold" && i + 1 < argc) {
            entropy_threshold = std::stod(argv[++i]);
        } else if (arg == "--block-scan" && i + 1 < argc) {
            block_size = std::stoi(argv[++i]);
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return 1;
        }
    }

    FileReader reader(filename);
    if (!reader.read_file()) {
        std::cerr << "Error reading file: " << reader.get_error_message() << "\n";
        return 1;
    }

    const std::vector<uint8_t>& data = reader.get_data();

    if (block_size > 0) {
        std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(
            data, 
            block_size, 
            entropy_threshold
        );
    
        if (results.empty()) {
            std::cout << "All blocks below threshold " << entropy_threshold << "\n";
        } else {
            for (const std::pair<size_t, double>& result : results) {
                size_t offset = result.first;
                double entropy = result.second;
                std::cout << "[!] High entropy detected: " << entropy
                          << " bits/byte at offset 0x" << std::hex << offset
                          << " (threshold: " << entropy_threshold << ")\n";
            }
        }    
        return 0;
    }

    EntropyCalculator calculator(data);
    double entropy = calculator.get_entropy();

    if (entropy_threshold > 0.0 && entropy >= entropy_threshold) {
        std::cout << "[!] High entropy detected: " << entropy 
            << " bits/byte — possible encryption or packing\n";
    } else {
        std::cout << "Entropy: " << entropy << " bits/byte";
        if (entropy_threshold > 0.0) {
            std::cout << " (below threshold " << entropy_threshold << ")";
        }
        std::cout << "\n";
    }

    return 0;
}
