typedef struct car {
  int id;
  int laps_count;
  int time_total;
  int time_best;
  int time_sectors[3];
  int status; // 0 : course terminée / 1 : en course / 2 : crashée
} car;

