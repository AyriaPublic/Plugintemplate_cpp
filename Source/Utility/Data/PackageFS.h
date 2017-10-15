/*
    Initial author: Convery (tcn@ayria.se)
    Started: 15-10-2017
    License: MIT
    Notes:
        Plugins keep their files in their archive.
        Initial test version, horribly inefficient.
*/

#pragma once
#include "../../Stdinclude.h"

namespace Package
{
    bool Findfiles(std::string Criteria, std::vector<std::string> *Filenames);
    void Write(std::string Filename, std::string &Buffer);
    std::string Read(std::string Filename);
    bool Exists(std::string Filename);
    void Delete(std::string Filename);
}
