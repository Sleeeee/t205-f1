void init_globalmemory(globalmemory* global_shm) {
  for (int i = 0; i < 3; i++) {
    global_shm->sector_best[i].car_id = -1;
    global_shm->sector_best[i].time = 1000000;
  }
}

void init_car(int id, int car_num, int sem_id, globalmemory* global_shm, int phase) {
  // Définition de la seed aléatoire basée sur le pid
  srand(time(NULL) ^ (getpid()));

  // Initialisation des données de la voiture
  global_shm->cars[id].id = car_num;
  global_shm->cars[id].laps_count = 0;
  global_shm->cars[id].time_total = 0;
  global_shm->cars[id].time_best = 1000000; // Temps plus long que ce qu'il est possible d'effectuer pour assurer la réécriture
  // Lancement de la course
  global_shm->cars[id].status = 1;
  start_phase(id, global_shm, sem_id, phase);
  global_shm->cars[id].status = 0;
}

