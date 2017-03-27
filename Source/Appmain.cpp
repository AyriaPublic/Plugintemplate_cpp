/*
    Initial author: Convery
    Started: 2017-03-27
    License: MIT
*/

#include "StdInclude.h"

// Clear the logfile on startup so we only save this session.
namespace { struct Deletelog { Deletelog() { Clearlogfile(); } }; static Deletelog Deleted{}; }

#ifdef _WIN32
#include <Windows.h>
BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
    switch ( nReason )
    {
        case DLL_PROCESS_ATTACH:
        {
            // Rather not handle all thread updates.
            DisableThreadLibraryCalls(hDllHandle);
            break;
        }
    }

    return TRUE;
}
#endif
