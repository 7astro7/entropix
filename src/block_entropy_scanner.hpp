#ifndef BLOCK_ENTROPY_SCANNER_HPP
#define BLOCK_ENTROPY_SCANNER_HPP

#include <vector>
#include <cstddef>
#include <utility>

class BlockEntropyScanner {
public:
    // Returns a vector of (offset, entropy) pairs for each block in the data
    // Only includes blocks with entropy >= min_entropy
    static std::vector<std::pair<size_t, double>> scan(
        const std::vector<unsigned char>& data,
        size_t block_size,
        double min_entropy = 0.0
    );
};

#endif // BLOCK_ENTROPY_SCANNER_HPP
