**But** : Intégrer un concept de profondeur pour les éléments de background des stages, et déterminer comment les positionner en fonction de la caméra afin de simuler la profondeur de manière réaliste.  

# Modèle conceptuel

## Espace 3D
On représente l'espace en trois dimensions.  

On introduit une **Caméra**, représentée par un point, une direction (vecteur) et deux angles : Cam(pos, dir, ax, ay) 
> angles : angle de vue / FOV horizontal et vertical. Généralement proportionels.

## Modèle 2D/3D

### Modèle 3D général

Soit C.dd la demi droite qui part d'une caméra C, et s'étend dans la direction de C.direction (C'est à dire, dd parallèle à C.direction, et pour tout point p de dd différent de C.position, (C.position->p) dot (C.direction) == 1)

On définit un **Plan de décor** vis à vis d'une caméra C un plan tq C.position ne se trouve pas sur le plan, et C.dd est perpendiculaire au plan (et le traverse). 
Pour des raisons de simplicité (et parce que le contexte 2D finira par nous l'imposer), on impose que tous les plans de décor sont parallèle au plan XY (et donc entre eux). Par conséquent, C.direction ne changera jamais.

On définit la propriété z d'un plan P de décor comme pp.z avec pp un point quelconque de P, tous les points de P ayant la même composante z.  
La distance entre C et un plan de décor quelconque P est |C.position.z| - |P.z|.

Pour des raisons de simplicité de l'explication, on s'intéresse ici uniquement aux positions et distances sur les axes X et Z (c'est à dire qu'on ne considère que le plan XZ), toute distance ou déplacement sur l'axe Y fonctionnant comme sur l'axe X. On peut donc ne considérer que deux dimensions des vecteurs en général,  et un seul angle de la caméra ; les plans parallèles à l'axe Y (c'est à dire tous les plans considérés ici) deviennent des droites. L'écran devient également un segment au lieu d'un plan.

On définit les **Limites de vue** horizontales d'une caméra C comme les deux demi-droites l1 et l2 partant de C.position, de direction d tq :
- l1 et l2 sont, comme tous les objets considérés ici, sur le plan XZ.
- l'angle entre li et C.direction est inférieur à 90°
- l'angle entre l1 et l2 est de C.ax
- C.dd est la bissectrice de l'angle (par conséquent, l'angle entre li et C.direction est de cx / 2).
D'un point de vue concret, il faut comprendre que tous les points situés entre l1 et l2 seront affichés à l'écran.

Soient bgauche(C, P) et bdroite(C, P) les intersections entre P et C.l1 et C.l2 respectivement.  
On définit une **Projection de caméra** C sur un plan P Pr(C, P) comme le segment entre bgauche et bdroite. Par définition de C.l1 et C.l2 (et par extension, de la caméra), tout point de Pr(C, P) sera affiché à l'écran.

Soit SW la largeur de l'écran, en pixels.  
On définit **position relative à l'écran** d'un point pp, pr(C, pp), tq pour P' le plan de décor passant par pp, pr = (pp.x - bgauche(C, P').x) / (bgauche(C, P').x - bdroite(C, P').x). On note que pr app [0, 1] <=> pp est entre C.l1 et C.l2, donc un point n'est affiché que si sa position relative app [0, 1] ; pp est affiché sur l'écran à la position px(pp) = pr(C, pp) * SW.

D'après les définitions de pr, bgauche et bdroite, et avec w(C, P) la longueur de la projection Pr(C, P), c'est à dire |bgauche(C, P')| - |bgauche(C, P')| on peut déterminer que  
pr(C, pp) = (pp.x - C.position.x + W(C, P')/2) / W(C, P')   

On peut également noter que W(C, P') = 2 * dz(C, P) * tan(C.ax / 2), avec dz(C, P) = |C.position.z - P'.z|.

### Parallax

Dans une hypothèse de pure 2D (= sans parallax) tous les éléments se trouvent sur le même plan de décor P0.

- La caméra qu'on utilsera ici sera toujours telle que P0 est un plan de décor. C'est la raison pour laquelle on a imposé plus tôt que tous les plans de décor soient parallèles au plan XY : au final ils doivent être parallèles à P0 de toute façon. 
- On note z0 la valeur de P0.z, et w0 la valeur de W(C, P0)

On est également dans une hypothèse de jeu sans zoom/dézoom, c'est à dire que les éléments de P0 seront toujours affichés à la même taille ; la zone couverte par l'écran est toujours de même taille. Cela veut dire que la taille de la projection de C sur P0, W(C, P0) ne changera jamais.  

> On note que cela signifie que d'un point de vue technique, tant que l'environement de jeu est entièrement 2D, la position des éléments à l'écran est en réalité triviale : 
> - comme on n'a pas de profondeur, on peut ne pas considérer la caméra mais seulement sa projection, c'est à dire le rectangle dont tout contenu sera affiché sur l'écran
> - comme on n'a ni zoom ni dézoom, on peut partir du principe que la taille de cette projection ne change jamais, donc ne retenir que la position de la caméra.
> Il nous suffit alors de décider que la représentation de la caméra dans l'environement de jeu, est égale à SW, pour arriver à la formule suivante pour la position à l'écran d'un point pp : px = (pp.x - C.position.x + SW / 2).

On ajoute maintenant d'autres plans de décor sur lesquels on veut pouvoir afficher des éléments (qui seront pour l'instant représentés par des points).  
Premièrement, on veut que l'hypothèse de non-zoom soit maintenue pour tous les plans.  
Cela veut dire que W(C, P) doit rester identique *pour tout plan*. D'après la formule de W(C, P) en fin de section précédente, cette taille dépend de dz(C, P) et de C.ax. Ces deux paramètres doivent donc rester fixes pour chaque plan. Par souci de simplicté on fixe C.position.z à 0 précisément. La formule de W(C, P) devient donc W(C, P) = 2 * P.z * tan(C.ax / 2). 

Maintenant, on voudrait deux choses : 
- Simplifier la formule de la position à l'écran d'un point pp (qui a l'heure actuelle implique un nombre important de produits/divisions et une tangente)
- Maintenir, pour P0, la formule triviale utilisée dans l'hypothèse de non-profondeur et non-zoom originale, x = (pp.x - C.position.x + SW / 2).

Afin de parvenir au second point, la condition était que la représentation de la caméra dans l'environement 2D, c'est à dire la projection de C sur P0, soit de la même taille que l'écran. Pour arriver à cette équation, il faut donc que
```
w0 = SW <=> 
2 * z0 * tan(C.ax / 2) = SW <=>
```    
Les inconnues ici sont C.ax et z0, et leur valeur ne nous intéresse en fait pas. On garde simplement en tête que leur valeur (qui reste fixe comme stipulé plus haut) est telle que w0 = SW.  

On veut maintenant savoir quelle est la valeur de px(C, pp) pour un pp quelconque hors P0. Et il s'avère que pour cela nous n'avons même pas beosin de la valeur de C.ax. En effet, on peut maintenant définir W(C, P) à partir de w0.  

```
| W(C, P) = 2 * P.z * tan(C.ax / 2)
| w0 = 2 * z0 * tan(C.ax / 2)
<=>
| tan(C.ax / 2) = W(C, P) / (2P.z)
| tan(C.ax / 2) = w0 / 2z0
<=>
W(C, P) / (2P.z) = w0 / 2z0
<=>
W(C, P) / P.z = w0 / z0
<=>
W(C, P) = w0 * P.z / z0
```

Plus besoin de calculer W(C, P) à partir de l'angle C.ax, on peut utiliser une relation de proportionalité avec w0 et z0 !

Par conséquent, pour tout pp,  
pr(C, pp) = (pp.x - C.position.x + (SW * pp.z / (2 * dz0))) / (SW * pp.z / dz0).  

Afin de simplifier cette formule, on fixe dz0 à 1 (ce qui par extension fixe C.Ax à 45° mais osef)  

pr(pp) = (pp.x - C.position.x + (SW * pp.z) / 2) / (SW * pp.z) 

Par définition de pr, la position à l'écran est  
x = ((pp.x - C.position.x + (SW * pp.z) / 2) / (SW * pp.z) ) * SW = (pp.x - C.position.x + (SW * pp.z) / 2) / pp.z    
= (pp.x - C.position.x) / pp.z + ((SW * pp.z / 2) / pp.z)  
**= (pp.x - C.position.x) / pp.z + SW / 2**


## Old

Dans une hypothèse de pure 2D (= sans parallax), tous les éléments se trouvent sur le même plan P0, parallèle aux axes X et Y.    

Soit C.dd la demi droite qui part d'une caméra C, et s'étend dans la direction de C.direction (C'est à dire, dd parallèle à C.direction, et pour tout point p de dd différent de C.position, (C.position->p) dot (C.direction) == 1)

On définit un **Plan de décor** vis à vis d'une caméra C un plan tq C.position ne se trouve pas sur le plan, et C.dd est perpendiculaire au plan (et le traverse).  

- La caméra qu'on utilsera ici sera toujours telle que P0 est un plan de décor. La direction C.direction dépend donc entièrement de P0 et de la position de C par rapprort à P0. 
- La caméra sera toujours du même côté de P0 (c.position.z > 0), donc une seule C.direction possible.  

C.position.z est la distance entre C et P0.  

On s'intéresse ici uniquement aux distances sur les axes X et Z, toute distance ou déplacement sur l'axe Y fonctionnant comme sur l'axe X. On peut donc ne considérer que deux dimensions des vecteurs en général, une seule dimension du plan P0, CàD une droite, et un seul angle de la caméra. Autrement dit, tout ce qu'on va définir par la suite se trouve sur le plan perpendiculaire à Y par lequel passe X

On définit les **Limites de vue** horizontales d'une caméra C comme les deux demi-droites l1 et l2 partant de C.position, de direction d tq :
- l'angle entre li et C.direction est inférieur à 90°
- l'angle entre l1 et l2 est de C.ax
- La droite d passant par C.position et parallèle à C.direction est la bissectrice de l'angle  

Soient bgauche et bdroite les intersections entre P et C.l1 et C.l2 respectivement.  
On définit une **Projection de caméra** C sur un plan P comme le segment entre bgauche et bdroite.  

La longueur de la projection de C sur P0 est la largeur de l'écran ; tout point de P0 situé sur cette projection sera affiché à l'écran.  

Soit W la largeur de l'écran en pixels.  
On définit la **Position relative** d'un point pp (aux coordonneés exprimées en pixels) sur l'écran comme pr de [0, 1] tq pr = pp / W.  

Un point p situé sur la projection de C sur un plan de décor P (comme P0) est affiché sur l'écran à la position relative |(p.x - bgauche.x) / (bgauche.x - bdroite.x)|. 

On est dans une hypothèse de jeu sans zoom/dézoom, c'est à dire que les éléments de P0 seront toujours affichés à la même taille ; la zone couverte par l'écran est toujours de même taille. Cela veut dire que la taille de la projection de C sur P0 ne changera jamais. Cette taille dépend (pythagore TMTC) de l'angle C.ax et de la distance entre P0 et C, c'est à dire C.position.z. On décide donc que ces paramètres ne changeront jamais.  
Afin de simplifier, on décide donc, puisque la distance entre bgauche et bdroite ne changera jamais, que l'unité de distance dans notre repère est telle que |bgauche.x - bdroite.x| == W. Ainsi, un point p de la projection est affiché à la position (en pixels) |p.x - bgauche.x|.  

On sait maintenant comment sont affichés les objets et distances de P0 sur l'écran, au travers du modèle de la caméra (l'écran étant la projection de la caméra sur P0). L'idée du parallax est d'introduire d'autres plans de décor (rappel : tout plan de décor est perpendiculaire à C.dd, donc parallèle à P0) ; il nous faut définir à quelle position sur l'écran sont affichés les objets situés sur ces autres plans de décor.  

# Implémentation
Drawer déjà fait, voir pour la gestion de composants statiques/dynamiques dans la profondeur.  

Idéalement : 
- Chaque drawable a une profondeur (statique), un layer et un order (dynamiques)
- Le drawer est une map de listes, qui sont retriées à chaque frame