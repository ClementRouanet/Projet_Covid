#ifndef ARBRE_H_
#define ARBRE_H_

#include "structures.h"


int comparer (Personne_t* p1, Personne_t* p2);  // Compare l'état de santé de deux personnes
void affiche_personne(Personne_t* pers);  // Affiche le numéro d'une personne
void tableau_personne(Personne_t* pers, int* tab);  // Met le numéro d'une personne apres la dernière personne
void affichage_parcours_infixe(Arbre_t* arbre); // Affiche sur le terminal l'arbre grâce à un parcours infixe
void parcours_infixe(Arbre_t* arbre, int* tab); // Met le numéro de chaque personne de l'arbre dans un tableau grâce à un parcours infixe

int cardinal(Arbre_t* arbre); // Renvoie le nombre de noeuds que contient l'arbre
int maxx(int p, int q); // Renvoie le maximum entre deux entiers
int hauteur(Arbre_t* arbre);  // Renvoie la hauteur de l'arbre

Arbre_t* maximum(Arbre_t* arbre); // Renvoie le noeud maximal de l'arbre
Arbre_t* minimum(Arbre_t* arbre); // Renvoie le noeud minimal de l'arbre

Arbre_t* predecesseur(Arbre_t* arbre);  // Renvoie l'arbre prédecesseur d'un arbre
Arbre_t* rechercher(Arbre_t* arbre, Personne_t* pers);  // Recherche une personne dans l'arbre

Arbre_t* nouveau_noeud(Personne_t* pers); // Ajoute un nouveau noeud à l'arbre binaire de recherche
void inserer(Arbre_t** arbre, Personne_t* pers);  // Insère une personne à l'arbre
void supprimer(Arbre_t** arbre1, Arbre_t* arbre2);  // Suppression d'un noeud (une personne) de l'arbre

void supprimer_arbre(Arbre_t** arbre);  // Supprime l'arbre

void afficher_arbre_aux(Arbre_t* arbre);  // Fonction de afficher_arbre
void afficher_arbre(Arbre_t* arbre);  // Permet d'afficher un arbre sur le terminal


#endif
