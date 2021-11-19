#include <iostream>
#include <string>

#include "Console.h"
#include "ApplicationInfo.h"
#include "WelcomePage.h"
#include "BrowsePage.h"

using namespace std;

int main()
{
    Console console(string(APPLICATION_NAME) + string(" - Setup"), WINDOW_SIZE.X, WINDOW_SIZE.Y);
    WelcomePage& welcome_page = get_welcome_page();
    BrowsePage& browse_page = get_browse_page();
    std::string path_selected;

    welcome_page.set_next_page(&browse_page);
    browse_page.set_previous_page(&welcome_page);

    Page *current_page = &welcome_page;
    while (true)
    {
        Page *temp = current_page->show(console, current_page->get_next_page(), current_page->get_previous_page());
        if (temp == nullptr)
        {
            break;
        }

        if (current_page == &browse_page)
        {
            path_selected = browse_page.path_selected;
        }
        current_page = temp;
    }
    return 0;
}