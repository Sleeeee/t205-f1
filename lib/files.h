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
  // TODO : écrire les meilleurs temps et autres infos
  printf("Résultats écrits dans le fichier %s avec succès\n", gp);
}

void delete_results(char* gp) {
  // TODO : supprimer tous les fichiers correspondant au GP
  return;
}

