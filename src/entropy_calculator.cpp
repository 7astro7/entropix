#include "entropy_calculator.hpp"
#include <cmath>
#include <iostream> 

EntropyCalculator::EntropyCalculator(const std::vector<unsigned char>& data)
    : data_(data), total_bytes_(data.size()) {

    byte_freq_.fill(0);  // zero initialize just in case

    for (unsigned char byte : data_) {
        byte_freq_[byte]++;
    }
}

void EntropyCalculator::calculate_entropy() {
    if (data_.empty()) 
        entropy_ = 0.0;  // No data, no entropy
    double entropy = 0.0;
    for (const auto& freq : byte_freq_) {
        if (freq > 0) {
            double probability = static_cast<double>(freq) / total_bytes_;
            entropy -= probability * log2(probability);
        }
    }
    entropy_ = entropy;
}

size_t EntropyCalculator::get_total_bytes() const {
    return total_bytes_;
}

double EntropyCalculator::estimate_compression_ratio() const {
    double entropy = get_entropy();
    return 1.0 - (entropy / 8.0);  // naive lower bound estimate
}

double EntropyCalculator::get_entropy() const {
    if (entropy_ < 0.0) {
        const_cast<EntropyCalculator*>(this)->calculate_entropy(); 
    }
    return entropy_;
}

const std::array<size_t, 256>& EntropyCalculator::get_histogram() const {
    return byte_freq_;
}