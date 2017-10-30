/*
    Initial author: Convery (tcn@ayria.se)
    Started: 29-10-2017
    License: MIT
    Notes:
        Plugins keep their files in their archive.
        Initial test version, horribly inefficient.
*/

#pragma once
#include "../../Stdinclude.h"

namespace Package
{
    using Archivehandle = void *;

    // Operations on the default archive.
    std::string Read(std::string Filename);
    void Write(std::string Filename, std::string &Buffer);
    bool Findfiles(std::string Criteria, std::vector<std::string> *Filenames);
    bool Exists(std::string Filename);
    void Delete(std::string Filename);

    // Operations on a specific archive.
    Archivehandle Loadarchive(std::string Filename);
    void Savearchive(Archivehandle &Handle, std::string Filename);
    std::string Read(Archivehandle &Handle, std::string Filename);
    void Write(Archivehandle &Handle, std::string Filename, std::string &Buffer);
    bool Findfiles(Archivehandle &Handle, std::string Criteria, std::vector<std::string> *Filenames);
    bool Exists(Archivehandle &Handle, std::string Filename);
    void Delete(Archivehandle &Handle, std::string Filename);
}

