/*
    Initial author: Convery (tcn@ayria.se)
    Started: 15-10-2017
    License: MIT
    Notes:
        Plugins keep their files in their archive.
        Initial test version, horribly inefficient.
*/

#include "../../Stdinclude.h"
#include "zip_file.hpp"

namespace Package
{
    bool Findfiles(std::string Criteria, std::vector<std::string> *Filenames)
    {
        // Workaround for dev-plugins not having the file.
        if (!Fileexists("./Plugins/" MODULENAME ".LNModule"))
        {
            miniz_cpp::zip_file Archive;
            Archive.save("./Plugins/" MODULENAME ".LNModule");
            return false;
        }

        // List all files.
        miniz_cpp::zip_file Archive("./Plugins/" MODULENAME ".LNModule");
        auto Filelist = Archive.namelist();

        // Enqueue the files matching the extension.
        for (auto &Item : Filelist)
            if (std::strstr(Item.c_str(), Criteria.c_str()))
                Filenames->push_back(Item);

        return Filenames->size() > 0;
    }
    void Write(std::string Filename, std::string &Buffer)
    {
        // Workaround for dev-plugins not having the file.
        if (!Fileexists("./Plugins/" MODULENAME ".Ayria"))
        {
            miniz_cpp::zip_file Archive;
            Archive.save("./Plugins/" MODULENAME ".Ayria");
        }

        Delete(Filename);

        // Write directly to the archive.
        miniz_cpp::zip_file Archive("./Plugins/" MODULENAME ".Ayria");
        Archive.writestr(Filename, Buffer);
        Archive.save("./Plugins/" MODULENAME ".Ayria");
    }
    std::string Read(std::string Filename)
    {
        // Workaround for dev-plugins not having the file.
        if (!Fileexists("./Plugins/" MODULENAME ".Ayria"))
        {
            miniz_cpp::zip_file Archive;
            Archive.save("./Plugins/" MODULENAME ".Ayria");
        }

        // Write directly to the archive.
        miniz_cpp::zip_file Archive("./Plugins/" MODULENAME ".Ayria");
        if (!Archive.has_file(Filename)) return {};
        else return Archive.read(Filename);
    }
    bool Exists(std::string Filename)
    {
        // Workaround for dev-plugins not having the file.
        if (!Fileexists("./Plugins/" MODULENAME ".Ayria"))
        {
            miniz_cpp::zip_file Archive;
            Archive.save("./Plugins/" MODULENAME ".Ayria");
            return false;
        }

        // Write directly to the archive.
        miniz_cpp::zip_file Archive("./Plugins/" MODULENAME ".Ayria");
        return Archive.has_file(Filename);
    }
    void Delete(std::string Filename)
    {
        if (!Exists(Filename))
            return;

        // List all files.
        miniz_cpp::zip_file Oldarchive("./Plugins/" MODULENAME ".Ayria");
        auto Filelist = Oldarchive.namelist();

        // Read all items we want to save.
        miniz_cpp::zip_file Newarchive;
        for (auto &Item : Filelist)
            if(0 != std::strcmp(Item.c_str(), Filename.c_str()))
                Newarchive.writestr(Item, Oldarchive.read(Item));

        // Delete the old archive.
        std::remove("./Plugins/" MODULENAME ".Ayria");
        Newarchive.save("./Plugins/" MODULENAME ".Ayria");
    }
}
