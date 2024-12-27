int random_int(int min, int max) {
  return (rand() % (max - min + 1)) + min;
}

void wait_stands(int time, int car_id, globalmemory* global_shm) {
  global_shm->cars[car_id].status = 2; // Changement du statut
  sleep(time / TIME_DIV);
}

int run_sector() {
  // TODO : implémenter la fonction et le crash
  return random_int(25000, 45000);
}

int run_lap(int car_id, int car_num, globalmemory* global_shm, int sem_id) {
  // TODO : implémenter le retour au stand
  global_shm->cars[car_id].status = 1; // Changement du statut
  int total = 0;
  // Exécution des trois secteurs
  for (int i = 0; i < 3; i++) {
    // Court un secteur, enregistre le temps, le processus dort une durée proportionnelle au temps couru
    int t = run_sector();
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

void start_practice(int car_id, int car_num, globalmemory* global_shm, int sem_id) {
  int time_left = 3600000; // 1 heure en ms
  while (time_left > 0) {
    int wants_to_run = random_int(0, 1);
    if (wants_to_run) {
      // Court entre 5 et 10 tours
      int laps_to_run = random_int(5, 10);
      for (int i = 0; i < laps_to_run; i++) {
        if (time_left > 45000) {
          // S'il reste au moins 45 secondes, la voiture peut démarrer
          int lap_time = run_lap(car_id, car_num, global_shm, sem_id);
          time_left -= lap_time;
        } else {
          // S'il ne reste plus assez de temps pour courir le prochain tour, retour au stands
          wait_stands(time_left, car_id, global_shm);
          time_left = 0;
        }
      }
    } else {
      // Attend pendant 5 à 10 minutes
      int time_to_wait = random_int(300, 600);
      if (time_left > time_to_wait) {
        // Si l'horloge permet d'attendre aussi longtemps
        wait_stands(time_to_wait, car_id, global_shm);
        time_left -= time_to_wait;
      } else {
        wait_stands(time_left, car_id, global_shm);
        time_left = 0;
      }
    }
  }
}

void start_qualif(int time_min, int car_num, int car_id, globalmemory* global_shm, int sem_id) {
  printf("qualifying\n");
}

void start_race(int num_laps, int car_id, int car_num, globalmemory* global_shm, int sem_id) {
  // TODO : définir un ou plusieurs passage(s) aux stands
  int t;
  for (int i = 0; i < num_laps; i++) {
    if ((t = run_lap(car_id , car_num, global_shm, sem_id)) < global_shm->cars[car_id].time_best) {
      global_shm->cars[car_id].time_best = t;
    }
    global_shm->cars[car_id].time_total += t;
    global_shm->cars[car_id].laps_count++;
  }
  global_shm->cars[car_id].status = 0; // Course terminée
}

void start_phase(int car_id, int car_num, globalmemory* global_shm, int sem_id, int phase) {
  switch (phase) {
    // Essais libres
    case 1:
    case 2:
    case 3:
    case 11:
      start_practice(car_id, car_num, global_shm, sem_id);
      break;
    // Qualifications
    case 4:
    case 16:
      start_qualif(18, car_id, car_num, global_shm, sem_id);
      break;
    case 5:
    case 17:
      start_qualif(15, car_id, car_num, global_shm, sem_id);
      break;
    case 6:
    case 12:
    case 18:
      start_qualif(12, car_id, car_num, global_shm, sem_id);
      break;
    case 13:
      start_qualif(10, car_id, car_num, global_shm, sem_id);
    case 14:
      start_qualif(8, car_id, car_num, global_shm, sem_id);
    // Course classique
    case 7:
    case 19:
      start_race(60, car_id, car_num, global_shm, sem_id); // TODO : compter le nombre de tours selon la longueur du circuit
      break;
    // Course sprint
    case 15:
      start_race(20, car_id, car_num, global_shm, sem_id);
      break;
  }
}

