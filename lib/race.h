void lap(int car_id, car* cars_shm) {
  int total = 0;
  // Exécution des trois secteurs
  for (int i = 0; i < 3; i++) {
    int t = random_int(25, 45);
    cars_shm[car_id].time_sectors[i] = t;
    cars_shm[car_id].time_total += t;
    sleep(t / 10);
  }
  cars_shm[car_id].laps_count++;
}

void race(int num_laps, int car_id, car* cars_shm) {
  for (int i = 0; i < num_laps; i++) {
    lap(car_id ,cars_shm);
  }
}

