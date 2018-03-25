# AYRIA Plugin-template for C++

In AYRIA we use plugins to perform all game-modification. These plugins do not have any rigid requirements, they are simply shared libraries (.dll/.so). We do however provide two entrypoints and a message API that we encourage everyone to use. The entrypoints, [implemented as callbacks](https://github.com/AyriaPublic/Plugintemplate_cpp/blob/master/Source/Appmain.cpp#L11-L32), notifies the plugin when it's appropriate to do .text and .data segment modifications respectively.

The plugins are distributed as a ZIP archive containing the plugin (usually both .ayria32 and .ayria64 extensions), [a manifest with metadata](#), and any configuration-files the plugin requires. This template provides a [utility-class](https://github.com/AyriaPublic/Plugintemplate_cpp/blob/master/Source/Utility/PackageFS.hpp) for reading and writing to this archive. Writing to the games directory is highly frowned upon as it may cause incompatibility with vanilla versions of the game and just clutters the directory in general. So please keep your files in your archive.

## Your first plugin

Simply clone this repository, edit the [CMakeLists.txt](https://github.com/AyriaPublic/Plugintemplate_cpp/blob/master/CMakeLists.txt) and [Defines.hpp](https://github.com/AyriaPublic/Plugintemplate_cpp/blob/master/Source/Configuration/Defines.hpp) to set your plugin name, and add your code to the [Appmain.cpp](https://github.com/AyriaPublic/Plugintemplate_cpp/blob/master/Source/Appmain.cpp#L11-L32). Easy.

While Linux developers can simply `cmake . && make -j` to build the plugin, Windows can be bothersome. As such we recommend that new developers simply get the optional VS2017 component for CMake (accessible from the VS2017 installer). This will allow you to open the CMakeLists.txt from msvc as a CMake project and it will do all the work for you. We also recommend their [package-manager](https://github.com/Microsoft/vcpkg) which will create a Linux-like environment for thirdparty libraries.

## Loading and debugging

The plugins are usually loaded into the game by the [Bootstrapper](https://github.com/AyriaPublic/Bootstrap_cpp) provided with the AYRIA client, but that is annoying for developers as you'd have to package the plugin on every build. To solve this, developers can sideload their plugins by naming their plugin `Developerplugin[.dll/.so]` and placing it in `./Gamedir/Plugins/`. If you don't want to use the bootstrapper, which can be annoying to debug with, you can build the [Bootstrapmodule](https://github.com/AyriaPublic/Bootstrapmodule_cpp) and inject that into the game on startup. Recommended ways is to edit the games IAT using CFF Explorer on Windows or simply naming it `Localbootstrap[.dll/.so]` so that the [Platformwrapper](https://github.com/Convery/Platformwrapper_cpp/blob/master/Source/Appmain.cpp#L187-L189) loads it on startup. The second method being preferred for Steam games as the platformwrapper can replace the steam_api library in the games directory.

