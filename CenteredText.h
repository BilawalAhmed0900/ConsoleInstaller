//
// Created by Bilawal Ahmed on 23-Mar-2020.
//

#ifndef CONSOLEINSTALLER_CENTEREDTEXT_H
#define CONSOLEINSTALLER_CENTEREDTEXT_H

#include <sstream>
#include <string>

std::string get_centered_text(const std::string& original_text, long line_length, bool char_present, char border_char)
{
    std::stringstream ss;
    std::string set_string = (original_text.size() > line_length - (char_present ? 4 : 2))
            ? original_text.substr(0, line_length - 4)
            : original_text;
    bool is_odd = set_string.size() % 2 != 0;
    long blank_length = ((line_length - 2) / 2) - ((long)set_string.size() / 2);
    if (is_odd)
    {
        blank_length--;
    }

    if (char_present)
    {
        ss << border_char;
    }
    else
    {
        ss << ' ';
    }


    for (int i = 0; i < blank_length; i++)
    {
        ss << ' ';
    }
    ss << set_string;

    if (is_odd)
    {
        blank_length++;
    }

    for (int i = 0; i < blank_length; i++)
    {
        ss << ' ';
    }

    if (char_present)
    {
        ss << border_char;
    }
    else
    {
        ss << ' ';
    }

    return ss.str();
}

#endif //CONSOLEINSTALLER_CENTEREDTEXT_H
