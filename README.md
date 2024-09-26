# Simulation de Grand Prix F1
Cours de Systèmes d'exploitation II - C. Masson - Septembre 2024
## Participants
CARMO SILVEIRINHA Tiago - HERTMANS Mathéo - STAS Justin - VANDERMEULEN Arnaud
## Objectif
Ecrire un programme en C sous Linux qui simule une compétition de Formule 1 via la CLI et enregistre les résultats dans un fichier texte
- ./f1 start lance le grand prix - vérification qu'il n'y a pas de GP en cours / proposition de recommencer à zéro
- ./f1 continue lance l'étape suivante - vérification de la situation du GP / proposition de rediriger vers f1 start si aucun trouvé
- ./f1 cancel met fin au grand prix - proposition de conserver ou non les fichiers
## Features
- La météo influence le choix de pneus
