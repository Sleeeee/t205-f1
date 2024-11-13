typedef struct car {
  int id;
  int laps_count;
  int time_total;
  int time_best;

  int time_sector1;
  int time_sector2;
  int time_sector3;

  int status; // 0 : course terminée / 1 : en course / 2 : au stand / 3 : crashée
} car;

