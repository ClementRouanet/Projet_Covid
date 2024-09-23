#include <stdio.h>
#include <stdlib.h>
#include "arbre.h"


/* Compare l'état de santé de deux personnes */
int comparer (Personne_t* p1, Personne_t* p2)
{
	if (p1->sante < p2->sante)
		return -1;

	if (p1->sante > p2->sante)
		return 1;

	return 0;	// Si égales
}


/* Affiche le numéro d'une personne */
void affiche_personne(Personne_t* pers)
{
	printf(" %d ", pers->numero);
}


/* Met le numéro d'une personne apres la dernière personne */
void tableau_personne(Personne_t* pers, int* tab)
{
	int i=0;

	while (tab[i] != -1)
		i++;

	tab[i] = pers->numero;
}


/* Affiche sur le terminal l'arbre grâce à un parcours infixe */
void affichage_parcours_infixe(Arbre_t* arbre)
{
	// Si l'arbre existe
	if (arbre)
	{
		affichage_parcours_infixe(arbre->gauche);
		affiche_personne(arbre->personne);
		affichage_parcours_infixe(arbre->droit);
	}
}


/* Met le numéro de chaque personne de l'arbre dans un tableau grâce à un parcours infixe */
void parcours_infixe(Arbre_t* arbre, int* tab)
{
	// Si l'arbre existe
	if (arbre)
	{
		parcours_infixe(arbre->gauche, tab);
		tableau_personne(arbre->personne, tab);
		parcours_infixe(arbre->droit, tab);
	}
}


/* Renvoie le nombre de noeuds que contient l'arbre */
int cardinal(Arbre_t* arbre)
{
	if (arbre)
		return cardinal(arbre->gauche) + cardinal(arbre->droit) + 1;

	else
		return 0;
}


/* Renvoie le maximum entre deux entiers */
int maxx(int p, int q)
{
		//return (p > q) ? p : q;

		if (p > q)
			return p;

		else
			return q;
}


/* Renvoie la hauteur de l'arbre */
int hauteur(Arbre_t* arbre)
{
	if (arbre)
		return 1 + maxx(hauteur(arbre->gauche), hauteur(arbre->droit));

	else
		return 0;
}


/* Renvoie le noeud maximal de l'arbre : la personne avec le plus grand état de santé */
Arbre_t* maximum(Arbre_t* arbre)
{
	if (!arbre)
		return NULL;

	if (!(arbre->droit))
		return arbre;

	return maximum(arbre->droit);
}


/* Renvoie le noeud minimal de l'arbre : la personne avec le plus petit état de santé */
Arbre_t* minimum(Arbre_t* arbre)
{
	if (!arbre)
		return NULL;

	if (!(arbre->gauche))
		return arbre;

	return minimum(arbre->gauche);
}


/* Renvoie l'arbre prédecesseur d'un arbre */
Arbre_t* predecesseur(Arbre_t* arbre)
{
	if (arbre->gauche)
		return maximum(arbre->gauche);

	while (arbre->parent && (arbre == (arbre->parent)->gauche))
		arbre = arbre->parent;

	return arbre->parent;
}


/* Recherche une personne dans l'arbre */
Arbre_t* rechercher(Arbre_t* arbre, Personne_t* pers)
{
	// Si l'arbre existe
	if (arbre)
	{
		switch (comparer(pers, arbre->personne))
		{
			case -1 :
				return rechercher(arbre->gauche, pers);

			case 0 :
				return arbre;

			case 1 :
				return rechercher(arbre->droit, pers);
		}
	}

	return NULL;	// Personnne introuvable
}


/* Ajoute un nouveau noeud à l'arbre binaire de recherche */
Arbre_t* nouveau_noeud(Personne_t* pers)
{
	Arbre_t* arbre;

	arbre = malloc(sizeof(*arbre));
	if (arbre == NULL)
	{
		printf("Problème allocation mémoire dans nouveau_noeud\n");
		exit(EXIT_FAILURE);
	}

	arbre->personne = pers;
	arbre->gauche = NULL;
	arbre->droit = NULL;
	arbre->parent = NULL;

	return arbre;
}


/* Insère une personne à l'arbre */
void inserer(Arbre_t** arbre, Personne_t* pers)
{
	// Si l'arbre n'existe pas
	if (!(*arbre))
		*arbre = nouveau_noeud(pers);

	// Si l'arbre existe
	else
	{
		Arbre_t* arb;
		Arbre_t* par;
		int cote;
		arb = *arbre;

		// On regarde où placer le nouveau noeud pour respecter les propriétés d'un arbre binaire de recherche
		while (arb)
		{
			par = arb;

			cote = comparer(pers, arb->personne);

			switch (cote)
			{
				case -1 :	// Cote gauche
					arb = arb->gauche;
					break;

				case 0 :	// On a deja l'élément 0
					arb = arb->droit;
					break;

				case 1 :	// Cote droit
					arb = arb->droit;
					break;
			}
		}

		arb = nouveau_noeud(pers);
		arb->parent = par;

		if (cote == -1)
			par->gauche = arb;

		else
			par->droit = arb;
	}
}


/* Suppression d'un noeud (une personne) de l'arbre */
void supprimer(Arbre_t** arbre1, Arbre_t* arbre2)
{
	if (arbre2->gauche && arbre2->droit)
	{
		Arbre_t* arb;	//Predecesseur de arbre2
		arb = predecesseur(arbre2);
		arbre2->personne = arb->personne;
		supprimer(arbre1, arb);
	}
	else
	{
		Arbre_t* arb;	// abr unique fils de arbre2 ou NULL s'il n'en a pas

		if (arbre2->gauche)
			arb = arbre2->gauche;

		else
			arb = arbre2->droit;

		// Le parent de  arbre2 devient le nouveau parent de arb
		if (arb)
			arb->parent = arbre2->parent;

		// Si arbre2 avait un parent alors arb remplace arbre2 auprès de ce parent
		if (arbre2->parent)
		{
			if (arbre2 == arbre2->parent->gauche)
				arbre2->parent->gauche = arb;

			else
				arbre2->parent->droit = arb;

			if (arb)
				arb->parent = arbre2->parent;
		}
		else	// Sinon arbre2 était une racine, il faut mettre l'arbre à jour
			*arbre1 = arb;

		free(arbre2);
	}
}


/* Supprime l'arbre : désaloccation dynamique */
void supprimer_arbre(Arbre_t** arbre)
{
	Arbre_t* arb;
	arb = *arbre;

	if (arb)
	{
		supprimer_arbre(&arb->gauche);
		supprimer_arbre(&arb->droit);
		free(arb);
		*arbre = NULL;
	}
}


/* Fonction de afficher_arbre */
void afficher_arbre_aux(Arbre_t* arbre)
{
	// Si l'arbre a un parent
	if (arbre->parent)
	{
		afficher_arbre_aux(arbre->parent);

		if (arbre == arbre->parent->droit)
		{
			if (arbre->parent->gauche)
				printf(" |      ");

			else
				printf("        ");
		}
	}
}


/* Permet d'afficher un arbre sur le terminal */
void afficher_arbre(Arbre_t* arbre)
{
	// Si l'arbre existe
	if (arbre)
	{
		affiche_personne(arbre->personne);

		if (arbre->droit)
		{
			printf("-----");
			afficher_arbre(arbre->droit);
		}
		else
			printf("\n");

		if (arbre->gauche)
		{
			afficher_arbre_aux(arbre->gauche);
			printf(" |      \n");
			afficher_arbre_aux(arbre->gauche);
			afficher_arbre(arbre->gauche);
		}
	}

	// Si l'arbre n'existe pas
	else
		printf("\n			(arbre vide)\n\n");
}
