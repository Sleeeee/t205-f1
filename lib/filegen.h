#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

char path[255] = "";
//Variable global contenant le chemin du fichier créé ou a créé

int path_create (char dir_name[], char file_name[]){
	//Crée un string indiquant le chemin du directory "dir_name" et le nom du fichier "file_name" suivi de l'extension ".txt"

	strcat(path, dir_name);
	strcat(path, file_name);
	strcat(path, ".txt");
	printf("%s \n", path);
}

int file_gen (char content[]){
	printf("le fichier à été créer : %s \n", path);

	int path_open = open(path, O_WRONLY | O_APPEND | O_CREAT, 0666);
	//Ouvre, append ou crée un fichier suivant "path"

	if(path_open == -1){
		perror("Une erreur d'ouverture s'est produite");
	}
	//Vérifie si il y a une erreur lors de l'ouverture

	ssize_t path_write = write(path_open, content, strlen(content));
	ssize_t path_write_enter = write(path_open, "\n", 1);
	//Ecrit "content" dans "path"

	if(path_write == -1 || path_write_enter == -1){
		perror("Une erreur d'écriture s'est produite");
		close(path_open);
		exit(EXIT_FAILURE);
	}
	//Vérifie si il y a une erreur lors de l'écriture

	close(path_open);
	return 0;
}
