/* Contrôle la continuité d'un championnat en détectant les grand prix en cours et en lançant les étapes adéquates
 * Phases d'un GP classique : 
 *   - 1 : P1 (1h)
 *   - 2 : P2 (1h)
 *   - 3 : P3 (1h)
 *   - 4 : Q1 (18min)
 *   - 5 : Q2 (15min)
 *   - 6 : Q3 (12min)
 *   - 7 : Course (~305km -> ~60 tours)
 * Phases d'un GP spécial : 
 *   - 11 : P1 (1h)
 *   - 12 : Q1 (12min)
 *   - 13 : Q2 (10min)
 *   - 14 : Q3 (8min)
 *   - 15 : Sprint (~100km -> ~20 tours)
 *   - 16 : Q1 (18min)
 *   - 17 : Q2 (15min)
 *   - 18 : Q3 (12min)
 *   - 19 : Course (~305km -> ~60 tours)
*/

int validate_input(char* message, char one, char two) {
  // Lance une boucle récupérant une valeur, et renvoie 1 si elle correspond à two, 0 si elle correspond à one, continue sinon
  char input;
  do {
    printf("%s ", message);
    scanf(" %c", &input);
    if ((input == one) || (input == two)) {
      return input == two;
    }
    printf("Valeur invalide. ");
  } while (1);
}

int validate_in_range(char* message, int max_val) {
  int input;
  do {
    printf("%s ", message);
    scanf(" %d", &input);
    if ((input >= 0) && (input < max_val)) {
      return input;
    }
    printf("Valeur invalide. ");
    while (getchar() != '\n'); // Vide le buffer pour éviter une boucle infinie
  } while (1);
}

void validate_not_exists(char* message, char* string_var, char* strings[], size_t count) {
  int exists;
  char temp[31];
  do {
    exists = 0;
    printf("%s", message);
    scanf("%30s", temp); // Scanne uniquement les 30 premiers caractères
    while (getchar() != '\n'); // Vide le buffer si plus de 30 caractères sont entrés
    for (size_t i = 0; i < count; i++) {
      if (!strcmp(strings[i], temp)) {
        exists = 1;
        printf("Cette valeur existe déjà. ");
        break;
      }
    }
  } while (exists);
  strncpy(string_var, temp, 31); // Copie de string sans overflow
  string_var[30] = '\0'; // Protection en cas de string de plus de 30 caractères
}

char* input_name(char* action) {
  // Récupère le nom du Grand Prix correspondant à la requête
  char *gp = malloc(31 * sizeof(char));
  size_t gps_count;
  char** gps = read_filenames(&gps_count);

  if (!strcmp(action, "start")) {
    // Créée un nouveau nom et vérifie qu'il n'existe pas
    validate_not_exists("Veuillez entrer le nom du Grand Prix (30 caractères maximum) :", gp, gps, gps_count);
  } else {
    // Propose les noms de Grand Prix en cours et récupère le nom correspondant à l'index entré
    printf("Voici vos options :\n");
    for (size_t i = 0; i < gps_count; i++) {
      printf("  [%d] %s\n", i, gps[i]);
    }
    int gp_index = validate_in_range("Veuillez sélectionner un grand prix :", gps_count);
    strcpy(gp, gps[gp_index]); // Place le nom du grand prix sélectionné dans la variable gp
    
    // Libération de la mémoire allouée pour les noms de Grand Prix une fois que la chaine de caractère est récupérée
    for (size_t i = 0; i < gps_count; i++) {
      free(gps[i]);
    }
    free(gps);
  }
  return gp;
}

int input_length() {
  return validate_in_range("Veuillez entrer la longueur du circuit en km (1-10) :", 10);
}

int get_car_count(int phase) {
  int count;
  switch (phase) {
    // Q2
    case 5:
    case 13:
    case 17:
      count = 15;
      break;
    // Q3
    case 6:
    case 14:
    case 18:
      count = 10;
      break;
    default:
      count = 20;
  }
  return count;
}

void fetch_initial_contestants(int* car_count, int* car_nums) {
  // Copie des variables des participants
  *car_count = INITIAL_CAR_COUNT;
  for (size_t i = 0; i < INITIAL_CAR_COUNT; i++) {
    car_nums[i] = INITIAL_CAR_NUMS[i];
  }
}

void fetch_contestants(char* gp, int phase, int* car_count, int* car_nums) {
  *car_count = get_car_count(phase); // Nombre de voitures pour la phase à lancer
  // Phases où tout le monde participe peu importe l'ordre
  if ((phase == 2) || (phase == 3) || (phase == 4) || (phase == 11) || (phase == 12) || (phase == 16)) {
    fetch_initial_contestants(car_count, car_nums);
  } else if ((phase == 7) || (phase == 15) || (phase == 19)) {
    // Les courses récupèrent dans l'ordre les participants des étapes de qualifications
    for (int i = 1; i < 4; i++) {
      char* path = get_path(gp, phase - i);
      int current_count;
      int current_start;
      switch (i) {
        case 1:
          // Q3
          current_count = 10;
          current_start = 0;
          break;
        default:
          // Q1 / Q2
          current_count = 5;
          current_start = i * 5; // On prend les indices 10-14 de Q2 et 15-19 de Q1
      }
      read_contestants(path, &car_nums[current_start], current_start, current_count);
    }
  } else {
    // Les autres phases récupèrent les car_count premiers de la phase précédente
    char* path = get_path(gp, phase - 1);
    read_contestants(path, car_nums, 0, *car_count); // Remplit car_nums de car_count voitures
  }
}

int start(char* gp, int* car_count, int* car_nums) {
  int is_special = validate_input("Quel type de championnat voulez-vous démarrer ? [C]lassique / [S]pecial", 'C', 'S');
  fetch_initial_contestants(car_count, car_nums);
  return is_special * 10 + 1; // 1 si classique, 11 si spécial
}

int next(char* gp, int* car_count, int* car_nums) {
  // gp possède déjà un ou plusieurs fichiers existant car le nom a déjà été vérifié
  int phase = read_highest_phase(gp) + 1;
  fetch_contestants(gp, phase, car_count, car_nums); // Remplit les variables dont les pointeurs sont donnés
  return phase;
}

int find_phase(char* action, char* gp, int* car_count, int* car_nums) {
  int phase;
  if (!strcmp(action, "start")) {
    phase = start(gp, car_count, car_nums);
  } else if (!strcmp(action, "next")) {
    phase = next(gp, car_count, car_nums);
  }
  return phase;
}

void results_season() {
  int points[INITIAL_CAR_COUNT];
  for (int i = 0; i < INITIAL_CAR_COUNT; i++) {
    points[i] = 0;
  }
  read_results_season(points);
  printf("Résultats de la saison\n");
  for (int i = 0; i < INITIAL_CAR_COUNT; i++) {
    printf("Voiture %d : %d points\n", INITIAL_CAR_NUMS[i], points[i]);
  }
}

void results_gp(char* gp) {
  printf("Résultats pour %s\n", gp);
  read_results_gp(gp);
}

int results() {
  int season = validate_input("Quels résultats voulez-vous afficher ? [G]rand Prix / [S]aison", 'G', 'S');
  if (season) {
    results_season();
  } else {
    char* gp = input_name("results");
    results_gp(gp);
  }
}
