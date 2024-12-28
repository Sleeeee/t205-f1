void init_globalmemory(globalmemory* global_shm) {
  for (int i = 0; i < 3; i++) {
    global_shm->sector_best[i].car_id = -1;
    global_shm->sector_best[i].time = 1000000;
    global_shm->best_lap.car_id = -1;
    global_shm->best_lap.time = 1000000;
  }
}

void init_timer(int phase, globalmemory* global_shm) {
  int time;
  switch (phase) {
    case 1:
    case 2:
    case 3:
    case 11:
      time = 3600000;
      break;
    case 4:
    case 16:
      time = 1080000;
      break;
    case 5:
    case 17:
      time = 900000;
      break;
    case 6:
    case 12:
    case 18:
      time = 720000;
      break;
    case 13:
      time = 600000;
      break;
    case 14:
      time = 480000;
      break;
    default:
      time = -1000;
  }
  global_shm->time_left = time;
}

void init_car(int id, int car_num, int sem_id, globalmemory* global_shm, int phase, int circuit_length) {
  // Définition de la seed aléatoire basée sur le pid
  srand(time(NULL) ^ (getpid()));

  // Initialisation des données de la voiture
  global_shm->cars[id].id = car_num;
  global_shm->cars[id].laps_count = 0;
  global_shm->cars[id].time_total = 0;
  global_shm->cars[id].time_best = 1000000; // Temps plus long que ce qu'il est possible d'effectuer pour assurer la réécriture

  start_phase(id, car_num, global_shm, sem_id, phase, circuit_length); // Lancement de la phase adéquate
}

