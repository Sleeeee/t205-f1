int random_int(int min, int max) {
  return (rand() % (max - min + 1)) + min;
}

void wait_stands(int time, int car_id, globalmemory* global_shm) {
  global_shm->cars[car_id].status = 2; // Changement du statut
  sleep(time / TIME_DIV);
}

int run_sector(int car_id, globalmemory* global_shm) {
  // Probabilité de crash
  if (!(rand() % 999)) {
    global_shm->cars[car_id].status = 3;
    exit(0);
  }
  return random_int(25000, 45000);
}

int run_lap(int car_id, int car_num, globalmemory* global_shm, int sem_id) {
  global_shm->cars[car_id].status = 1; // Changement du statut
  int total = 0;
  // Exécution des trois secteurs
  for (int i = 0; i < 3; i++) {
    // Court un secteur, enregistre le temps, le processus dort une durée proportionnelle au temps couru
    int t = run_sector(car_id, global_shm);
    global_shm->cars[car_id].time_sectors[i] = t; // Zone mémoire non susceptible à être modifiée par un autre processus
    
    // Première vérification pour bloquer le sémaphore uniquement lorsque c'est nécessaire
    if (t < global_shm->sector_best[i].time) {
      // Zone mémoire susceptible d'être modifiée par n'importe qui, on utilise donc le sémaphore
      if (semop(sem_id, &lock, 1) == -1) {
        perror("Erreur de blocage du sémaphore");
        exit(1);
      }
      // Deuxième vérification redondante au cas où un autre processus a interféré avec les données avant le blocage
      if (t < global_shm->sector_best[i].time) {
        // Section critique
        global_shm->sector_best[i].time = t;
        global_shm->sector_best[i].car_id = car_num;
      }
      if (semop(sem_id, &unlock, 1) == -1) {
        perror("Erreur de déblocage du sémaphore");
        exit(1);
      }
    }
    total += t;
    sleep(t / TIME_DIV);
  }
  return total;
}

void start_practice_qualif(int time_sec, int car_id, int car_num, globalmemory* global_shm, int sem_id) {
  while (global_shm->time_left > 0) {
    if (rand() % 2) {
      // Court un nombre de tours proportionnel au temps de la phase
      int laps_to_run = random_int(max(1, time_sec/720), max(2, time_sec/360));
      for (int i = 0; i < laps_to_run; i++) {
        if (global_shm->time_left > 65000) {
          // S'il reste au moins 45 secondes, la voiture peut démarrer
          int lap_time = run_lap(car_id, car_num, global_shm, sem_id);
          if (lap_time < global_shm->cars[car_id].time_best) {
            global_shm->cars[car_id].time_best = lap_time;
          }
          global_shm->cars[car_id].laps_count++;
        } else {
          // S'il ne reste plus assez de temps pour courir le prochain tour, sortie de piste
          break;
        }
      }
    } else {
      // Attend pendant un temps proportionnel au temps de la phase
      int time_to_wait = random_int(time_sec*250/3, time_sec*500/3);
      if (global_shm->time_left > time_to_wait) {
        // Si l'horloge permet d'attendre aussi longtemps
        wait_stands(time_to_wait, car_id, global_shm);
      } else {
        break;
      }
    }
  }
  global_shm->cars[car_id].status = 0;
}

void start_race(int num_laps, int car_id, int car_num, globalmemory* global_shm, int sem_id, int is_sprint) {
  int t;
  int* stand_laps = NULL; // Alloue initialement un entier
  int stand_count = 0;
  int first_iter = 1;
  // Génération des tours auxquels le pilote se rend aux stands (max 3)
  do {
    if (is_sprint && first_iter) {
      first_iter = 0;
    } else {
      int lap = random_int(1, num_laps);
      stand_laps = realloc(stand_laps, (stand_count+1) * sizeof(int));
      if (stand_laps == NULL) {
        perror("Échec d'allocation de mémoire");
        exit(1);
      }
      stand_laps[stand_count] = lap;
    }
  } while ((rand() % 2) && ++stand_count < 3);

  for (int i = 0; i < num_laps; i++) {
    // Analyse s'il doit aller auxs stands
    for (int j = 0; j < stand_count; j++) {
      if (i == stand_laps[j]) {
        wait_stands(random_int(20000, 30000), car_id, global_shm); // Le temps est énorme comparé à un vrai pit stop mais sinon on ne le remarque pas dans l'affichage
        break;
      }
    }
    // Lancement d'un tour
    if ((t = run_lap(car_id , car_num, global_shm, sem_id)) < global_shm->cars[car_id].time_best) {
      global_shm->cars[car_id].time_best = t;
    }
    global_shm->cars[car_id].time_total += t;
    global_shm->cars[car_id].laps_count++;
  }
  global_shm->cars[car_id].status = 0; // Course terminée
  free(stand_laps);
}

void start_phase(int car_id, int car_num, globalmemory* global_shm, int sem_id, int phase, int circuit_length) {
  switch (phase) {
    // Essais libres
    case 1:
    case 2:
    case 3:
    case 11:
      start_practice_qualif(3600, car_id, car_num, global_shm, sem_id);
      break;
    // Qualifications
    case 4:
    case 16:
      start_practice_qualif(1080, car_id, car_num, global_shm, sem_id);
      break;
    case 5:
    case 17:
      start_practice_qualif(900, car_id, car_num, global_shm, sem_id);
      break;
    case 6:
    case 12:
    case 18:
      start_practice_qualif(720, car_id, car_num, global_shm, sem_id);
      break;
    case 13:
      start_practice_qualif(600, car_id, car_num, global_shm, sem_id);
    case 14:
      start_practice_qualif(480, car_id, car_num, global_shm, sem_id);
    // Course classique
    case 7:
    case 19:
      start_race(300/circuit_length, car_id, car_num, global_shm, sem_id, 0);
      break;
    // Course sprint
    case 15:
      start_race(100/circuit_length, car_id, car_num, global_shm, sem_id, 1);
      break;
  }
}

