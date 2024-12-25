/* Contrôle la continuité d'un championnat en détectant les grand prix en cours et en lançant les étapes adéquates
 * Phases d'un GP classique : 
 *   - 1 : P1 (1h)
 *   - 2 : P2 (1h)
 *   - 3 : P3 (1h)
 *   - 4 : Q1 (18min)
 *   - 5 : Q2 (15min)
 *   - 6 : Q3 (12min)
 *   - 7 : Course
 * Phases d'un GP spécial : 
 *   - 11 : P1 (1h)
 *   - 12 : Q1 (12min)
 *   - 13 : Q2 (10min)
 *   - 14 : Q3 (8min)
 *   - 15 : Sprint (100km)
 *   - 16 : Q1 (18min)
 *   - 17 : Q2 (15min)
 *   - 18 : Q3 (12min)
 *   - 19 : Course
*/

int validate_input(char* message, char one, char two) {
  // Lance une boucle récupérant une valeur, et renvoie 1 si elle correspond à two, 0 si elle correspond à one, continue sinon
  char input;
  printf("%s ", message);
  scanf(" %c", &input);
  while ((input != one) && (input != two)) {
    printf("Valeur invalide. %s ", message);
    scanf(" %c", &input);
  }
  return input == two;
}

char* input_name(char* action) {
  char *gp = malloc(31 * sizeof(char));
  printf("Bienvenue dans le logiciel de gestion de Grand Prix !\n");
  if (!strcmp(action, "start")) {
    // Créée un nouveau nom et vérifie qu'il n'existe pas
    printf("Veuillez entrer le nom du Grand Prix (30 caractères maximum) :\n");
    scanf("%30s", gp); // Scanne uniquement les 30 premiers caractères
  } else {
    // Propose les noms de Grand Prix en cours
    // TODO : implémenter les fonctions de recherche de fichiers
    char *gps[] = {"Tokyo", "Dubai", "Londondondondondondon"};
    size_t gps_count = sizeof(gps)/sizeof(gps[0]);
    for (size_t i = 0; i < gps_count; i++) {
      printf("%s\n", gps[i]);
    }
  }
  return gp;
}

int start() {
  int is_classic = validate_input("Quel type de championnat voulez-vous démarrer ? [C]lassique / [S]pecial", 'C', 'S');
  return is_classic * 10 + 1; // 1 si classique, 11 si spécial
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

int find_phase(char* action) {
  if (!strcmp(action, "start")) {
    return start();
  } else if (!strcmp(action, "next")) {
    return next();
  } else if (!strcmp(action, "cancel")) {
    return cancel();
  }
}

