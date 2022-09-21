# Kuribrawl

## Dependencies 
Kuribrawl requires the following libraries : 
- sdl2
- SDL_image

Instructions on how to install them are included in the patform specific instructions below.

## On Windows
You will need a C++ compiler that supports C++17, and a way to manage the external dependencies. We recommend using the MinGW/MSYS environment.

### Using MinGW/MSYS
Install the 64 or 32 bits version from the [MSYS website](https://www.msys2.org/) depending on your system.  

Run the MSYS terminal and update your system :  
`pacman -Syu`  
then navigate to the c++ directory in the kuribrawl repository, and install the required dependencies by running : 
`pacman -S --needed - < pkglist.txt`

Once the packages are installed, running `make` in this directory will compile and build the executable (`kuribrawl.exe`). It must be launched from the MinGW terminal !

# Tools
Instruction to build and install the kuribrawl tools will be provided soon.