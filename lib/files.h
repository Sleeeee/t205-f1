char * get_phase_name(int phase) {
  char phase_name[7];
  switch (phase) {
    case 1:
    case 11:
      strcpy(phase_name, "P1");
      break;
    case 2:
      strcpy(phase_name, "P2");
      break;
    case 3:
      strcpy(phase_name, "P3");
      break;
    case 4:
    case 12:
    case 16:
      strcpy(phase_name, "Q1");
      break;
    case 5:
    case 13:
    case 17:
      strcpy(phase_name, "Q2");
      break;
    case 6:
    case 14:
    case 18:
      strcpy(phase_name, "Q3");
      break;
    case 7:
    case 19:
      strcpy(phase_name, "Course");
      break;
    case 15:
      strcpy(phase_name, "Sprint");
  }
}

char* get_path(char* gp, int phase) {
    // Taille nécessaire : "results/" + gp + _ + phase (2 chiffres) + ".txt" + '\0'
    size_t path_size = strlen(gp) + 16;
    char *path = malloc(path_size);
    if (path == NULL) {
        perror("Erreur d'allocation de mémoire\n");
        exit(1);
    }
    char phase_string[4];
    snprintf(phase_string, sizeof(phase_string), "_%02d", phase);

    strcpy(path, "results/");
    strcat(path, gp);
    strcat(path, phase_string);
    strcat(path, ".txt");
    return path;
}

char** read_filenames(size_t* count) {
  DIR *dir = opendir("./results");
  if (dir == NULL) {
    perror("Erreur d'ouverture du dossier ./results . Veuillez vous assurer qu'il existe avant de continuer");
    exit(1);
  }
  char** gps = malloc(10 * sizeof(char *)); // Allocation de mémoire pour 10 noms distincts
  struct dirent *entry;
  *count = 0;

  while ((entry = readdir(dir)) != NULL) {
    // Récupère uniquement les fichiers visibles
    if (entry->d_name[0] != '.') {
      int is_unique = 1;
      char* gp = strtok(entry->d_name, "_"); // Récupère la chaine de caractères qui précède le premier underscore
      // N'ajoute que les noms uniques
      for (size_t i = 0; i < *count; i++) {
        if (!strcmp(gps[i], gp)) {
          is_unique = 0;
        }
      }
      if (is_unique && *count < 10) {
        gps[*count] = strdup(gp); // Enregistre une copie du nom récupéré
        (*count)++;
      } else if (is_unique) {
        // Si il y a plus de 10 Grand Prix en cours, envoi d'un avertissement et renvoie le résultat avec les 10 premiers noms
        printf("Attention, il y a plus de 10 Grand Prix en cours. Les 10 premiers sont disponibles, mais il est possible que celui que vous souhaitez ne soit pas disponible\n");
        break;
      }
    }
  }
  closedir(dir);
  return gps;
}

int read_highest_phase(char* gp) {
  DIR *dir = opendir("./results");
  if (dir == NULL) {
    perror("Erreur d'ouverture du dossier ./results . Veuillez vous assurer qu'il existe avant de continuer");
    exit(1);
  }
  int highest_phase = -1;
  struct dirent *entry;

  while ((entry = readdir(dir)) != NULL) {
    char* entry_name = entry->d_name;
    if (entry_name[0] != '.') {
      char* underscore = strchr(entry_name, '_');
      if (underscore == NULL) {
        perror("Format de fichier erroné");
        exit(1);
      }
      // Vérifie la similitude des noms jusqu'à l'underscore
      size_t entry_gp_length = underscore - entry_name;
      if (!strncmp(gp, entry_name, entry_gp_length)) {
        int number = atoi(underscore + 1); // Parsing du nombre après l'underscore
        if (number > highest_phase) {
          highest_phase = number;
        }
      }
    }
  }
  return highest_phase;
}

void read_contestants(char* path, int* car_nums, int start, int count) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    perror("Erreur d'ouverture du fichier");
    exit(1);
  }
  char line[64];
  if (fgets(line, sizeof(line), file) == NULL) {
    perror("Erreur de lecture du fichier");
    fclose(file);
    exit(1);
  }
  char* token = strtok(line, ",");
  // Passe les start premières valeurs
  for (int i = 0; i < start; i++) {
    token = strtok(NULL, ",");
    if (token == NULL) {
      perror("Erreur de parsing des valeurs du fichier");
      exit(1);
    }
  }
  // Récupère les count valeurs suivantes et les insère dans l'array donné en pointeur
  for (int i = 0; i < count; i++) {
    if (token == NULL) {
      perror("Erreur de parsing des valeurs du fichier");
      exit(1);
    }
    car_nums[i] = atoi(token); // On insère le nombre parsé dans CAR_NUMS
    token = strtok(NULL, ",");
  }
  fclose(file);
}

void write_results(char* gp, int phase, globalmemory* shm_copy, int car_count) {
  // On écrit seulement le nombre de voitures ayant participé même si la shm a 20 places allouées
  char* path = get_path(gp, phase);
  FILE *file = fopen(path, "w");
  if (file == NULL) {
    perror("Erreur d'ouverture du fichier");
    exit(1);
  }
  for (int i = 0; i < car_count; i++) {
    if (i) {
      fprintf(file, ","); // Virgule avant le numéro de voiture si ce n'est pas la première
    }
    fprintf(file, "%d", shm_copy->cars[i].id);
  }
  fprintf(file, "\n");
  // Meilleurs temps de secteurs
  for (int i = 0; i < 3; i++) {
    fprintf(file, "%d,", shm_copy->sector_best[i].time);
    fprintf(file, "%d\n", shm_copy->sector_best[i].car_id);
  }
  // Meilleur tour
  fprintf(file, "%d,", shm_copy->best_lap.time);
  fprintf(file, "%d\n", shm_copy->best_lap.car_id);
  printf("Résultats écrits dans le fichier %s avec succès\n", path);
}

void read_results_season(int* points) {
  DIR *dir = opendir("./results");
  if (dir == NULL) {
    perror("Erreur d'ouverture du dossier ./results . Veuillez vous assurer qu'il existe avant de continuer");
    exit(1);
  }
  int car_nums[INITIAL_CAR_COUNT];
  struct dirent *entry;

  while ((entry = readdir(dir)) != NULL) {
    char* entry_name = entry->d_name;
    if (entry_name[0] != '.') {
      char* underscore = strchr(entry_name, '_');
      if (underscore == NULL) {
        perror("Format de fichier erroné");
        exit(1);
      }
      int number = atoi(underscore + 1); // Parsing du nombre après l'underscore
      if (number == 15) {
        // Sprint
        int p;
        char path[32];
        strcpy(path, "results/");
        strcat(path, entry->d_name);
        printf("%s\n", path);
        read_contestants(path, car_nums, 0, 8);
        for (int i = 0; i < 8; i++) {
          for (int j = 0; j < INITIAL_CAR_COUNT; j++) {
            if (INITIAL_CAR_NUMS[j] == car_nums[i]) {
              points[j] += 8 - i;
            }
          }
        }
      } else if ((number == 7) || (number == 19)) {
        // Courses
        int p;
        char path[32];
        strcpy(path, "results/");
        strcat(path, entry->d_name);
        read_contestants(path, car_nums, 0, 10);
        for (int i = 0; i < 10; i++) {
          switch (i) {
            case 0:
            case 1:
            case 2:
            case 3:
              p = 25 - (i * 5);
              break;
            case 4:
            case 5:
              p = 8 - ((i-4) * 2);
              break;
            case 6:
              p = 5;
              break;
            case 7:
              p = 3;
              break;
            case 8:
              p = 2;
              break;
            case 9:
              p = 1;
              break;
          }
          for (int j = 0; j < INITIAL_CAR_COUNT; j++) {
            if (INITIAL_CAR_NUMS[j] == car_nums[i]) {
              points[j] += p;
            }
          }
        }
        FILE *file = fopen(path, "r");
        char line[64];
        if (file != NULL) {
          int current_line = 0;
          while (fgets(line, sizeof(line), file)) {
            current_line++;
            if (current_line == 5) {
            char* comma = strchr(line, ',');
              if (comma != NULL) {
                int car_id = atoi(comma + 1);
                for (int i = 0; i < 10; i++) {
                  if (car_nums[i] == car_id) {
                    for (int j = 0; j < INITIAL_CAR_COUNT; j++) {
                      if (INITIAL_CAR_NUMS[j] == car_nums[i]) {
                        points[j] += 1;
                      }
                    }
                  }
                }
              }
              break;
            }
          }
        }
      }
    }
  }
  closedir(dir);
}

void read_results_gp(char* gp) {
  FILE *file;
  int phase = 1;
  char* path = get_path(gp, phase);
  file = fopen(path, "r");
  while (file != NULL) {
    char line[64];
    int position = 0;
    if (fgets(line, sizeof(line), file) != NULL) {
      char* token = strtok(line, ",");
      char* phase_name = get_phase_name(phase);
      printf("Phase %s\n", phase_name);
      while (token != NULL) {
        printf("%d. Voiture %s", ++position, token);
        token = strtok(NULL, ",");
        printf("\n");
      }
      for (int i = 0; i < 4; i++) {
        if (fgets(line, sizeof(line), file) != NULL) {
          char* time_string = strtok(line, ",");
          char* car_id = strtok(NULL, ",");
          if ((time != NULL) && (car_id != NULL)) {
            int time = atoi(time_string);
            if (i == 3) {
              printf("Meilleur tour : %.3f secondes - Voiture %s\n", (float) time/1000, car_id);
            } else {
              printf("Meilleur temps du secteur %d : %.3f secondes - Voiture %s\n", i+1, (float) (time%60000)/1000, car_id);
            }
          }
        }
      }
    }
    // Tentative d'ouverture du prochain fichier
    path = get_path(gp, ++phase);
    file = fopen(path, "r");
  }
}
