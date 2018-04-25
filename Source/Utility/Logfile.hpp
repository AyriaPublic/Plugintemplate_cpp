/*
    Initial author: Convery (tcn@ayria.se)
    Started: 08-01-2018
    License: MIT
    Notes:
        Prints information to the logfile and console.
*/

#pragma once
#include "Variadicstring.hpp"
#include "../Stdinclude.hpp"

// Internal state.
namespace Internal
{
    #if !defined(MODULENAME)
        #define MODULENAME "Invalid"
    #endif

    constexpr const char *Filepath = "./Plugins/Logs/" MODULENAME ".log";
    static std::mutex Threadguard;
    static std::FILE *Filehandle;
}

// Output to file, assumes null-terminated strings.
inline void Logprint(std::string_view Message)
{
    // Prevent multiple writes to the file.
    Internal::Threadguard.lock();
    {
        // Open the logfile.
        if (!Internal::Filehandle)
        {
            Internal::Filehandle = std::fopen(Internal::Filepath, "a");
        }

        // Append to the logfile.
        if (Internal::Filehandle)
        {
            std::fputs(Message.data(), Internal::Filehandle);
            std::fputs("\n", Internal::Filehandle);
            std::fflush(Internal::Filehandle);
        }
    }
    Internal::Threadguard.unlock();

    // Duplicate the message to STDERR.
    #if !defined(NDEBUG)
        std::fputs(Message.data(), stderr);
        std::fputs("\n", stderr);
    #endif
}
inline void Logprintasync(std::string Message)
{
    std::thread([&](){ Logprint(Message);}).detach();
}

// Formatted output, [Type][Time][Message]
inline void Logformatted(std::string_view Message, char Prefix)
{
    auto Now = std::time(NULL);
    char Buffer[80]{};

    std::strftime(Buffer, 80, "%H:%M:%S", std::localtime(&Now));
    Logprint(va("[%c][%-8s] %*s", Prefix, Buffer, static_cast<int>(Message.size()), Message.data()));
}

// Delete the log and create a new one.
inline void Clearlog()
{
    std::remove(Internal::Filepath);
    
    // NOTE(Convery): We might not want to create a log by default.
    // Logformatted(MODULENAME " - Starting up..", 'I');
}
