#include <string>
#include <iostream>

namespace game::tools
{
    inline void printl_message(const std::string& message)
    {
        std::cout << "-- " << message << '\n';
    }
}
