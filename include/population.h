#ifndef POPULATION_H
#define POPULATION_H

#include "structures.h"


void initialisation_population(Population_t* population, int nombre, double proba); // Initialisation de la population
Personne_t initialisation_personne(int i, double proba);  // Initialisation d'une personne de la population

int choix_sexe(); // Initialisation du sexe de la personne
int choix_age();  // Initialisation de l'âge de la personne
double choix_sante(); // Initialisation de l'état de santé de la personne

int min(int p, int q);  // Renvoie le minimum entre 2 entiers
int max(int p, int q);  // Renvoie le maximum entre 2 entiers

Graphe_t* familles(Population_t* population); // Création du graphe qui représentera les liens familiaux
Graphe_t* amis(Population_t* population); // Création du graphe qui représentera les liens amicaux

void afficher_population(Population_t* population); // Permet d'afficher la population

void supprimer_population(Population_t* population);  // Permet de supprimer la population


#endif
