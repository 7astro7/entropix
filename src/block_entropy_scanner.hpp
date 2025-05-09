#ifndef BLOCK_ENTROPY_SCANNER_HPP
#define BLOCK_ENTROPY_SCANNER_HPP

#include <vector>
#include <cstddef>
#include <utility>

/**
 * @class BlockEntropyScanner
 * @brief Provides functionality to compute entropy on fixed-size blocks of input data.
 *
 * This class offers a static method to scan raw byte data in non-overlapping blocks
 * and compute the Shannon entropy for each block. It is useful for detecting
 * high-entropy regions within binary files, such as encrypted or compressed segments.
 *
 * The scanner can be configured to include only blocks whose entropy exceeds
 * a given threshold.
 */
class BlockEntropyScanner {
public:

    /**
     * @brief Scans the input data in fixed-size blocks and computes entropy for each block.
     *
     * Processes the input data in non-overlapping blocks of the specified size,
     * computes the Shannon entropy for each block, and returns a vector of
     * (offset, entropy) pairs for blocks whose entropy is greater than or equal
     * to the specified minimum threshold.
     *
     * @param data The input byte data to scan.
     * @param block_size The size (in bytes) of each block to analyze.
     * @param min_entropy The minimum entropy threshold for a block to be included in the result.
     *                    Defaults to 0.0, which includes all blocks.
     *
     * @return A vector of (offset, entropy) pairs for each qualifying block.
     *         Offset is the starting byte index of the block within the input data.
     */
    static std::vector<std::pair<size_t, double>> scan(
        const std::vector<unsigned char>& data,
        size_t block_size,
        double min_entropy = 0.0
    );
};

#endif // BLOCK_ENTROPY_SCANNER_HPP
