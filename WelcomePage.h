//
// Created by Bilawal Ahmed on 23-Mar-2020.
//

#include <string>
#include <sstream>

#include "CenteredText.h"
#include "Page.h"
#include "Console.h"
#include "ApplicationInfo.h"

std::string get_welcome_text()
{
    const char *WELCOME_TO = "Welcome to setup wizard of";
    const char *GUIDE_YOU = "The setup wizard will guide you through the rest";
    const char *GUIDE_YOU_CONT = "of the setup process";
    const char *NEXT_HELP = "Press \'n\' to go to the next page";
    const char *BACK_HELP = "Press \'b\' to go to the previous page";
    const char *QUIT_HELP = "Press \'q\' to quit the installation process";
    const char *BUTTON_HELP = R"('b': Back    'q': Quit    'n': Next)";
    std::stringstream ss;

    for (int i = 0; i < WINDOW_SIZE.X; i++)
        ss << '=';

    long lines_written = 0;
    ss << get_centered_text(WELCOME_TO, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(APPLICATION_NAME, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text("", WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(GUIDE_YOU, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(GUIDE_YOU_CONT, WINDOW_SIZE.X, true, '|');
    lines_written += 6;

    for (int i = lines_written; i < WINDOW_SIZE.Y - 8; i++)
    {
        ss << get_centered_text("", WINDOW_SIZE.X, true, '|');
    }

    ss << get_centered_text(NEXT_HELP, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(BACK_HELP, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(QUIT_HELP, WINDOW_SIZE.X, true, '|');
    ss << get_centered_text("", WINDOW_SIZE.X, true, '|');
    ss << get_centered_text("", WINDOW_SIZE.X, true, '|');
    ss << get_centered_text(BUTTON_HELP, WINDOW_SIZE.X, true, '|');

    for (int i = 0; i < WINDOW_SIZE.X; i++)
        ss << '=';

    return ss.str();
}

class WelcomePage : public MinimalPage
{
    void before_showing(const Console &console, Page *next, Page *previous) override
    {

    }

    void while_showing(const Console &console, Page *next, Page *previous) override
    {
        console.set_cursor_position(0, 0);
        console.cursor_visibility(false);
        //console.scroll_to_start();
    }

    void before_returning(const Console &console, Page *next, Page *previous) override
    {

    }

public:
    WelcomePage()
    {
        text = get_welcome_text();
    }
};

WelcomePage& get_welcome_page()
{
    static WelcomePage welcome_page;
    return welcome_page;
}
