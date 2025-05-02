#include <gtest/gtest.h>
#include "entropy_calculator.hpp"  
#include <vector>
#include <random> 
#include <algorithm> 

TEST(EntropyCalculatorTest, ZeroEntropyForUniformData) {
    std::vector<unsigned char> data(1000, 'A');  
    EntropyCalculator calculator(data);
    double entropy = calculator.get_entropy();
    EXPECT_NEAR(entropy, 0.0, 0.01);  
}

TEST(EntropyCalculatorTest, HighEntropyForRandomData) {
    std::vector<unsigned char> data;
    for (int i = 0; i < 256; ++i) {
        data.push_back(static_cast<unsigned char>(i));  // 0, 1, 2, ..., 255
    }
    EntropyCalculator calculator(data);
    double entropy = calculator.get_entropy();
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

TEST(EntropyCalculatorTest, GetHistogramCountsCorrectly) {
    std::vector<unsigned char> data = {'A', 'B', 'A', 'C'};  // ASCII 65, 66, 65, 67
    EntropyCalculator calc(data);
    const auto& hist = calc.get_histogram();

    EXPECT_EQ(hist[65], 2);  // 'A'
    EXPECT_EQ(hist[66], 1);  // 'B'
    EXPECT_EQ(hist[67], 1);  // 'C'

    // Spot-check a few others for zero
    EXPECT_EQ(hist[68], 0);  // 'D'
    EXPECT_EQ(hist[0], 0);   // Null byte
}
TEST(EntropyCalculatorTest, EstimateCompressionRatio) {
    std::vector<unsigned char> data = {'A', 'B', 'C', 'D', 'E'};
    EntropyCalculator calculator(data);
    double compression_ratio = calculator.estimate_compression_ratio();
    EXPECT_GT(compression_ratio, 0.0);  
}
TEST(EntropyCalculatorTest, EmptyData) {
    std::vector<unsigned char> data;
    EntropyCalculator calculator(data);
    EXPECT_EQ(calculator.get_entropy(), 0.0);  
}

TEST(EntropyCalculatorTest, EntropyOfFiveUniqueBytes) {
    std::vector<unsigned char> data = {'1', '2', '3', '4', '5'};
    EntropyCalculator calc(data);
    EXPECT_NEAR(calc.get_entropy(), std::log2(5.0), 0.01);
}

//     Mostly repeated data + one oddball (e.g. 99 'A's and 1 'Z')

TEST(EntropyCalculatorTest, MostlyRepeatedData) {
    std::vector<unsigned char> data(100, 'A');  
    data[99] = 'Z';  // One oddball
    EntropyCalculator calculator(data);
    double entropy = calculator.get_entropy();
    EXPECT_NEAR(entropy, 0.0803, 0.001);
}

