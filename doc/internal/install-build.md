# Introduction

All modules are built separately, and have their own build system(s).
If you have `make`, they can all be built at once by running `make` in the root directory, if you meet the requirements for them (see below). `make release` packages all the executables in the `release` subdirectory, and `make [game|editor|dfm]` build only the specified module.  
Each module has its own build system that can be used individually as described below ; some of them can be built without `make`. 

# Kuribrawl
The game itself.
## Dependencies and requirements
`make` is mandatory to build Kuribrawl.

Kuribrawl requires the following libraries : 
- sdl2
- SDL2_image

Instructions on how to install them are included in the platform specific instructions below.  
In any case, you will first have to navigate to the `c++` subdirectory. 

## On Windows
You will need a C++ compiler that supports C++20, and a way to manage the external dependencies. We recommend using the MinGW/MSYS environment.

### Using MinGW/MSYS
Install the 64 or 32 bits version from the [MSYS website](https://www.msys2.org/) depending on your system.  

Run the MSYS terminal and update your system :  
`pacman -Syu`  
then navigate to the c++ directory in the kuribrawl repository, and install the required dependencies by running : 
`pacman -S --needed - < pkglist.txt`

Once the packages are installed, running `make` in this directory will compile and build the executable (`kuribrawl.exe`). It must be launched from the MinGW terminal !  

- `make release` will copy all the files needed to run the game (from the terminal) in the `release` directory.  

# Tools
The Kuribrawl project includes the game itself as well as several smaller programs (more information on that coming soon). These programs are located in a submodule repository, under the name `tools`. If you did not use git to obtain the source, or did not clone the submodules, you need to either install the submodule with `git submodule update --init`, or download it at `https://gitlab.com/TwilCynder/kuribrawl-tools`

## Kuribrawl Editor
The Kuribrawl GameData Editor, written in Java, is located in the `tools/editor/javaEditor` subdirectory. It is an unmanaged project (pure source code, no Gradle, Maven or Eclipse); there are many ways to run and build the project, depending on what you have. In any case, you will need [Java (JDK) 17 or more](https://www.oracle.com/fr/java/technologies/downloads/). 

### VSCode
Developping (including running and building) unmanaged projects works just fine in VSCode, as long as you install the [Java Extension Pack by Red Hat](https://marketplace.visualstudio.com/items?itemName=vscjava.vscode-java-pack). As long as your JDK is correctly configured, VSCode should be able to find it ; in that case, you can simply open a Java source file (and notice the Eclipse-like project explorer opening on the left bar), press F5 to run the project, and use the `>Java: Export Jar` command to build an executable Jar.  

If you do not wish to use VSCode as your IDE, you can use whatever editor you want and follow the the following build/run instructions.  

### Using Ant
- `ant run` will run the application.  
- `ant build` will build an executable Jar. It can be run with the `java -jar kuribrawl-editor.jar` command, or by double-clicking it on windows (or using the `start` command) if Java is properly installed.  
- `ant build-dependant` will buid an executable dependant Jar that requires the library jars located in the `lib` subdirectory (the libs must always exist in a `lib` subdirectory, relative to where the jar is started from).
- `ant dist` will make a zip file containining the dependant Jar and the `lib` subdirectory.  

### Using Make  
This is the system that is used if you simply run `make [install]` or `make editor` in the root directory of Kuribrawl. It uses ant if available. 
- `make` or `make standalone` will build an executable Jar. It can be run with the `java -jar kuribrawl-editor.jar` command, or by double-clicking it on windows (or using the `start` command) if Java is properly installed.  
- `make dependant` will buid an executable dependant Jar that requires the library jars located in the `lib` subdirectory (the libs must always exist in a `lib` subdirectory, relative to where the jar is started from).
- `make release` will build the standalone version and copy it to the `release` directory. Only works if the `kuribrawl-tools` directory is located directly in the `kuribrawl` directory.

### Using nothing else than Java
Simply run the `build.sh` script. It will build the application in the form of an executable Jar. By default, this jar needs the `lib` folder to work, but using the `-s` option builds it in standalone mode, allowing it to work anywhere by itself. The jar can then be run with the `java -jar kuribrawl-editor.jar` command, or by double-clicking it on windows (or using the `start` command) if Java is properly installed.    
The `-r` option implies `-s` and copies the jar to the `release` directory. Only works if the `kuribrawl-tools` directory is located directly in the `kuribrawl` directory.

## Datafile Maker
More information can be found in [the root readme](../../README.md) and the [DFM readme](../../tools/gamefile%20manager/readme.md). 

It written in Purebasic 