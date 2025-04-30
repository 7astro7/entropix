#include "file_reader.hpp"
#include <fstream>
#include <iterator>

FileReader::FileReader(const std::string& filepath)
    : filepath_(filepath), file_size_(0), valid_(false), error_message_("") {}

bool FileReader::read_file() {
    std::ifstream file(filepath_, std::ios::binary);
    if (!file.is_open()) {
        valid_ = false;
        error_message_ = "Failed to open file: " + filepath_; 
        return valid_;
    }

    data_ = std::vector<unsigned char>(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    valid_ = true;
    error_message_.clear(); 
    return valid_; 
}

const std::vector<uint8_t>& FileReader::get_data() const {
    return data_;
}

size_t FileReader::get_file_size() const {
    return file_size_;
}

bool FileReader::is_valid() const {
    return valid_;
}

const std::string& FileReader::get_error_message() const {
    return error_message_;
}

