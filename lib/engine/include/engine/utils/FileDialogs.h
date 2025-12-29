#pragma once
#include <string>


namespace engine
{
    class FileDialogs
    {
    public:
        static std::string open_file(const char* filter);
        static std::string save_file(const char* filter);
    };
}
