#include "reseautrophique1.h"

// Fonction pour sauvegarder le réseau trophique dans deux fichiers CSV : un pour les nœuds (espèces) et un pour les liens (relations)
void sauvegarder_csv(ReseauTrophique* reseau, const char* nom_fichier_base) {
    char nom_fichier_noeuds[100];
    char nom_fichier_liens[100];

    // Création des noms de fichiers pour les nœuds et les liens
    sprintf(nom_fichier_noeuds, "%snoeuds.csv", nom_fichier_base);
    sprintf(nom_fichier_liens, "%sliens.csv", nom_fichier_base);

    // Sauvegarde des nœuds (espèces)
    FILE* file_noeuds = fopen(nom_fichier_noeuds, "w");
    if (file_noeuds == NULL) {
        printf("Erreur lors de l ouverture du fichier pour la sauvegarde des nœuds.\n");
        return;
    }

    // Écriture de l'en-tête du fichier des nœuds
    fprintf(file_noeuds, "id,nom,type,niveau_trophique,population\n");

    // Écriture des données de chaque espèce
    for (int i = 0; i < reseau->nb_especes; i++) {
        Espece* e = &reseau->especes[i]; // Obtention d'un pointeur vers l'espèce courante

        fprintf(file_noeuds, "%d,%s,%s,%d,%.2f\n", i, e->nom, e->type, e->niveau_trophique, e->population);         // Écriture des données de l'espèce dans le fichier CSV : id, nom, type, niveau trophique, population
    }
    fclose(file_noeuds);

    // Sauvegarde des liens (relations entre espèces)
    FILE* file_liens = fopen(nom_fichier_liens, "w");
    if (file_liens == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour la sauvegarde des liens.\n");
        return;
    }

    // Écriture de l'en-tête du fichier des liens
    fprintf(file_liens, "source,target,weight\n");

    // Écriture des données de chaque lien (relation prédateur-proie)
    for (int i = 0; i < reseau->nb_especes; i++) {
        Espece* e = &reseau->especes[i];  // Obtention d'un pointeur vers l'espèce prédatrice courante
        for (int j = 0; j < e->nb_proies; j++) {
            float poids = e->poids_proies[j]; // Récupération du poids associé à la proie
            fprintf(file_liens, "%d,%d,%.2f\n", i, e->proies[j], poids); // Écriture des données de la relation dans le fichier CSV : id prédateur, id proie, poids de la relation
        }
    }
    fclose(file_liens);

    printf("Fichiers CSV sauvegardes avec succes : %s et %s\n", nom_fichier_noeuds, nom_fichier_liens);
}

// Fonction pour ajouter une nouvelle espèce au réseau trophique
void ajouter_espece(ReseauTrophique* reseau, const char* nom, const char* type, int niveau_trophique) {
    // Vérifier si le réseau n'a pas atteint sa capacité maximale
    if (reseau->nb_especes < MAX_ESPECES) {
        Espece* e = &reseau->especes[reseau->nb_especes];  // Obtenir un pointeur vers la nouvelle espèce à ajouter

        // Copier le nom et le type de l'espèce (avec limitation de la taille)
        strncpy(e->nom, nom, MAX_NOM - 1);
        strncpy(e->type, type, MAX_NOM - 1);

        // Initialiser les attributs de l'espèce
        e->niveau_trophique = niveau_trophique;
        e->nb_predateurs = 0;
        e->nb_proies = 0;
        e->population = 100.0; // Population initiale par défaut
        reseau->nb_especes++;      // Incrémenter le nombre total d'espèces dans le réseau
    }
}

// Fonction pour trouver l'index d'une espèce dans le réseau trophique par son nom
int trouver_espece(ReseauTrophique* reseau, const char* nom) {
    // Parcourir toutes les espèces du réseau
    for (int i = 0; i < reseau->nb_especes; i++) {
        // Comparer le nom de l'espèce courante avec le nom recherché
        if (strcmp(reseau->especes[i].nom, nom) == 0) {
            return i; // Retourner l'index de l'espèce si trouvée
        }
    }
    return -1; // Retourner -1 si l'espèce n'est pas trouvée
}

// Fonction pour ajouter une relation trophique entre deux espèces
void ajouter_relation(ReseauTrophique* reseau, const char* predateur_nom, const char* proie_nom, float poids) {

    // Trouver les indices des espèces prédateur et proie dans le réseau
    int pred_index = trouver_espece(reseau, predateur_nom);
    int proie_index = trouver_espece(reseau, proie_nom);

    // Vérifier si les deux espèces existent dans le réseau
    if (pred_index != -1 && proie_index != -1) {
        // Obtenir des pointeurs vers les espèces prédateur et proie
        Espece* predateur = &reseau->especes[pred_index];
        Espece* proie = &reseau->especes[proie_index];

        // Ajouter la proie à la liste des proies du prédateur
        predateur->proies[predateur->nb_proies] = proie_index;
        predateur->poids_proies[predateur->nb_proies] = poids;
        predateur->nb_proies++;
// Ajouter le prédateur à la liste des prédateurs de la proie
        proie->predateurs[proie->nb_predateurs] = pred_index;
        proie->poids_predateurs[proie->nb_predateurs] = poids;
        proie->nb_predateurs++;
    }
}


// Fonction pour afficher le réseau trophique
void afficher_reseau(ReseauTrophique* reseau) {
    printf("Reseau Trophique:\n");

    // Parcourir toutes les espèces du réseau
    for (int i = 0; i < reseau->nb_especes; i++) {
        Espece* e = &reseau->especes[i];
        // Afficher les informations de base de l'espèce
        printf("\nEspece: %s\n", e->nom);
        printf("Type: %s\n", e->type);
        printf("Niveau trophique: %d\n", e->niveau_trophique);

        // Afficher les prédateurs de l'espèce
        printf("Predateurs: ");
        for (int j = 0; j < e->nb_predateurs; j++) {
            printf("%s ", reseau->especes[e->predateurs[j]].nom);
        }

        // Afficher les proies de l'espèce
        printf("\nProies: ");
        for (int j = 0; j < e->nb_proies; j++) {
            printf("%s ", reseau->especes[e->proies[j]].nom);
        }
        printf("\n");
    }
}

void afficher_contexte_reseau(ReseauTrophique* reseau) {
    printf("Contexte du reseau :\n");
    printf("Ecosysteme : %s\n", reseau->ecosysteme);
    printf("Region : %s\n", reseau->region);
    printf("Climat : %s\n", reseau->climat);
}


void initialiser_Reseau_Prairie(ReseauTrophique* reseau) {
    strcpy(reseau->ecosysteme, "Ecosysteme de Prairie");
    strcpy(reseau->region, "Prairie ");
    strcpy(reseau->climat, "Tropical");

    // Producteurs
    ajouter_espece(reseau, "Graminees", "Producteur", 1);
    ajouter_espece(reseau, "Arbustes", "Producteur", 1);

    // Consommateurs Primaires
    ajouter_espece(reseau, "Gazelle", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Zebre", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Antilope", "Consommateur primaire", 2);

    // Consommateurs Secondaires
    ajouter_espece(reseau, "Hyene", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Lion", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Guepard", "Consommateur secondaire", 3);

    // Relations Trophiques
    ajouter_relation(reseau, "Gazelle", "Graminees", 0.7);
    ajouter_relation(reseau, "Zebre", "Graminees", 0.8);
    ajouter_relation(reseau, "Antilope", "Arbustes", 0.6);
    ajouter_relation(reseau, "Antilope", "Graminees", 0.5);
    ajouter_relation(reseau, "Hyene", "Gazelle", 0.5);
    ajouter_relation(reseau, "Hyene", "Zebre", 0.4);
    ajouter_relation(reseau, "Lion", "Gazelle", 0.6);
    ajouter_relation(reseau, "Lion", "Zebre", 0.5);
    ajouter_relation(reseau, "Lion", "Antilope", 0.5);
    ajouter_relation(reseau, "Guepard", "Antilope", 0.7);
    ajouter_relation(reseau, "Guepard", "Gazelle", 0.6);
}
void initialiser_reseau_foret_temperee(ReseauTrophique* reseau) {
    strcpy(reseau->ecosysteme, "Ecosysteme de foret temperee");
    strcpy(reseau->region, "Europe centrale");
    strcpy(reseau->climat, "Tempere continental");

    // Producteurs primaires
    ajouter_espece(reseau, "Arbres", "Producteur", 1);
    ajouter_espece(reseau, "Arbustes", "Producteur", 1);
    ajouter_espece(reseau, "Plantes_herbacees", "Producteur", 1);

    // Consommateurs primaires
    ajouter_espece(reseau, "Cerfs", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Ecureuils", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Chenilles", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Campagnols", "Consommateur primaire", 2);

    // Consommateurs secondaires
    ajouter_espece(reseau, "Renards", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Chouettes", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Pics_verts", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Belettes", "Consommateur secondaire", 3);

    // Consommateurs tertiaires
    ajouter_espece(reseau, "Lynx", "Consommateur tertiaire", 4);
    ajouter_espece(reseau, "Aigles", "Consommateur tertiaire", 4);

    // Décomposeurs
    ajouter_espece(reseau, "Champignons", "Decomposeur", 1);

    // Relations trophiques
    ajouter_relation(reseau, "Cerfs", "Arbres", 0.7);
    ajouter_relation(reseau, "Cerfs", "Arbustes", 0.6);
    ajouter_relation(reseau, "Ecureuils", "Arbres", 0.5);
    ajouter_relation(reseau, "Chenilles", "Plantes_herbacees", 0.8);
    ajouter_relation(reseau, "Campagnols", "Plantes_herbacees", 0.7);
    ajouter_relation(reseau, "Renards", "Ecureuils", 0.6);
    ajouter_relation(reseau, "Renards", "Campagnols", 0.7);
    ajouter_relation(reseau, "Chouettes", "Ecureuils", 0.5);
    ajouter_relation(reseau, "Chouettes", "Campagnols", 0.6);
    ajouter_relation(reseau, "Pics_verts", "Chenilles", 0.7);
    ajouter_relation(reseau, "Belettes", "Campagnols", 0.8);
    ajouter_relation(reseau, "Lynx", "Cerfs", 0.4);
    ajouter_relation(reseau, "Lynx", "Renards", 0.3);
    ajouter_relation(reseau, "Aigles", "Ecureuils", 0.3);
    ajouter_relation(reseau, "Aigles", "Campagnols", 0.4);
    ajouter_relation(reseau, "Champignons", "Arbres", 0.2);
}
void initialiser_reseau_mer_caraibes(ReseauTrophique* reseau) {
    strcpy(reseau->ecosysteme, "Ecosysteme marin de la mer des Caraibes");
    strcpy(reseau->region, "Mer des Caraibes");
    strcpy(reseau->climat, "Tropical maritime");

    // Niveau 1 : Producteurs primaires
    ajouter_espece(reseau, "Phytoplancton", "Producteur", 1);
    ajouter_espece(reseau, "Algues_benthiques", "Producteur", 1);
    ajouter_espece(reseau, "Mangroves", "Producteur", 1);
    ajouter_espece(reseau, "Herbiers_marins", "Producteur", 1);

    // Niveau 2 : Consommateurs primaires
    ajouter_espece(reseau, "Zooplancton", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Coraux", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Mollusques_herbivores", "Consommateur primaire", 2);
    ajouter_espece(reseau, "Oursins", "Consommateur primaire", 2);

    // Niveau 3 : Consommateurs secondaires
    ajouter_espece(reseau, "Petits_poissons_recifaux", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Crustaces", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Cephalopodes", "Consommateur secondaire", 3);
    ajouter_espece(reseau, "Etoiles_de_mer", "Consommateur secondaire", 3);

    // Niveau 4 : Consommateurs tertiaires
    ajouter_espece(reseau, "Poissons_predateurs_moyens", "Consommateur tertiaire", 4);
    ajouter_espece(reseau, "Tortues_marines", "Consommateur tertiaire", 4);
    ajouter_espece(reseau, "Raies", "Consommateur tertiaire", 4);
    ajouter_espece(reseau, "Petits_requins", "Consommateur tertiaire", 4);

    // Niveau 5 : Consommateurs quaternaires
    ajouter_espece(reseau, "Grands_poissons_predateurs", "Consommateur quaternaire", 5);
    ajouter_espece(reseau, "Requins_moyens", "Consommateur quaternaire", 5);
    ajouter_espece(reseau, "Dauphins", "Consommateur quaternaire", 5);

    // Niveau 6 : Prédateurs apex
    ajouter_espece(reseau, "Grands_requins", "Predateur apex", 6);
    ajouter_espece(reseau, "Orques", "Predateur apex", 6);

    // Relations trophiques
    ajouter_relation(reseau, "Zooplancton", "Phytoplancton", 0.9);
    ajouter_relation(reseau, "Coraux", "Phytoplancton", 0.7);
    ajouter_relation(reseau, "Mollusques_herbivores", "Algues_benthiques", 0.6);
    ajouter_relation(reseau, "Oursins", "Algues_benthiques", 0.5);

    // Connexions entre consommateurs primaires et secondaires
    ajouter_relation(reseau, "Petits_poissons_recifaux", "Zooplancton", 0.8);
    ajouter_relation(reseau, "Crustaces", "Zooplancton", 0.7);

    // Connexions entre consommateurs secondaires et tertiaires
    ajouter_relation(reseau, "Cephalopodes", "Petits_poissons_recifaux", 0.6);
    ajouter_relation(reseau, "Etoiles_de_mer", "Coraux", 0.4);

    // Connexions avec les consommateurs tertiaires
    ajouter_relation(reseau, "Poissons_predateurs_moyens", "Petits_poissons_recifaux", 0.7);

    // Ajout d'autres relations pour renforcer la connexité
    ajouter_relation(reseau,"Tortues_marines","Mollusques_herbivores" ,0.5 );
    ajouter_relation(reseau,"Raies","Crustaces" ,0.6 );
    ajouter_relation(reseau,"Petits_requins","Cephalopodes" ,0.5 );

    // Connexions avec les consommateurs quaternaires
    ajouter_relation(reseau,"Grands_poissons_predateurs","Poissons_predateurs_moyens" ,0.6 );

    // Connexions avec les prédateurs apex
    ajouter_relation(reseau,"Requins_moyens","Raies" ,0.5 );
    ajouter_relation(reseau,"Dauphins","Petits_poissons_recifaux" ,0.7 );

    // Relations avec les prédateurs apex
    ajouter_relation(reseau,"Grands_requins","Requins_moyens" ,0.4 );
    ajouter_relation(reseau,"Orques","Dauphins" ,0.3 );
    ajouter_relation(reseau,"Grands_requins","Tortues_marines" ,0.3 );
    ajouter_relation(reseau,"Orques","Grands_poissons_predateurs" ,0.2 );
    ajouter_relation(reseau,"Dauphins","Cephalopodes" ,0.5 );
    ajouter_relation(reseau,"Grands_poissons_predateurs","Cephalopodes" ,0.4 );
    ajouter_relation(reseau,"Requins_moyens","Tortues_marines" ,0.3 );

    // Ajout de relations entre producteurs et consommateurs pour assurer la connexité
    ajouter_relation(reseau,"Mangroves","Oursins" ,0.3 );
    ajouter_relation(reseau,"Herbiers_marins","Zooplancton" ,0.4 );
}

// Fonction pour trouver et afficher les producteurs primaires dans le réseau trophique
void trouver_producteurs_primaires(ReseauTrophique* reseau_actuel) {
    printf("\nProducteurs primaires: ");
    // Parcourir toutes les espèces du réseau
    for (int i = 0; i < reseau_actuel->nb_especes; i++) {
        // Vérifier si l'espèce n'a pas de proies (caractéristique des producteurs primaires)
        if (reseau_actuel->especes[i].nb_proies == 0) {
            printf("%s ", reseau_actuel->especes[i].nom);
        }
    }
    printf("\n");
}

// Fonction pour trouver et afficher les consommateurs supérieurs dans le réseau trophique
void trouver_consommateurs_superieurs(ReseauTrophique* reseau_actuel) {
    printf("Consommateurs superieurs: ");
    // Parcourir toutes les espèces du réseau
    for (int i = 0; i < reseau_actuel->nb_especes; i++) {
        // Vérifier si l'espèce n'a pas de prédateurs (caractéristique des consommateurs supérieurs)
        if (reseau_actuel->especes[i].nb_predateurs == 0) {
            printf("%s ", reseau_actuel->especes[i].nom);
        }
    }
    printf("\n");
}

// Fonction pour trouver et afficher les espèces ayant une seule proie dans le réseau trophique
void trouver_especes_une_seule_proie(ReseauTrophique* reseau_actuel) {
    printf("Especes avec une seule proie: ");
    // Parcourir toutes les espèces du réseau
    for (int i = 0; i < reseau_actuel->nb_especes; i++) {
        // Vérifier si l'espèce a exactement une proie
        if (reseau_actuel->especes[i].nb_proies == 1) {
            printf("%s ", reseau_actuel->especes[i].nom);
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

// Parcourir toutes les espèces pour calculer les métriques de complexité
    for (int i = 0; i < nb_especes; i++) {
        // Trouver la hauteur trophique maximale
        if (reseau->especes[i].niveau_trophique > hauteur_trophique) {
            hauteur_trophique = reseau->especes[i].niveau_trophique;
        }
        // Compter le nombre total de liens trophiques
        nb_liens += reseau->especes[i].nb_proies;
        // Calculer le degré de chaque espèce (nombre total de connexions)
        degres[i] = reseau->especes[i].nb_predateurs + reseau->especes[i].nb_proies;
    }

    // Calculer la densité de liaison du réseau
    double densite = (double)nb_liens / (nb_especes * (nb_especes - 1));

    // Afficher les résultats de l'analyse de complexité
    printf("Complexite du reseau:\n");
    printf("Nombre d especes: %d\n", nb_especes);
    printf("Hauteur trophique: %d\n", hauteur_trophique);
    printf("Densite de liaison: %.4f\n", densite);
    printf("Distribution des degres:\n");
    for (int i = 0; i < nb_especes; i++) {
        printf("%s: %d\n", reseau->especes[i].nom, degres[i]);
    }
}


// Note: Cette fonction explore à la fois les proies et les prédateurs
// pour assurer une exploration complète du réseau dans les deux directions
void dfs(ReseauTrophique* reseau, int espece_index, bool* visite) {
    visite[espece_index] = true;
    // Obtenir un pointeur vers l'espèce courante pour faciliter l'accès à ses propriétés
    Espece* e = &reseau->especes[espece_index];
        // Explorer récursivement toutes les proies non visitées
    for (int i = 0; i < e->nb_proies; i++) {
        // Vérifier si la proie n'a pas encore été visitée
        if (!visite[e->proies[i]]) {
            dfs(reseau, e->proies[i], visite);
        }
    }
    // Explorer récursivement tous les prédateurs non visités
    for (int i = 0; i < e->nb_predateurs; i++) {
        // Vérifier si le prédateur n'a pas encore été visité
        if (!visite[e->predateurs[i]]) {
            dfs(reseau, e->predateurs[i], visite);
        }
    }
}


// Fonction pour vérifier la connexité du réseau trophique
bool verifier_connexite(ReseauTrophique* reseau) {
    // Si le réseau est vide, il est considéré comme connexe
    if (reseau->nb_especes == 0) return true;

    // Tableau pour marquer les espèces visitées
    bool visite[MAX_ESPECES] = {false};

    // Lancer un parcours en profondeur à partir de la première espèce
    dfs(reseau, 0, visite);
    dfs(reseau, 0, visite);

// Vérifier si toutes les espèces ont été visitées
    for (int i = 0; i < reseau->nb_especes; i++) {
        if (!visite[i]) return false;  // Si une espèce n'est pas visitée, le réseau n'est pas connexe
    }
    return true; // Toutes les espèces sont visitées, le réseau est connexe
}



// Fonction récursive pour explorer les chaînes alimentaires

void dfs_chaine(ReseauTrophique* reseau, int espece_index, char* chaine) {
    // Obtenir un pointeur vers l'espèce courante
    Espece* e = &reseau->especes[espece_index];

    char nouvelle_chaine[1000]; // Construire la nouvelle chaîne en ajoutant l'espèce courante

    sprintf(nouvelle_chaine, "%s -> %s", chaine, e->nom);

    // Si l'espèce n'a pas de proies, c'est la fin d'une chaîne alimentaire
    if (e->nb_proies == 0) {
        printf("%s\n", nouvelle_chaine + 4); // Afficher la chaîne (en sautant le " -> " initial)
        return;
    }

    // Explorer récursivement toutes les proies de l'espèce courante
    for (int i = 0; i < e->nb_proies; i++) {
        dfs_chaine(reseau, e->proies[i], nouvelle_chaine);
    }
}

// Fonction pour afficher les chaînes alimentaires d'une espèce donnée
void afficher_chaines_alimentaires(ReseauTrophique* reseau, const char* nom_espece) {
    // Trouver l'index de l'espèce dans le réseau
    int index = trouver_espece(reseau, nom_espece);

    // Vérifier si l'espèce existe dans le réseau
    if (index == -1) {
        printf("Espece non trouvee.\n");
        return;
    }
    // Afficher l'en-tête pour les chaînes alimentaires
    printf("Chaines alimentaires pour %s :\n", nom_espece);
    // Lancer la recherche en profondeur pour trouver toutes les chaînes alimentaires
    dfs_chaine(reseau, index, "");


}






// Fonction pour calculer la centralité de degré de chaque espèce dans le réseau
void calculer_centralite_degre(ReseauTrophique* reseau) {
    printf("Centralite de degre pour chaque espece :\n");

    // Parcourir toutes les espèces du réseau
    for (int i = 0; i < reseau->nb_especes; i++) {
        Espece* e = &reseau->especes[i];
        // Calculer le degré total (nombre de prédateurs + nombre de proies)
        int degre = e->nb_predateurs + e->nb_proies;
        // Afficher la centralité de degré pour l'espèce courante
        printf("%s : %d\n", e->nom, degre);
    }
}


// Fonction pour calculer l'intermédiarité de chaque espèce dans le réseau trophique
void calculer_intermediarite(ReseauTrophique* reseau) {
    int n = reseau->nb_especes;
    // Tableau pour stocker l'intermédiarité de chaque espèce
    double* intermediarite = calloc(n, sizeof(double));

    // Pour chaque paire de sommets (s,t) dans le réseau
    for (int s = 0; s < n; s++) {
        for (int t = s + 1; t < n; t++) {
            // BFS pour trouver tous les plus courts chemins entre s et t
            int* predecesseurs = calloc(n, sizeof(int));
            int* distance = calloc(n, sizeof(int));
            int* nb_chemins = calloc(n, sizeof(int));

            // Initialisation des distances à -1 (non visité)
            for (int i = 0; i < n; i++) {
                distance[i] = -1;
            }
            // File d'attente pour le BFS
            int queue[MAX_ESPECES];
            int debut = 0, fin = 0;
            queue[fin++] = s;
            distance[s] = 0;
            nb_chemins[s] = 1;
            // BFS pour trouver les plus courts chemins
            while (debut != fin) {
                int v = queue[debut++];
                Espece* espece_v = &reseau->especes[v];

                // Parcourir les voisins (prédateurs et proies)
                for (int i = 0; i < espece_v->nb_predateurs; i++) {
                    int w = espece_v->predateurs[i]; // Obtenir l'index du prédateur
                    // Si le prédateur n'a pas encore été visité
                    if (distance[w] == -1) {
                        queue[fin++] = w;  // Ajouter le prédateur à la file d'attente
                        distance[w] = distance[v] + 1; // Mettre à jour la distance du prédateur
                    }
                    if (distance[w] == distance[v] + 1) { // Si c'est un chemin de longueur minimale
                        nb_chemins[w] += nb_chemins[v];  // Mettre à jour le nombre de chemins
                        predecesseurs[w] = v;  // Mettre à jour le prédécesseur
                    }
                }

                // Parcourir les proies de l'espèce courante
                for (int i = 0; i < espece_v->nb_proies; i++) {
                    int w = espece_v->proies[i]; // Obtenir l'index de la proie

                    // Si la proie n'a pas encore été visitée
                    if (distance[w] == -1) {
                        queue[fin++] = w;  // Ajouter la proie à la file d'attente
                        distance[w] = distance[v] + 1;   // Mettre à jour la distance de la proie
                    }
                    // Si c'est un chemin de longueur minimale
                    if (distance[w] == distance[v] + 1) {
                        nb_chemins[w] += nb_chemins[v]; // Mettre à jour le nombre de chemins
                        predecesseurs[w] = v;// Mettre à jour le prédécesseur
                    }
                }
            }

            // Calculer la contribution de chaque sommet intermédiaire
            double* delta = calloc(n, sizeof(double));  // Allouer et initialiser le tableau delta
            for (int v = n - 1; v >= 0; v--) { // BOUCLE FOR ... Parcourir les sommets en ordre décroissant
                if (v != s && v != t) { // SI ... Ignorer les sommets source et destination
                    if (nb_chemins[v] > 0) { // SI ... Vérifier s'il y a des chemins passant par v
                        delta[predecesseurs[v]] += (1.0 + delta[v]) * nb_chemins[predecesseurs[v]] / nb_chemins[v]; // Calcul de la contribution du sommet v à l'intermédiarité de son prédécesseur
                        intermediarite[predecesseurs[v]] += delta[v];// Mise à jour de l'intermédiarité du prédécesseur de v
                    }
                }
            }
            // Libération de la mémoire allouée pour cette itération
            free(predecesseurs); // Libère l'espace mémoire predecesseurs
            free(distance); // Libère l'espace mémoire distance
            free(nb_chemins); // Libère l'espace mémoire nb_chemins
            free(delta); // Libère l'espace mémoire delta
        }
    }

    // Trouver la valeur maximale d'intermédiarité pour la normalisation
    double max_intermediarite = 0;
    for (int i = 0; i < n; i++) {  // Parcourir toutes les espèces
        if (intermediarite[i] > max_intermediarite) { // SI ...
            max_intermediarite = intermediarite[i];  // Mettre à jour le maximum
        }
    }

    printf("Intermediarite normalisee pour chaque espece :\n"); // DIRE ...Afficher l'intermédiarité normalisée pour chaque espèce
    for (int i = 0; i < n; i++) {  // Normalise l'intermédiarité en divisant par la valeur maximale (si elle n'est pas nul)
        printf("%s : %.4f\n", reseau->especes[i].nom, max_intermediarite > 0 ? intermediarite[i] / max_intermediarite : 0);
    }
    free(intermediarite); // Libérer la mémoire allouée pour le tableau d'intermédiarité
}

void estimer_importance_especes(ReseauTrophique* reseau) { // Fonction principale pour estimer l'importance relative des espèces
    printf("Estimation de l importance relative des especes :\n\n"); // DIRE ...
    calculer_centralite_degre(reseau); // Appel de la fonction pour calculer la centralité de degré des espèces
    printf("\n"); // retour à la ligne
    calculer_intermediarite(reseau); // Appel de la fonction pour calculer l'intermédiarité des espèces
}

// Fonction récursive pour trouver les niveaux trophiques
void trouver_niveaux_trophiques(ReseauTrophique* reseau, int espece_index, bool* visite, int* niveaux, int* nb_niveaux) {
    visite[espece_index] = true; // Marquer l'espèce comme visitée
    Espece* espece = &reseau->especes[espece_index]; // Obtenir un pointeur vers l'espèce actuelle
    // Ajouter le niveau trophique actuel si ce n'est pas déjà présent
    if (*nb_niveaux == 0 || niveaux[*nb_niveaux - 1] != espece->niveau_trophique) { // SI...
        niveaux[*nb_niveaux] = espece->niveau_trophique; // Ajouter le niveau trophique actuel
        (*nb_niveaux)++; // Incrémenter le nombre de niveaux
    }

    // Explorer les proies (espèces consommées)
    for (int i = 0; i < espece->nb_proies; i++) {
        int proie_index = espece->proies[i]; // Obtenir l'index de la proie
        if (!visite[proie_index]) { // Si la proie n'a pas été visitée
            trouver_niveaux_trophiques(reseau, proie_index, visite, niveaux, nb_niveaux); // Appel récursif
        }
    }
}

void afficher_niveaux_trophiques(ReseauTrophique* reseau, const char* nom_espece) {
    int index = trouver_espece(reseau, nom_espece); // Trouver l'index de l'espèce
    if (index == -1) { // Si l'espèce n'est pas trouvée
        printf("Espece non trouvee.\n"); // DIRE ...
        return; // Fin sous-programme
    }

    bool visite[MAX_ESPECES] = {false};  // Initialiser le tableau des espèces visitées
    int niveaux[MAX_ESPECES]; // Tableau pour stocker les niveaux trophiques
    int nb_niveaux = 0; // Compteur de niveaux trophiques

    // Appel à la fonction pour trouver tous les niveaux
    trouver_niveaux_trophiques(reseau, index, visite, niveaux, &nb_niveaux);

    // Utiliser un ensemble pour éviter les doublons
    int unique_levels[MAX_ESPECES] = {0}; // Tableau pour stocker les niveaux uniques
    int unique_count = 0; // Compteur de niveaux uniques

    // Éliminer les doublons dans les niveaux trophiques
    for (int i = 0; i < nb_niveaux; i++) {
        bool found = false; // Initialiser un drapeau pour indiquer si le niveau a été trouvé
        for (int j = 0; j < unique_count; j++) {  // Parcourir les niveaux uniques déjà trouvés
            if (unique_levels[j] == niveaux[i]) { // Vérifier si le niveau actuel existe déjà dans les niveaux uniques
                found = true; // Marquer comme trouvé et sortir de la boucle interne
                break; // Sortir
            }
        }
        if (!found) { // Si le niveau n'a pas été trouvé, l'ajouter aux niveaux uniques
            unique_levels[unique_count++] = niveaux[i]; // Ajouter le niveau uniqu
        }
    }

    printf("Niveaux trophiques pour %s : ", nom_espece); // DIRE ...
    for (int i = 0; i < unique_count; i++) { // Parcourir et afficher chaque niveau trophique unique
        printf("%d ", unique_levels[i]); // DIRE ...
    }
    printf("\n"); // retour à la ligne
}

void simuler_disparition_espece(ReseauTrophique* reseau, const char* nom_espece) { // Fonction pour simuler la disparition d'une espèce dans le réseau trophique
    int index = trouver_espece(reseau, nom_espece); // Trouver l'index de l'espèce dans le réseau
    if (index == -1) { // Vérifier si l'espèce existe dans le réseau
        printf("Espece non trouvee.\n"); // DIRE ...
        return; // Fin-sous-programme
    }
    // Afficher l'espèce qui va être supprimée
    printf("Suppression de l'espece : %s\n", nom_espece); // DIRE ...
    // Supprimer l'espèce des listes de proies de ses prédateurs
    for (int i = 0; i < reseau->especes[index].nb_predateurs; i++) {
        int predateur_index = reseau->especes[index].predateurs[i]; // Obtenir l'index du prédateur
        Espece* predateur = &reseau->especes[predateur_index]; // Pointer vers le prédateur

        // Retirer l'espèce disparue de la liste des proies
        for (int j = 0; j < predateur->nb_proies; j++) {
            if (predateur->proies[j] == index) { // Supprimer cette proie en décalant les éléments suivants
                // Supprimer cette proie
                for (int k = j; k < predateur->nb_proies - 1; k++) {
                    predateur->proies[k] = predateur->proies[k + 1];
                }
                predateur->nb_proies--; // Réduire le nombre de proies du prédateur
                break; // sortir
            }
        }
    }

    // Supprimer l'espèce des listes de prédateurs de ses proies
    for (int i = 0; i < reseau->especes[index].nb_proies; i++) {
        int proie_index = reseau->especes[index].proies[i]; // Obtenir l'index de la proie
        Espece* proie = &reseau->especes[proie_index]; // Pointer vers la proie

        // Retirer l'espèce disparue de la liste des prédateurs
        for (int j = 0; j < proie->nb_predateurs; j++) {
            if (proie->predateurs[j] == index) {
                // Supprimer ce prédateur
                for (int k = j; k < proie->nb_predateurs - 1; k++) {  // Supprimer ce prédateur en décalant les éléments suivants
                    proie->predateurs[k] = proie->predateurs[k + 1];
                }
                proie->nb_predateurs--;  // Réduire le nombre de prédateurs de la proie
                break; //sortir
            }
        }
    }

    // Marquer l'espèce comme inactive ou supprimer complètement
    for (int i = index; i < reseau->nb_especes - 1; i++) { // Supprimer l'espèce du réseau en décalant les espèces suivantes
        reseau->especes[i] = reseau->especes[i + 1];
    }
    reseau->nb_especes--;  // Réduire le nombre total d'espèces dans le réseau
    printf("Simulation de la disparition de %s effectuee.\n", nom_espece); // DIRE ...
    afficher_reseau(reseau); // Afficher le réseau après suppression
}

void configurer_parametres_population(ReseauTrophique* reseau) { // Fonction pour configurer les paramètres de population pour chaque espèce
    for (int i = 0; i < reseau->nb_especes; i++) { // Parcourir toutes les espèces du réseau
        Espece* e = &reseau->especes[i]; // Pointer vers l'espèce courante
        printf("Configurer %s:\n", e->nom); // DIRE ...
        // Demander et lire le taux de croissance
        printf("Entrez le taux de croissance (r) pour %s (actuel: %.2f): ", e->nom, e->taux_croissance); // DIRE ...
        scanf("%lf", &e->taux_croissance);
        // Demander et lire la capacité de charge
        printf("Entrez la capacite de charge (K) pour %s (actuel: %.2f): ", e->nom, e->capacite_charge); // DIRE ...
        scanf("%lf", &e->capacite_charge);
        // Demander et lire la population initiale
        printf("Entrez la population initiale pour %s (actuel: %.2f): ", e->nom, e->population); // DIRE ...
        scanf("%lf", &e->population);
        printf("\n"); // retour a la ligne
    }
}

void simuler_dynamique_populations(ReseauTrophique* reseau, int iterations) { // Fonction pour simuler la dynamique des populations sur plusieurs itérations
    for (int t = 0; t < iterations; t++) { // Boucle pour chaque itération
        printf("Iteration %d:\n", t + 1); // DIRE ...
        for (int i = 0; i < reseau->nb_especes; i++) { // Parcourir toutes les espèces du réseau
            Espece* e = &reseau->especes[i]; // Pointer vers l'espèce courante
            // Récupérer les paramètres de l'espèce
            double N = e->population;
            double r = e->taux_croissance;
            double K = e->capacite_charge;

            // Calculer le changement de population
            double dN = r * N * (1 - N / K);
            e->population += dN; // Mettre à jour la population

            // S'assurer que la population ne devient pas négative
            if (e->population < 0) {
                e->population = 0;
            }

            printf("%s: Population = %.2f\n", e->nom, e->population); // DIRE ...
        }
        printf("\n"); // retour à la ligne
    }
}

void generer_fichier_dot(ReseauTrophique* reseau, const char* nom_fichier_dot) { // Fonction pour générer un fichier DOT représentant le réseau trophique
    FILE* file = fopen(nom_fichier_dot, "w"); // Ouvrir le fichier en mode écriture
    if (file == NULL) {  // Vérifier si l'ouverture du fichier a réussi
        printf("Erreur lors de l'ouverture du fichier DOT.\n"); // DIRE ...
        return; // sortir sous-programme
    }

    fprintf(file, "digraph ResearchNetwork {\n"); // Écrire l'en-tête du fichier DOT
    fprintf(file, "  node [shape=box];\n"); // Définir la forme des nœuds comme des boîtes

    for (int i = 0; i < reseau->nb_especes; i++) { // Parcourir toutes les espèces du réseau
        fprintf(file, "  \"%s\" [label=\"%s\"];\n", reseau->especes[i].nom, reseau->especes[i].nom); // Écrire le nœud pour l'espèce courante
        for (int j = 0; j < reseau->especes[i].nb_proies; j++) {  // Écrire les arêtes pour chaque proie de l'espèce courante
            fprintf(file, "  \"%s\" -> \"%s\";\n", reseau->especes[i].nom, reseau->especes[reseau->especes[i].proies[j]].nom); // DIRE ...
        }
    }

    fprintf(file, "}\n"); // Fermer l'accolade du graphe
    fclose(file); // Fermer le fichier
}
void visualiser_reseau(ReseauTrophique* reseau) { // Fonction pour visualiser le réseau trophique
    const char* nom_fichier_dot = "reseau_trophique.dot"; // Définir le nom du fichier DOT
    generer_fichier_dot(reseau, nom_fichier_dot); // Générer le fichier DOT
    printf("Fichier DOT généré : %s\n", nom_fichier_dot); // Afficher un message indiquant que le fichier a été généré


#ifdef _WIN32 // Ouvrir directement le fichier DOT
    char commande[150]; // Pour Windows
    snprintf(commande, sizeof(commande), "start %s", nom_fichier_dot);
    system(commande);
#else
    char commande[150];
        snprintf(commande, sizeof(commande), "xdot %s", nom_fichier_dot);
        system(commande);
#endif
    printf("Le fichier DOT a été ouvert. Appuyez sur Entrée pour continuer...\n"); // Afficher un message et attendre que l'utilisateur appuie sur Entrée
    getchar(); // Attendre que l'utilisateur appuie sur Entrée
}
