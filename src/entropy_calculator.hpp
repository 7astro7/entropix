#ifndef ENTROPY_CALCULATOR_HPP
#define ENTROPY_CALCULATOR_HPP

#include <vector>
#include <array>
#include <cstdint>
#include <string>

class EntropyCalculator {
public:
    // Constructor: pass in raw byte data
    EntropyCalculator(const std::vector<uint8_t>& data);

    // Calculate Shannon entropy in bits per byte (0.0 - 8.0)
    double calculate_entropy() const;

    // Estimate compressibility based on entropy
    std::string estimate_compressibility() const;

    // Return byte frequency histogram (index = byte value 0-255)
    const std::array<size_t, 256>& get_histogram() const;

    // Get total number of bytes processed
    size_t get_total_bytes() const;

private:
    std::array<size_t, 256> byte_freq_;  // Byte frequencies
    size_t total_bytes_;                 // Total number of bytes

    std::size_t compute_total_bytes() const;
};

#endif // ENTROPY_CALCULATOR_HPP

