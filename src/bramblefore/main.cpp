// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "subsystem/coordinator.hpp"
#include "util/check-macros.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>

int main(const int argc, const char * const argv[])
{
    try
    {
        bramblefore::Settings settings;
        if (argc > 1)
        {
            settings.media_path =
                std::filesystem::current_path() / std::filesystem::path{ argv[1] };
        }
        else
        {
            settings.media_path = std::filesystem::current_path() / "media";
        }

        M_CHECK(
            std::filesystem::exists(settings.media_path),
            "Error:  The media path does not exist:"
                << settings.media_path
                << "\nPut the media path on the command line or put the 'media' folder here.");

        settings.media_path = std::filesystem::canonical(settings.media_path);

        bramblefore::Coordinator coordinator(settings);
        coordinator.play();
    }
    catch (const std::exception & ex)
    {
        std::cerr << "Exception Error: \"" << ex.what() << "\"\n";
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Non-Standard Exception Error\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
