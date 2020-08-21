# Kuribrawl Modding Tutorial  
This guide will show you how to add content to Kuribrawl.  

## Intro
All the data of the game (informations about a character, animations, etc) is contained within the `data.twl` file. This file is built by the DataFileMaker.exe (DFM) executable, using files listed in the `project_db.txt` file.  
To add characters or stages, you will have to list all the realted to them (animation spritesheets, info files, etc) in this file, and then simply run DFM.

## project_db.txt
`project_db.txt` must contain a list of all files that will be used, following this syntax : 
```
path_to_file
Type:tag info
```
- `Type` is a short code (1/2 letters) indicating the type of the file 
- `Tag` is an identifier for the file. It may need to follow a specific sytax, depending on the file type.
- `info` is a string that will provide additional info, depending on the file type.

## Content
Just before we start, just a quick reminder on the *content* you are goind to add. There are 2 types of content you can add, i.e. :
- **Champions** : A champion is defined by a *descriptor file* containing all its informations (physical properties, landing lags, etc), and some *animation* files.
- **Stages** : A champion is defined by a *descriptor file* containing all its informations (size, camera zone, platform positions, etc), and some *animation* files (for the background and platforms).

## Files
Just a last thing before we start, about how to read this document. When describing text you are supposed to write, the syntax will be the following : 
- Text between `<>`: replace entirely (including the brackets) with the value described in the brackets.
- Text between `[]` : facultative.
- Text between `{}` : can be repeated any number of times.
- Text between `〔|〕` : Include *one* of the parts separated by a `|`.
### Champion descriptor
A champion descriptor is a text file containing all the informations of a champions that isn't already part of the info of one of its animations.  
Its description in the Project DB must follow this syntax : 
```
C:Tag
```
`Tag` is the name of the Champion. (note : the *name* is only the internal identifier, the name that will be displayed is the *display name*).

- The first line of a Champion Descriptor must contain its *display name*. If you want it to be the same as it's internal name, just leave the line empty (it is important that you still include the empty line, as the first line will be considered as the display name regardless of its content).
- The second line must contain all the *values* of this champions, separated by spaces. These values are (in this order): 
    - Walk Speed
    - Dash Speed
    - Initial Dash Speed
    - Dash Turn Acceleration (backwards acceleration aaplied during a dash turn)
    - Max Air Speed (maximum air drift horizontal speed)
    - Air Acceleration (acceleration applied when air drifting)
    - Traction (deceleration/backwards acceleration applied while on the ground)
    - Jump Speed (full hop starting vertical speed)
    - Jumpsquat Duration (duration of the jumpsquat, in frames. Leave at 0 to set it to the jumpsquat animation length)
    - Dash Stop Duration (duration of the dash stop state. Leave at 0 to set it to the dashstop animation length)
    - Dash Start Duration (duration of the initial dash state. Leave at 0 to set it to the dashstart animation length)
    - Dash Turn Duration (duration of the dash turn state. Leave at 0 to set it to the dashturn animation length)
    - Short Hop Speed (short hop starting vertical speed)
    - Double Jump Speed (double jump starting vertical speed)
    - Max Fall Speed (maximum vertical speed when falling)
    - Fast Fall Speed
    - Air Friction (deceleration/backwards acceleration applied when airborne)
    - Landing duration (duration of the landing state. Leave to 0 to set it to the duration of the lading animation)  

After the first 2 lines, all lines indicate facultative info, and can be included in any order.

- `〔m<id> | m:<move name>〕[l<land lag>]`
    - `id` is the numerical identifier of a move, `move name` is the text identifier of a move.
    - You can only specify the landing lag of an aerial move (aerials including ZAir, and specials if they are configured a certain way).
- `〔u<id> | u:<move name>〕{<part end frame> <part start frame>}` : make the specified move a *multimove* (a move with different parts, allowing you to stop at the end of one of the parts; typically a smash/rivals jab).
    - `id` is the numerical identifier of a move, `move name` is the text identifier of a move.
    - `part end frame` is the frame where a part ends, `part start frame` is the frame where the next part starts. The first part always starts at frame 0, the last part always stops on the last frame.  
    A multimove will always, by default, stop just before reaching the `part start frame` of a part unless you have pressed A during the last part. If you press A between the `part end frame` of the current part and the `part start frame` of the next frame, the game will instantly skip all frames until the said `part start frame`

Please note that the presence of a Champion Descriptor is not required to create the described champion : in fact, a champion is created as soon as anything related to it as added (animation or descriptor), the descriptor only configures it. In clear, it means that you do not need to include the Champion Descriptor before animations of this champion.

### Stage Descriptor
not yet lol

### Animation
An animation file is, in fact, a combination of two files : 
- An image file, called the spritesheet, containing all the frames of this animation (with the same dimensions), aligned horizontally
- In most cases, a descriptor file, containing all info on this animation (speed, hitboxes, etc)

Its description in the Project DB must follow this syntax : 
```
A:Tag info
```
The `Tag` is composed of the name of the champion or stage (with a `_` in front of it if its a stage) it is attacked to, and the name of this specific animation, separated by a slash : `Character/animName` or `_Stage/animName`.   
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

An animations marked with a ** indicates that it will be played on loop (with no limit of duration). It means they can be still still animations.  
An animation marked with a * indicates that it might be played for a set duration regardless of its length, generally the duration of the state it is associated with, in which case it is slowed down/accelerated to match this duration, but also that this duration can be set to depend on the animation (in which case the duration of the state is the length of the animatin). Typically, the initial dash animation will be played during a number of frames specified in the champions descriptor, but if the specified duration is 0, then the game will simply play the animation at its normal pace and the initial dash state's duration will be determined by the animation.

#### Animation descriptor
- The first line must be the number of frames of the animation.

All subsequent lines describe facultative info and can be omitted and included in any order.
- `s<speed>` : the speed of the animation. To undertand properly how animation speed works, it is important to separate *animation frames* (the actual frames of the animation) and *real frames* (which is a unit of *time*, one cycle of the game) ; by default an animation frame is displayed every real frame, but if the speed is set an animation frame can be displayed during several real frames before changing. This value can be either : 
    - An integer number, in which case this number will be the total number of frames  the animation will last, regardless of its number of actual frames (example : if a 3-frame animation has a speed of 13, its frames will be displayed during 4 or 5 frames so that the animation ends after 13 real frames).
    - A decimal number, in which case this number will be a multiplier for the animation speed. For example, if the speed of an animation is 0.25, it will be played 4 times slower (with one animation frame lasting 4 real frames.). In the end, the animation will last `Int(1 / speed)` real frames.