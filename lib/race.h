int lap(int car_id, globalmemory* global_shm, int sem_id) {
  int total = 0;
  // Exécution des trois secteurs
  for (int i = 0; i < 3; i++) {
    // Court un secteur, enregistre le temps, le processus dort une durée proportionnelle au temps couru
    int t = random_int(25000, 45000);
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
        global_shm->sector_best[i].car_id = car_id;
      }
      if (semop(sem_id, &unlock, 1) == -1) {
        perror("Erreur de déblocage du sémaphore");
        exit(1);
      }
    }
    total += t;
    sleep(t / 20000);
  }
  return total;
}

void start_practice(int car_id, globalmemory* global_shm, int sem_id) {
  printf("practicing\n");
}

void start_qualif(int time_min, int car_id, globalmemory* global_shm, int sem_id) {
  printf("qualifying\n");
}

void start_race(int num_laps, int car_id, globalmemory* global_shm, int sem_id) {
  int t;
  for (int i = 0; i < num_laps; i++) {
    if ((t = lap(car_id , global_shm, sem_id)) < global_shm->cars[car_id].time_best) {
      global_shm->cars[car_id].time_best = t;
    }
    global_shm->cars[car_id].time_total += t;
    global_shm->cars[car_id].laps_count++;
  }
}

void start_phase(int car_id, globalmemory* global_shm, int sem_id, int phase) {
  switch (phase) {
    // Essais libres
    case 1:
    case 2:
    case 3:
    case 11:
      start_practice(car_id, global_shm, sem_id);
      break;
    // Qualifications
    case 4:
    case 16:
      start_qualif(18, car_id, global_shm, sem_id);
      break;
    case 5:
    case 17:
      start_qualif(15, car_id, global_shm, sem_id);
      break;
    case 6:
    case 12:
    case 18:
      start_qualif(12, car_id, global_shm, sem_id);
      break;
    case 13:
      start_qualif(10, car_id, global_shm, sem_id);
    case 14:
      start_qualif(8, car_id, global_shm, sem_id);
    // Course classique
    case 7:
    case 19:
      start_race(60, car_id, global_shm, sem_id); // TODO : compter le nombre de tours selon la longueur du circuit
      break;
    // Course sprint
    case 15:
      start_race(20, car_id, global_shm, sem_id);
      break;
  }
}

