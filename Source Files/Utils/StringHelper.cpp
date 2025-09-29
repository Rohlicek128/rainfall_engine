#include "StringHelper.h"
#include <sstream>

std::string StringHelper::unescape_string(const std::string& str)
{
    std::ostringstream out;
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] == '\\') out << '\\' << '\\';
        else out << str[i];
        //out << unescape_char(str[i], str[i + 1]);
    }
    //out << str.back();
    return out.str();
}

char StringHelper::unescape_char(const char c, const char c_next)
{
    if (c == '\\')
    {
        switch (c_next)
        {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case '\\': return '\\';
        case '\'': return '\'';
        case '"': return '"';
        case '0': return '\0';
        default:
            return c_next;
        }
    }
    return c;
}
