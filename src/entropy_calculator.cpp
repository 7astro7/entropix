#include "entropy_calculator.hpp"
#include <cmath>

double EntropyCalculator::calculate_entropy() const {
    double entropy = 0.0;
    for (const auto& freq : byte_freq_) {
        if (freq > 0) {
            double probability = static_cast<double>(freq) / total_bytes_;
            entropy -= probability * log2(probability);
        }
    }
    return entropy;
}

size_t EntropyCalculator::get_total_bytes() const {
    return total_bytes_;
}
