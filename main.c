#include "reseautrophique1.h"
#include "ssprogrammers1.c"


// Fonction pour rechercher et afficher les sommets particuliers
void menu_recherche_sommets(ReseauTrophique* reseau, ReseauTrophique* reseau_mer_baltique ) {
    int choix;
    do {
        printf("\nRecherche de sommets particuliers:\n");
        printf("1. Afficher les producteurs primaires (sans predateurs)\n");
        printf("2. Afficher les consommateurs superieurs (sans proies)\n");
        printf("3. Afficher les especes avec une seule source d alimentation\n");
        printf("4. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                trouver_producteurs_primaires(reseau);
                trouver_producteurs_primaires2(reseau_mer_baltique);

                break;
            case 2:
                trouver_consommateurs_superieurs(reseau);
                trouver_consommateurs_superieurs2(reseau_mer_baltique);

                break;
            case 3:
                trouver_especes_une_seule_proie(reseau);
                trouver_especes_une_seule_proie2(reseau_mer_baltique);

                break;
            case 4:
                printf("Retour au menu principal.\n");
                break;
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
        }
    } while (choix != 4);
}



int main() {
    ReseauTrophique reseau_foret = {0};
    ReseauTrophique reseau_mer_baltique = {0};
    ReseauTrophique reseau_mer_caraibes = {0};

    ReseauTrophique* reseau_actuel = NULL;

    int choix;

    do {
        printf("\nMenu principal:\n");
        printf("1. Charger le reseau de foret temperee\n");
        printf("2. Charger le reseau cotier de la mer Baltique\n");
        printf("3. Charger le reseau cotier de la mer des Caraibes\n");

        printf("4. Afficher le reseau actuel\n");
        printf("5. Rechercher des sommets particuliers\n");
        printf("6. Afficher les chaines alimentaires d'une espece\n");
        printf("7. Estimer l'importance relative des especes\n");
        printf("8. Afficher les niveaux trophiques d'une espece\n");
        printf("9. Simuler la disparition d'une espece\n");
        printf("10. Simuler la dynamique de la population\n");
        printf("11. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch(choix) {
            case 1:

                initialiser_reseau_foret_temperee(&reseau_foret);
                printf("Reseau de foret temperee charge.\n");
                afficher_contexte_reseau(&reseau_foret);
                sauvegarder_csv(&reseau_foret, "reseau_foret");
                // Calculer et afficher la complexité
                calculer_complexite(&reseau_foret);

                // Vérification de la connexité
                if (verifier_connexite(&reseau_foret)) {
                    printf("Le reseau de foret temperee est CONNEXE.\n");
                } else {
                    printf("Le reseau de foret temperee n'est PAS CONNEXE.\n");
                }

                reseau_actuel = &reseau_foret;



                break;

            case 2:
                initialiser_reseau_mer_baltique(&reseau_mer_baltique);
                printf("Reseau cotier de la mer Baltique charge.\n");
                afficher_contexte_reseau(&reseau_mer_baltique);

                sauvegarder_csv(&reseau_mer_baltique, "reseau_mer_baltique");
                // Calculer et afficher la complexité
                calculer_complexite(&reseau_mer_baltique);

                // Vérification de la connexité
                if (verifier_connexite(&reseau_mer_baltique)) {
                    printf("Le reseau cotier de la mer Baltique est CONNEXE.\n");
                } else {
                    printf("Le reseau cotier de la mer Baltique n'est PAS CONNEXE.\n");
                }

                reseau_actuel = &reseau_mer_baltique;

                break;
           case 3:
                initialiser_reseau_mer_caraibes(&reseau_mer_caraibes);
                printf("Reseau trophique de la mer des Caraibes charge.\n");
                afficher_contexte_reseau(&reseau_mer_caraibes);

                sauvegarder_csv(&reseau_mer_caraibes, "reseau_mer_caraibes");
                // Calculer et afficher la complexité
                calculer_complexite(&reseau_mer_caraibes);

                // Vérification de la connexité
                if (verifier_connexite(&reseau_mer_caraibes)) {
                    printf("Le reseau trophique de la mer des Caraibes est CONNEXE.\n");
                } else {
                    printf("Le reseau trophique de la mer des Caraibes n'est PAS CONNEXE.\n");
                }

                reseau_actuel = &reseau_mer_caraibes;

                break;

            case 4:
                if (reseau_actuel) {
                    afficher_reseau(reseau_actuel);
                } else {
                    printf("Aucun reseau n'est actuellement charge.\n");
                }
                break;
                break;
            case 5:
                // Menu pour rechercher des sommets particuliers
                menu_recherche_sommets(&reseau_foret, &reseau_mer_baltique); // ou &reseau_mer_baltique

                break;
            case 6:
                if (reseau_actuel) {
                    char espece[MAX_NOM];
                    printf("Entrez le nom de l'espece : ");
                    scanf("%s", espece);
                    afficher_chaines_alimentaires(reseau_actuel, espece);
                } else {
                    printf("Aucun reseau n est actuellement charge.\n");
                }

                break;
            case 7:
                if (reseau_actuel) {
                    estimer_importance_especes(reseau_actuel);
                } else {
                    printf("Aucun reseau n est actuellement charge.\n");
                }

                break;
            case 8:
                if (reseau_actuel) {
                    char espece[MAX_NOM];
                    printf("Entrez le nom de l'espece : ");
                    scanf("%s", espece);

                    //   Afficher le nom entré pour débogage
                    afficher_niveaux_trophiques(reseau_actuel, espece);
                }

                break;
            case 9: // Simuler la disparition d'une espèce
                if (reseau_actuel) {
                    char espece[MAX_NOM];
                    printf("Entrez le nom de l espece a supprimer : ");
                    scanf("%s", espece);
                    simuler_disparition_espece(reseau_actuel, espece);
                } else {
                    printf("Aucun reseau n est actuellement charge.\n");
                }
                break;
            case 10:
                if (reseau_actuel) {
                    // Initialiser les paramètres
                    configurer_parametres_population(reseau_actuel);

                    int iterations;
                    printf("Entrez le nombre d'iterations: ");
                    scanf("%d", &iterations);

                    // Exécuter la simulation
                    simuler_dynamique_populations(reseau_actuel, iterations);
                } else {
                    printf("Aucun reseau n'est actuellement charge.\n");
                }
                break;
            case 11:
                printf("Au revoir !\n");

                break;
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
        }
    } while (choix != 6);

    return 0;
}

