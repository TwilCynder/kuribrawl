# Kuribrawl

## Dependencies 
Kuribrawl requires the following libraries : 
- sdl2
- SDL2_image

Instructions on how to install them are included in the patform specific instructions below.

## On Windows
You will need a C++ compiler that supports C++20, and a way to manage the external dependencies. We recommend using the MinGW/MSYS environment.

### Using MinGW/MSYS
Install the 64 or 32 bits version from the [MSYS website](https://www.msys2.org/) depending on your system.  

Run the MSYS terminal and update your system :  
`pacman -Syu`  
then navigate to the c++ directory in the kuribrawl repository, and install the required dependencies by running : 
`pacman -S --needed - < pkglist.txt`

Once the packages are installed, running `make` in this directory will compile and build the executable (`kuribrawl.exe`). It must be launched from the MinGW terminal !

# Tools
The Kuribrawl project includes the game itself as well as several smaller programs (more information on that coming soon). These programs are located in a submodule repository, under the name `tools`. If you did not use git to obtain the source, or did not clone the submodules, you need to either install the submodule with `git submodule update --init`, or download it at `https://gitlab.com/TwilCynder/kuribrawl-tools`

## Kuribrawl Editor
The Kuribrawl GameData Editor, written in Java, is located in the `tools/editor/javaEditor` subdirectory. It is an unmanaged project (pure source code, no Gradle, Maven or Eclipse); there are many ways to run and build the project, depending on what you have. In any case, you will need [Java (JDK) 17 or more](https://www.oracle.com/fr/java/technologies/downloads/). 

### VSCode
Developping (including running and building) unmanaged projects works just fine in VSCode, as long as you install the [Java Extension Pack by Red Hat](https://marketplace.visualstudio.com/items?itemName=vscjava.vscode-java-pack). As long as your JDK is correctly configured, VSCode should be able to find it ; in that case, you can simply open a Java source file (and notice the Eclipse-like project explorer opening on the left bar), press F5 to run it, and use the `>Java: Export Jar` command to build an executable Jar.  

If you do not wish to use VSCode as your IDE, you can use whatever editor you want and follow the the following build/run instructions.  

### Using Ant
- `ant run` will run the application.  
- `ant build` will build an executable Jar. It can be run with the `java -jar kuribrawl-editor.jar` command, or by double-clicking it on windows (or using the `start` command) if Java is properly installed.  
- `ant build-dependant` will buid an executable dependant Jar that requires the library jars located in the `lib` subdirectory (the libs must always exist in a `lib` subdirectory, relative to where the jar is started from).
- `ant dist` will make a zip file containining the dependant Jar and the `lib` subdirectory.  

### Using Make  
Coming soon

### Using nothing else than Java
Coming soon