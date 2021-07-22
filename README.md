Pong++ - The C++ Pong Online Implementation
===========================================

This is a little demo project that implements a C++ pong client and server based on the standalone Asio library and the SDL2 framework, with a custom webserver framework, game and ui engines; aimed at macOS and linux, though it should work on Windows with only minor tweaks.


Dependencies
------------

- **Asio** (http://think-async.com/)
- **SDL2** (https://www.libsdl.org/)


Building the server
-------------------

Make sure to download the header-only asio library (tested with v.1.18.2), the SDL2 framework and update the Makefile variables ```${INCLUDE}```, ```${FRAMEWORKS}``` and ```${LINKER_FLAGS}``` on both the client and server project folders given the installation/include paths.


License
-------
