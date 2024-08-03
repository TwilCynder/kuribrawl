Système d'action script : script pour chaque animation, si y en a sur des animations autres que d'attaque, donc en gros qui vont être utilisées dans des états autres que ATTACK, qui ont leur propres fonctionnements, l'AS pourra éventuellement override ce fonctionnement

## Multimoves 
On va faire un système de cancel de moves plus général.  
L'idée c'est qu'un move peut, sur une frame donnée, être cancel dans
- anything
- une action, qui peut être un move built-in
- un move non built-in 