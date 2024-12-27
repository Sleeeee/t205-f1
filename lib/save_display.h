char *path_create(char file_name[]) {
    // Taille nécessaire : "results/" + file_name + ".txt" + '\0'
    size_t path_size = strlen("results/") + strlen(file_name) + strlen(".txt") + 1;

    char *path = malloc(path_size);
    if (path == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return NULL;
    }

    strcpy(path, "results/");
    strcat(path, file_name);
    strcat(path, ".txt");

    return path;
}

int file_refresh(char path[]){
    int path_open = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0666);

	if(path_open == -1){
		perror("Une erreur d'ouverture s'est produite");
    return -1;
	}

	close(path_open);
}

void format_car_info(char *buffer, car car) {
    snprintf(
        buffer, 256,
        "car %d, %d laps, best lap %.3f, time total %.3f, s1 %.3f, s2 %.3f, s3 %.3f, status %d\n",
        car.id,
        car.laps_count,
        (float) car.time_best / 1000,
        (float) car.time_total / 1000,
        (float) car.time_sectors[0] / 1000,
        (float) car.time_sectors[1] / 1000,
        (float) car.time_sectors[2] / 1000,
        car.status
    );
}

int file_gen(char path[], char content[]){
	printf("le fichier à été créer : %s \n", path);

	int path_open = open(path, O_WRONLY | O_APPEND | O_CREAT, 0666);

	if(path_open == -1){
		perror("Une erreur d'ouverture s'est produite");
    return -1;
	}

	ssize_t path_write = write(path_open, content, strlen(content));
	ssize_t path_write_enter = write(path_open, "\n", 1);

	if(path_write == -1 || path_write_enter == -1){
		perror("Une erreur d'écriture s'est produite");
		close(path_open);
		return -1;
	}

	close(path_open);
    
	return 0;
}
