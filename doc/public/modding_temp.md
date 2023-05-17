# Kuribrawl Modding Tutorial  
This guide will explain the specification of the files used by the games, i.e. the *source files* used to make the Data File (which is in turn read by the game). This specification is available   

Since additional Data Files can be used to extend the content of the game, built from the same source files,  this guide will also show you how to add content to Kuribrawl.  

# Intro
All the data of the game (informations about a character, animations, etc) is contained within the `data.twl` file. This file is built by the DFM.exe (DataFileMaker) executable, using files listed in the `project_db.txt` file.   
These files include "media files" such as images and sounds, Lua scripts, and "Descriptor files", text files with a ".dat" extension that contain information following a [defined syntax](./resource-format-latest.md#annexe).  
To add characters or stages, you will have to list all the files related to them (animation spritesheets, info files, etc) in this file, and then simply run DFM.

Just before we start, just a quick reminder on the *content* you are goind to add. There are 2 types of content you can add, i.e. :

- **Champions** : A [champion](#champion) is defined by a *descriptor file* (see below) containing all its informations (physical properties, landing lags, etc).
- **Stages** : A [stage](#stage) is defined by a *descriptor file* containing all its informations (size, camera zone, platform positions, etc)
- **Animations** : An [animation](#animation), bound to a champion or a stage, is defined by an image and potentially a *descriptor file* containing all its information (speed, origin point, hitboxes for champion animations, etc).
- Lua **Scripts** that will be run at various stages of the game. (Not implemented yet)

### About animations and how Champions/Stages use them
To clarify : animations are usually bound to a *domain* (an animation pool), which is a champion or stage, but they are not defined *by* that domain, i.e. the descriptor for that champion or stage doesn't define a list of all its animation; rather, the animations are created as separate objects, that include their domain in their name.  

Animations can then be used by their domain through their name : 
- Some names have [predefined uses](#default-animations) : for example, for each player state there is a default animation name ; if there is, in the animation pool of a champion, an animation with the name bound to a state, it will be used when the a fighter based on that champion enters this state (this is oversimplified)
- Some informations in a descriptor may refer to an animation by its name.  
- Lua scripts may refer to an animation by its name. 

When the game is running, a currently displayed animation can be "owned" by an entity : this means that the animation is bound to this entity (will generally move with it etc), and may affect the behavior of the entty in some ways, or vice-versa

# Files organization
All source files must be located in the same directory (and potentially subdirectories !)

## project_db.txt
A file named`project_db.txt`, at the root of the directory must contain a list of all files that will be used, following this syntax : 
```
path_to_file
Type:tag info
```
(repeated)  

# Content and Files

## Champion
A Champion is a "character" : it contains all the information that defines how a character is played. Keep in mind that the entity controlled by the player in-game, with an evolutive *state*, is called a Fighter ; a champion can be seen as all the character-dependant information a Fighter will be initialized with (physics values, animations, etc). In kuribrawl terminology, we say that the Fighter controlled by a player who selected a specific Champion is an instance of this Champion. 

A champion is an [animation domain](#about-animations-and-how-championsstages-use-them) : animations can be bound to a champion, which basically mean that they are put in the animation pool of that champion.  

A champion descriptor is a text file containing all the informations on a champions that isn't already part of the info of one of its animations.

- The first line of a Champion Descriptor must contain its *display name*. If you want it to be the same as it's internal name, just leave the line empty (it is important that you still include the empty line, as the first line will be considered as the display name regardless of its content).
- The second line must contain all the *values* (numerical properties) of this champions, separated by spaces. These values can be found in the [appendix of the descriptor specification](./resource-format-latest.md#annexe)

It is important to note that two special values may be used :  
- `x` indicate the default value (this is the case for any value in a descriptor that has a default value, but all champion values have an implicit default value)
- for all states durations, `-1` indicates "the length of the corresponding animation".

After the first 2 lines, all lines indicate facultative info, and can be included in any order.

### Move

A **move** is *formally* defined as "A Champion-specific state tied to an action", which means an action that will put your fighter in a state that, unlike basic states like DASH or GUARD is specific to your champion. It can be as simple as an action that starts a specific animation, and includes, most importantly, what is commonly reffered to as "attacks" (in fact, atm the only moves used by the game are attacks).  
Moves have a name, and can be started from a lua script, however the game will by default start moves with certain names when a specific input is performed : for example, if a Champion has a move called `nair`, the game will start it automatically if you perform a Neutral Air input (press the attack button, with the control stick in neutral position, while in the air) and your fighter is in a state that allows attacking. See [the appendix](#default-moves) for more information.  

In the long term, moves will be able to completely redefine the behavior of a fighter while they are on (like any built-in state), but for now they just have a few properties : 
- **land lag** : When a Fighter lands on the ground, they are put in a landing state (and animation) for a certain duration. If you land during a move, this duration is the land lag property of the move in question.

### Multimove
/!\ NOT IMPLEMENTED YET

### Note 

Please note that the presence of a Champion Descriptor is not required to create the described champion : in fact, a champion is created as soon as anything related to it is added (animation or descriptor), the descriptor only configures it. In clear, it means that you do not need to include the Champion Descriptor before animations of this champion.

## Stage
Stages are the arenas where games take place. Note that there is a subtle difference between a "Stage", the environement in-game, with a state that can evolve (platforms can move, etc), and a "Stage Model", often referred to simply as "Stage" too depending on the context, which is the model for in-game stages, containing all the constant/static information about a playable stage. In the context of game files, it is of course what we are talking about.  

A stage is an [animation domain](#about-animations-and-how-championsstages-use-them) : animations can be bound to a stage, which basically mean that they are put in the animation pool of that stage.  

A stage descriptor is a text file containing all the informations on a stage.

- The first line of a Stage Descriptor must contain its *display name*. If you want it to be the same as it's internal name, just leave the line empty (it is important that you still include the empty line, as the first line will be considered as the display name regardless of its content).
- The second line must contain all the *values* of this stage, separated by spaces. These values are (in this order): 
    - **Width** : width of the stage (or, following the smahsy way of putting it, of its blastzone). Any fighter getting out of the limits dies.
    - **Height** : height of the stage
    - **Camera Zone X offset** : the camera zone is the area in which the camera may move ; anything outside of this area is never on-camera. By default it is centered on the stage, this value allows you to offset the camera zone horizontally. 
    - **Camera Zone Y offset** : remember it's an upwards Y position, meaning a positive value will place the camera zone higher. 
    - **Camera Zone Width**
    - **Camera Zone Height**

After the first 2 lines, all lines indicate facultative info, and can be included in any order.

### Platforms
Currently, only traversable platforms can be made. 

Platforms are defined by
- a position, which is the position of their **center point**, the center of the line on which fighters may stand, relative to the center of the stage. 
- a width (keep in mind that since the position is the center point, the platform will extend to a distance of width / 2 in both directions from that position)

Platforms *may* also have an animation. You specify the animation of a platform through its name only ; the animation itself must already be defined, and be one of the animations of the Stage (see [Animations](#animation))   
Keep in mind, again, that the position of the platform is the center of the ground they make, which is generally at the top of their sprite ; because the origin point of the animation will be displayed at this position. 

### Background element
A stage background element is simply an animation that is displayed on the stage and doesn't interact with anything. It has a **position** relative to the center of the stage, and of course an animation (concretely, an animation name ; see above).  

These elements may also have a **depth**, which enables a "Parallax" display, which means that the element will be displayed (relative to camera movements) as if it was farther (or closer) from the camera than the actual entites (fighters, platforms, etc). Keep in mind that no matter this value, the *size* of the element will not be changed. This means that for example, if your stage features a huge building far away in the background, meaning it will appear small due to the distance, you need to make the sprite already small. The **depth** parameter will only affect how the sprite moves when the camera moves, to create a fake perspective.  
- A value of 1 means on the same depth as the stage (you can omit depth if its 1). 
  - A value of 2 means twice as far from camera as the stage
  - A value between 0 and 1 means that the element is between the camera and the stage (meaning it can hide parts of the stage from view, so be careful, as it can reduce the visibility of the game on your stage !)
- A value of -1 means "infinity" : a distance so great that the element does not ever move on the screen. For example, if you have a sunny background, and the sun is a standalone background element, it should have a deth of -1. 

## Animation
An animation may be defined by two files : 
- An image file, called the spritesheet, containing all the frames of this animation (with the same dimensions, i.e. taking the same amount of space no matter the the size occupied by non-transparent pixels), aligned horizontally.
- In most cases, a descriptor file, containing all info on this animation (speed, hitboxes, etc)

The tag of an animation holds special meaning. Indeed, an animation may (and in most cases will) be bound to a domain : see [this section](#about-animations-and-how-championsstages-use-them). See the [specification](./resource-format-latest.md#animation) for more info on how the domain of an animation is defined.

Note that you *don't* really need to know any of the content of this part, as animation descriptors are integrally managed by the Editor (a guide on how to use this software will come soon). 

The "info" part that follow the animation tag in the file list may be a file name, ending in `.dat`, in which case this file will be used as the descriptor ; or a few informations if they are the only ones needed
- a frame number (the only mandatory info an animation will ever require)
- a speed (see below)

Information about an animation include : 

### Animation speed
#### About time units and what "frame" means
In videogames, especially fighting games, there are 2 very different meaning associated with the word "frame"
- The common meaning is a frame *of an animation*, one of the images the animation is made of. In this document this will be referred to as "frame".
- In fighting games, it can also be a frame *of the game*, one "display cycle", either the point of time where the game is redrawn or the time between two consecutive redraws. In this case, "frame" is a time unit. The difference can be confusing, because even at normal speed a single frame *of an animation* can be displayed during several frames *of the game*. Therefore, in this document the latter will be referred to as "cycles". 

One last thing, sometimes, the "duration" of a frame means "the time (all the cycles) during which this animation frame is displayed". Example : if we're already talking about a frame of an animation, "during the entire frame" means "as long as this frame is the one being displayed"

#### Back to animation speed

The speed of an animation, which is a simple number, may hold different meanings depending on its value. 
- A speed of 0 or -1 means the animations is still (if it has more than one frames, only the first one will ever be displayed)
- An integer >= 1 means the total duration of the animation, in cycles, of the animation.  
Keep in mind that this behavior (which only allows evenly-distributed duration across all frames of the animation) can be overriden by the "duration" property of individual frames.

<br><div style = "border-top: solid 1px; width: 50%"></div><br>

After the animation speed, you may add any number of lines, each one describing the properties of a certain element of the animation, as [specified here](./resource-format-latest.md#animation)

### Frames
To refer to a specific frame, for example at the beginning of a frame properties line (`f<frame number> {properties ...}*`), you need to specify its number : the number of the first frame is 0.  

The properties that can be defined for a frame are : 

- Duration : duration of the frame in [cycles](#about-time-units-and-what-frame-means)
- Origin : A point on the frame, relative to the upper left corner, used to position the frame when it is displayed. When an animation is displayed, it is displayed at a precise point (for example, the animation of a fighter is displayed at the position of that fighter, which is its "ground position", the point used to detect collision with the ground. So basically, supposed to be at their feet). The actual image, the current frame of the animation, which is obviously larger than a single point/pixel, is displayed in a way such that the point on the frame that is defined as the origin of that frames, appears at the precise position the animation is being displayed at.  
  Note that if you don't specify the origin, it will automatically be placed at [width / 2; height] (middle of the bottom border)
- Frame movement : Modifies the speed of the entity owning the animation when this frame is displayed. Will only work in a context where the animation is owned by an entity compatible with this mechanic : currently it only includes fighters.  
This property is separated in two components, which are two "words" separated by a comma, for the x and y speed, which can work independently. 
    - An "s" at the beginning indicates that the entity's speed is "set" to the given value ; no "s" indicates that the speed is added.
    - A "w" indicates that the modification occurs every cycle for the duration of the frame (no matter if the speed is added or set) ; if these is no "w" the modification only occurs once when the frame starts.  
    - Then you always have a number, which is the speed in pixels/cycle

There are also elements that only exist on a specific frame, but they are not defined in a frame properties line, but in their own line. 

### Collision boxes
Collision boxes are specific areas on a frame (and only on a specific frame) that can interact with the collision boxes of another fighter in different ways. There are two types of collision boxes : hurtboxes "c" and hitboxes "h" (you can't just add a "collision box" to a frame, it has to be a hurtbox or a hitbox)

In the context of the descriptor, you can define which frame a hutbox belongs to by either specifying the frame's index after the "c" indicator ( `c4 ...` to make a hurtbox on frame 4 (which is the fifth frame, remember frame numbers start at 0)), OR by not specifying any frame number on the hurtboxe's line, in which case it will be bound to the last frame to be referred to. For example, 

```
f4 o80 100
c 0 0 40 40
```
Or 
```
c4 20 20 100 10
c 0 0 40 40
```
will both add hurtboxes to frame 4.  

Currently, collision boxes can only be rectangular. As such, a collision box line always starts with rectangle coordinates : x y position *relative to the upper-left corner*, width, height.

### Hurtboxes
Hurtboxes are areas where a fighter can "get hit", which means that they will interact with other entities hitboxes.  

Besides the rectangle coordinates, a hurtbox can indicate a type. "Normal" (0) is the most common, while the other indicate an invincible hurtbox. 
- Protected (1) means that hitboxes colliding with this hurtbox will result in a hit that does no damage or knockback (still apllies hitlag and some other things)
- Intangible (2) means that this hurtbox will not collide with this hurtbox (so it's almost like there is no hurtbox, it only exists for very specific purposes, such as scripts being able to change the type of a hurtbox instead of having to remove them)

### Hitboxes
Hitboxes are areas of a frame that  may damage entities. 

Besides the rectangle coordinates, a hitbox must indicate a type, and then information specific to that hitbox type.
- Normal hitboxes (0) will simply hit entities when colliding with their hurtboxes. This means dealing damage and knockback, and putting them in a "Hurt" state for a certain number of frames.
  - **damage** is the amount of damage dealt
  - Knockback sends the target in a certain direction given by **angle**, with a speed based on a base value (base knockback, **bkb**) and the current damage of the target multiplied by a factor called scaling knockback (**skb**).
  - Hitboxes belong to a "hit" : multiple hitboxes in the same "hit" will hit a given target only once (if your character hits once on the animation but that hit is represented by many hitboxes, on the same frame or not, you don't want the same enemy actually getting hit twice because 2 hitboxes collided). Hits are represented by a number, called **hitID**
  - If multiple hitboxes in the same hit connect on the same cycle, the one with a higher **prio** is used. If there is a tie (well first if there is a tie and the hitboxes are not identical that's bad design) it is unspecified which hitbox will actually hit.
- Grab hitboxes (1) are currently unsupported
- Wind hitboxes (2) or windboxes don't "hit" targets, they simply "push" them (give them a certain amount of speed in a given direction, added to their current speed)
- Special hitboxes (3) are currently unsupported

<br><div style = "border-top: solid 1px; width: 50%"></div><br>

# Appendix

## Default moves
When a player performs certain inputs, the game may start a move with a certain name, if it exists. Here is a complete list (`move name` : Input)
- `dashattack` : A while in dash/run
- `jab` : A on the ground with control stick in neutral position
- `fstrong` : A on the ground after quickly flicking* the control stick left or right ** 
- `dstrong` : A on the ground after quickly flicking* the control stick down ** 
- `fstrong` : A on the ground after quickly flicking* the control stick up **
- `ftilt` : A on the ground with the control stick tilted left or right **
- `dtilt` : A on the ground with the control stick tilted down **
- `utilt` : A on the ground with the control stick tilted up **
- `nair` : A in the air with the control stick in neutral position
- `fair` : A in the air with the constrol stick tilted forward (in the direction the fighter is looking at) **
- `bair` : A in the air with the control sttick tilted backward (in the direction the fighter is not looking at) **
- `uair` : A in the air with the constrol stick tilted up **
- `dair` : A in the air with the constrol stick tilted down **
- `nspecial` : B with the control stick in neutral position
- `fspecial` : B with the control stick tilted left or right ***
- `uspecial` : B with the control stick tilted up ***
- `dspecial` : B with the control stick tilted down ***

(*) "Quickly flicking" a stick means tilting it entirely from neutral position in a short time. This time can be configured in the player controller configuration  
(**) If on the player's input mapping, "Attack" is mapped to the second stick, tilting this stick will trigger a `tilt` move depending on the direction, without the need for a A press. If "Strong attack" is mapped to this stick, tilting it will trigger a `strong` move depending on the direction, without the need for a A press or any quick flick. In both cases, tilting this stick while in the air will trigger a `air` move, depending on the direction.    
(***) If on the player's input mapping, "Special" is mapped to the second stick, tilting this stick will trigger a `special` move.

## Default animations
As explained in [this part](#about-animations-and-how-championsstages-use-them), champions and stages have a number of default animations that will be displayed automatically by the game in some situations. More precisely, for each type of animation domain, there are *names* that are bound to a special situation, and if there is an animation with this name in the animation pool ("domain") of this entity, it is used for this situation. Some of these are mandatory (no animation with this name results in an error), some of them are facultative and default to another animation. 

In the following lists, if an animation is facultative, the animation displayed instead is indicated ; no such indication means that the animation is mandatory.

### For champions
  - `idle` : Idle grounded animation. *
  - `air_idle` : Idle airborne animation *
  - `walk` : Walking animation *
  - `dash_start` : initial dash animation
  - `dash` : dash animation *
  - `dash_stop` : dash stop animation 
  - `dash_turn` : dash turn 
  - `jumpsquat` : Jumpsquat animation (state a Fighter stays in for a short, character dependant duration before leaving the ground wehn jumping) 
  - `jump` : used when the fighter jumps from the ground, and transitions on the `air_idle` animation when it ends (leaving the ground during a ground jump instantly puts a fighter in the idle/free state, so the the fighter is already in this state when this animation plays, the animaiton transition has no effect gameplay-wise and does not denote a change of state). If not specified, the champion will just enter the airIdle animation on the start of the jump (Defaults to `air_idle`)
  - `jump_f` and `jump_b` : work like `jump`, used when jumping forward of backward. (Both default to `jump`)
  - `air_jump` : Air jump, aka double jump, used when a fighter jumps while in the air. Transitions to `air_idle` (note that, just like for ground jumps, air jumps instantly puts the fighter in the idle state, so when this animation plays the fighter is already idle/free, the transition has no effect gameplay-wise) (Defaults to `jump`)
  - `air_jump_f` and `air_jump_b` : Work like `air_jump`, used when air jumping forward or backward. (Both default to air_jump)
  - `land` : Landing animation.
  - `guard_start` : Guard startup animation
  - `guard` : Guard *
  - `guard_stop` : Exiting the guard state
  - `hurt` : Animation set when going in non-tumble hitstun (when you get hit but not too hard). When the hitstun state ends, the fighter is put in the idle state but the animation is set to `air_idle_after_hurt`. *
  - `air_idle_after_hurt` : Used when the fighter becomes actionable again after exiting the hitstun state. (Defaults to `hurt`) *
  - **All moves** have a corresponding default animation, that will be started when that move is started. 
  - All animation with other names will have no use unless a script specifically uses them.

- ( * ) Animations marked with a * will be played on loop if they are not still. They are typically used in a situation for which we don't know *when* it will end (such as a state that ends only when the player does something).
- Animations with the mention "transitions to" will transition to another animation once they end, in most cases without any change gameplay-wise
- Other animation will be used in a situation that has a "predicatble ending", which means that when it starts we know when it's supposed to end (unless it is somehow canceled). These animations generally last for the entire duration of the state they are used for
  - By default, the animation is what decides the length of the state (state ends when animation ends)
  - However, if the "duration override" mention is present the state  will last for the specified amount of time, regardless of the speed of the animation (which will be slowed or accelerated to match the duration). When that duration is the name of a Champion value (the numerical properties of champions, [see here](#champion)), that champion value may be set (in the descriptor, so by you !) to -1, in which case there will be no fixed duration, and the animation decides the length of the state

