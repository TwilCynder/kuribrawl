**But** : Intégrer un concept de profondeur pour les éléments de background des stages, et déterminer comment les positionner en fonction de la caméra afin de simuler la profondeur de manière réaliste.  

## Modèle conceptuel
### Espace 3D
On représente l'espace en trois dimensions.  

On introduit une **Caméra**, représentée par un point, une direction (vecteur) et deux angles : Cam(pos, dir, ax, ay) 
> angles : angle de vue / FOV horizontal et vertical. Généralement proportionels.

### Modèle 2D/3D
Dans une hypothèse de pure 2D (= sans parallax), tous les éléments se trouvent sur le même plan P0, parallèle aux axes X et Y.    

On s'intéresse ici uniquement aux distances sur les axes X et Z, toute distance ou déplacement sur l'axe Y fonctionnant comme sur l'axe X. On peut donc ne considérer que deux dimensions des vecteurs en général, une seule dimension du plan P0, CàD une droite, et un seul angle de la caméra.  

On définit les **Limites de vue** horizontales d'une caméra C comme les deux demi-droites l1 et l2 partant de C.position, de direction d tq :
- l'angle entre li et C.direction est inférieur à 90°
- l'angle entre l1 et l2 est de C.ax
- La droite d passant par C.position et parallèle à C.direction est la bissectrice de l'angle 

On définit une **Projection de caméra** C sur un plan P comme le segment entre l'intersection 