#ifndef COVID_H
#define COVID_H

#include "population.h"
#include "hopital.h"
#include "arbre.h"
#include "graphe.h"


void initialisation_covid(Covid_t* covid, Population_t* pop); // Initialisation de la structure Covid_t
void supprimer_covid(Covid_t* covid); // Suppression de la structure Covid_t

void stockage_nb_malades(Population_t* pop, int* tab, int t); // Stockage du nombre de nouveaux malades à chaque temps dans un tableau
void contamination(Graphe_t* famille, Graphe_t* amis, Covid_t* covid, Population_t* population, int t); // Simule les contaminations entre les individus à chaque temps (jours)
void temps_malade(Population_t* population);  // On incrémente le temps de chaque personne infectée ou malade
void vaccination(Population_t* population, Covid_t* covid, int t);  // Simule la vaccination de la population à chaque temps

void ecriture_evolution(Population_t* pop, int tab[NB_POPULATION][DUREE], char* fichier); // Ecrit dans un fichier les différents états (sain, infecté, malade, immunisé ou mort) pour chaque individu à tous les temps
void ecriture_chiffres(Covid_t* covid, char* fichier);  // Ecrit dans un fichier les chiffres (nbMalades, nbMorts, nbVaccines, nbLitsOccupes) à chaque temps pour tracer les courbes avec gnuplot
void affichage_chiffres(Covid_t* covid, Population_t* pop); // Affiche sur le terminal les chiffres (nbMalades, nbMorts, nbVaccines, nbLitsOccupes) à chaque temps

void tracage_gnuplot(char* fichier);  // Permet de tracer les différenes courbes (nbMalades, nbMorts, nbVaccines, nbLitsOccupes) au cours du temps avec gnuplot
void simulation();  // Fonction principale : permet de simuler l'évolution du covid dans le temps


#endif
