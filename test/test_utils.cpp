#include <gtest/gtest.h>
#include "utils.hpp"
#include <filesystem>

namespace fs = std::filesystem;

TEST(Utils, NonRecursive) {
    fs::path nonrec_dir = fs::path(SOURCE_DIR) / "test"/ "data" / "nonrec_dir";
    std::vector<fs::path> files = utils::collect_files(nonrec_dir, false);
    // e.g. expect only "a.txt" and "b.bin" in the root
    EXPECT_EQ(files.size(), 2);
}

TEST(Utils, Recursive) {
    fs::path rec_dir = fs::path(SOURCE_DIR) / "test"/ "data" / "rec_dir";
    std::vector<fs::path> files = utils::collect_files(rec_dir, true);
    // expect files from subdirectories too
    EXPECT_GT(files.size(), 2);
}

