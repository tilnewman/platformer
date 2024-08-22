#ifndef FILESYSTEM_UTIL_HPP_INCLUDED
#define FILESYSTEM_UTIL_HPP_INCLUDED
//
// filesystem-util.hpp
//
#include <string>
#include <vector>
#include <filesystem>

namespace util
{

    inline std::vector<std::filesystem::path> findFilesInDirectory(
        const std::filesystem::path & dirPath, const std::string & extension)
    {
        std::vector<std::filesystem::path> files;
        
        if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath))
        {
            return files;
        }

        files.reserve(256); // just a guess

        for (const auto & entry : std::filesystem::directory_iterator{ dirPath })
        {
            if (entry.is_regular_file() && (entry.path().extension() == extension))
            {
                files.push_back(entry.path());
            }
        }

        if (!files.empty())
        {
            std::sort(std::begin(files), std::end(files), [](const auto & lhs, const auto & rhs) {
                return (lhs.filename().string() < rhs.filename().string());
            });
        }

        return files;
    }

} // namespace util

#endif // FILESYSTEM_UTIL_HPP_INCLUDED
