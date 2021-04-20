#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    int fichier1;
    int fichier2;
    int nombreLecture;
    char buffer[BUFSIZ];

    if (argc != 3) {
        printf("Le programme attend 2 arguments !\n");
        return EXIT_FAILURE;
    }

    printf("Ouverture fichier : %s\n", argv[1]);
    if ((fichier1 = open(argv[1], O_RDONLY)) < 0) {
        printf("Erreur ouverture fichier 1\n");
        return EXIT_FAILURE;
    }

    printf("Creation fichier : %s\n", argv[2]);
    if ((fichier2 = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR)) < 0) {
        printf("Erreur ouverture fichier 2\n");
        return EXIT_FAILURE;
    }

    printf("Copie des fichiers\n");
    while ((nombreLecture = read(fichier1, buffer, BUFSIZ)) > 0) {
        if (write(fichier2, buffer, nombreLecture) != nombreLecture) {
            printf("Erreur écriture fichier 2\n");
            close(fichier1);
            close(fichier2);

            return EXIT_FAILURE;
        }

    }

    close(fichier1);
    close(fichier2);

    //unlink(argv[1]);
    return EXIT_SUCCESS;
}
