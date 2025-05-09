#include <utils.hpp>
#include <filesystem>
#include <system_error>
#include <vector>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
namespace utils {

std::vector<fs::path> collect_files(const fs::path& root, bool recursive, const std::string& extension) {
    std::vector<fs::path> out;
    std::error_code ec;

    if (!fs::exists(root, ec)) {
        throw std::invalid_argument(root.string() + " does not exist");
    }
    if (fs::is_regular_file(root, ec)) {
        if (extension.empty() || root.extension() == extension) {
            out.push_back(root);
        }
    }
    else if (fs::is_directory(root, ec)) {
        if (recursive) {
            // skip_permission_denied avoids throwing on unreadable dirs
            for (auto& entry : fs::recursive_directory_iterator(root,
                    fs::directory_options::skip_permission_denied, ec)) {
                if (!ec && entry.is_regular_file(ec))
                    if (extension.empty() || entry.path().extension() == extension) {
                        out.push_back(entry.path());
                    }
            }
            ec.clear();
        } else {
            for (auto& entry : fs::directory_iterator(root,
                    fs::directory_options::skip_permission_denied, ec)) {
                if (!ec && entry.is_regular_file(ec))
                    if (extension.empty() || entry.path().extension() == extension) {
                        out.push_back(entry.path());
                    }
            }
            ec.clear();
        }
    }
    else {
        throw std::invalid_argument(root.string() + " is neither file nor directory");
    }

    return out;
}

std::string make_report_filename(const std::string& prefix) {
    auto now   = std::chrono::system_clock::now();
    auto t_c   = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ts;
    ts << std::put_time(std::localtime(&t_c), "%Y%m%d-%H%M%S");
    return prefix + "-" + ts.str() + ".json";
}

// method to make testable output
void write_json_output(std::ostream& out, const nlohmann::json& data) {
    out << data.dump(2) << "\n";
}

} // namespace utils