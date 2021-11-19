//
// Created by Bilawal Ahmed on 21-Mar-2020.
//

#ifndef CONSOLEINSTALLER_APPLICATIONINFO_H
#define CONSOLEINSTALLER_APPLICATIONINFO_H

#include <utility>
#include <array>

#include <windows.h>

constexpr const char *APPLICATION_NAME = "Game Title Here";
constexpr bool IS_64BIT = false;
constexpr std::array<std::pair<const char *, double>, 1> FILES_INFO
        {{
            {"data-1.bin", 100.0}
        }};

/*
 * This is number of characters not pixels
 * So, font can be changed and the console will have ensured
 * width of X characters and height of Y characters
 */
constexpr COORD WINDOW_SIZE{90, 28};

#endif //CONSOLEINSTALLER_APPLICATIONINFO_H
