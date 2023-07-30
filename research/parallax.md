**But** : Intégrer un concept de profondeur pour les éléments de background des stages, et déterminer comment les positionner en fonction de la caméra afin de simuler la profondeur de manière réaliste.  

## Modèle conceptuel
### Espace 3D
On représente l'espace en trois dimensions.  

On introduit une **Caméra**, représentée par un point, une direction (vecteur) et deux angles : Cam(pos, dir, ax, ay) 
> angles : angle de vue / FOV horizontal et vertical. Généralement proportionels.

### Modèle 2D/3D

#### Modèle général
Soit C.dd la demi droite qui part d'une caméra C, et s'étend dans la direction de C.direction (C'est à dire, dd parallèle à C.direction, et pour tout point p de dd différent de C.position, (C.position->p) dot (C.direction) == 1)

On définit un **Plan de décor** vis à vis d'une caméra C un plan tq C.position ne se trouve pas sur le plan, et C.dd est perpendiculaire au plan (et le traverse). 
Pour des raisons de simplicité (et parce que le contexte 2D finira par nous l'imposer), on impose que tous les plans de décor sont parallèle au plan XY (et donc entre eux). Par conséquent, C.direction ne changera jamais.

La distance entre C et un plan de décor quelconque P est |C.position.z - pp.z|, avec pp un point quelconque de P.

Pour des raisons de simplicité de l'explication, on s'intéresse ici uniquement aux positions et distances sur les axes X et Z (c'est à dire qu'on ne considère que le plan XZ), toute distance ou déplacement sur l'axe Y fonctionnant comme sur l'axe X. On peut donc ne considérer que deux dimensions des vecteurs en général,  et un seul angle de la caméra ; les plans parallèles à l'axe Y (c'est à dire tous les plans considérés ici) deviennent des droites. L'écran devient également un segment au lieu d'un plan.

On définit les **Limites de vue** horizontales d'une caméra C comme les deux demi-droites l1 et l2 partant de C.position, de direction d tq :
- l1 et l2 sont, comme tous les objets considérés ici, sur le plan XZ.
- l'angle entre li et C.direction est inférieur à 90°
- l'angle entre l1 et l2 est de C.ax
- C.dd est la bissectrice de l'angle (par conséquent, l'angle entre li et C.direction est de cx / 2).
D'un point de vue concret, il faut comprendre que tous les points situés entre l1 et l2 seront affichés à l'écran.

Soient bgauche(P) et bdroite(P) les intersections entre P et C.l1 et C.l2 respectivement.  
On définit une **Projection de caméra** C sur un plan P Pr(C, P) comme le segment entre bgauche et bdroite. Par définition de C.l1 et C.l2 (et par extension, de la caméra), tout point de Pr(C, P) sera affiché à l'écran.

Soit W la largeur de l'écran.  
On définit **position relative à l'écran** d'un point pp, pr(pp), tq pour P' le plan de décor passant par pp, pr = (pp.x - bgauche(P').x) / (bgauche(P').x - bdroite(P').x). On note que pr app [0, 1] <=> pp est entre C.l1 et C.l2, donc un point n'est affiché que si sa position relative app [0, 1] ; pp est affiché sur l'écran à la position pr(pp) * W.

D'après les définitions de bgauche/bdroite, on peut déterminer que  
pr(pp) = C.position.x + 

tan(opp/adj)

-------

Dans une hypothèse de pure 2D (= sans parallax), tous les éléments se trouvent sur le même plan P0, parallèle aux axes X et Y.    

- La caméra qu'on utilsera ici sera toujours telle que P0 est un plan de décor. C.direction ne changera donc jamais. 
- P0 étant perpendiculaire aux axes X et Y, C.position.z est la distance entre C et P0.



La longueur de la projection de C sur P0 est la largeur de l'écran ; tout point de P0 situé sur cette projection sera affiché à l'écran.  

Soit W la largeur de l'écran en pixels.  
On définit la **Position relative** d'un point pp (aux coordonneés exprimées en pixels) sur l'écran comme pr de [0, 1] tq pr = pp / W.  

Un point p situé sur la projection de C sur un plan de décor P (comme P0) est affiché sur l'écran à la position relative |(p.x - bgauche.x) / (bgauche.x - bdroite.x)|. 

On est dans une hypothèse de jeu sans zoom/dézoom, c'est à dire que les éléments de P0 seront toujours affichés à la même taille ; la zone couverte par l'écran est toujours de même taille. Cela veut dire que la taille de la projection de C sur P0 ne changera jamais. Cette taille dépend (pythagore TMTC) de l'angle C.ax et de la distance entre P0 et C, c'est à dire C.position.z. On décide donc que ces paramètres ne changeront jamais.  
Afin de simplifier, on décide donc, puisque la distance entre bgauche et bdroite ne changera jamais, que l'unité de distance dans notre repère est telle que |bgauche.x - bdroite.x| == W. Ainsi, un point p de la projection est affiché à la position (en pixels) |p.x - bgauche.x|.  

On sait maintenant comment sont affichés les objets et distances de P0 sur l'écran, au travers du modèle de la caméra (l'écran étant la projection de la caméra sur P0). L'idée du parallax est d'introduire d'autres plans de décor (rappel : tout plan de décor est perpendiculaire à C.dd, donc parallèle à P0) ; il nous faut définir à quelle position sur l'écran sont affichés les objets situés sur ces autres plans de décor.  

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