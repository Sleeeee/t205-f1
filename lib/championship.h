// Contrôle la continuité d'un championnat en détectant les grand prix en cours et en lançant les étapes adéquates

int start() {
  printf("%s\n", "started");
  // TODO : demander à l'utilisateur d'entrer les paramètres et initialiser le fichier
  return 1;
}

int next() {
  // TODO : proposer les différents choix et return l'id de la phase suivante, ou bien return 0 si aucun existant
  printf("%s\n", "next please");
  return 1; 
}

int cancel() {
  printf("%s\n", "cancelled");
  return 0;
}

int find_phase(char *action) {
  if (!strcmp(action, "start")) {
    start();
  } else if (!strcmp(action, "next")) {
    next();
  } else if (!strcmp(action, "cancel")) {
    cancel();
  }
}

