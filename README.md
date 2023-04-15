# kuribrawl
A platform fighter (fighting game with a platforming aspect) inspired mostly by Rivals of Aether, traditional fighters, and of course Super Smash Bros

Written in C++ with my own engine (only me, my gcc and the SDL library, which is, turns out, not the best idea i've had), the game aims to offer extensive Mod support (allowing anyone to create their characters, among other types of contents), using Lua scripts.

Gameplay-wise, Kuribrawl aims to be "fun while technical" : the game (and by that i mean a decent gaming exeprience, fun-wise) should be accessible for anyone without experience of fighting games, however the goal is to reward high levels of skill at platform fighters if the game is played with a competitive intent, with a high skill ceiling.

## Installing
Of course, proper packaged releases with a ready-to-run executable will be published in time. For now, kuribrawl can be built from its source code by following the [installation guide](./doc/internal/install-build.md#Kuribrawl)

## Project organization

The Kuribrawl project is made of multiple modules : the Game, the Datafile Maker, and the Editor. 

### Kuribrawl (the game)
The game itself 