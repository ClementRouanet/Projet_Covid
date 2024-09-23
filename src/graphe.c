#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"


/* Création d'un graphe */
Graphe_t* creation_graphe(Population_t* population)
{
  int i;
  Graphe_t* graphe;

  graphe = malloc(sizeof(*graphe));
  if (graphe == NULL)
  {
    printf("Problème allocation mémoire dans creation_graphe\n");
    exit(EXIT_FAILURE);
  }

  graphe->nbSommets = population->nombre;

  graphe->voisins = malloc(sizeof(ListeVoisin_t)*population->nombre);
  if (graphe->voisins == NULL)
  {
    printf("Problème allocation mémoire dans creation_graphe\n");
    exit(EXIT_FAILURE);
  }

  for (i=0; i<population->nombre; i++)
    graphe->voisins[i].premiere = NULL;

  return graphe;
}


/* Teste si le graphe est vide (existe) */
Bool graphe_est_vide(Graphe_t* graphe)
{
  // Si le graphe existe
  if(graphe == NULL)
    return true;

  return false; // Si le graphe n'existe pas
}


/* Ajoute un noeud au graphe */
ListePersonne_t* ajouter_noeud(Population_t* pop, int p)
{
  // Allocation dynamique
  ListePersonne_t* l = malloc(sizeof(ListePersonne_t));
  if (l == NULL)
  {
    printf("Problème allocation mémoire dans ajouter_noeud\n");
    exit(EXIT_FAILURE);
  }

  l->personne = &pop->liste[p];
  l->suivante = NULL;

  return l;
}


/* Ajoute une liaison non orientée entre deux noeuds du graphe */
void ajouter_liaison(Graphe_t* graphe, Population_t* pop, int p1, int p2)
{
  ListePersonne_t* l = ajouter_noeud(pop, p2);
  l->suivante = graphe->voisins[p1].premiere;
  graphe->voisins[p1].premiere = l;

  l = ajouter_noeud(pop, p1);
  l->suivante = graphe->voisins[p2].premiere;
  graphe->voisins[p2].premiere = l;
}


/* Renvoie le nombre de voisins (famille ou amis) d'une personne */
void taille_voisins(ListePersonne_t* pers, int* compteur)
{
  // Remet les valeurs à zéro avant les sommes
  compteur[0] = 0;
  compteur[1] = 0;
  compteur[2] = 0;
  compteur[3] = 0;
  compteur[4] = 0;

  int age, sexe;

  // Tant que la personne a un voisin
  while(pers != NULL)
  {
    age = pers->personne->age;
    sexe = pers->personne->sexe;

    if (age <= 25)
      compteur[0] += 1;

    else if (age>25 && age < 60)
    {
      if (sexe == 0)
        compteur[1] += 1;

      else
        compteur[2] += 1;
    }

    else
    {
      if (sexe == 0)
        compteur[3] += 1;

      else
        compteur[4] += 1;
    }

    pers = pers->suivante;
  }
}


/* Permet d'afficher un graphe */
void afficher_graphe(Graphe_t* graphe, Population_t* pop)
{
  int i;
  char s;

  // Tant qu'on a pas parcouru tous les sommets
  for (i=0; i<graphe->nbSommets; i++)
  {
    ListePersonne_t* p = graphe->voisins[i].premiere;

    if (pop->liste[i].sexe == 0)
      s = 'F';

    else
      s = 'H';

    printf("(%d) : \n", pop->liste[i].numero);
    printf("\t sexe : %c\n", s);
    printf("\t age : %d\n", pop->liste[i].age);
    printf("\t sante : %.4f\n", pop->liste[i].sante);
    printf("\t liaisons : ");

    // Tant que la personne a un voisin
    while(p != NULL)
    {
      printf("%d ", p->personne->numero);
      p = p->suivante;
    }

    printf("NULL\n");
  }
}


/* Permet de supprimer un graphe */
void supprimer_graphe(Graphe_t* graphe)
{
  // Si le graphe est vide
  if(graphe_est_vide(graphe))
	{
		printf("Rien a effacer, le Graphe n'existe pas.\n");
		return;
	}

  //Si il existe des sommets adjacents
	if(graphe->voisins)
	{
		int i;

		for(i=0; i<graphe->nbSommets; i++)
		{
			ListePersonne_t* p = graphe->voisins[i].premiere;

			while(p != NULL)
			{
				ListePersonne_t* tmp = p;
				p = p->suivante;
				free(tmp);
			}
		}

		//Libération de la liste d'adjacence
		free(graphe->voisins);
  }

  //Libération du graphe de la mémoire
  free(graphe);
}
