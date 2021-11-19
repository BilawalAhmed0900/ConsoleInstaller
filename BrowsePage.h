//
// Created by Bilawal Ahmed on 24-Mar-2020.
//

#ifndef CONSOLEINSTALLER_BROWSEPAGE_H
#define CONSOLEINSTALLER_BROWSEPAGE_H

#include <string>
#include <sstream>
#include <shlobj.h>

#include "CenteredText.h"
#include "Page.h"
#include "Console.h"
#include "ApplicationInfo.h"

std::string get_browse_text(const std::string& path)
{
    const char *GUIDE = "Please select a directory to install to:";
    const char *BROWSE_BUTTON = "| R: Browse |";
    const char *NEXT_HELP = "Press \'n\' to go to the next page";
    const char *BACK_HELP = "Press \'b\' to go to the previous page";
    const char *QUIT_HELP = "Press \'q\' to quit the installation process";
    const char *BROWSE_HELP = "Press \'r\' to browse directory";
    const char *BUTTON_HELP = R"('b': Back    'r': Browse    'q': Quit    'n': Next)";
    std::stringstream ss;

    for (int i = 0; i < WINDOW_SIZE.X; i++)
        ss << '=';

    long lines_written = 0;
    std::string custom_path = (path.size() > WINDOW_SIZE.X - 4 - strlen(BROWSE_BUTTON) - 1) ? path.substr(0, WINDOW_SIZE.X - 4 - strlen(BROWSE_BUTTON) - 1) : path;
    custom_path += std::string(1, ' ') + BROWSE_BUTTON;
    ss << get_centered_text(GUIDE, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(custom_path, WINDOW_SIZE.X, true, '|');
    lines_written += 2;

    for (int i = lines_written; i < WINDOW_SIZE.Y - 10; i++)
    {
        ss << get_centered_text("", WINDOW_SIZE.X, true, '|');
    }

    ss << get_centered_text(NEXT_HELP, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(BACK_HELP, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(BROWSE_HELP, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(QUIT_HELP, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text("", WINDOW_SIZE.X, true, '|');
    ss << get_centered_text("", WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(BUTTON_HELP, WINDOW_SIZE.X, true, '|');

    for (int i = 0; i < WINDOW_SIZE.X; i++)
        ss << '=';

    return ss.str();
}

class BrowsePage : public Page
{
public:
    void before_showing(const Console &console, Page *next, Page *previous) override
    {

    }

    void while_showing(const Console &console, Page *next, Page *previous) override
    {
        console.set_cursor_position(0, 0);
        console.cursor_visibility(false);
    }

    void before_returning(const Console &console, Page *next, Page *previous) override
    {

    }

    Page *show(const Console &console, Page *next, Page *previous) override
    {
        this->before_showing(console, next, previous);
        text = get_browse_text(path_selected);
        if (console.write(text) != text.size())
        {
            throw std::runtime_error("MinimalPage.show(): console.write returned invalid value.");
        }

        this->while_showing(console, next, previous);

        Page *result = nullptr;
        while (true)
        {
            console.flush_input();
            char in = (char)_getch();
            if (toupper(in) == BACK_KEY)
            {
                if (previous == nullptr)
                {
                    continue;
                }
                result = previous;
                break;
            }
            else if (toupper(in) == NEXT_KEY)
            {
                result = next;
                break;
            }
            else if (toupper(in) == QUIT_KEY)
            {
                break;
            }
            else if (toupper(in) == BROWSE_KEY)
            {
                char folder_name[MAX_PATH] = { 0 };
                BROWSEINFOA browse_info;
                memset(&browse_info, 0, sizeof(browse_info));
                browse_info.pszDisplayName = folder_name;
                browse_info.lpszTitle = "Select Directory";
                browse_info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_VALIDATE | BIF_USENEWUI;
                browse_info.iImage = -1;

                OleInitialize(nullptr);
                LPITEMIDLIST pidl = SHBrowseForFolderA(&browse_info);
                if (pidl != nullptr)
                {
                    SHGetPathFromIDListA(pidl, folder_name);
                    CoTaskMemFree(pidl);

                    path_selected = folder_name;
                }
                OleUninitialize();
            }

            text = get_browse_text(path_selected);
            if (console.write(text) != text.size())
            {
                throw std::runtime_error("MinimalPage.show(): console.write returned invalid value.");
            }
        }

        this->before_returning(console, next, previous);
        return result;
    }

public:
    std::string path_selected;
    BrowsePage()
    {
        char buffer[MAX_PATH];

        if (SHGetSpecialFolderPathA(nullptr, buffer, IS_64BIT ? CSIDL_PROGRAM_FILES : CSIDL_PROGRAM_FILESX86, false) == 0)
        {
            throw std::runtime_error("Error: SHGetSpecialFolderPathA.");
        }

        path_selected = std::string(buffer) + "\\" + std::string(APPLICATION_NAME);
    }

    ~BrowsePage()
    {
        CoUninitialize();
    }
};

BrowsePage& get_browse_page()
{
    static BrowsePage browse_page;
    return browse_page;
}

#endif //CONSOLEINSTALLER_BROWSEPAGE_H
