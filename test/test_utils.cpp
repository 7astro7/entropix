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

// add tests for new utils functions here
TEST(UtilsTest, ReportFilenameHasExpectedFormat) {
    std::string name = utils::make_report_filename("testreport");
    EXPECT_EQ(name.compare(0, 11, "testreport-"), 0);
    EXPECT_TRUE(name.ends_with(".json"));            // ends with .json
    EXPECT_GT(name.size(), std::string("testreport_YYYYMMDD-HHMMSS.json").size() - 4);
}

TEST(UtilsTest, WriteJsonOutputProducesCorrectJson) {
    nlohmann::json j;
    j["filename"] = "abc.txt";
    j["file_entropy"] = 5.25;

    std::ostringstream oss;
    utils::write_json_output(oss, j);

    nlohmann::json parsed = nlohmann::json::parse(oss.str());
    EXPECT_EQ(parsed["filename"], "abc.txt");
    EXPECT_DOUBLE_EQ(parsed["file_entropy"], 5.25);
}
