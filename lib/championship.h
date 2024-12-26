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
  printf("Bienvenue dans le logiciel de gestion de Grand Prix !\n");

  if (!strcmp(action, "start")) {
    // Créée un nouveau nom et vérifie qu'il n'existe pas
    validate_not_exists("Veuillez entrer le nom du Grand Prix (30 caractères maximum) : ", gp, gps, gps_count);
  } else {
    // Propose les noms de Grand Prix en cours et récupère le nom correspondant à l'index entré
    printf("Voici vos options :\n");
    for (size_t i = 0; i < gps_count; i++) {
      printf("  [%d] %s\n", i, gps[i]);
    }
    int gp_index = validate_in_range("Veuillez sélectionner un grand prix : ", gps_count);
    strcpy(gp, gps[gp_index]); // Place le nom du grand prix sélectionné dans la variable gp
    
    // Libération de la mémoire allouée pour les noms de Grand Prix une fois que la chaine de caractère est récupérée
    for (size_t i = 0; i < gps_count; i++) {
      free(gps[i]);
    }
    free(gps);
  }
  return gp;
}

int start(char* gp, int* car_count, int* car_nums) {
  int is_special = validate_input("Quel type de championnat voulez-vous démarrer ? [C]lassique / [S]pecial", 'C', 'S');
  // Copie des variables des participants
  *car_count = INITIAL_CAR_COUNT;
  for (size_t i = 0; i < INITIAL_CAR_COUNT; i++) {
    car_nums[i] = INITIAL_CAR_NUMS[i];
  }
  return is_special * 10 + 1; // 1 si classique, 11 si spécial
}

int next(char* gp, int* car_count, int* car_nums) {
  // gp possède déjà un ou plusieurs fichiers existant car le nom a déjà été vérifié
  int last_phase = read_highest_phase(gp);
  fetch_contestants(gp, last_phase, car_count, car_nums); // Remplit les variables dont les pointeurs sont donnés
  return last_phase + 1;
}

int cancel(char* gp) {
  // Supprime les fichiers correspondant au gp
  return 0;
}

int find_phase(char* action, char* gp, int* car_count, int* car_nums) {
  int phase;
  if (!strcmp(action, "start")) {
    phase = start(gp, car_count, car_nums);
  } else if (!strcmp(action, "next")) {
    phase = next(gp, car_count, car_nums);
  } else if (!strcmp(action, "cancel")) {
    phase = cancel(gp);
  }
  return phase;
}

