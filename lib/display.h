void display_refresh() {
  printf("\033[H\033[J"); // Caractère de rafraichissement de l'affichage
}

void display_car(car car) {
  printf("car %d, %d laps, best lap %d, time total %d, s1 %d, s2 %d, s3 %d, status %d\n", car.id, car.laps_count, car.time_best, car.time_total, car.time_sectors[0], car.time_sectors[1], car.time_sectors[2], car.status);
}

void sort_cars(car* cars, int count, int phase) {
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      if (cars[j].time_best > cars[j+1].time_best) {
        car temp = cars[j];
        cars[j] = cars[j+1];
        cars[j+1] = temp;
      }
    }
  }
}

