#ifndef ENTROPY_CALCULATOR_HPP
#define ENTROPY_CALCULATOR_HPP

#include <vector>
#include <array>
#include <cstdint>
#include <string>

/**
 * @class EntropyCalculator
 * @brief Computes Shannon entropy and related statistics for a given byte sequence.
 *
 * This class provides methods for analyzing raw binary data to compute its Shannon entropy,
 * estimate its theoretical compression potential, and inspect the byte frequency distribution.
 *
 * Designed for use in applications such as digital forensics, data classification,
 * or compression analysis.
 *
 * After construction, call calculate_entropy() to perform the analysis before
 * querying results such as entropy value or histogram.
 */
class EntropyCalculator {
public:

    /**
     * @brief Constructs an EntropyCalculator with the provided data.
     *
     * Initializes the entropy calculator using a vector of raw byte values.
     * The data is stored internally for entropy analysis.
     *
     * @param data A vector of unsigned bytes representing the input data to analyze.
     */
    EntropyCalculator(const std::vector<unsigned char>& data);

    /**
     * @brief Calculates the Shannon entropy of the input data.
     *
     * Analyzes the frequency of each byte value (0–255) in the input data
     * and computes the Shannon entropy in bits per byte.
     * 
     * The result is cached internally and can be retrieved using get_entropy().
     */
    void calculate_entropy();

    /**
     * @brief Returns the Shannon entropy of the input data.
     *
     * This value is computed by calculate_entropy() and represents the
     * average number of bits per byte required to encode the input data.
     * 
     * @return The entropy value in bits per byte. Returns -1.0 if entropy
     *         has not been calculated yet.
     */
    double get_entropy();

    /**
     * @brief Estimates the fraction of the input data that could be compressed.
     *
     * Uses the Shannon entropy to estimate the proportion of redundant information
     * in the input data. The value ranges from 0.0 to 1.0, where higher values
     * indicate greater potential compressibility.
     * 
     * Computed as: 1.0 - (entropy / 8.0), assuming 8 bits per byte.
     *
     * @return A value between 0.0 and 1.0 representing the estimated fraction
     *         of compressible data. Returns 0.0 if entropy is 8.0 (maximum entropy).
     */
    double estimate_compression_ratio();    

    /**
     * @brief Returns the byte frequency histogram of the input data.
     *
     * Provides an array where each index (0–255) corresponds to a byte value,
     * and the value at that index represents how many times that byte appears
     * in the input data.
     * 
     * The histogram is computed during calculate_entropy().
     *
     * @return A reference to an array of 256 elements representing byte frequencies.
     */
    const std::array<size_t, 256>& get_histogram() const;

    /**
     * @brief Returns the total number of bytes in the input data.
     *
     * This value corresponds to the size of the input data passed to
     * the constructor and used during entropy analysis.
     *
     * @return The total number of bytes in the input data.
     */
    size_t get_total_bytes() const;

private:
    std::array<size_t, 256> byte_freq_;  // Byte frequencies
    size_t total_bytes_;                 // Total number of bytes
    std::vector<unsigned char> data_; 
    mutable double entropy_ = -1.0;
    std::size_t compute_total_bytes() const;
};

#endif // ENTROPY_CALCULATOR_HPP

