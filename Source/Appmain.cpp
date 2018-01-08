/*
    Initial author: Convery (tcn@ayria.se)
    Started: 08-01-2018
    License: MIT
    Notes:
        Provides the entrypoint for Windows and Nix.
*/

#include "Stdinclude.hpp"

#if defined _WIN32
BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
    // Opt-out of further thread notifications.
    DisableThreadLibraryCalls(hDllHandle);

    // Clear the previous sessions logfile.
    Clearlog();

    return TRUE;
}
#else
__attribute__((constructor)) void DllMain()
{
    // Clear the previous sessions logfile.
    Clearlog();
}
#endif
