#include<stdio.h>

// argc : nombre d’arguments
// argv : le tableau contenant la valeur des arguments
int main(int argc, char *argv[]) {
    int a = 1;
    printf(" le chiffre est %d \n ", a);

    char clavier[100];
    printf(" Saisissez le texte : ");
    scanf("%s", clavier);
    printf("vous avez saisie : %s \n", clavier);

    printf("\nMainenant, on va lister tous les arguments (argv)\n");
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s \n", argv[i]);
    }

    return 0;
}
