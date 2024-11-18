#include "reseautrophique1.h"
#include "ssprogrammers1.c"


// Fonction pour rechercher et afficher les sommets particuliers
void menu_recherche_sommets(ReseauTrophique* reseau, ReseauTrophique* reseau_mer_baltique ) {
    int choix;
    do {
        printf("\nRecherche de sommets particuliers:\n"); // DIRE ...
        printf("1. Afficher les producteurs primaires (sans predateurs)\n"); // DIRE ...
        printf("2. Afficher les consommateurs superieurs (sans proies)\n"); // DIRE ...
        printf("3. Afficher les especes avec une seule source d alimentation\n"); // DIRE ...
        printf("4. Retour au menu principal\n"); // DIRE ...
        printf("Votre choix : "); // DIRE ...
        scanf("%d", &choix); // // DIRE choix

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
                printf("Retour au menu principal.\n"); // DIRE ...
                break;
            default:
                printf("Choix invalide. Veuillez reessayer.\n"); // DIRE ...
        }
    } while (choix != 4);
}



int main() { // DEBUT
    ReseauTrophique reseau_foret = {0};
    ReseauTrophique reseau_mer_baltique = {0};
    ReseauTrophique reseau_mer_caraibes = {0};

    ReseauTrophique* reseau_actuel = NULL;

    int choix; // ENTIER CHOIX

    do { // FAIRE ...
        printf("\nMenu principal:\n"); // DIRE ...
        printf("1. Charger le reseau de foret temperee\n"); // DIRE ...
        printf("2. Charger le reseau cotier de la mer Baltique\n"); // DIRE ...
        printf("3. Charger le reseau cotier de la mer des Caraibes\n"); // DIRE ...

        printf("4. Afficher le reseau actuel\n"); // DIRE ...
        printf("5. Rechercher des sommets particuliers\n"); // DIRE ...
        printf("6. Afficher les chaines alimentaires d'une espece\n"); // DIRE ...
        printf("7. Estimer l'importance relative des especes\n"); // DIRE ...
        printf("8. Afficher les niveaux trophiques d'une espece\n"); // DIRE ...
        printf("9. Simuler la disparition d'une espece\n"); // DIRE ...
        printf("10. Simuler la dynamique de la population\n"); // DIRE ...
        printf("11. Quitter\n"); // DIRE ...
        printf("Votre choix : "); // DIRE ...
        scanf("%d", &choix); // DIRE ...

        switch(choix) { // SWITCH CASE avec comme paramètre choix
            case 1: // CASE 1

                initialiser_reseau_foret_temperee(&reseau_foret);
                printf("Reseau de foret temperee charge.\n"); // DIRE ...
                afficher_contexte_reseau(&reseau_foret);
                sauvegarder_csv(&reseau_foret, "reseau_foret"); // DIRE ...
                // Calculer et afficher la complexité
                calculer_complexite(&reseau_foret);

                // Vérification de la connexité
                if (verifier_connexite(&reseau_foret)) { // SI ...
                    printf("Le reseau de foret temperee est CONNEXE.\n"); // DIRE ...
                } else { // SINON ...
                    printf("Le reseau de foret temperee n'est PAS CONNEXE.\n"); // DIRE ...
                }

                reseau_actuel = &reseau_foret;

                break; // SORTIR

            case 2: // CASE 2
                initialiser_reseau_mer_baltique(&reseau_mer_baltique);
                printf("Reseau cotier de la mer Baltique charge.\n"); // DIRE ...
                afficher_contexte_reseau(&reseau_mer_baltique);

                sauvegarder_csv(&reseau_mer_baltique, "reseau_mer_baltique"); // DIRE ...
                // Calculer et afficher la complexité
                calculer_complexite(&reseau_mer_baltique);

                // Vérification de la connexité
                if (verifier_connexite(&reseau_mer_baltique)) { // SI ...
                    printf("Le reseau cotier de la mer Baltique est CONNEXE.\n"); // DIRE ...
                } else { // SINON ...
                    printf("Le reseau cotier de la mer Baltique n'est PAS CONNEXE.\n"); // DIRE ...
                }

                reseau_actuel = &reseau_mer_baltique;

                break; // SORTIR
           case 3: // CASE 3
                initialiser_reseau_mer_caraibes(&reseau_mer_caraibes);
                printf("Reseau trophique de la mer des Caraibes charge.\n"); // DIRE ...
                afficher_contexte_reseau(&reseau_mer_caraibes);

                sauvegarder_csv(&reseau_mer_caraibes, "reseau_mer_caraibes"); // DIRE ...
                // Calculer et afficher la complexité
                calculer_complexite(&reseau_mer_caraibes);

                // Vérification de la connexité
                if (verifier_connexite(&reseau_mer_caraibes)) { // SI ...
                    printf("Le reseau trophique de la mer des Caraibes est CONNEXE.\n"); // DIRE ...
                } else { // SINON ...
                    printf("Le reseau trophique de la mer des Caraibes n'est PAS CONNEXE.\n"); // DIRE ...
                }

                reseau_actuel = &reseau_mer_caraibes;

                break; // SORTIR

            case 4: // CASE 4 
                if (reseau_actuel) { // SI ...
                    afficher_reseau(reseau_actuel);
                } else { // SINON ...
                    printf("Aucun reseau n'est actuellement charge.\n"); // DIRE ...
                }
                break; // SORTIR
            case 5: // CASE 5
                // Menu pour rechercher des sommets particuliers
                menu_recherche_sommets(&reseau_foret, &reseau_mer_baltique); // ou &reseau_mer_baltique

                break; // SORTIR
            case 6: // CASE 6
                if (reseau_actuel) { // SI ...
                    char espece[MAX_NOM];
                    printf("Entrez le nom de l'espece : "); // DIRE ...
                    scanf("%s", espece); // LIRE espace
                    afficher_chaines_alimentaires(reseau_actuel, espece);
                } else { // SINON
                    printf("Aucun reseau n est actuellement charge.\n"); // DIRE ...
                }

                break; // SORTIR
            case 7: // CASE 7
                if (reseau_actuel) { // SI ...
                    estimer_importance_especes(reseau_actuel);
                } else { // SINON ...
                    printf("Aucun reseau n est actuellement charge.\n"); // DIRE ...
                }

                break; // SORTIR
            case 8: // CASE 8
                if (reseau_actuel) { // SI ...
                    char espece[MAX_NOM];
                    printf("Entrez le nom de l'espece : "); // DIRE ...
                    scanf("%s", espece); // LIRE ...

                    //   Afficher le nom entré pour débogage
                    afficher_niveaux_trophiques(reseau_actuel, espece);
                }

                break; // SORTIR
            case 9: // CASE 9
                if (reseau_actuel) { // SI ...
                    char espece[MAX_NOM];
                    printf("Entrez le nom de l espece a supprimer : "); // DIRE ...
                    scanf("%s", espece); // LIRE espace
                    simuler_disparition_espece(reseau_actuel, espece);
                } else { // SINON
                    printf("Aucun reseau n est actuellement charge.\n"); // DIRE ...
                }
                break; // SORTIR
            case 10: // CASE 10
                if (reseau_actuel) { // SI ...
                    // Initialiser les paramètres
                    configurer_parametres_population(reseau_actuel);

                    int iterations; // ENTIER iterations
                    printf("Entrez le nombre d'iterations: "); // DIRE ...
                    scanf("%d", &iterations); // LIRE iterations

                    // Exécuter la simulation
                    simuler_dynamique_populations(reseau_actuel, iterations);
                } else { // SINON 
                    printf("Aucun reseau n'est actuellement charge.\n"); // DIRE ...
                }
                break; // SORTIR
            case 11: // CASE 11
                printf("Au revoir !\n"); // DIRE ...

                break; // SORTIR
            default:
                printf("Choix invalide. Veuillez reessayer.\n"); // DIRE ...
        }
    } while (choix != 6); // TANT QUE choix different de 6

    return 0; // FIN
}


