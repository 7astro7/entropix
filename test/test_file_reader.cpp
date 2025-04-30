#include <gtest/gtest.h>
#include "file_reader.hpp"
#include <fstream>


TEST(FileReaderTest, ReadSmallFileSuccessfully) {
    std::string filepath = std::string(SOURCE_DIR) + "/test_data/small.txt";
    FileReader reader(filepath);
    bool read_success = reader.read_file();

    EXPECT_TRUE(read_success);            
    EXPECT_TRUE(reader.is_valid());        
    EXPECT_GT(reader.get_data().size(), 0); 
}

TEST(FileReaderTest, HandleNonexistentFile) {
    FileReader reader("test_data/nonexistent.txt");
    bool read_success = reader.read_file();

    EXPECT_FALSE(read_success);           
    EXPECT_FALSE(reader.is_valid());       
}

TEST(FileReaderTest, GetFileSizeCorrectly) {
    std::string temp_filename = "temp_test_file.txt";
    std::ofstream outfile(temp_filename, std::ios::binary);
    outfile << "this is a test";
    outfile.close();

    FileReader reader(temp_filename);
    bool read_success = reader.read_file();

    EXPECT_TRUE(read_success);
    EXPECT_TRUE(reader.is_valid());

    EXPECT_EQ(reader.get_data().size(), 14);

    std::remove(temp_filename.c_str());
}

TEST(FileReaderTest, GetErrorMessageOnSuccess) {
    std::string temp_filename = "temp_test_file.txt";
    std::ofstream outfile(temp_filename, std::ios::binary);
    outfile << "this is a test";
    outfile.close();

    FileReader reader(temp_filename);
    bool read_success = reader.read_file();

    EXPECT_TRUE(read_success);
    EXPECT_TRUE(reader.is_valid());
    EXPECT_EQ(reader.get_error_message(), "");  // Should be empty on success

    std::remove(temp_filename.c_str());
}

TEST(FileReaderTest, GetErrorMessageOnFailure) {
    FileReader reader("nonexistent_file.txt");
    bool read_success = reader.read_file();

    EXPECT_FALSE(read_success);
    EXPECT_FALSE(reader.is_valid());
    EXPECT_FALSE(reader.get_error_message().empty());  // Should not be empty on failure
}
