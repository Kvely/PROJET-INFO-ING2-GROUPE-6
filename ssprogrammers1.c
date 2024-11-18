#include "reseautrophique1.h"


void sauvegarder_csv(ReseauTrophique* reseau, const char* nom_fichier_base) {
    char nom_fichier_noeuds[100];
    char nom_fichier_liens[100];

    sprintf(nom_fichier_noeuds, "%snoeuds.csv", nom_fichier_base);
    sprintf(nom_fichier_liens, "%sliens.csv", nom_fichier_base);

    // Sauvegarde des nœuds
    FILE* file_noeuds = fopen(nom_fichier_noeuds, "w");
    if (file_noeuds == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour la sauvegarde des nœuds.\n");
        return;
    }

    fprintf(file_noeuds, "id,nom,type,niveau_trophique,population\n");
    for (int i = 0; i < reseau->nb_especes; i++) {
        Espece* e = &reseau->especes[i];
        fprintf(file_noeuds, "%d,%s,%s,%d,%.2f\n", i, e->nom, e->type, e->niveau_trophique, e->population);
    }
    fclose(file_noeuds);

    // Sauvegarde des liens
    FILE* file_liens = fopen(nom_fichier_liens, "w");
    if (file_liens == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour la sauvegarde des liens.\n");
        return;
    }

    fprintf(file_liens, "source,target\n");
    for (int i = 0; i < reseau->nb_especes; i++) {
        Espece* e = &reseau->especes[i];
        for (int j = 0; j < e->nb_proies; j++) {
            fprintf(file_liens, "%d,%d\n", i, e->proies[j]);
        }
    }
    fclose(file_liens);

    printf("Fichiers CSV sauvegardes avec succes : %s et %s\n", nom_fichier_noeuds, nom_fichier_liens);
}


void ajouter_espece(ReseauTrophique* reseau, const char* nom, const char* type, int niveau_trophique) {
    if (reseau->nb_especes < MAX_ESPECES) {
        Espece* e = &reseau->especes[reseau->nb_especes];
        strncpy(e->nom, nom, MAX_NOM - 1);
        strncpy(e->type, type, MAX_NOM - 1);
        e->niveau_trophique = niveau_trophique;
        e->nb_predateurs = 0;
        e->nb_proies = 0;
        e->population = 100.0;
        reseau->nb_especes++;
    }
}

int trouver_espece(ReseauTrophique* reseau, const char* nom) {
    for (int i = 0; i < reseau->nb_especes; i++) {
        if (strcmp(reseau->especes[i].nom, nom) == 0) {
            return i;
        }
    }
    return -1; // Espèce non trouvée
}

void ajouter_relation(ReseauTrophique* reseau, const char* predateur_nom, const char* proie_nom) {
    int pred_index = trouver_espece(reseau, predateur_nom);
    int proie_index = trouver_espece(reseau, proie_nom);

    if (pred_index != -1 && proie_index != -1) {
        Espece* predateur = &reseau->especes[pred_index];
        Espece* proie = &reseau->especes[proie_index];

        predateur->proies[predateur->nb_proies++] = proie_index;
        proie->predateurs[proie->nb_predateurs++] = pred_index;
    }
}

void afficher_reseau(ReseauTrophique* reseau) {
    printf("Reseau Trophique:\n");
    for (int i = 0; i < reseau->nb_especes; i++) {
        Espece* e = &reseau->especes[i];
        printf("\nEspece: %s\n", e->nom);
        printf("Type: %s\n", e->type);
        printf("Niveau trophique: %d\n", e->niveau_trophique);

        printf("Predateurs: ");
        for (int j = 0; j < e->nb_predateurs; j++) {
            printf("%s ", reseau->especes[e->predateurs[j]].nom);
        }

        printf("\nProies: ");
        for (int j = 0; j < e->nb_proies; j++) {
            printf("%s ", reseau->especes[e->proies[j]].nom);
        }
        printf("\n");
    }
}

void trouver_producteurs_primaires(ReseauTrophique* reseau) {
    printf("\nProducteurs primaires: ");
    for (int i = 0; i < reseau->nb_especes; i++) {
        if (reseau->especes[i].nb_proies == 0) {
            printf("%s ", reseau->especes[i].nom);
        }
    }
    printf("\n");
}

void trouver_consommateurs_superieurs(ReseauTrophique* reseau) {
    printf("Consommateurs superieurs: ");
    for (int i = 0; i < reseau->nb_especes; i++) {
        if (reseau->especes[i].nb_predateurs == 0) {
            printf("%s ", reseau->especes[i].nom);
        }
    }
    printf("\n");
}

void trouver_especes_une_seule_proie(ReseauTrophique* reseau) {
    printf("Especes avec une seule proie: ");
    for (int i = 0; i < reseau->nb_especes; i++) {
        if (reseau->especes[i].nb_proies == 1) {
            printf("%s ", reseau->especes[i].nom);
        }
    }
    printf("\n");
}

void dfs(ReseauTrophique* reseau, int espece_index, bool* visite) {
    visite[espece_index] = true;
    Espece* e = &reseau->especes[espece_index];

    for (int i = 0; i < e->nb_proies; i++) {
        if (!visite[e->proies[i]]) {
            dfs(reseau, e->proies[i], visite);
        }
    }

    for (int i = 0; i < e->nb_predateurs; i++) {
        if (!visite[e->predateurs[i]]) {
            dfs(reseau, e->predateurs[i], visite);
        }
    }
}

bool verifier_connexite(ReseauTrophique* reseau) {
    if (reseau->nb_especes == 0) return true;

    bool visite[MAX_ESPECES] = {false};
    dfs(reseau, 0, visite);

    for (int i = 0; i < reseau->nb_especes; i++) {
        if (!visite[i]) return false;
    }
    return true;
}

void sauvegarder_reseau(ReseauTrophique* reseau, const char* nom_fichier) {
    FILE* file = fopen(nom_fichier, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour la sauvegarde.\n");
        return;
    }

    for (int i = 0; i < reseau->nb_especes; i++) {
        Espece* e = &reseau->especes[i];
        fprintf(file, "%s,%s,%d", e->nom, e->type, e->niveau_trophique);

        for (int j = 0; j < e->nb_predateurs; j++) {
            fprintf(file, ",%s", reseau->especes[e->predateurs[j]].nom);
        }
        fprintf(file, ";");

        for (int j = 0; j < e->nb_proies; j++) {
            fprintf(file, ",%s", reseau->especes[e->proies[j]].nom);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void charger_reseau(ReseauTrophique* reseau, const char* nom_fichier) {
    FILE* file = fopen(nom_fichier, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour le chargement.\n");
        return;
    }

    reseau->nb_especes = 0;
    char ligne[1000];

    while (fgets(ligne, sizeof(ligne), file)) {
        char nom[MAX_NOM], type[MAX_NOM];
        int niveau_trophique;
        sscanf(ligne, "%[^,],%[^,],%d", nom, type, &niveau_trophique);
        ajouter_espece(reseau, nom, type, niveau_trophique);
    }

    fseek(file, 0, SEEK_SET);

    while (fgets(ligne, sizeof(ligne), file)) {
        char* token = strtok(ligne, ",");
        char nom[MAX_NOM];
        strcpy(nom, token);

        token = strtok(NULL, ";");
        token = strtok(NULL, ",");

        while (token != NULL) {
            ajouter_relation(reseau, nom, token);
            token = strtok(NULL, ",");
        }
    }

    fclose(file);
}


void initialiser_reseau_foret_temperee(ReseauTrophique* reseau) {
    strcpy(reseau->ecosysteme, "Ecosysteme de foret temperee");
    strcpy(reseau->region, "Europe centrale");
    strcpy(reseau->climat, "Tempere continental");


    // Producteurs
    ajouter_espece(reseau, "Arbres", "Producteur", 1);
    ajouter_espece(reseau, "Plantes_herbacees", "Producteur", 1);

    // Consommateurs primaires
    ajouter_espece(reseau, "Pucerons", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Autres_insectes_herbivores", "Consommateur primaire", 2);

    // Consommateurs secondaires
    ajouter_espece(reseau, "Coccinelles", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Syrphes", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Chrysopes", "Consommateur secondaire", 3);

    // Consommateurs tertiaires
    ajouter_espece(reseau, "Oiseaux_insectivores", "Consommateur tertiaire", 4);
    ajouter_espece(reseau, "Araignees", "Consommateur tertiaire", 4);

    // Décomposeurs
    ajouter_espece(reseau, "Champignons", "Decomposeur", 1);
    ajouter_espece(reseau, "Bacteries_du_sol", "Decomposeur", 1);

    // Ajout des relations trophiques
    ajouter_relation(reseau, "Pucerons", "Arbres");
    ajouter_relation(reseau, "Pucerons", "Plantes_herbacees");
    ajouter_relation(reseau, "Autres_insectes_herbivores", "Arbres");
    ajouter_relation(reseau, "Autres_insectes_herbivores", "Plantes_herbacees");
    ajouter_relation(reseau, "Coccinelles", "Pucerons");
    ajouter_relation(reseau, "Syrphes", "Pucerons");
    ajouter_relation(reseau, "Chrysopes", "Pucerons");
    ajouter_relation(reseau, "Oiseaux_insectivores", "Coccinelles");
    ajouter_relation(reseau, "Oiseaux_insectivores", "Syrphes");
    ajouter_relation(reseau, "Oiseaux_insectivores", "Chrysopes");
    ajouter_relation(reseau, "Araignees", "Pucerons");
    ajouter_relation(reseau, "Araignees", "Autres_insectes_herbivores");
}
void initialiser_reseau_mer_baltique(ReseauTrophique* reseau) {
    strcpy(reseau->ecosysteme, "Ecosysteme cotier de la mer Baltique");
    strcpy(reseau->region, "Mer Baltique");
    strcpy(reseau->climat, "Tempere maritime");

    // Producteurs primaires
    ajouter_espece(reseau, "Phytoplancton", "Producteur", 1);
    ajouter_espece(reseau, "Macroalgues", "Producteur", 1);
    ajouter_espece(reseau, "Plantes_aquatiques", "Producteur", 1);
    ajouter_espece(reseau, "Cyanobacteries", "Producteur", 1);
    ajouter_espece(reseau, "Herbes_marines", "Producteur", 1);
    ajouter_espece(reseau, "Algues_brunes", "Producteur", 1); // Nouvelle espèce
    ajouter_espece(reseau, "Algues_vertes", "Producteur", 1); // Nouvelle espèce
    ajouter_espece(reseau, "Zosteres", "Producteur", 1); // Nouvelle espèce

    // Consommateurs primaires
    ajouter_espece(reseau, "Zooplancton", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Moules", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Mysidaces", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Crevettes", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Coques", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Ostreiculteurs", "Consommateur primaire", 2); // Nouvelle espèce

    // Consommateurs secondaires
    ajouter_espece(reseau, "Sprat", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Hareng", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Gobies", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Sole", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Truite_de_mer", "Consommateur secondaire", 3); // Nouvelle espèce

    // Consommateurs tertiaires
    ajouter_espece(reseau, "Morue", "Consommateur tertiaire", 4);
    ajouter_espece(reseau, "Flet", "Consommateur tertiaire", 4);
    ajouter_espece(reseau, "Loup_de_mer", "Consommateur tertiaire", 4);
    ajouter_espece(reseau, "Raie," ,"Consommateur tertiaire" ,4); // Nouvelle espèce

    // Consommateurs quaternaires
    ajouter_espece(reseau, "Phoques," ,"Consommateur quaternaire" ,5);
    ajouter_espece(reseau, "Orques," ,"Consommateur quaternaire" ,5);
    ajouter_espece(reseau,"Aigle_de_mer","Consommateur quaternaire" ,5); // Nouvelle espèce

    // Décomposeurs
    ajouter_espece(reseau,"Bacteries_marines","Decomposeur" ,1);
    ajouter_espece(reseau,"Vers_marins","Decomposeur" ,2);

    // Ajout des relations trophiques
    ajouter_relation(reseau,"Zooplancton","Phytoplancton");
    ajouter_relation(reseau,"Moules","Phytoplancton");
    ajouter_relation(reseau,"Mysidaces","Phytoplancton");
    ajouter_relation(reseau,"Mysidaces","Macroalgues");

    // Nouvelles relations pour les nouvelles espèces
    ajouter_relation(reseau,"Crevettes","Phytoplancton");
    ajouter_relation(reseau,"Coques","Phytoplancton");

    // Relations pour les consommateurs secondaires
    ajouter_relation(reseau,"Sprat","Zooplancton");
    ajouter_relation(reseau,"Hareng","Zooplancton");

    // Nouvelles relations pour les nouveaux consommateurs secondaires
    ajouter_relation(reseau,"Sole","Mysidaces");
    ajouter_relation(reseau,"Truite_de_mer","Mysidaces");

    // Relations pour les consommateurs tertiaires
    ajouter_relation(reseau,"Morue","Sprat");
    ajouter_relation(reseau,"Morue","Hareng");
    ajouter_relation(reseau,"Morue","Gobies");

    // Nouvelles relations pour les consommateurs tertiaires
    ajouter_relation(reseau,"Flet","Moules");
    ajouter_relation(reseau,"Flet","Mysidacés");
    ajouter_relation(reseau,"Loup_de_mer","Sprat");
    ajouter_relation(reseau,"Loup_de_mer","Morue");
    ajouter_relation(reseau,"Raie","Moules");

    // Relations pour les consommateurs quaternaires
    ajouter_relation(reseau,"Phoques","Morue");
    ajouter_relation(reseau,"Phoques","Flet");
    ajouter_relation (reseau , "Orques","Phoques");
    ajouter_relation (reseau , "Aigle_de_mer", "Phoques ");

    // Relation avec les décomposeurs
    ajouter_relation (reseau , "Bacteries_marines ", " Phytoplancton ");
    ajouter_relation (reseau , "Bacteries_marines","Macroalgues");

}
void trouver_producteurs_primaires2(ReseauTrophique* reseau_mer_baltique) {
    printf("\nProducteurs primaires: ");
    for (int i = 0; i < reseau_mer_baltique->nb_especes; i++) {
        if (reseau_mer_baltique->especes[i].nb_proies == 0) {
            printf("%s ", reseau_mer_baltique->especes[i].nom);
        }
    }
    printf("\n");
}

void trouver_consommateurs_superieurs2(ReseauTrophique* reseau_mer_baltique) {
    printf("Consommateurs superieurs: ");
    for (int i = 0; i < reseau_mer_baltique->nb_especes; i++) {
        if (reseau_mer_baltique->especes[i].nb_predateurs == 0) {
            printf("%s ", reseau_mer_baltique->especes[i].nom);
        }
    }
    printf("\n");
}

void trouver_especes_une_seule_proie2(ReseauTrophique* reseau_mer_baltique) {
    printf("Especes avec une seule proie: ");
    for (int i = 0; i < reseau_mer_baltique->nb_especes; i++) {
        if (reseau_mer_baltique->especes[i].nb_proies == 1) {
            printf("%s ", reseau_mer_baltique->especes[i].nom);
        }
    }
    printf("\n");
}






// Fonction pour calculer la complexité du réseau
void calculer_complexite(ReseauTrophique* reseau) {
    int nb_especes = reseau->nb_especes;
    int hauteur_trophique = 0;
    int nb_liens = 0;
    int degres[MAX_ESPECES] = {0};

    for (int i = 0; i < nb_especes; i++) {
        if (reseau->especes[i].niveau_trophique > hauteur_trophique) {
            hauteur_trophique = reseau->especes[i].niveau_trophique;
        }
        nb_liens += reseau->especes[i].nb_proies;
        degres[i] = reseau->especes[i].nb_predateurs + reseau->especes[i].nb_proies;
    }

    double densite = (double)nb_liens / (nb_especes * (nb_especes - 1));

    printf("Complexite du reseau:\n");
    printf("Nombre d especes: %d\n", nb_especes);
    printf("Hauteur trophique: %d\n", hauteur_trophique);
    printf("Densite de liaison: %.4f\n", densite);
    printf("Distribution des degres:\n");
    for (int i = 0; i < nb_especes; i++) {
        printf("%s: %d\n", reseau->especes[i].nom, degres[i]);
    }
}


void dfs_chaine(ReseauTrophique* reseau, int espece_index, char* chaine) {
    Espece* e = &reseau->especes[espece_index];
    char nouvelle_chaine[1000];
    sprintf(nouvelle_chaine, "%s -> %s", chaine, e->nom);

    if (e->nb_proies == 0) {
        printf("%s\n", nouvelle_chaine + 4); // +4 pour sauter le " -> " initial
        return;
    }

    for (int i = 0; i < e->nb_proies; i++) {
        dfs_chaine(reseau, e->proies[i], nouvelle_chaine);
    }
}

void afficher_chaines_alimentaires(ReseauTrophique* reseau, const char* nom_espece) {
    int index = trouver_espece(reseau, nom_espece);
    if (index == -1) {
        printf("Espece non trouvee.\n");
        return;
    }

    printf("Chaines alimentaires pour %s :\n", nom_espece);
    dfs_chaine(reseau, index, "");
}



void afficher_contexte_reseau(ReseauTrophique* reseau) {
    printf("Contexte du reseau :\n");
    printf("Ecosysteme : %s\n", reseau->ecosysteme);
    printf("Region : %s\n", reseau->region);
    printf("Climat : %s\n", reseau->climat);
}



// Fonction pour calculer la centralité de degré
void calculer_centralite_degre(ReseauTrophique* reseau) {
    printf("Centralite de degre pour chaque espece :\n");
    for (int i = 0; i < reseau->nb_especes; i++) {
        Espece* e = &reseau->especes[i];
        int degre = e->nb_predateurs + e->nb_proies;
        printf("%s : %d\n", e->nom, degre);
    }
}
void calculer_intermediarite(ReseauTrophique* reseau) {
    int n = reseau->nb_especes;
    double* intermediarite = calloc(n, sizeof(double));

    // Pour chaque paire de sommets (s,t)
    for (int s = 0; s < n; s++) {
        for (int t = s + 1; t < n; t++) {
            // BFS pour trouver tous les plus courts chemins entre s et t
            int* predecesseurs = calloc(n, sizeof(int));
            int* distance = calloc(n, sizeof(int));
            int* nb_chemins = calloc(n, sizeof(int));

            for (int i = 0; i < n; i++) {
                distance[i] = -1;
            }

            int queue[MAX_ESPECES];
            int debut = 0, fin = 0;
            queue[fin++] = s;
            distance[s] = 0;
            nb_chemins[s] = 1;

            while (debut != fin) {
                int v = queue[debut++];
                Espece* espece_v = &reseau->especes[v];

                // Parcourir les voisins (prédateurs et proies)
                for (int i = 0; i < espece_v->nb_predateurs; i++) {
                    int w = espece_v->predateurs[i];
                    if (distance[w] == -1) {
                        queue[fin++] = w;
                        distance[w] = distance[v] + 1;
                    }
                    if (distance[w] == distance[v] + 1) {
                        nb_chemins[w] += nb_chemins[v];
                        predecesseurs[w] = v;
                    }
                }
                for (int i = 0; i < espece_v->nb_proies; i++) {
                    int w = espece_v->proies[i];
                    if (distance[w] == -1) {
                        queue[fin++] = w;
                        distance[w] = distance[v] + 1;
                    }
                    if (distance[w] == distance[v] + 1) {
                        nb_chemins[w] += nb_chemins[v];
                        predecesseurs[w] = v;
                    }
                }
            }

            // Calculer la contribution de chaque sommet intermédiaire
            double* delta = calloc(n, sizeof(double));
            for (int v = n - 1; v >= 0; v--) {
                if (v != s && v != t) {
                    if (nb_chemins[v] > 0) { // Éviter la division par zéro
                        delta[predecesseurs[v]] += (1.0 + delta[v]) * nb_chemins[predecesseurs[v]] / nb_chemins[v];
                        intermediarite[predecesseurs[v]] += delta[v];
                    }
                }
            }

            free(predecesseurs);
            free(distance);
            free(nb_chemins);
            free(delta);
        }
    }

    // Normaliser et afficher les résultats
    double max_intermediarite = 0;
    for (int i = 0; i < n; i++) {
        if (intermediarite[i] > max_intermediarite) {
            max_intermediarite = intermediarite[i];
        }
    }

    printf("Intermediarite normalisee pour chaque espece :\n");
    for (int i = 0; i < n; i++) {
        printf("%s : %.4f\n", reseau->especes[i].nom, max_intermediarite > 0 ? intermediarite[i] / max_intermediarite : 0);
    }

    free(intermediarite);
}
// Fonction principale pour estimer l'importance relative des espèces
void estimer_importance_especes(ReseauTrophique* reseau) {
    printf("Estimation de l importance relative des especes :\n\n");
    calculer_centralite_degre(reseau);
    printf("\n");
    calculer_intermediarite(reseau);
}

void trouver_niveaux_trophiques(ReseauTrophique* reseau, int espece_index, bool* visite, int* niveaux, int* nb_niveaux) {
    visite[espece_index] = true;
    Espece* espece = &reseau->especes[espece_index];

    // Ajouter le niveau trophique actuel si ce n'est pas déjà présent
    if (*nb_niveaux == 0 || niveaux[*nb_niveaux - 1] != espece->niveau_trophique) {
        niveaux[*nb_niveaux] = espece->niveau_trophique;
        (*nb_niveaux)++;
    }

    // Explorer les proies (espèces consommées)
    for (int i = 0; i < espece->nb_proies; i++) {
        int proie_index = espece->proies[i];
        if (!visite[proie_index]) {
            trouver_niveaux_trophiques(reseau, proie_index, visite, niveaux, nb_niveaux);
        }
    }
}

void afficher_niveaux_trophiques(ReseauTrophique* reseau, const char* nom_espece) {
    int index = trouver_espece(reseau, nom_espece);
    if (index == -1) {
        printf("Espece non trouvee.\n");
        return;
    }

    bool visite[MAX_ESPECES] = {false};
    int niveaux[MAX_ESPECES];
    int nb_niveaux = 0;

    // Appel à la fonction pour trouver tous les niveaux
    trouver_niveaux_trophiques(reseau, index, visite, niveaux, &nb_niveaux);

    // Utiliser un ensemble pour éviter les doublons
    int unique_levels[MAX_ESPECES] = {0};
    int unique_count = 0;

    for (int i = 0; i < nb_niveaux; i++) {
        bool found = false;
        for (int j = 0; j < unique_count; j++) {
            if (unique_levels[j] == niveaux[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            unique_levels[unique_count++] = niveaux[i];
        }
    }

    printf("Niveaux trophiques pour %s : ", nom_espece);
    for (int i = 0; i < unique_count; i++) {
        printf("%d ", unique_levels[i]);
    }
    printf("\n");
}

void simuler_disparition_espece(ReseauTrophique* reseau, const char* nom_espece) {
    int index = trouver_espece(reseau, nom_espece);
    if (index == -1) {
        printf("Espece non trouvee.\n");
        return;
    }

    // Afficher l'espèce qui va être supprimée
    printf("Suppression de l'espece : %s\n", nom_espece);

    // Supprimer l'espèce des listes de proies de ses prédateurs
    for (int i = 0; i < reseau->especes[index].nb_predateurs; i++) {
        int predateur_index = reseau->especes[index].predateurs[i];
        Espece* predateur = &reseau->especes[predateur_index];

        // Retirer l'espèce disparue de la liste des proies
        for (int j = 0; j < predateur->nb_proies; j++) {
            if (predateur->proies[j] == index) {
                // Supprimer cette proie
                for (int k = j; k < predateur->nb_proies - 1; k++) {
                    predateur->proies[k] = predateur->proies[k + 1];
                }
                predateur->nb_proies--;
                break;
            }
        }
    }

    // Supprimer l'espèce des listes de prédateurs de ses proies
    for (int i = 0; i < reseau->especes[index].nb_proies; i++) {
        int proie_index = reseau->especes[index].proies[i];
        Espece* proie = &reseau->especes[proie_index];

        // Retirer l'espèce disparue de la liste des prédateurs
        for (int j = 0; j < proie->nb_predateurs; j++) {
            if (proie->predateurs[j] == index) {
                // Supprimer ce prédateur
                for (int k = j; k < proie->nb_predateurs - 1; k++) {
                    proie->predateurs[k] = proie->predateurs[k + 1];
                }
                proie->nb_predateurs--;
                break;
            }
        }
    }

    // Marquer l'espèce comme inactive ou supprimer complètement
    for (int i = index; i < reseau->nb_especes - 1; i++) {
        reseau->especes[i] = reseau->especes[i + 1];
    }
    reseau->nb_especes--;

    printf("Simulation de la disparition de %s effectuee.\n", nom_espece);

    // Afficher le réseau après suppression
    afficher_reseau(reseau);
}
void configurer_parametres_population(ReseauTrophique* reseau) {
    for (int i = 0; i < reseau->nb_especes; i++) {
        Espece* e = &reseau->especes[i];

        printf("Configurer %s:\n", e->nom);

        // Demander le taux de croissance
        printf("Entrez le taux de croissance (r) pour %s (actuel: %.2f): ", e->nom, e->taux_croissance);
        scanf("%lf", &e->taux_croissance);

        // Demander la capacité de charge
        printf("Entrez la capacité de charge (K) pour %s (actuel: %.2f): ", e->nom, e->capacite_charge);
        scanf("%lf", &e->capacite_charge);

        // Demander la population initiale
        printf("Entrez la population initiale pour %s (actuel: %.2f): ", e->nom, e->population);
        scanf("%lf", &e->population);
    }
}
void simuler_dynamique_populations(ReseauTrophique* reseau, int iterations) {
    for (int t = 0; t < iterations; t++) {
        printf("Iteration %d:\n", t + 1);
        for (int i = 0; i < reseau->nb_especes; i++) {
            Espece* e = &reseau->especes[i];
            double N = e->population;
            double r = e->taux_croissance; // Taux de croissance
            double K = e->capacite_charge; // Capacité de charge

            // Calculer le changement de population
            double dN = r * N * (1 - N / K);
            e->population += dN;

            // S'assurer que la population ne devient pas négative
            if (e->population < 0) {
                e->population = 0;
            }

            printf("%s: Population = %.2f\n", e->nom, e->population);
        }
        printf("\n");
    }
}
