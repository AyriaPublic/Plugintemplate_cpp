/*
    Initial author: Convery (tcn@ayria.se)
    Started: 08-01-2018
    License: MIT
    Notes:
        Creates a readable string from variadic input.
*/

#pragma once
#include "../Stdinclude.hpp"

/*
    Usage example:
    auto String = va("Hello %s", "World");
    while(true) va(String, "\nIt's %llu sec since 1970", time(NULL));
*/

// Adjust the default blocksize.
#if !defined VABLOCKSIZE
#define VABLOCKSIZE 2048
#endif

inline std::string va(std::string_view Format, ...)
{
    auto Resultbuffer = std::make_unique<char[]>(VABLOCKSIZE);
    std::va_list Varlist;

    // Create a new string from the arguments and truncate as needed.
    va_start(Varlist, Format);
    std::vsnprintf(Resultbuffer.get(), VABLOCKSIZE, Format.data(), Varlist);
    va_end(Varlist);

    return std::move(Resultbuffer.get());
}
inline void va(std::string &Result, std::string_view Format, ...)
{
    auto Resultbuffer = std::make_unique<char[]>(VABLOCKSIZE);
    std::va_list Varlist;

    // Create a new string from the arguments and truncate as needed.
    va_start(Varlist, Format);
    std::vsnprintf(Resultbuffer.get(), VABLOCKSIZE, Format.data(), Varlist);
    va_end(Varlist);

    Result.append(Resultbuffer.get());
}
