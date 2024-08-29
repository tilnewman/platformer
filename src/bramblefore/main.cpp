// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "subsystem/coordinator.hpp"
#include "util/check-macros.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>

int main(const int argc, const char * const argv[])
{
    try
    {
        using namespace bramblefore;

        Settings settings;
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

        // make this a pointer only because it uses too much stack
        std::unique_ptr<Coordinator> coordinatorUPtr{ std::make_unique<Coordinator>(settings) };

        coordinatorUPtr->play();
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
