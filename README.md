UTBM 2017 projet LO41

Projet de LO41 P2017, ce projet consiste sur la simulation de livraison par drone.
Réaliser par Michel HUANG et Alban ELEZI INFO 02. 

Pour compiler: make
Pour exécuter le programme avec les valeurs par défauts: make run.
Pour nettoyer les fichiers de compilation: make clean.
Ou faire directement le nottoyage et l'éxécution: make clean run.

Option de paramètre de compilation:

De base: le nombre de drone est à 10, le nombre de colis par slot a 10, le nombre de client a 10. 

Le premier paramètre est le nombre de drônes.
Le second, le nombre de client.
Le dernier, le nombre de colis par slot.

Pour compiler avec les paramètres personnalisés: ./APP_LO41 [Nombre Drone] [Nombre Client] [Nombre de colis].

Tout les valeurs peuvent être modifié dans le fichier "structures.h", puis recompiler le projet pour le tester.

Signal:

Vous pouvez utiliser la touche CTRL+C pour le nottoyage ensuite interrompre le processus grâce à l'envoie d'un signal SIGINT.