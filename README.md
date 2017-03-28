# Ayria Plugintemplate for C++

This repository serves as an introduction to the environment for C++ developers.
It provides utilities for extra rapid plugin development.
The area of note however is the exports in `Appmain.cpp`, these callbacks are essential for the bootstrapper.
Even if you decide to create a plugin from scratch, they are needed.
You may also want to familiarize yourself with the `Bytebuffer` class as it's used everywhere.

Further reading:
* [How to manually load plugins]("#")
* [How to add plugins to the marketplace]("#")
* [Plugintemplate for Rust developers]("#")
