

#ifndef PROJETINFOING2_RESEAUTROPHIQUE1_H
#define PROJETINFOING2_RESEAUTROPHIQUE1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOM 100
#define MAX_ESPECES 100


typedef struct {
    char nom[MAX_NOM];
    char type[MAX_NOM];
    int niveau_trophique;
    double population;
    double taux_croissance;
    double capacite_charge;
    int predateurs[MAX_ESPECES];
    float poids_predateurs[MAX_ESPECES];
    int nb_predateurs;
    int proies[MAX_ESPECES];
    float poids_proies[MAX_ESPECES];
    int nb_proies;
} Espece;



typedef struct ReseauTrophique {
    Espece especes[MAX_ESPECES];
    int nb_especes;
    char ecosysteme[MAX_NOM];
    char region[MAX_NOM];
    char climat[MAX_NOM];
} ReseauTrophique;

void sauvegarder_csv(ReseauTrophique* reseau, const char* nom_fichier_base);
void ajouter_espece(ReseauTrophique* reseau, const char* nom, const char* type, int niveau_trophique);
int trouver_espece(ReseauTrophique* reseau, const char* nom);

void ajouter_relation(ReseauTrophique* reseau, const char* predateur_nom, const char* proie_nom, float poids);
void afficher_reseau(ReseauTrophique* reseau);
void trouver_producteurs_primaires(ReseauTrophique* reseau);
void trouver_consommateurs_superieurs(ReseauTrophique* reseau);

void trouver_especes_une_seule_proie(ReseauTrophique* reseau);
void dfs(ReseauTrophique* reseau, int espece_index, bool* visite);


void initialiser_reseau_foret_temperee(ReseauTrophique* reseau);

void calculer_complexite(ReseauTrophique* reseau);


void dfs_chaine(ReseauTrophique* reseau, int espece_index, char* chaine);
void afficher_chaines_alimentaires(ReseauTrophique* reseau, const char* nom_espece);

void afficher_contexte_reseau(ReseauTrophique* reseau);
bool verifier_connexite(ReseauTrophique* reseau);

void initialiser_Reseau_Prairie(ReseauTrophique* reseau);


void trouver_producteurs_primaires2(ReseauTrophique* reseau_prairie);
void trouver_consommateurs_superieurs2(ReseauTrophique* reseau_prairie);
void trouver_especes_une_seule_proie2(ReseauTrophique* reseau_prairie);

void trouver_producteurs_primaires3(ReseauTrophique* reseau_mer_caraibes);
void trouver_consommateurs_superieurs3(ReseauTrophique* reseau_mer_caraibes);
void trouver_especes_une_seule_proie3(ReseauTrophique* reseau_mer_caraibes);




void calculer_centralite_degre(ReseauTrophique* reseau);
void calculer_intermediarite(ReseauTrophique* reseau);
void estimer_importance_especes(ReseauTrophique* reseau);

void trouver_niveaux_trophiques(ReseauTrophique* reseau, int espece_index, bool* visite, int* niveaux, int* nb_niveaux);
void afficher_niveaux_trophiques(ReseauTrophique* reseau, const char* nom_espece);

void configurer_parametres_population(ReseauTrophique* reseau);
void simuler_dynamique_populations(ReseauTrophique* reseau, int iterations);
void initialiser_reseau_mer_caraibes(ReseauTrophique* reseau);


//Bonnus
void simuler_degradation_habitat(ReseauTrophique* reseau, double facteur_degradation);
void simuler_flux_energetiques(ReseauTrophique* reseau);
void generer_graphique_populations(ReseauTrophique* reseau, int iterations);

void visualiser_reseau(ReseauTrophique reseau);
#endif //PROJETINFOING2_RESEAUTROPHIQUE1_H
