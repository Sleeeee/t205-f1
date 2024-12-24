void display_refresh() {
  printf("\033[H\033[J"); // Caractère de rafraichissement de l'affichage
}

void display_car(int id, int laps_count, int time_total, int time_sector1, int time_sector2, int time_sector3, int status) {
  printf("car %d, %d laps, time total %d, s1 %d, s2 %d, s3 %d, status %d\n", id, laps_count, time_total, time_sector1, time_sector2, time_sector3, status);
}
