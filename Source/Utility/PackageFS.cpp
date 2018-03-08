/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 08-01-2018
    License: MIT
    Notes:
        Plugins should keep all their files in their archive.
*/

#include "../Stdinclude.hpp"

namespace Package
{
    // Ensure that the names are defined.
    #if !defined (MODULENAME)
        #define MODULENAME "Invalid"
    #endif
    #if !defined (MODULEEXTENSION)
        #define MODULEEXTENSION "Invalid"
    #endif

    // Perform file IO.
    void Deletefile(std::string_view Filename) { return; }
    std::string Readfile(std::string_view Filename) { return {}; }
    void Writefile(std::string_view Filename, std::string Filebuffer) { return; }

    // Get file-information.
    std::vector<std::string> Findfiles(std::string Criteria) { return {}; }
    bool Fileexists(std::string Filename) { return {}; }
}
