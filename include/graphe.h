#ifndef GRAPHE_H_
#define GRAPHE_H_

#include "structures.h"


Graphe_t* creation_graphe(Population_t* pop); // Création d'un graphe
Bool graphe_est_vide(Graphe_t* graphe); // Teste si le graphe est vide

ListePersonne_t* ajouter_noeud(Population_t* pop, int p); // Ajoute un noeud au graphe
void ajouter_liaison(Graphe_t* graphe, Population_t* pop, int p1, int p2);  // Ajoute une liaison non orientée entre deux noeuds du graphe
void taille_voisins(ListePersonne_t* personne, int* compteur);  // Renvoie le nombre de voisins (famille ou amis) d'une personne

void afficher_graphe(Graphe_t* graphe, Population_t* pop);  // Permet d'afficher un graphe

void supprimer_graphe(Graphe_t* graphe);  // Permet de supprimer un graphe


#endif
