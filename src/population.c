#include <stdio.h>
#include <stdlib.h>

#include "population.h"
#include "graphe.h"


/* Initialisation de la population */
void initialisation_population(Population_t* population, int nombre, double proba)
{
  int i;

  population->nombre = nombre;

  // Allocation dynamique
  population->liste = malloc(sizeof(Personne_t)*nombre);
  if (population->liste == NULL)
  {
    printf("Problème allocation mémoire dans initialisation_population\n");
    exit(EXIT_FAILURE);
  }

  // Création de chaque personne de la population
  for (i=0; i<nombre; i++)
    population->liste[i] = initialisation_personne(i, proba);
}


/* Initialisation d'une personne de la population */
Personne_t initialisation_personne(int i, double proba)
{
  Personne_t personne;
  double RND = (double)(rand() / (double)RAND_MAX); // Nombre réel aléatoire entre 0 et 1

  personne.numero = i;
  personne.sexe = choix_sexe();
  personne.age = choix_age();
  personne.sante = choix_sante();

  if (RND <= proba)
  {
    personne.malade = 2;
    personne.temps = 1;
  }
  else
  {
    personne.malade = 0;
    personne.temps = 0;
  }

  return personne;
}


/* Initialisation du sexe de la personne */
int choix_sexe()
{
  double RND = (double)(rand() / (double)RAND_MAX); // Nombre réel aléatoire entre 0 et 1
  int sexe;

  // 50% de chance d'avoir une fille ou un garçon
  if (RND <= 0.5)
    sexe = 1;

  else
    sexe = 0;

  return sexe;
}


/* Initialisation de l'âge de personne */
int choix_age()
{
  double RND = (double)(rand() / (double)RAND_MAX); // Nombre réel aléatoire entre 0 et 1
  int age;

  if (RND <= 0.24)
    age = 1 + (int)(rand() / (double)RAND_MAX * 21); // Nombre réel aléatoire entre 1 et 20

  else if (RND>0.24 && RND<=0.47)
    age = 20 + (int)(rand() / (double)RAND_MAX * 21); // Nombre réel aléatoire entre 21 et 40

  else if (RND>0.47 && RND<=0.73)
    age = 40 + (int)(rand() / (double)RAND_MAX * 21); // Nombre réel aléatoire entre 41 et 60

  else if (RND>0.73 && RND<=0.9)
    age = 60 + (int)(rand() / (double)RAND_MAX * 21); // Nombre réel aléatoire entre 61 et 80

  else
    age = 80 + (int)(rand() / (double)RAND_MAX * 21); // Nombre réel aléatoire entre 81 et 100

  return age;
}


/* Initialisation de l'état de santé de la personne */
double choix_sante()
{
  double RND = (double)(rand() / (double)RAND_MAX); // Nombre réel aléatoire entre 0 et 1
  double sante;

  if (RND <= 0.25)
    sante = (double)(rand() / (double)RAND_MAX); // Nombre réel aléatoire entre 0 et 1

  else
    sante = 1;

  return sante;
}


/* Renvoie le minimum entre 2 entiers */
int min(int p, int q)
{
		if (p > q)
			return q;

		else
			return p;
}


/* Renvoie le maximum entre 2 entiers */
int max(int p, int q)
{
		if (p > q)
			return p;

		else
			return q;
}


/* Création du graphe qui représentera les liens familiaux */
Graphe_t* familles(Population_t* population)
{
  Graphe_t* graphe = NULL;
  graphe = creation_graphe(population);

  int tailleAdoF=0, tailleAdoG=0, tailleMere=0, taillePere=0, tailleMamie=0, taillePapi=0;

  // On regarde combien il y a d'ados, adultes et personnes agées en fonction du sexe
  for (int i=0; i<population->nombre; i++)
  {
    Personne_t* pers = &population->liste[i];

    if (pers->age <= 25 && pers->sexe==0)
      tailleAdoF += 1;

    if (pers->age <= 25 && pers->sexe==1)
      tailleAdoG += 1;

    if (pers->age>25 && pers->age<=60 && pers->sexe==0)
      tailleMere += 1;

    if (pers->age>25 && pers->age<=60 && pers->sexe==1)
      taillePere += 1;

    if (pers->age>60 && pers->sexe==0)
      tailleMamie += 1;

    if (pers->age>60 && pers->sexe==1)
      taillePapi += 1;
  }

  // On met toutes les ados dans un tableau
  int adoF[tailleAdoF];
  int j = 0;
  for (int i=0; i<population->nombre; i++)
  {
    Personne_t* pers = &population->liste[i];

    if (pers->age <= 25 && pers->sexe==0)
    {
      adoF[j] = pers->numero;
      j += 1;
    }
  }

  // On met tous les ados dans un tableau
  int adoG[tailleAdoG];
  j = 0;
  for (int i=0; i<population->nombre; i++)
  {
    Personne_t* pers = &population->liste[i];

    if (pers->age <= 25 && pers->sexe==1)
    {
      adoG[j] = pers->numero;
      j += 1;
    }
  }

  // On met toutes les mères dans un tableau
  int mere[tailleMere];
  j = 0;
  for (int i=0; i<population->nombre; i++)
  {
    Personne_t* pers = &population->liste[i];

    if (pers->age>25 && pers->age<=60 && pers->sexe==0)
    {
      mere[j] = pers->numero;
      j += 1;
    }
  }

  // On met tous les pères dans un tableau
  int pere[taillePere];
  j = 0;
  for (int i=0; i<population->nombre; i++)
  {
    Personne_t* pers = &population->liste[i];

    if (pers->age>25 && pers->age<=60 && pers->sexe==1)
    {
      pere[j] = pers->numero;
      j += 1;
    }
  }

  // On met toutes les mamies dans un tableau
  int mamie[tailleMamie];
  j = 0;
  for (int i=0; i<population->nombre; i++)
  {
    Personne_t* pers = &population->liste[i];

    if (pers->age>60 && pers->sexe==0)
    {
      mamie[j] = pers->numero;
      j += 1;
    }
  }

  // On met tous les papis dans un tableau
  int papi[taillePapi];
  j = 0;
  for (int i=0; i<population->nombre; i++)
  {
    Personne_t* pers = &population->liste[i];

    if (pers->age>60 && pers->sexe==1)
    {
      papi[j] = pers->numero;
      j += 1;
    }
  }

  int min1 = min(tailleAdoF, min(tailleAdoG, min(tailleMere, taillePere)));
  int max1 = max(tailleAdoF, max(tailleAdoG, max(tailleMere, taillePere)));

  // On crée les liens familiaux entre fille - garçon - mère - père
  for (int i=0; i<max1; i++)
  {
    if (i < min1)
    {
      ajouter_liaison(graphe, population, adoF[i], adoG[i]);
      ajouter_liaison(graphe, population, adoF[i], mere[i]);
      ajouter_liaison(graphe, population, adoF[i], pere[i]);

      ajouter_liaison(graphe, population, adoG[i], mere[i]);
      ajouter_liaison(graphe, population, adoG[i], pere[i]);

      ajouter_liaison(graphe, population, mere[i], pere[i]);
    }

    if (min1==tailleAdoF && i>=min1)
    {
      int min2 = min(tailleAdoG, min(tailleMere, taillePere));

      if (i < min2)
      {
        ajouter_liaison(graphe, population, adoG[i], mere[i]);
        ajouter_liaison(graphe, population, adoG[i], pere[i]);
        ajouter_liaison(graphe, population, mere[i], pere[i]);
      }

      int min3 = min(tailleMere, taillePere);
      if (min2==tailleAdoG && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, mere[i], pere[i]);

      min3 = min(tailleAdoG, taillePere);
      if (min2==tailleMere && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, adoG[i], pere[i]);

      min3 = min(tailleMere, tailleAdoG);
      if (min2==taillePere && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, mere[i], adoG[i]);
    }

    if (min1==tailleAdoG && i>=min1)
    {
      int min2 = min(tailleAdoF, min(tailleMere, taillePere));

      if (i < min2)
      {
        ajouter_liaison(graphe, population, adoF[i], mere[i]);
        ajouter_liaison(graphe, population, adoF[i], pere[i]);
        ajouter_liaison(graphe, population, mere[i], pere[i]);
      }

      int min3 = min(tailleMere, taillePere);
      if (min2==tailleAdoF && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, mere[i], pere[i]);

      min3 = min(tailleAdoF, taillePere);
      if (min2==tailleMere && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, adoF[i], pere[i]);

      min3 = min(tailleMere, tailleAdoF);
      if (min2==taillePere && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, mere[i], adoF[i]);
    }

    if (min1==tailleMere && i>=min1)
    {
      int min2 = min(tailleAdoF, min(tailleAdoG, taillePere));

      if (i < min2)
      {
        ajouter_liaison(graphe, population, adoF[i], adoG[i]);
        ajouter_liaison(graphe, population, adoF[i], pere[i]);
        ajouter_liaison(graphe, population, adoG[i], pere[i]);
      }

      int min3 = min(tailleAdoG, taillePere);
      if (min2==tailleAdoF && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, adoG[i], pere[i]);

      min3 = min(tailleAdoF, taillePere);
      if (min2==tailleAdoG && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, adoF[i], pere[i]);

      min3 = min(tailleAdoF, tailleAdoG);
      if (min2==taillePere && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, adoF[i], adoG[i]);
    }

    if (min1==taillePere && i>=min1)
    {
      int min2 = min(tailleAdoF, min(tailleAdoG, tailleMere));

      if (i < min2)
      {
        ajouter_liaison(graphe, population, adoF[i], adoG[i]);
        ajouter_liaison(graphe, population, adoF[i], mere[i]);
        ajouter_liaison(graphe, population, adoG[i], mere[i]);
      }

      int min3 = min(tailleAdoG, tailleMere);
      if (min2==tailleAdoF && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, adoG[i], mere[i]);

      min3 = min(tailleAdoF, tailleMere);
      if (min2==tailleAdoG && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, adoF[i], mere[i]);

      min3 = min(tailleAdoF, tailleAdoG);
      if (min2==tailleMere && i>=min2 && i<min3)
        ajouter_liaison(graphe, population, adoF[i], adoG[i]);
    }
  }

  min1 = min(tailleMamie, taillePapi);

  // On crée les liens familiaux entre mamie & papi
  for (int i=0; i<min1; i++)
    ajouter_liaison(graphe, population, mamie[i], papi[i]);

  return graphe;
}


/* Création du graphe qui représentera les liens amicaux */
Graphe_t* amis(Population_t* population)
{
  int i, j, nbMax=7;
  int compteur[5] = {0,0,0,0,0}; // ado, mere, pere, mamie, papi
  int compt[5] = {0,0,0,0,0};

  Graphe_t* graphe;
  graphe = creation_graphe(population);

  for (i=0; i<population->nombre; i++)
  {
    Personne_t pers = population->liste[i];
    ListePersonne_t* p = graphe->voisins[i].premiere;

    taille_voisins(p, compteur);  // Regarde nombre de voisins en fonction de leurs ages
    int age = pers.age;

    // Si la personne est un ado
    if (age <= 25)
    {
      for (j=i+1; j<population->nombre; j++)
      {
        Personne_t perso = population->liste[j];
        ListePersonne_t* pp = graphe->voisins[j].premiere;

        taille_voisins(pp,compt);

        if (perso.age<=25 && compteur[0]<nbMax && compt[0]<nbMax)
        {
          ajouter_liaison(graphe, population, i, j);
          compteur[0] += 1;
        }
      }
    }

    // Si la personne est un adulte
    if (age>25 && age<=60)
    {
      for (j=i+1; j<population->nombre; j++)
      {
        Personne_t perso = population->liste[j];
        ListePersonne_t* pp = graphe->voisins[j].premiere;

        taille_voisins(pp,compt);

        if (perso.age>25 && perso.age<60)
        {
          if ((compteur[1]+compteur[2])<nbMax && (compt[1]+compt[2])<nbMax)
          {
            ajouter_liaison(graphe, population, i, j);
            compteur[2] += 1;
          }
        }
      }
    }

    // Si la peronne est une personne agée
    if (age > 60)
    {
      for (j=i+1; j<population->nombre; j++)
      {
        Personne_t perso = population->liste[j];
        ListePersonne_t* pp = graphe->voisins[j].premiere;

        taille_voisins(pp,compt);

        if (perso.age > 60)
        {
          if ((compteur[3]+compteur[4])<nbMax && (compt[3]+compt[4])<nbMax)
          {
            ajouter_liaison(graphe, population, i, j);
            compteur[4] += 1;
          }
        }
      }
    }
  }

  return graphe;
}


/* Permet d'afficher la population */
void afficher_population(Population_t* population)
{
  char s;

  for(int i=0;i<population->nombre; i++)
  {
    if (population->liste[i].sexe == 1)
      s = 'H';

    else
      s = 'F';

    printf("(%d) :\n", population->liste[i].numero);
    printf("\t sexe : %c\n", s);
    printf("\t age : %d\n", population->liste[i].age);
    printf("\t état de sante : %lf\n", population->liste[i].sante);
    printf("\t malade : %d\n", population->liste[i].malade);
    printf("\t temps : %d\n\n", population->liste[i].temps);
  }
}


/* Permet de supprimer la population */
void supprimer_population(Population_t* population)
{
  free(population->liste);
    population->liste = NULL;

  population->nombre = 0;
}
