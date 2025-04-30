#include <gtest/gtest.h>
#include "entropy_calculator.hpp"  
#include <vector>

TEST(EntropyCalculatorTest, ZeroEntropyForUniformData) {
    std::vector<unsigned char> data(1000, 'A');  
    EntropyCalculator calculator(data);
    double entropy = calculator.calculate_entropy();

    EXPECT_NEAR(entropy, 0.0, 0.01);  
}

TEST(EntropyCalculatorTest, HighEntropyForRandomData) {
    std::vector<unsigned char> data;
    for (int i = 0; i < 256; ++i) {
        data.push_back(static_cast<unsigned char>(i));  // 0, 1, 2, ..., 255
    }
    EntropyCalculator calculator(data);
    double entropy = calculator.calculate_entropy();

    EXPECT_GT(entropy, 7.5);  // Entropy should be high (> 7.5 bits per byte)
}

TEST(EntropyCalculatorTest, GetTotalBytes) {
    std::vector<unsigned char> data = {
        'A', 'B', 'C', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
        'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q'
    };  

    EntropyCalculator calculator(data);
    EXPECT_EQ(calculator.get_total_bytes(), 20);
}
