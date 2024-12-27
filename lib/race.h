int lap(int car_id, globalmemory* global_shm) {
  int total = 0;
  // Exécution des trois secteurs
  for (int i = 0; i < 3; i++) {
    // Court un secteur, enregistre le temps, le processus dort une durée proportionnelle au temps couru
    int t = random_int(25, 45);
    global_shm->cars[car_id].time_sectors[i] = t;
    total += t;
    sleep(t / 20);
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
    if ((t = lap(car_id , global_shm)) < global_shm->cars[car_id].time_best) {
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

