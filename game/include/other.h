#pragma once

#include <string>

namespace game
{

    class Another
    {
    public:
        static const std::string get_message()
        {
            return "From other.h";
        }
    };

}
