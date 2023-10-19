# kuribrawl
A platform fighter (fighting game with a platforming aspect) inspired mostly by Rivals of Aether, traditional fighters, and of course Super Smash Bros

Written in C++ with my own engine (only me, my gcc and the SDL library, which is, turns out, not the best idea i've had), the game aims to offer extensive Mod support (allowing anyone to create their characters, among other types of contents), using Lua scripts.

Gameplay-wise, Kuribrawl aims to be "fun while technical" : the game (and by that i mean a decent gaming exeprience, fun-wise) should be accessible for anyone without experience of fighting games, however the goal is to reward high levels of skill at platform fighters if the game is played with a competitive intent, with a high skill ceiling.

## Installing
Of course, proper packaged releases with a ready-to-run executable will be published in time. For now, kuribrawl can be built from its source code by following the [installation guide](./doc/internal/install-build.md#Kuribrawl)

## Project organization

The Kuribrawl project is made of multiple modules : the Game, the Datafile Maker, and the Editor. 

### Kuribrawl (the game)
The game itself (and its source code) is located in the `c++` directory. It is written in C++ (C++20 to be precise)

### Tools

Kuribrawl also comes with two tools, used to manage game files and data. These are **not** located in the same repository, but in a separate repository called [`kuribrawl-tools`](https://github.com/TwilCynder/kuribrawl-tools), which is a Git submodule (a repository *integrated* to another repo as a component) of this repo. To obtain it (and integrate it correctly into this repo) : 
- If you are using Git and haven't cloned this repo, specifying `--recurse-submodule` when you do (`git clone https://github.com/TwilCynder/kuribrawl --recurse-submodules`)
- If you already cloned, run `git submodule update --init`, `git pull` and then `git submodule update --recursive`.  
- If you are not using Git an downloaded the code as an archive, simply download [`kuribrawl-tools`](https://github.com/TwilCynder/kuribrawl-tools) as well and place it in the root directory of this repo, under the name `tools`. 

### Datafile Maker
The files of the game (i.e. animation source images, data files) are not read directly as-is by the game, but compiled into a single large file called the "datafile". The Datafile Maker's job is to turn the base files (called "source files") into this file. It can be used in command-line, but also comes with a simple UI. It is written in Purebasic.  
More information about its usage can be found in [its own readme](./tools/gamefile%20manager/readme.md), the specification of the source files can be found [here](./doc/internal/ressource%20file%20format/0.3.4.md) (french only for now) and a more comprehensive explanation of how they work can found in the [modding doc](./doc/public/modding.md).

### Kuribrawl Editor
A visual interface allowing the user to edit game data (mainly, the information contained in [descriptors](./doc/public/modding.md/#)). Written in java, it is compiled as a Jar archive, which requires java to run.  
A proper user manual will be provided """soon""". 