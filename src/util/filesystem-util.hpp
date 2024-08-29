#ifndef FILESYSTEM_UTIL_HPP_INCLUDED
#define FILESYSTEM_UTIL_HPP_INCLUDED
//
// filesystem-util.hpp
//
#include <filesystem>
#include <ranges>
#include <string>
#include <vector>

namespace util
{

    inline std::vector<std::filesystem::path> findFilesInDirectory(
        const std::filesystem::path & t_dirPath, const std::string & t_extension)
    {
        std::vector<std::filesystem::path> files;

        if (!std::filesystem::exists(t_dirPath) || !std::filesystem::is_directory(t_dirPath))
        {
            return files;
        }

        files.reserve(64); // based on how many files I know are in the media folders

        // GCC wasn't able to compile the ranges correctly -zTn 2024-8-29
        //auto regularFilesWithExtension =
        //    std::filesystem::directory_iterator{ t_dirPath } |
        //    std::views::filter([](const auto & entry) { return entry.is_regular_file(); }) |
        //    std::views::filter(
        //        [&](const auto & entry) { return (entry.path().extension() == t_extension); });
        //
        //std::ranges::copy(regularFilesWithExtension, std::back_inserter(files));

        for (const auto & entry : std::filesystem::directory_iterator{ t_dirPath })
        {
            if (entry.is_regular_file() && (entry.path().extension() == t_extension))
            {
                files.push_back(entry.path());
            }
        }

        std::ranges::sort(files);
        return files;
    }

} // namespace util

#endif // FILESYSTEM_UTIL_HPP_INCLUDED
