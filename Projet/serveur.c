#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define PORT 6000
#define MAX_BUFFER 1000
#define MAX_CLIENTS 30
#define ROWS 12
#define COLUMNS 20
#define GHOSTS 2
#define STARS 20

typedef struct{
    int rows;
    int columns;
    int ghosts;
    int stars;
} Pacman;

const char *EXIT = "exit";

void lireMessage(char *tampon) {
    printf("Saisir un message à envoyer :\n");
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int testQuitter(char tampon[]) {
    return strcmp(tampon, EXIT) == 0;
}

int countPoints(char** grid){
    int total = STARS*100;
    for (int i = 1; i < ROWS; i++){
        for (int j = 1; j < COLUMNS; j++){
            if (grid[i][j] == '*' || grid[i][j] == '0'){
                total -= 100;
            }
        }
    }
    return total;
}

char *generateArray() {
    char *tableau = (char *) malloc(COLUMNS * sizeof(char ));
    int i = 0;
    if (tableau == NULL) {
        printf("Allocation echouée\n");
    }
    for (int i = 0; i < COLUMNS; i++) {
        tableau[i] = ' ';
    }
    return tableau;
}

char **generateMatrix() {
    char **matrice = (char **) malloc(ROWS * sizeof(char *));
    if (matrice == NULL) {
        printf("L'allocation a échouée!\n");
    }
    for (int i = 0; i < ROWS; i++) {
        matrice[i] = generateArray();
    }
    return matrice;
}

void generateBlankGrid(char** grid) {
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLUMNS; j++){
            if (i == 0 || j == 0 || i == ROWS-1 || j == COLUMNS-1){
                grid[i][j] = '#';
            }
        }
    }
}

void convertMatrixToArray(char** matrix, char* tableau){
    int curs = 0;
    char temp[MAX_BUFFER];
    sprintf(temp, "Votre score : %d", countPoints(matrix));
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLUMNS; j++){
            tableau[curs] = matrix[i][j];
            curs++;
        }
        tableau[curs] = '|';
        curs++;
    }
    for (int k = 0; k < strlen(temp); k++){
        tableau[curs] = temp[k];
        curs++;
    }
}

void addGhosts(char** grid){
    int x, y;
    for (int i = 0; i < GHOSTS; i++) {
        do {
            x = 1 + rand() % COLUMNS-1;
            y = 1 + rand() % ROWS-1;
        } while (grid[y][x] != ' ');
        grid[y][x] = 'O';
    }
}

void addTheGreatestPacman(char** grid){
    int x, y;
        do {
            x = 1 + rand() % (COLUMNS-1);
            y = 1 + rand() % (ROWS-1);
        } while (grid[y][x] != ' ');
        grid[y][x] = '<';
    }

void addDots(char** grid){
    int x, y;
    for (int i = 0; i < STARS; i++) {
    do {
        x = 1 + rand() % COLUMNS-1;
        y = 1 + rand() % ROWS-1;
    } while (grid[y][x] != ' ');
    grid[y][x] = '*';
    }
}

void initGame(char** grid){
    generateBlankGrid(grid);
    addGhosts(grid);
    addTheGreatestPacman(grid);
    addDots(grid);
}

int getPacmanXPosition(char** grid){
    for (int i = 1; i < ROWS; i++){
        for (int j = 1; j < COLUMNS; j++){
            if (grid[i][j] == '<'){
                return j;
            }
        }
    }
    return 0;
}

int getPacmanYPosition(char** grid){
    for (int i = 1; i < ROWS; i++){
        for (int j = 1; j < COLUMNS; j++){
            if (grid[i][j] == '<'){
                return i;
            }
        }
    }
    return 0;
}

int getGhostXPosition(char** grid, int lastx, int lasty){
    for (int i = lasty; i < ROWS; i++){
        for (int j = 1; j < COLUMNS; j++){
            if ((i != lasty && j != lastx) && (grid[i][j] == 'O' || grid[i][j] == '0')){
                return j;
            }
        }
    }
    return 0;
}

int getGhostYPosition(char** grid, int lastx, int lasty){
    for (int i = lasty; i < ROWS; i++){
        for (int j = 1; j < COLUMNS; j++){
            if ((i != lasty && j != lastx) && (grid[i][j] == 'O' || grid[i][j] == '0')){
                return i;
            }
        }
    }
    return 0;
}

void moveGhosts(char** grid){
    int x;
    int y;
    int currentX = 0;
    int currentY = 0;
    int i = 0;
    int movable = 0;
    int random;

    while (i < GHOSTS){
        x = getGhostXPosition(grid, currentX, currentY);
        y = getGhostYPosition(grid, currentX, currentY);
        do{
            random = 1 + rand() % 3;
            if (random == 1 && y > 1) {
                if (grid[y][x] == '0'){
                    grid[y][x] = '*';
                } else {
                    grid[y][x] = ' ';
                }
                y -= 1;
                movable = 1;
            } else if (random == 2 && x > 1){
                if (grid[y][x] == '0'){
                    grid[y][x] = '*';
                } else {
                    grid[y][x] = ' ';
                }
                x -= 1;
                movable = 1;
            } else if (random == 3 && y < ROWS-2){
                if (grid[y][x] == '0'){
                    grid[y][x] = '*';
                } else {
                    grid[y][x] = ' ';
                }
                y += 1;
                movable = 1;
            } else if (random == 4 && x < COLUMNS-2){
                if (grid[y][x] == '0'){
                    grid[y][x] = '*';
                } else {
                    grid[y][x] = ' ';
                }
                x += 1;
                movable = 1;
            }
        } while (!movable);

        if (grid[y][x] == '<'){
            grid[y][x] = 'X';
        } else if (grid[y][x] == '*'){
            grid[y][x] = '0';
        } else if (grid[y][x] == ' '){
            grid[y][x] = 'O';
        }

        i++;
        movable = 0;
        currentX = x;
        currentY = y;
}
}

void movePacman(char** grid, char move[]){
    int x = getPacmanXPosition(grid);
    int y = getPacmanYPosition(grid);
    grid[y][x] = ' ';
    if (move[0] == 'z' && y > 1) {
        y -= 1;
    } else if (move[0] == 'q' && x > 1){
        x -= 1;
    } else if (move[0] == 's' && y < ROWS-2){
        y += 1;
    } else if (move[0] == 'd' && x < COLUMNS-2){
        x += 1;
    }

    if (grid[y][x] == 'O' || grid[y][x] == '0') {
        grid[y][x] = 'X';
    } else {
        grid[y][x] = '<';
    }

}

int gameOver(char** grid){
    int x = getPacmanXPosition(grid);
    int y = getPacmanYPosition(grid);

    if (x == 0 && y == 0){
        return 1;
    }

    return 0;
}

int main(int argc, char const *argv[]) {
    int fdSocketAttente;
    int fdSocketCommunication;
    struct sockaddr_in coordonneesServeur;
    struct sockaddr_in coordonneesAppelant;
    char tampon[MAX_BUFFER];
    char** grid = generateMatrix();
    int nbRecu;
    int longueurAdresse;
    int pid;


    fdSocketAttente = socket(PF_INET, SOCK_STREAM, 0);

    if (fdSocketAttente < 0) {
        printf("erreur de socket\n");
        exit(EXIT_FAILURE);
    }

    // On prépare l’adresse d’attachement locale
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse);

    coordonneesServeur.sin_family = PF_INET;
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_addr.s_addr = htonl(INADDR_ANY);
    // toutes les interfaces locales disponibles
    coordonneesServeur.sin_port = htons(PORT);

    if (bind(fdSocketAttente, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
        printf("erreur de bind\n");
        exit(EXIT_FAILURE);
    }

    if (listen(fdSocketAttente, 5) == -1) {
        printf("erreur de listen\n");
        exit(EXIT_FAILURE);
    }

    socklen_t tailleCoord = sizeof(coordonneesAppelant);

    int nbClients = 0;
    while (nbClients < MAX_CLIENTS) {
        if ((fdSocketCommunication = accept(fdSocketAttente, (struct sockaddr *) &coordonneesAppelant,
                                            &tailleCoord)) == -1) {
            printf("erreur de accept\n");
            exit(EXIT_FAILURE);
        }

        printf("Client connecté - %s:%d\n",
               inet_ntoa(coordonneesAppelant.sin_addr),
               ntohs(coordonneesAppelant.sin_port));

        if ((pid = fork()) == 0) {
            close(fdSocketAttente);
            initGame(grid);
            while (1) {
                srand(time(NULL)); //initialise le générateur aléatoire
                convertMatrixToArray(grid, tampon);
                send(fdSocketCommunication, tampon, strlen(tampon), 0);
                nbRecu = recv(fdSocketCommunication, tampon, MAX_BUFFER, 0);

                if (nbRecu > 0) {
                    tampon[nbRecu] = 0;
                    printf("Recu de %s:%d : %s\n",
                           inet_ntoa(coordonneesAppelant.sin_addr),
                           ntohs(coordonneesAppelant.sin_port),
                           tampon);

                    if (testQuitter(tampon)) {
                        printf("Client déconnecté - %s:%d\n",
                               inet_ntoa(coordonneesAppelant.sin_addr),
                               ntohs(coordonneesAppelant.sin_port));
                        break; // on quitte la boucle
                    }
                }
                if (testQuitter(tampon)) {
                    send(fdSocketCommunication, tampon, strlen(tampon), 0);
                    break; // on quitte la boucle
                }
                movePacman(grid, tampon);
                if (gameOver(grid)){
                    convertMatrixToArray(grid, tampon);
                    send(fdSocketCommunication, tampon, strlen(tampon), 0);
                    break;
                }

                moveGhosts(grid);
                if (gameOver(grid)){
                    convertMatrixToArray(grid, tampon);
                    send(fdSocketCommunication, tampon, strlen(tampon), 0);
                    break;
                }
            }
            printf("Client déconnecté - %s:%d\n",
                   inet_ntoa(coordonneesAppelant.sin_addr),
                   ntohs(coordonneesAppelant.sin_port));
            exit(EXIT_SUCCESS);
        }
        nbClients++;
    }

    close(fdSocketCommunication);
    close(fdSocketAttente);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        wait(NULL);
    }

    printf("Fin du programme.\n");
    return EXIT_SUCCESS;
}
