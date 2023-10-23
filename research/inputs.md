## New
En gros on a toujorus un problème de reconnaissance des manettes. On utilise actuellement le système de mapping de SDL qui est pas très pratique a utiliser mais très pratique une fois utilisé. L'idéal serait de pouvoir mixer les deux. Pour ça on pourrait avoir une classe controllerManager, qui encapsule toute la partie events et requete d'état de manette, et aurait 2 fonctionnements, soit SDL_GameController soit SDL_Joystick avec un mapping custom. 

Voir si c'est même possible de connecter certaines manettes en mode JS et d'autres en mode Controller.  
Voir aussi le vieux bug des events bouton joysticks  

## Old
REMPLACER LES FIGHTER* DES INPUT HANDLERS PAR DES PLAYERFIGHTER* (fait)

Deux types de bindings : 
- Menu binding (mappage entre signfication générale d'un bouton pour les menus et button id)
- Game binding (mappage entre input in-game et button id)

Chaque controller type a un menu binding fixe (le récupérer directement du mappage SDL ? ou se débaraser du système de mappage SDL ?).
Chaque playerfighter a un input binding propre, déterminé à partir du binding choisi dans le css (le playerfighter n'est créé qu'au moment du lancement de la partie, donc sur le CSS on sait juste que tel port, qui a sélectionné tel perso, utilise les touches (input binding) de tel nom)

Chaque controller type peut avoir un control stick et/ou des touches directionnelles

Quand un playerfighter check une direction, il peut en fonction de son binding lire les touches directionnelles et/ou le stick

(gérer les manettes inconnues avec une sort de controller type "par défaut", en mode on les traite comme des manettes de XBox)

atm plus ou moins deux solutions :
- Le menu binding est juste le mappage SDL : implique l'utilisation du mappage SDL, donc impossible d'utiliser une manette inconnue (à voir ?)
- se débarasser complètement du mappage SDL (donc de tout SDL_GameController) : permettrait de gérer un layout "par défaut" pour les manettes inconnue, grosse compatibilité

une espèce de solution serait de pouvoir faire
- quand un controller est bind à un port, lui assigner un controllertype (déjà le cas)
- vérifier avec le système de détection du type de controller de SDL (du système de mappage SDL plus précisément) si on le connait et qu'il a un controllerType spécial, sinon controllertype par défaut (et donc chaque controllertype a un mapping complet, indépendant de celui filé par SDL qu'on n'utiliserait de toute façon pas)
(en gros on utiliser le système de mapping SDL juste pour nous dire quel type de controller c'est mais c'est tout, pas de SDL_GameController)