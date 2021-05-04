#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 6000
#define MAX_BUFFER 1000
#define ROWS 12
#define COLUMNS 20
#define GHOSTS 2

const char *EXIT = "exit";

void nextMove(char *tampon) {
    printf("\nSaisir le prochain déplacement (ZQSD ou exit pour quitter) :\n");
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int testQuitter(char tampon[]) {
    return strcmp(tampon, EXIT) == 0;
}

void showGridArray(char* grid){
    for (int i = 0; i < strlen(grid); i++){
        if (i%COLUMNS == 0) {
            printf("\n");
        }
        printf("%c", grid[i]);
    }
}


int main(int argc , char const *argv[]) {
    int fdSocket;
    int nbRecu;
    struct sockaddr_in coordonneesServeur;
    int longueurAdresse;
    char tampon[MAX_BUFFER];

    fdSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (fdSocket < 0) {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // On prépare les coordonnées du serveur
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse);

    coordonneesServeur.sin_family = PF_INET;
    // adresse du serveur
    inet_aton("127.0.0.1", &coordonneesServeur.sin_addr);
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT);

    if (connect(fdSocket, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
        printf("connexion impossible\n");
        exit(EXIT_FAILURE);
    }

    printf("Successfully connected !\n");
    printf("Welcome to the Pacman !\n");

    while (1) {
        nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0);
        if (testQuitter(tampon)) {
            printf("Partie terminée !");
            send(fdSocket, tampon, strlen(tampon), 0);
            break; // on quitte la boucle
        }
        showGridArray(tampon);
        nextMove(tampon);
        // on envoie le message au serveur
        send(fdSocket, tampon, strlen(tampon), 0);
    }

    close(fdSocket);

    return EXIT_SUCCESS;
}