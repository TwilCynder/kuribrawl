

# Format des fichiers resource

Spécification de la structure et du contenu des fichiers ressource utilisés pour générer le datafile.
Systèmes affectés par cette specification : 
- datafile maker
- editor

### Notes générales 
"Descripteur" aka "Info source file" = fichier texte (avec extention .dat) contenant des informations.  
Nom d'animation : alphanumérique `[a-zA-Z_0-9]` ou \w

## Liste des resources

Fichiers utilisés listés dans `project_db.txt`. Structure :

```
{
<filename>
<file_info>
}*
```

`file_info` est de forme 
`<Type>:<tag> <info>`  
Type étant une lettre indiquant le type de fichier (qui déterminera la manière dont DFM interprétera cette ligne et les champs info et encodera le fichier)  

## Animation 
Indicateur de type : **A**  

Le fichier est l'**image source** (contenant toutes les frames).  
Les frames doivent être alignées **horizontalement** placées "régulièrement" : l'image sera découpée en régions égales correspondant à chaque frames, toutes les frames prennent donc autant de place même si visuellement certains sont moins larges.

Sémantique de file_info :  
`A:[[<type de domaine>/]<domaine>/]<nom> {<nb_frames> [<speed> [c]] | <descriptor_filename>}`

- Type de domaine : voir plus bas
- Domaine : élément de gamplay auquel l'animation appartient (un champion, un stage, ...).
- descriptor_filename est le nom d'un fichier texte contenant des infos sur l'anim (Descripteur d'animation)

Contenu du descripteur d'animation

### Descripteur de fichier d'animation 
(un "-" = une ligne) : 

```
- <nb frames>
- [s<speed>] (vitesse de l'animation)
{
- f<n° de frame> [d<durée>] [o<x origine> <y origine>] [m[<x mode**>:<x value>],[<y mode**>:<y value>]] (infos de frame) |
- c{[[<n° de frame>] <x> <y> <w> <h> <Mode>* (info de hurtbox) | whole] | all } |
- h[<n° de frame>] <x> <y> <w> <h> <type> <infos spécifiques au type> (info de hitbox)
- l <n° de frame> {l [<durée>] (normal) | a <nom animation> [<durée>] (animation) | n (rien)} (comportement à l'aterrissage***)
- e helpless (Le fighter passe en helpless à la fin de l'animation)
}*
```

Infos de hitbox spécifiques au type (numéro de type entre parenthèses) : 
- Damage hitbox (0) : `<damage> <angle> <bkb> <skb> <hitID> <priority>`
- Grab hitbox (1) : unsupported
- Wind hitbox (2) : rien
- Special hitbox (3) : unsupported

(*) Hurtbox mode : 
- 0 = Normal
- 1 = Protected
- 2 = Intangible

(**) Frame movement mode : `[[s|w]]` ("s" indique set_speed, "w" indique whole_frame)

(***) Comportement à l'aterrissage : ce que fait le Fighter quand il aterrit pendant cette animation.  
- Normal : Passage à l'animaiton d'aterrissage par défaut
- Animation : Passage à une animation spécifique
- Rien : Rien ne se passe, l'animation continue au sol

### Types de domaines : 
- **Champion** : "Champion", "Champ" ou "C"
- **Stage** : "Stage", "S"

Sans type de domaine, on considère que le domaine est un Champion ; un domaine commençant par $ sera interprété comme nom de stage.

### Animation de background 
Toute animation dans un domaine de type Stage sera considérée comme une Animation de Background.  

Les animations de background sont sans intéraction avec les entités : tout élément d'interaction (voir plus bas)
génèrera une erreur.  


Éléments d'interaction :
- hitboxes
- hurtboxes
- frame movements

## LeftAnim
FICHIER LEFTANIM : **L**  
Ne sera pas encodé, n'existe que pour la legacy

## Champion
FICHIER DE CHAMPION : **C**.  
Tag interprété comme nom du champion :  
`C:champion_name`

Fichier texte interprété comme un Descripteur.

```
- <Nom d'affichage> 
- <attributs de gameplay séparées par espaces> 
```

attributs de gameplay : voir annexe

## Stage
Indicateur de type : **S**  
Tag interprété comme nom de stage :  
`S:stage_name`

Fichier texte interprété comme un Descripteur.

```
- <Nom d'affichage>
- <attributs de gameplay séparées par des espaces>
{
- p <x> <y> <width> [<animation>] (infos de plateforme)
- b <animation> [<x> <y> [<profondeur = 0>]]
}*
```

attributs de gameplay : voir annexe


## Image
Indicateur de type : I  
Tag interprété comme nom de l'image :  
`I:image_name`

Fichier image qui sera encodé tel quel et chargé comme texture, associé à un nom. Le nom de l'image peut déterminer son rôle, mais n'importe quel nom peut être utilisé.  


## Fichier divers

Fichier qui sera encodé tel quel, le tag correspond à son rôle. I est juste un cas spécial de X, qui a son propre type juste pour des questions de simplicité.


# Annexe 
Values with the mention `default : <value>`, or written in the form `<value_description=<value>>` will take the indicated value if replaced by a "x".

It is important to note that for every gameplay attribute indicating the duration of a state,  
a value of -1 means that the duration of that state's animation should be used.  
The default value (noted x) for most of those attributes is -1, even if not explicitly indicated.   

Attributs de gameplay champion : 
- 0 : Walk Speed
- 1 : Dash Speed
- 2 : Initial Dash Speed
- 3 : Dash Turn Acceleration (backwards acceleration aaplied during a dash turn)
- 4 : Dash stop Decceleration
- 5 : Traction (deceleration/backwards acceleration applied while on the ground)
- 6 : Max Air Speed (maximum air drift horizontal speed)
- 7 : Air Acceleration (acceleration applied when air drifting)
- 8 : Air Friction (deceleration/backwards acceleration applied when airborne)
- 9 : Jump Speed (full hop starting vertical speed)
- 10 : Short Hop Speed (short hop starting vertical speed)
- 11 : Double Jump Speed (double jump starting vertical speed)
- 12 : Grounded forward jump H speed
- 13 : Grounded backward jump H speed
- 14 : Air forward jump H speed
- 15 : Air backward jump H speed
- 16 : Gravity
- 17 : Max Fall Speed (maximum vertical speed when falling)
- 18 : Fast Fall Speed
- 19 : Weight
- 20 : Jumpsquat Duration (duration of the jumpsquat, in frames. Leave at 0 to set it to the jumpsquat animation length)
- 21 : Dash Start Duration (duration of the dash stop state. Leave at 0 to set it to the dashstop animation length)
- 22 : Dash Stop Duration (duration of the initial dash state. Leave at 0 to set it to the dashstart animation length)
- 23 : Dash Turn Duration (duration of the dash turn state. Leave at 0 to set it to the dashturn animation length)
- 24 : Landing duration (duration of the landing state. Leave to 0 to set it to the duration of the lading animation)  
- 25 : Shield Startup : number of frames between the frame you press Guard and the frame your shield appears.
- 26 : Shield Endlag : number of frames between the frame you release Guard (and your shield disappears) and the frame you can act again.  
- 27 : Shield size
- 28 : Shield x
- 29 : Shield y
- 30 : Jumps

Attributs de gameplay stage : 
- 0 : Blastzone w
- 1 : Blastzone h
- 2 : Camera limit box x offset (relative to a position where the camera limit box is centered in the stage)
- 3 : Camera limit box y offset (relative to a position where the camera limit box is centered in the stage)
- 4 : Camera limit box w
- 5 : Camera limit box h

## Motif
Motif de cette version : systèmes d'animations de stages 
