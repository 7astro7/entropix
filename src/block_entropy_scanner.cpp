#include "block_entropy_scanner.hpp"
#include "entropy_calculator.hpp"
#include <algorithm>
#include <stdexcept>

std::vector<std::pair<size_t, double>> BlockEntropyScanner::scan(
    const std::vector<unsigned char>& data,
    size_t block_size,
    double min_entropy
) {
    if (data.empty()) {
        throw std::invalid_argument("Data must not be empty");
    }    
    if (block_size <= 0) {
        throw std::invalid_argument("Block size must be positive.");
    }
    if (min_entropy < 0.0 || min_entropy > 8.0) {
        throw std::invalid_argument("Entropy threshold must be in range [0.0, 8.0].");
    }    
    std::vector<std::pair<size_t, double>> results;
    size_t offset = 0;

    while (offset < data.size()) {
        size_t end = std::min(offset + block_size, data.size());
        std::vector<unsigned char> block(data.begin() + offset, data.begin() + end);

        EntropyCalculator calc(block);
        double entropy = calc.get_entropy();

        if (entropy >= min_entropy) {
            results.emplace_back(offset, entropy);
        }

        offset += block_size;
    }

    return results;
}
