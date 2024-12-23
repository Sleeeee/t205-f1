int random_int(int min, int max) {
    // Génère un float aléatoire
    return (rand() % (max - min + 1)) + min;
}

int stand(int prob) {
    // Renvoie un booléen indiquant si la voiture se rend au stand
    // Retour au stand si le random atteint un nombre inférieur à prob
    return random_int(0, 999) <= prob;
}

int crash(int prob) {
    // Renvoie un booléen indiquant si la voiture se crashe
    // Crash si le random atteint un nombre inférieur à prob 
    return random_int(0, 999) <= prob;
}

int sector(int size_meters, int time_seed, int crash_prob) {
    // Renvoie le temps en millisecondes du pilote pour parcourir le secteur / 0 si la voiture se crashe
    if (crash(crash_prob)) {
      return 0;
    }
    
    // TODO : update les valeurs pour avoir quelque chose de cohérent
    int min_time = time_seed * size_meters * 0.7;
    int max_time = time_seed * size_meters * 1.3;
    return random_int(min_time, max_time);
}

