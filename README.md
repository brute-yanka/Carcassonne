# Carcassonne
This project is an implementation of a Carcassonne game (single player) using the C++ programming language and SFML.

This game using Visual C++ 15 (2017) - 64-bit SFML, Download from: https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip
!Link libraries before building!

Carcassonne -> Properties:
- Configuration -> All Configurations
- Platform: x64

C/C++:
- General -> Additional Include Directories -> Include the path for the *include* folder

Linker:
- General -> Additional Library Directories -> Include the path for the *lib* folder
- Input -> Additional Dependencies -> Add the followings:
sfml-graphics.lib
sfml-window.lib
sfml-system.lib
sfml-audio.lib
sfml-network.lib

Change the Configuration -> Debug
Linker:
- Input -> Additional Dependencies -> Add the followings:
sfml-graphics-d.lib
sfml-window-d.lib
sfml-system-d.lib
sfml-audio-d.lib
sfml-network-d.lib

Click apply! Enjoy
