#include "block_entropy_scanner.hpp"
#include <gtest/gtest.h>

TEST(BlockEntropyScannerTest, SingleUniformBlockIsLowEntropy) {
    // test that a block of identical bytes has low entropy
    std::vector<unsigned char> data(512, 'A');
    std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(data, 512);
    ASSERT_EQ(results.size(), 1);
    EXPECT_LT(results[0].second, 0.01);  // Expect very low entropy
    EXPECT_EQ(results[0].first, 0);
}

TEST(BlockEntropyScannerTest, SingleRandomBlockIsHighEntropy) {
    // test that a block of random bytes has high entropy
    std::vector<unsigned char> data;
    for (int i = 0; i < 256; ++i) {
        data.push_back(static_cast<unsigned char>(i));
        data.push_back(static_cast<unsigned char>((i + 137) % 256));
    }
    std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(data, 512);
    ASSERT_EQ(results.size(), 1);
    EXPECT_GT(results[0].second, 7.5);
    EXPECT_EQ(results[0].first, 0);
}

TEST(BlockEntropyScannerTest, FiltersOutLowEntropyBlocks) {
    // test that blocks below the threshold are not included
    std::vector<unsigned char> data(1024, 'B');
    std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(data, 512, 0.5);
    EXPECT_TRUE(results.empty());
}

TEST(BlockEntropyScannerTest, OnlyIncludesBlocksMeetingThreshold) {
    // test that only blocks at or above the threshold are included
    std::vector<unsigned char> data;
    data.insert(data.end(), 512, 'A');  // low entropy
    for (int i = 0; i < 256; ++i) {
        data.push_back(static_cast<unsigned char>(i));
        data.push_back(static_cast<unsigned char>((i + 53) % 256));
    } // high entropy

    std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(data, 512, 7.0);
    ASSERT_EQ(results.size(), 1);
    EXPECT_GE(results[0].second, 7.0);
    EXPECT_EQ(results[0].first, 512);
}

TEST(BlockEntropyScannerTest, EmptyData) {
    // test that empty data returns an empty result
    std::vector<unsigned char> data;
    EXPECT_THROW(BlockEntropyScanner::scan(data, 512, 0.0), std::invalid_argument);
}

TEST(BlockEntropyScannerTest, BlockSizeLargerThanData) {
    // test that if the block size is larger than the data, it returns the entropy of the whole data
    std::vector<unsigned char> data(100, 'A');
    std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(data, 512, 0.0);
    ASSERT_EQ(results.size(), 1);
    EXPECT_NEAR(results[0].second, 0.0, 0.01);
}

TEST(BlockEntropyScannerTest, AllBlocksBelowThreshold) {
    // test that no blocks are returned if all are below the threshold
    std::vector<unsigned char> data(1024, 'A');
    std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(data, 512, 0.5);
    EXPECT_TRUE(results.empty());
}

TEST(BlockEntropyScannerTest, SomeBlocksAboveThreshold) {
    // test that some blocks are returned if they meet the threshold
    std::vector<unsigned char> data(1024, 'A');
    data[100] = 'Z'; // Make one block slightly more entropic
    std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(data, 512, 0.01);
    ASSERT_EQ(results.size(), 1);
    EXPECT_GT(results[0].second, 0.01);
}

TEST(BlockEntropyScannerTest, EntropyThresholdEdge) {
    // test that blocks with entropy exactly equal to the threshold are included
    std::vector<unsigned char> data = {'A', 'A', 'A', 'B'}; // entropy ~0.8113
    std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(data, 4, 0.811278);
    ASSERT_EQ(results.size(), 1);
    EXPECT_NEAR(results[0].second, 0.811278, 0.01);
}

TEST(BlockEntropyScannerTest, MultipleBlocksDifferentEntropy) {
    // test that multiple blocks with different entropy are handled correctly
    std::vector<unsigned char> data;
    data.insert(data.end(), 512, 'A'); // Low entropy block
    for (int i = 0; i < 512; ++i) data.push_back(static_cast<unsigned char>(i % 256)); // High entropy block

    std::vector<std::pair<size_t, double>> results = BlockEntropyScanner::scan(data, 512, 1.0);
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].first, 512);
    EXPECT_GT(results[0].second, 1.0);
}
