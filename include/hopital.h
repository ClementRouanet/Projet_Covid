#ifndef HOPITAL_H
#define HOPITAL_H

#include "structures.h"


Bool est_hopital(Arbre_t* arbre, Personne_t* pers); // Permet de savoir si un patient est à l'hopital

Arbre_t* nouveau_patient(Arbre_t* attente, Personne_t* pers); // Ajoute un patient en attente ou en quarantaine
Arbre_t* initialisation_hopital(Arbre_t* attente, Population_t* pop, Covid_t* covid, int t);  // Initialisation des patients à l'hopital au temps t = 0

void retirer_hopital(Covid_t* covid, Population_t* pop, int t); // Retire les patients de l'hopital
Arbre_t* retirer_incubation(Arbre_t* attente, Population_t* pop, Covid_t* covid, int t);  // Retire de l'incubation et l'ajoute en attente ou quarantaine
Arbre_t* retirer_attente(Arbre_t* attente, Population_t* pop, Covid_t* covid, int t); // Retire de l'attente et l'ajoute à l'hopital


#endif
