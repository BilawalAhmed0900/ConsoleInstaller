//
// Created by Bilawal Ahmed on 23-Mar-2020.
//

#ifndef CONSOLEINSTALLER_PAGE_H
#define CONSOLEINSTALLER_PAGE_H

#include <iostream>

#include <conio.h>

#include <stdexcept>
#include <string>
#include "Console.h"

constexpr char BACK_KEY = 'B';
constexpr char NEXT_KEY = 'N';
constexpr char QUIT_KEY = 'Q';
constexpr char BROWSE_KEY = 'R';
constexpr char FINISH_KEY = 'F';

/*
 * Interface class of Page of our console Installer
 */
class Page
{
public:
    /*
     * This function will be called at start of show() function
     * The page hasn't been shown (console not written)
     */
    virtual void before_showing(const Console &console, Page *next, Page *previous) = 0;

    /*
     * This function will be called when the page has been shown.
     * This is actual work of the page (like installation)
     *
     * This will (if not threaded) block input keys, currently
     */
    virtual void while_showing(const Console &console, Page *next, Page *previous) = 0;

    /*
     * This function will called before returning from show
     */
    virtual void before_returning(const Console &console, Page *next, Page *previous) = 0;

    /*
     * The main function. The actual working of page. This will be responsible for
     * calling all functions from above and writing content to console
     */
    virtual Page *show(const Console &console, Page *next, Page *previous) = 0;

    void set_next_page(Page *_next_page)
    {
        this->next_page = _next_page;
    }

    void set_previous_page(Page *_previous_page)
    {
        this->previous_page = _previous_page;
    }

    [[nodiscard]] Page *get_previous_page() const
    {
        return previous_page;
    }

    [[nodiscard]] Page *get_next_page() const
    {
        return next_page;
    }

private:
    Page *previous_page, *next_page;

public:
    /*
     * This is the text that will shown on the console
     */
    std::string text;

    Page()
        : text(""), previous_page(nullptr), next_page(nullptr)
    {

    }
};

/*
 * Class that has minimal show() implemented
 */
class MinimalPage : public Page
{
public:
    Page *show(const Console &console, Page *next, Page *previous) override
    {
        this->before_showing(console, next, previous);
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
        }

        this->before_returning(console, next, previous);
        return result;
    }
};

#endif //CONSOLEINSTALLER_PAGE_H
