/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 08-01-2018
    License: MIT
    Notes:
        Plugins should keep all their files in their archive.
*/

#pragma once
#include "../Stdinclude.hpp"

namespace Package
{
    // Perform file IO.
    void Deletefile(std::string_view Filename);
    std::string Readfile(std::string_view Filename);
    void Writefile(std::string_view Filename, std::string Filebuffer);

    // Get file-information.
    std::vector<std::string> Findfiles(std::string Criteria);
    bool Fileexists(std::string Filename);
}
