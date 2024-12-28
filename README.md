# Simulation de Grand Prix F1
Cours de Systèmes d'exploitation II - C. Masson - Septembre à Décembre 2024
## Participants
CARMO SILVEIRINHA Tiago - HERTMANS Mathéo - STAS Justin - VANDERMEULEN Arnaud
## Objectif
Ecrire un programme en C sous Linux qui simule une compétition de Formule 1 via la CLI et enregistre les résultats dans un fichier texte.

Pour cela il y'a 4 commandes :
- ./f1 start lance le grand prix - vérification qu'il n'y a pas de GP en cours / proposition de lancer un nouveau grand prix en parallèle
- ./f1 next lance l'étape suivante - vérification de la situation du GP / proposition de rediriger vers f1 start si aucun trouvé / proposition de continuer les différents grand prix à la progression actuelle (donnée par le programme) ou de lancer un nouveau
- ./f1 cancel met fin au grand prix (affichage de tous les grand prix en cours) - proposition de conserver ou non les fichiers
- ./f1 results permet d'afficher les résultats d'un grand prix spécifique ou de la saison.
## Features
- Chaque pilote a un temps de référence qui influence ses performances (distribution statistique)
