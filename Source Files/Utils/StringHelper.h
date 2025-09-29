#pragma once
#include <string>

class StringHelper
{
public:
    static std::string unescape_string(const std::string&);
    static char unescape_char(char, char);
};
