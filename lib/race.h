int lap(int car_id, car* cars_shm) {
  int total = 0;
  // Exécution des trois secteurs
  for (int i = 0; i < 3; i++) {
    // Court un secteur, enregistre le temps, le processus dort une durée proportionnelle au temps couru
    int t = random_int(25, 45);
    cars_shm[car_id].time_sectors[i] = t;
    total += t;
    sleep(t / 10);
  }
  cars_shm[car_id].time_total += total;
  cars_shm[car_id].laps_count++;
  return total;
}

void race(int num_laps, int car_id, car* cars_shm) {
  int t;
  for (int i = 0; i < num_laps; i++) {
    // Parcourt un tour et modifie le meilleur temps si nécessaire
    if ((t = lap(car_id ,cars_shm)) < cars_shm[car_id].time_best) {
      cars_shm[car_id].time_best = t;
    }
  }
}

