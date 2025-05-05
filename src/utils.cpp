#include <utils.hpp>
#include <filesystem>
#include <system_error>
#include <vector>

namespace fs = std::filesystem;
namespace utils {
/**
 * collect_files
 *
 * @param root      the file or directory to scan
 * @param recursive if true, descend into subdirs
 * @throws std::invalid_argument if `root` doesn’t exist or isn’t a file/dir
 * @returns a flat vector of all regular files found
 */
std::vector<fs::path> collect_files(const fs::path& root, bool recursive) {
    std::vector<fs::path> out;
    std::error_code ec;

    if (!fs::exists(root, ec)) {
        throw std::invalid_argument(root.string() + " does not exist");
    }

    if (fs::is_regular_file(root, ec)) {
        out.push_back(root);
    }
    else if (fs::is_directory(root, ec)) {
        if (recursive) {
            // skip_permission_denied avoids throwing on unreadable dirs
            for (auto& entry : fs::recursive_directory_iterator(root,
                    fs::directory_options::skip_permission_denied, ec)) {
                if (!ec && entry.is_regular_file(ec))
                    out.push_back(entry.path());
                ec.clear();
            }
        } else {
            for (auto& entry : fs::directory_iterator(root,
                    fs::directory_options::skip_permission_denied, ec)) {
                if (!ec && entry.is_regular_file(ec))
                    out.push_back(entry.path());
                ec.clear();
            }
        }
    }
    else {
        throw std::invalid_argument(root.string() + " is neither file nor directory");
    }

    return out;
}

} // namespace utils