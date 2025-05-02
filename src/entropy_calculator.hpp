#ifndef ENTROPY_CALCULATOR_HPP
#define ENTROPY_CALCULATOR_HPP

#include <vector>
#include <array>
#include <cstdint>
#include <string>

class EntropyCalculator {
public:

    EntropyCalculator(const std::vector<unsigned char>& data);

    // Calculate Shannon entropy in bits per byte (0.0 - 8.0)
    void calculate_entropy();

    double get_entropy() const;

    double estimate_compression_ratio() const;    

    // Return byte frequency histogram (index = byte value 0-255)
    const std::array<size_t, 256>& get_histogram() const;

    // Get total number of bytes processed
    size_t get_total_bytes() const;

private:
    std::array<size_t, 256> byte_freq_;  // Byte frequencies
    size_t total_bytes_;                 // Total number of bytes
    std::vector<unsigned char> data_; 
    mutable double entropy_ = -1.0;
    std::size_t compute_total_bytes() const;
};

#endif // ENTROPY_CALCULATOR_HPP

