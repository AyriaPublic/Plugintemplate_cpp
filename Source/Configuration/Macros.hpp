/*
    Initial author: Convery (tcn@ayria.se)
    Started: 08-01-2018
    License: MIT
    Notes:
        C preprocessor macros for /Utility code.
*/

#pragma once

// Debug information logging.
#if defined(NDEBUG)
    #define Printfunction()
    #define Debugprint(string)
#else
    #define Printfunction() Logformatted(__FUNCTION__, 'T')
    #define Debugprint(string) Logformatted(string, 'D')
#endif

// General information.
#define Infoprint(string) Logformatted(string, 'I')
#define vaprint(format, ...) Logprint(va(format, __VA_ARGS__))

// Some performance tweaking.
#if defined(_WIN32)
    #define likely(x)       x
    #define unlikely(x)     x
    #define ainline         __forceinline
    #define ninline         __declspec(noinline)
#else
    #define likely(x)       __builtin_expect(!!(x), 1)
    #define unlikely(x)     __builtin_expect(!!(x), 0)
    #define ainline         inline __attribute__((__always_inline__))
    #define ninline         __attribute__((__noinline__))
#endif

// Pattern-scanning.
#define Findpattern(Segment, String) Pattern::_Findpattern(Segment, Pattern::Stringtopattern(String), Pattern::Stringtomask(String))
#define Findpatterntext(String) Findpattern(Pattern::Textsegment, String)
#define Findpatterndata(String) Findpattern(Pattern::Datasegment, String)
