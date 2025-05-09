#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <string>
#include <vector>
#include <cstdint>

class FileReader {
public:
    /**
     * @brief Constructs a FileReader for the given file path.
     * 
     * Initializes a FileReader object with the provided file path. 
     * This does not automatically read the file; call read_file() to load contents.
     *
     * @param filepath The path to the file to be read.
     */

    explicit FileReader(const std::string& filepath);

    /**
     * @brief Reads the file and loads its contents into memory.
     * 
     * This function reads the file specified in the constructor and stores its contents
     * in a vector of bytes. It also sets the file size and validity status.
     *
     * @return true if the file was read successfully, false otherwise.
     */
    bool read_file();

    /**
     * @brief Returns the contents of the file as a vector of bytes.
     *
     * Retrieves the data read from the file. This method should be called
     * only after a successful call to read_file(), otherwise the contents
     * may be invalid or empty.
     *
     * @return A reference to a vector containing the file's contents.
     */
    const std::vector<uint8_t>& get_data() const;

    /**
     * @brief Returns the size of the file in bytes.
     *
     * This method returns the size of the file that was read,
     * as determined during the last successful call to read_file().
     *
     * @note This function may return 0 or an invalid size if read_file()
     *       was not called or failed.
     *
     * @return The size of the file in bytes.
     */
    size_t get_file_size() const;

    /**
     * @brief Checks whether the last file read operation was successful.
     *
     * This method returns true if the most recent call to read_file()
     * completed without errors and valid data was loaded.
     *
     * @return true if the file was read successfully; false otherwise.
     */
    bool is_valid() const;

    /**
     * @brief Retrieves the error message from the last file operation, if any.
     *
     * If the most recent call to read_file() failed, this method returns
     * a string describing the error. If the file was read successfully,
     * the returned string may be empty.
     *
     * @return A reference to the last error message.
     */
    const std::string& get_error_message() const;

private:
    std::string filepath_;
    std::vector<uint8_t> data_;
    size_t file_size_;
    bool valid_;
    std::string error_message_;
};

#endif 