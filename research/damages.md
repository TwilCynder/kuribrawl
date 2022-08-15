# Damage related technical aspects

## Knockback Formula

### In other PFs

|||
|-|-|
| p | percentage of the target (after damage application) |
| d | attack damage |
| w | target weight |
| s | scaling knockback of the attack |
| b | base knockback of the attack |
| r | other ratios |



- Super Smash Bros. 64  (SSB64)  
![LateX Equation (via Codecogs)](https://latex.codecogs.com/png.latex?{\color{White}\(\(\(\(\(\(\frac{p}{10}+\frac{p+d}{20}\)\times%20w\)\times1.4\)+18\)\times%20s\)+b\)\times%20r})  

- Super Smash Bros. Melee-Ultimate  (SSBM, SSBB, SSB4, SSBU)  
![LateX Equation (via Codecogs)](https://latex.codecogs.com/png.latex?{\color{White}\(\(\(\(\(\(\frac{p}{10}+\frac{p+d}{20}\)\times%20\frac{200}{w+100}\)\times1.4\)+18\)\times%20s\)+b\)\times%20r})

- Rivals of Aether (RoA)  
![LaTeX Equation (via Codecogs)](https://latex.codecogs.com/png.latex?{\color{White}b+\(p\times%20w\times%20s\times0.12\)})

### Weight
There are two main ways to handle weight : making it a multiplier to the knockback (in that case, a higher value means higher knockbacks, making the character *lighter*), or instead multiplying the KB by a multiplicative inverse of this value (in that case, a higher value reduces KBs, which means the character is *heavier*).  
SSB64 and RoA use the former method, which simplifies the formula but make the weight values pretty unintuitive, while Smash games from Melee onward use the latter (![LateX Equation (via Codecogs)](https://latex.codecogs.com/png.latex?\dpi{100}{\color{White}200/\(w+100\)}), which means a weight of 100 makes the multiplier 1).
#### Some values :

|Character|Game|SSB Weight|Multiplier|
|-|-|-|-
Lucina|Ult|90|1.05
Pichu|Ult|62|1.23
DK|Ult|127|0.88
Bowser|Melee|117|0.91
Marth|Melee|85|1.08
Puff|Melee|60|1.25
DK|64|140|0.83
Kirby|64|68|1.19
Ranno|Rivals|90|1.05
Kragg|Rivals|122|0.90
Wrastor|Rivals|66|1.20
Acid R.|Rivals|80|1.11

### Kuribrawl
The RoA formula seems particularily adapted, since it is, like Kuribrawl, a 2D game with fixed distances. That being said, it can be improved, and the SSBM weight system will probably be used.  
Also, this formula might be a little to linear (in a sense that very low KBs seem very possible at low %), and i might add a fixed base value, like the Smash formulas that add 18 before applying scaling KB.



It is unclear whether i am going to consider the target's damages after application of the attack damages (like in every other PF) or before (making the damage of the attack completely unrelated to KB if not directly included in the formula).

Temporary result :   
![LaTeX Equation (via Codecogs)](https://latex.codecogs.com/png.latex?{\color{White}b+\(\(p\times%20\frac{200}{w+100}+10\)\times%20s\times0.12\)})

## Hitstun