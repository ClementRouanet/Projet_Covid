#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <math.h>

#define NB_POPULATION 20000 // Taille population
#define DUREE 20  // Duree de la simulation (en jours)
#define PROBA 0.01  // Pourcentage de population malade à  t = 0
#define NB_LITS ceil(NB_POPULATION/5) // Nombre de lits d'hôpital total
#define VACCINATION ceil(NB_POPULATION/100) // Nombre maximal de personnes vaccinées chaque jour

#define TAUX_TRANS_FAMILLE 0.7  // Taux de tramsmission du virus dans la famille
#define TAUX_TRANS_AMIS 0.6  // Taux de tramsmission du virus entre les amis

#define TPS_INCUBATION 4  // Temps d'incubation de la maladie
#define TPS_MALADE 7  // Temps pendant lequel on est malade


/* Définition d'un Booléen */
typedef enum
{
  false,
  true
}Bool;


/* Définition d'une personne */
typedef struct {
  int numero;
  int temps;
  int sexe; // Homme(1) & femme(0)
  int age;  // Entre 1 et 100
  double sante; // Entre 0 et 1
  int malade; // Mort(-1) & sain(0) & contaminé(1) & malade(2) & immunisé(3)
}Personne_t;


/* Définition de la population */
typedef struct{
    int nombre;
    Personne_t* liste;
}Population_t;


/* Définition de l'évolution de l'épidémie chaque jour */
typedef struct{
  int temps;
  int* nbMalades;
  int* nbMorts;
  int* nbVaccines;
  int* nbLitsOccupes;
}Covid_t;


/* Définition d'une liste de personnes */
struct ListePersonne{
  Personne_t* personne;
  struct ListePersonne* suivante;
};
typedef struct ListePersonne ListePersonne_t;


/* Définition d'une liste d'adjacence (tableau) */
typedef struct {
  ListePersonne_t* premiere;
}ListeVoisin_t;


/* Définition d'un graphe */
typedef struct{
    int nbSommets;
    ListeVoisin_t* voisins;
}Graphe_t;


/* Définition d'un Arbre */
struct Arbre{
	Personne_t* personne;
	struct Arbre* gauche;
	struct Arbre* droit;
	struct Arbre* parent;
};
typedef struct Arbre Arbre_t;


#endif
