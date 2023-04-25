# Kuribrawl Modding Tutorial  
This guide will explain the specification of the files used by the games, i.e. the *source files* used to make the Data File (which is in turn read by the game). This specification is available   

Since additional Data Files can be used to extend the content of the game, built from the same source files,  this guide will also show you how to add content to Kuribrawl.  

# Intro
All the data of the game (informations about a character, animations, etc) is contained within the `data.twl` file. This file is built by the DFM.exe (DataFileMaker) executable, using files listed in the `project_db.txt` file.   
These files include "media files" such as images and sounds, Lua scripts, and "Descriptor files", text files with a ".dat" extension that contain information following a [defined syntax](../internal/ressource%20file%20format/0.3.4.md).  
To add characters or stages, you will have to list all the files related to them (animation spritesheets, info files, etc) in this file, and then simply run DFM.

# Content
Just before we start, just a quick reminder on the *content* you are goind to add. There are 2 types of content you can add, i.e. :

- **Champions** : A champion is defined by a *descriptor file* (see below) containing all its informations (physical properties, landing lags, etc).
- **Stages** : A champion is defined by a *descriptor file* containing all its informations (size, camera zone, platform positions, etc)
- **Animations** : An animation, bound to a champion or a stage, is defined by an image and potentially a *descriptor file* containing all its information (speed, origin point, hitboxes for champion animations, etc).
- Lua **Scripts** that will be run at various stages of the game. (Not implemented yet)

### About animations and how Champions/Stages use them
To clarify : animations are usually bound to a *domain*, which is a champion or stage, but they are not defined *by* that domain, i.e. the descriptor or that champion or stage doesn't define a list of all its animation; rather, the animations are created as separate objects, that include their domain in their name.  

Animations can then be used by their domain through their name : 
- Some names have predefined uses : for example, for each player state there is a default animation name ; if there is an animation with this name it will be used for that state (this is oversimplified)
- Some informations in a descriptor may require an animation name : stage descriptors include.
- Lua scripts may refer to an animation by its name. 

# Files organization
All source files must be located in the same directory (and potentially subdirectories !)

## project_db.txt
A file named`project_db.txt`, at the root of the directory must contain a list of all files that will be used, following this syntax : 
```
path_to_file
Type:tag info
```
(repeated)  

# Files
Keep in mind that everytime a **file descriptor** is presented, its syntax can be found [here](../internal/ressource%20file%20format/0.3.4.md)

## Champion descriptor
A champion descriptor is a text file containing all the informations of a champions that isn't already part of the info of one of its animations. 

- The first line of a Champion Descriptor must contain its *display name*. If you want it to be the same as it's internal name, just leave the line empty (it is important that you still include the empty line, as the first line will be considered as the display name regardless of its content).
- The second line must contain all the *values* (numerical properties) of this champions, separated by spaces. These values can be found in the [appendix of the descriptor specification](../internal/ressource%20file%20format/0.3.4.md#Annexe)

It is important to note that two special values may be used :  
- `x` indicate the default value (this is the case for any value in a descriptor that has a default value, but all champion values have an implicit default value)
- for all states durations, `-1` indicates "the length of the corresponding animation".

After the first 2 lines, all lines indicate facultative info, and can be included in any order.

### Move

A **move** is *formally* defined as "A Champion-specific state tied to an action", which means an action that will put your fighter in a state that, unlike basic states like DASH or GUARD is specific to your champion. It can be as simple as an action that starts a specific animation, and includes, most importantly, what is commonly reffered to as "attacks" (in fact, atm the only moves used by the game are attacks).  
Moves have a name, and can be started from a lua script, however the game will by default start moves with certain names when a specific input is performed : for example, if a Champion has a move called `nair`, the game will start is automatically if you press the attack button, with the control stick in neutral position, while in the air (and your fighter is in a state that allows attacking).   
Moves can be referred to in two ways : 
- a numerical id (for built-in moves, i.e. the ones that are used natively by the engine)
- a name 

In the long term, moves will be able to completely redefine the behavior of a fighter while they are on (like any built-in state), but for now they just have a few properties : 
- **land lag** : When a Fighter lands on the ground, they are put in a landing state (and animation) for a certain duration. If you land during a move, this duration is the land lag property of the move in question.

### Note 

Please note that the presence of a Champion Descriptor is not required to create the described champion : in fact, a champion is created as soon as anything related to it as added (animation or descriptor), the descriptor only configures it. In clear, it means that you do not need to include the Champion Descriptor before animations of this champion.

## Stage Descriptor
A champion descriptor is a text file containing all the informations of a champions that isn't already part of the info of one of its animations.  
Its description in the Project DB must follow this syntax : 
```
S:Tag
```
`Tag` is the name of the Stage. (note : the *name* is only the internal identifier, the name that will be displayed is the *display name*).

- The first line of a Stage Descriptor must contain its *display name*. If you want it to be the same as it's internal name, just leave the line empty (it is important that you still include the empty line, as the first line will be considered as the display name regardless of its content).
- The second line must contain all the *values* of this stage, separated by spaces. These values are (in this order): 
    - Width
    - Height
    - Camera Zone X : x position of the camera zone. The camera zone is the area where the camera can move. Any object outside of this area will always be offstreen, but can still be in the stage. 
    - Camera Zone Y : y position of the camera zone (relative to the lower side of the stage. This means a y position of 200 will place the camera zone 200 pixels *above* the bottom of the stage.)
    - Camera Zone Width
    - Camera Zone Height

After the first 2 lines, all lines indicate facultative info, and can be included in any order.

- `p <x> <y> <width> [<animation>]` : adds a platform.
    - `x` and `y` will be the position of the upper left corner of the platform, relative to the lower left corner of the stage.
    - `width` the width of the platform.
    - `animationName`  must be the name of an animation of the stage.


## Animation
An animation file is, in fact, a combination of two files : 
- An image file, called the spritesheet, containing all the frames of this animation (with the same dimensions), aligned horizontally
- In most cases, a descriptor file, containing all info on this animation (speed, hitboxes, etc)

Its description in the Project DB must follow this syntax : 
```
A:Tag info
```
The `Tag` is composed of the name of the champion or stage (with a `_` in front of it if its a stage) it is attacked to, and the name of this specific animation, separated by a slash : 
`Character/animName` or `_Stage/animName`.   
The `info` if the name/path of the animation descriptor file. (*this file must has a .dat extension !*). The info can also be a simple number, in which case there will simply be no animation descriptor, and this number will be used as the number of frames of this animation.


The usage of the animation depends of its name : 
- For a stage, the animation named `background` will be the background of the stage.
    - All other animations have no particular use but can be used by platforms as their animation.
- For a champion, a lot of animation names are associated with a move or a state. Most of them are required (facultative animations are indicated in italics)
    - `idle` : Idle grounded animation. **
    - `airIdle` : Idle airborne animation **
    - `walk` : walk animation **
    - `dashstart` : initial dash animation *
    - `dash` : dash animation **
    - `dashstop` : dash stop animation *
    - `dashturn` : dash turn *
    - `jumpsquat` : jumpsqat animation (just before jumping) *
    - `jump` : *used when the champion jumps, and transitions on the airIdle animation when it ends. If not specified, the champion will just enter the airIdle animation on the start of the jump*.
    - `doublejump` : *double jump animation. If not specified, double jump will simply work like normal jumps*
    - `land` : Landing animation. *
    - `hurt` : Animation set when going in hitstun (note : when hitstun ends the animation stays until it is changed by an action). **
    - `hurtground` : *Same as `hurt` but for hits that don't make you leave the ground. Defaults to `hurt`* **
    - `hurtheavy` : *Same as `hurt` but for heavy hits (hits that send you in tubmle). Defaults to `hurt`* **
    - `hurtup` : *Same as `hurt` but for heavy hits that also send you upwards. Defaults to `hurt`* **
    - `hurtdown` : *Same as `hurt` but for heavy hits that also send you downwards. Defaults to `hurt`* **
    - `hurtbounce` : *Animation set when you hit the ground in hitstun and bounce off it. Defaults to `hurt`* **
    - `jab`
    - `ftilt`
    - `utilt`
    - `dtilt`
    - `nair`
    - `fair`
    - `bair`
    - `uair`
    - `dair`
    - `zair`
    - `nspecial`
    - `fspecial`
    - `dspecial`
    - `uspecial`
    - All animation with other names will have no use unless a script manually uses them.

An animations marked with a \*\* indicates that it will be played on loop (with no limit of duration). It means they can be still animations.  
An animation marked with a \* indicates that it might be played for a set duration regardless of its length, generally the duration of the state it is associated with, in which case it is slowed down/accelerated to match this duration, but also that this duration can be set to depend on the animation (in which case the duration of the state is the length of the animatin). Typically, the initial dash animation will be played during a number of frames specified in the champions descriptor, but if the specified duration is 0, then the game will simply play the animation at its normal pace and the initial dash state's duration will be determined by the animation. These animation can be still if they are played in a context where they have a set duration, but you need to be careful as they but will cause problem if they are ever played without a set duration. (example : since the `jumpsquat` animation is only ever played during jumpsquat, it can be still with no problem if the champion has a set jumpsquat duration).

### Animation descriptor
First, you must note that you *don't* really need to know any of the content of this part, as animation descriptors are integrally managed by the Editor (a guide on how to use this software will come soon). 

- The first line must be the number of frames of the animation.

All subsequent lines describe facultative info and can be omitted and included in any order.
- `s<speed>` : the speed of the animation. To undertand properly how animation speed works, it is important to separate *animation frames* (the actual frames of the animation) and *real frames* (which is a unit of *time*, one cycle of the game) ; by default an animation frame is displayed every real frame, but if the speed is set an animation frame can be displayed during several real frames before changing. This value can be either : 
    - An integer number, in which case this number will be the total number of frames  the animation will last, regardless of its number of actual frames (example : if a 3-frame animation has a speed of 13, its frames will be displayed during 4 or 5 frames so that the animation ends after 13 real frames). If this number is -1, the animation will be still.
    - A decimal number, in which case this number will be a multiplier for the animation speed. For example, if the speed of an animation is 0.25, it will be played 4 times slower (with one animation frame lasting 4 real frames.). In the end, the animation will last `Int(1 / speed)` real frames.

- `f<frame id> [d<duration>] [o<origin x> <origin y>] [m<speed mode> <vx> <vy>]` : properties of a specific frame
    - `frame id` is the frame numer (the number of the first frame is 0)
    - `duration` will be the durationof the frame, which means the time (in real frames) during which the frame will be displayed. Setting this value overrides the animation speed system (in fact, i advise never using frames with set durations in an animation with a set speed)
    - `origin x` and `origin y` is the position of the frame's *origin*. The origin of a frame is the point that matches the fighter's position : when displaying the frame, it will be placed so that the origin point is placed exactly on the fighter position.
    By default, the origin of a frame is placed at {width/2 ; height} (center bottom) for a champion, and {0;0} for a stage.
    - `speed mode` is a number indicating how *frame movement* works with this frame. `vx` and `vy` are the speed that will be applied. Frame movement causes a frame to change the speed of a fighter when it is displayed. This number must be written in decimal in the descriptor, but is interpreted as a binary number, bit-by-bit : every bit indicates a specific property of this frame's movement :
        - bit 1 (lsb) : indicates that frame movement is enabled. If it is 0, there will be no frame movement at all regardless of all other bits.
        - bit 2 : 0 means that the speed is added to the fighter's current speed, 1 means the speed replaces the fighter's current speed.
        - bit 3 : (for frames that last more than one real frame) 0 indicates that the speed is applied (added or set, depending on bit 2) only once, when the frame starts being displayed, 1 indicates that the speed is applied every frame until the animation goes on another frame.
        - bit 4 : disables x speed. If this bit is 1, only vertical speed will be applied.
        - bit 5 (msb) : disables y speed. If this bit is 1, only horizontal speed will be applied. (if both bits 4 and 5 are 1, frame movement has no effect)

- `c<frame id> <x> <y> <w> <h>` : creates a hurtbox on the specified frame, at the specified coordinates (relative to the origin)

- `h<frame id> <x> <y> <w> <h> <damages> <angle> [<hitID>] [<priority>]` : creates a hitbox on the secified frame, with the specified damage and angle.
    - `hitID` is the numerical identifier of the hit this hitbox belongs to. If an hitbox connects with another fighter that has already been hit by an hitbox with the same hitID, it will do nothing. This means that only two hitboxes with different hitIDs can both connect. For most moves hitID will always be 0, but different hitIDs allow for the creation of multihits.
    - `priority` if two hitboxes connect on the same frame, the hitbox with the highest priority is used.
    