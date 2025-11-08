#pragma once

#include <string>

namespace game
{

    class Testing
    {
    public:
        static const std::string get_message()
        {
            return "From test.h";
        }
    };

}
