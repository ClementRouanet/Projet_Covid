#include <stdio.h>
#include <stdlib.h>

#include "hopital.h"
#include "arbre.h"


/* Permet de savoir si un patient est à l'hopital */
Bool est_hopital(Arbre_t* arbre, Personne_t* pers)
{
  int taille = cardinal(arbre); // Taille de l'arbre

  int tab[taille];
  int i;

  // Initialisation du tableau
  for (i=0; i<taille; i++)
    tab[i] = -1;

  // Met dans tab les numéros des personnes qui se trouvent dans l'arbre
  parcours_infixe(arbre, tab);

  for (i=0; i<taille; i++)
  {
    if (pers->numero == tab[i])
      return true;
  }

  return false;
}


/* Ajoute un patient en attente ou en quarantaine */
Arbre_t* nouveau_patient(Arbre_t* attente, Personne_t* pers)
{
  pers->malade = 2; // Personne malade

  // la personne va en attente de l'hopital si elle a un état de sante < 1 ou un âge > 60
  if (pers->sante<1 || pers->age>60)
    inserer(&attente, pers);

  // Sinon la personne va en quarantaine
  else
    pers->temps = 1;

  return attente;
}


/* Initialisation des patients à l'hopital au temps t = 0 */
Arbre_t* initialisation_hopital(Arbre_t* attente, Population_t* pop, Covid_t* covid, int t)
{
  int i;
  Personne_t* pers;

  for (i=0; i<pop->nombre; i++)
  {
    pers = &pop->liste[i];

    if (pers->malade == 2)
      attente = nouveau_patient(attente, pers); // Ajoute un patient en attente ou en quarantaine
  }

  return attente;
}


/* Retire les patients de l'hopital */
void retirer_hopital(Covid_t* covid, Population_t* pop, int t)
{
  covid->nbMorts[t] = 0;

  for (int i=0; i<pop->nombre; i++)
  {
    Personne_t* pers = &pop->liste[i];

    // Si la personne à fini son temps de maladie, égal à TPS_MALADE
    if (pers->malade==2 && pers->temps==TPS_MALADE)
    {
      // Si la personne a des commorbidités ou agée alors elle risque de mourir
      if (pers->sante<1 && pers->age>60)
      {
        double RND = (double)(rand() / (double)RAND_MAX);

        if (pers->sante*(1.75-pers->age/100) < RND)
        {
          covid->nbMorts[t] += 1;
          pers->malade = -1;
        }
        else
          pers->malade = 3;
      }
      else
        pers->malade = 3;

      pers->temps = 0;
    }
  }
}


/* Retire de l'incubation et l'ajoute en attente ou quarantaine */
Arbre_t* retirer_incubation(Arbre_t* attente, Population_t* pop, Covid_t* covid, int t)
{
  for (int i=0; i<pop->nombre; i++)
  {
    Personne_t* pers = &pop->liste[i];

    // Si la personne à fini son temps d'incubation, égal à TPS_INCUBATION
    if (pers->malade==1 && pers->temps==TPS_INCUBATION)
    {
      pers->temps = 0;
      attente = nouveau_patient(attente, pers); // Ajoute un patient en attente ou en quarantaine
    }
  }

  return attente;
}


/* Retire de l'attente et l'ajoute à l'hopital */
Arbre_t* retirer_attente(Arbre_t* attente, Population_t* pop, Covid_t* covid, int t)
{
  Arbre_t* arbMin;

  int nbLitsOccupes = 0;

  // Calcule le nombre de lits déjà occupés
  for (int i=0; i<pop->nombre; i++)
  {
    Personne_t* pers = &pop->liste[i];

    if (pers->malade==2 && pers->sante<1 && pers->age>60 && pers->temps>0)
      nbLitsOccupes += 1;
    }

  covid->nbLitsOccupes[t] = 0;

  // Ajoute les patients en attente tant qu'il y a des lits de libres
  while (attente && nbLitsOccupes<NB_LITS)
  {
    arbMin = minimum(attente);
    arbMin->personne->temps = 1;
    supprimer(&attente, rechercher(attente, arbMin->personne));

    covid->nbLitsOccupes[t] += 1;
    nbLitsOccupes += 1;
  }
  return attente;
}
