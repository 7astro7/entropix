#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <string>
#include <vector>
#include <cstdint>

class FileReader {
public:
    explicit FileReader(const std::string& filepath);

    // Reads the entire file into memory
    bool read_file();

    const std::vector<uint8_t>& get_data() const;

    // Returns the file size in bytes
    size_t get_file_size() const;

    bool is_valid() const;

    // Returns last error message, if any
    const std::string& get_error_message() const;

private:
    std::string filepath_;
    std::vector<uint8_t> data_;
    size_t file_size_;
    bool valid_;
    std::string error_message_;
};

#endif 