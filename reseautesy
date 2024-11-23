#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ESPECES 100
#define MAX_NOM 50

typedef struct {
    char nom[MAX_NOM];
    int population;
} Espece;

typedef struct {
    int source;
    int destination;
    float poids;
} Relation;

typedef struct {
    Espece especes[MAX_ESPECES];
    Relation* relations;
    int nb_especes;
    int nb_relations;
    char nom[MAX_NOM];
} ReseauTrophique;

ReseauTrophique* creer_reseau(const char* nom) {
    ReseauTrophique* reseau = malloc(sizeof(ReseauTrophique));
    if (reseau == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(1);
    }
    strcpy(reseau->nom, nom);
    reseau->nb_especes = 0;
    reseau->nb_relations = 0;
    reseau->relations = NULL;
    return reseau;
}

void ajouter_espece(ReseauTrophique* reseau, const char* nom, int population) {
    if (reseau->nb_especes >= MAX_ESPECES) {
        fprintf(stderr, "Nombre maximum d'espèces atteint\n");
        return;
    }
    strcpy(reseau->especes[reseau->nb_especes].nom, nom);
    reseau->especes[reseau->nb_especes].population = population;
    reseau->nb_especes++;
}

void sauvegarder_csv(ReseauTrophique* reseau, const char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        fprintf(stderr, "Impossible d ouvrir le fichier %s\n", nom_fichier);
        return;
    }

    fprintf(fichier, "Nom du reseau,%s\n", reseau->nom);
    fprintf(fichier, "Espece,Population\n");

    for (int i = 0; i < reseau->nb_especes; i++) {
        fprintf(fichier, "%s,%d\n", reseau->especes[i].nom, reseau->especes[i].population);
    }

    fclose(fichier);
    printf("Fichier CSV cree avec succes : %s\n", nom_fichier);
}

int main() {
    ReseauTrophique* reseau = creer_reseau("Foret temperee");
    ajouter_espece(reseau, "Chene", 100);
    ajouter_espece(reseau, "Lapin", 50);
    ajouter_espece(reseau, "Renard", 10);

    sauvegarder_csv(reseau, "reseau_trophique.csv");

    free(reseau->relations);
    free(reseau);

    return 0;
}
