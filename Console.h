//
// Created by Bilawal Ahmed on 21-Mar-2020.
//

#ifndef CONSOLEINSTALLER_CONSOLE_H
#define CONSOLEINSTALLER_CONSOLE_H

#include <string>

#include <stdexcept>
#include <windows.h>

#include "ApplicationInfo.h"

struct Console
{
    Console(const std::string& title_str, unsigned width, unsigned height )
        : hConOut(nullptr), csbi{0}
    {
        SMALL_RECT r;
        COORD      c;
        hConOut = GetStdHandle( STD_OUTPUT_HANDLE );
        hConIn = GetStdHandle( STD_INPUT_HANDLE );

        HWND hwnd = GetConsoleWindow();
        DWORD style = GetWindowLong(hwnd, GWL_STYLE);

        /*
         * Disable WS_OVERLAPPEDWINDOW, reduced the window to mere buffer
         * WS_BORDER gives it small borders
         */
        style &= ~WS_OVERLAPPEDWINDOW;
        style |= WS_BORDER;
        SetWindowLong(hwnd, GWL_STYLE, style);

        if (!GetConsoleScreenBufferInfo( hConOut, &csbi ))
        {
            throw std::runtime_error( "Error: GetConsoleScreenBufferInfo." );
        }

        /*
         * Now resizing the console window is a bit tricky
         * There are two things to consider when resizing the console window
         *
         * 1. The size of buffer that is shown in the window
         * 2. The size of actual window
         *
         * Constraints:
         * 1. The size of buffer cannot be smaller than size of window
         * 2. The size of window cannot be larger than size of buffer -1 (width-1, height-1)
         *
         * The whole thing is divided in 4 scenerios
         */
        if (width < csbi.srWindow.Right - 1 && height < csbi.srWindow.Bottom - 1)
        {

            /*
             * We now have to set width that is smaller than previous size
             *
             * That is easy, just set console size smaller and resize buffer accordingly
             * No issue
             */
            c.X = width;
            c.Y = height;

            r.Left   =
            r.Top    = 0;
            r.Right  = width -1;
            r.Bottom = height -1;

            if (SetConsoleWindowInfo( hConOut, TRUE, &r ) == 0)
            {
                throw std::runtime_error("Error: SetConsoleWindowInfo.");
            }

            if (SetConsoleScreenBufferSize( hConOut, c ) == 0)
            {
                throw std::runtime_error("Error: SetConsoleScreenBufferSize.");
            }
        }
        else if (width >= csbi.srWindow.Right - 1 && height >= csbi.srWindow.Bottom - 1)
        {

            /*
             * We now have to set width that is larger than previous size
             *
             * That is easy, just set buffer size larger then resize console window
             * No issue
             */
            c.X = width;
            c.Y = height;

            r.Left   =
            r.Top    = 0;
            r.Right  = width -1;
            r.Bottom = height -1;

            if (SetConsoleScreenBufferSize( hConOut, c ) == 0)
            {
                throw std::runtime_error("Error: SetConsoleScreenBufferSize.");
            }

            if (SetConsoleWindowInfo( hConOut, TRUE, &r ) == 0)
            {
                throw std::runtime_error("Error: SetConsoleWindowInfo.");
            }
        }
        /*
         * Issue arise in these two cases
         * When one dimension has to be set smaller and one larger
         *
         * Solution:
         *  Compute which is smaller of two
         *
         *  Set window to that size
         *  Resize buffer to that
         *
         *  Set buffer to larger one
         *  Resize window to that
         */
        else if (width >= csbi.srWindow.Right - 1 && height < csbi.srWindow.Bottom - 1)
        {

            c.X = csbi.srWindow.Right;
            c.Y = height;

            r.Left   =
            r.Top    = 0;
            r.Right  = csbi.srWindow.Right -1;
            r.Bottom = height -1;

            if (SetConsoleWindowInfo( hConOut, TRUE, &r ) == 0)
            {
                throw std::runtime_error("Error: SetConsoleWindowInfo.");
            }

            if (SetConsoleScreenBufferSize( hConOut, c ) == 0)
            {
                throw std::runtime_error("Error: SetConsoleScreenBufferSize.");
            }

            c.X = width;
            c.Y = height;

            r.Left   =
            r.Top    = 0;
            r.Right  = width -1;
            r.Bottom = height -1;

            if (SetConsoleScreenBufferSize( hConOut, c ) == 0)
            {
                throw std::runtime_error("Error: SetConsoleScreenBufferSize.");
            }

            if (SetConsoleWindowInfo( hConOut, TRUE, &r ) == 0)
            {
                throw std::runtime_error("Error: SetConsoleWindowInfo.");
            }
        }
        else if (width < csbi.srWindow.Right - 1 && height >= csbi.srWindow.Bottom - 1)
        {
            c.X = width;
            c.Y = csbi.srWindow.Bottom;

            r.Left   =
            r.Top    = 0;
            r.Right  = width -1;
            r.Bottom = csbi.srWindow.Bottom -1;

            if (SetConsoleWindowInfo( hConOut, TRUE, &r ) == 0)
            {
                std::string error = "Error: SetConsoleWindowInfo(" + std::to_string(GetLastError()) + ").";
                throw std::runtime_error(error.c_str());
            }

            if (SetConsoleScreenBufferSize( hConOut, c ) == 0)
            {
                std::string error = "Error: SetConsoleScreenBufferSize(" + std::to_string(GetLastError()) + ").";
                throw std::runtime_error(error.c_str());
            }

            c.X = width;
            c.Y = height;

            r.Left   =
            r.Top    = 0;
            r.Right  = width -1;
            r.Bottom = height -1;

            if (SetConsoleWindowInfo( hConOut, TRUE, &r ) == 0)
            {
                throw std::runtime_error("Error: SetConsoleWindowInfo.");
            }

            if (SetConsoleScreenBufferSize( hConOut, c ) == 0)
            {
                throw std::runtime_error("Error: SetConsoleScreenBufferSize.");
            }
        }

        title(title_str);
    }

    ~Console() = default;
    /*
     * Not needed, as windows remember default console attrubutes. Commenting out
     */
    /*{
        SetConsoleTextAttribute(    hConOut,        csbi.wAttributes );
        SetConsoleScreenBufferSize( hConOut,        csbi.dwSize      );
        SetConsoleWindowInfo(       hConOut, TRUE, &csbi.srWindow    );
    }*/

    void color(WORD color = 0x07) const
    {
        SetConsoleTextAttribute( hConOut, color );
    }

    static void title(const std::string& title)
    {
        if (SetConsoleTitleA(title.c_str()) == 0)
        {
#ifdef DEBUG
            std::string error_message = "Error: SetConsoleTitleA (" + std::to_string(GetLastError()) + ").";
            throw std::runtime_error(error_message.c_str());
#else
            throw std::runtime_error("Error: SetConsoleTitleA.");
#endif
        }
    }

    [[nodiscard]] DWORD write(const std::string& text) const
    {
        DWORD written = 0;
        const char *c_text = text.c_str();
        WriteConsoleA(hConOut, c_text, strlen(c_text), &written, nullptr);

        return written;
    }

    void flush_input() const
    {
        if (FlushConsoleInputBuffer(hConIn) == 0)
        {
#ifdef DEBUG
            std::string error_message = "Error: FlushConsoleInputBuffer (" + std::to_string(GetLastError()) + ").";
            throw std::runtime_error(error_message.c_str());
#else
            throw std::runtime_error("Error: FlushConsoleInputBuffer.");
#endif
        }
    }

    void cursor_visibility(bool show = false) const
    {
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        if (SetConsoleCursorInfo(hConOut, &info) == 0)
        {
#ifdef DEBUG
            std::string error_message = "Error: SetConsoleCursorInfo (" + std::to_string(GetLastError()) + ").";
            throw std::runtime_error(error_message.c_str());
#else
            throw std::runtime_error("Error: SetConsoleCursorInfo.");
#endif
        }
    }

    void set_cursor_position(DWORD X = 0, DWORD Y = 0) const
    {
        COORD pos;
        pos.X = X;
        pos.Y = Y;
        if (SetConsoleCursorPosition(hConOut, pos) == 0)
        {
#ifdef DEBUG
            std::string error_message = "Error: SetConsoleCursorPosition (" + std::to_string(GetLastError()) + ").";
            throw std::runtime_error(error_message.c_str());
#else
            throw std::runtime_error("Error: SetConsoleCursorPosition.");
#endif
        }
    }

    HANDLE                     hConOut, hConIn;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
};

#endif //CONSOLEINSTALLER_CONSOLE_H
