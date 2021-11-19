cmake_minimum_required(VERSION 3.14)
project(ConsoleInstaller)

set(CMAKE_CXX_STANDARD 17)

add_executable(ConsoleInstaller main.cpp ApplicationInfo.h Console.h Page.h WelcomePage.h CenteredText.h BrowsePage.h)