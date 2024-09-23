#include <stdio.h>
#include <stdlib.h>

#include "covid.h"


/* Initialisation de la structure Covid_t */
void initialisation_covid(Covid_t* covid, Population_t* pop)
{
  int i;

  covid->temps = DUREE;

  // Allocation dynamique
  covid->nbMalades = malloc(sizeof(int)*DUREE);
  covid->nbMorts = malloc(sizeof(int)*DUREE);
  covid->nbVaccines = malloc(sizeof(int)*DUREE);
  covid->nbLitsOccupes = malloc(sizeof(int)*DUREE);

  if (covid->nbMalades==NULL || covid->nbMorts==NULL || covid->nbVaccines==NULL || covid->nbLitsOccupes==NULL)
  {
    printf("Problème allocation mémoire dans initialisation_covid\n");
    exit(EXIT_FAILURE);
  }

  covid->nbMalades[0] = 0;
  covid->nbMorts[0] = 0;
  covid->nbVaccines[0] = 0;
  covid->nbLitsOccupes[0] = 0;

  // Initialisation des malades au temps t = 0
  for (i=0; i<pop->nombre; i++)
  {
    if (pop->liste[i].malade == 2)
      covid->nbMalades[0] += 1;
  }
}


/* Suppression de la structure Covid_t */
void supprimer_covid(Covid_t* covid)
{
  free(covid->nbMalades);
    covid->nbMalades = NULL;

  free(covid->nbMorts);
    covid->nbMorts = NULL;

  free(covid->nbVaccines);
    covid->nbVaccines = NULL;

  free(covid->nbLitsOccupes);
    covid->nbLitsOccupes = NULL;

  covid->temps = 0;
}


/* Stockage du nombre de nouveaux malades à chaque temps dans un tableau */
void stockage_nb_malades(Population_t* pop, int* tab, int t)
{
  for (int i=0; i<pop->nombre; i++)
    *(tab+t+i*DUREE) = pop->liste[i].malade;  // Passage par pointeur d'un tableau de dimension 2
}


/* Simule les contaminations entre les individus à chaque temps (jours) */
void contamination(Graphe_t* famille, Graphe_t* amis, Covid_t* covid, Population_t* population, int t)
{
  int i;
  double somme;
  Personne_t* pers;

  covid->nbMalades[t] = 0;

  // On regarde les liens de chaque personne de la population avec des malades ou non
  for (i=0; i<population->nombre; i++)
  {
    pers = &population->liste[i];
    somme = 0;

    // Si la personne n'est pas malade aors elle peut être contaminée
    if(pers->malade==0)
    {
      // On regarde sa famille
      ListePersonne_t* p = famille->voisins[i].premiere;
      while (p != NULL)
      {
        if (p->personne->malade==1 || p->personne->malade==2) // Si un membre de sa famille est infecté ou malade
          somme += TAUX_TRANS_FAMILLE;

        p = p->suivante;  // Prochain membre de la famille
      }

      // On regarde ses amis
      p = amis->voisins[i].premiere;
      while (p != NULL)
      {
        if (p->personne->malade==1 || p->personne->malade==2) // Si un de ses amis est infecté ou malade
          somme += TAUX_TRANS_AMIS;

        p = p->suivante;  // Prochain amis
      }

      // Condition pour que la personne tombe malade
      if (somme > pers->sante)
      {
        pers->malade = 1;
        pers->temps= 1;
        covid->nbMalades[t] += 1;
      }
    }
  }
}


/* On incrémente le temps de chaque personne infectée ou malade */
void temps_malade(Population_t* population)
{
  int i;
  Personne_t* pers;

  for (i=0; i<population->nombre; i++)
  {
    pers = &population->liste[i];

    if (pers->temps != 0)
    {
      pers->temps += 1;
    }
  }
}


/* Simule la vaccination de la population à chaque temps */
void vaccination(Population_t* population, Covid_t* covid, int t)
{
  int i;
  Personne_t* pers;

  covid->nbVaccines[t] = 0;

  // On a fait 4 boucles pour ne pas vacciner des personnes conséctuviment et ainsi ne pas vacciner un foyer familial après l'autre

  for (i=0; i<population->nombre; i+=4)
  {
    pers = &population->liste[i];

    if(pers->malade==0 && covid->nbVaccines[t]<VACCINATION)
    {
      pers->malade = 3;
      covid->nbVaccines[t] += 1;
    }
  }

  for (i=population->nombre; i>1; i-=4)
  {
    pers = &population->liste[i];

    if(pers->malade==0 && covid->nbVaccines[t]<VACCINATION)
    {
      pers->malade = 3;
      covid->nbVaccines[t] += 1;
    }
  }

  for (i=2; i<population->nombre; i+=4)
  {
    pers = &population->liste[i];

    if(pers->malade==0 && covid->nbVaccines[t]<VACCINATION)
    {
      pers->malade = 3;
      covid->nbVaccines[t] += 1;
    }
  }

  for (i=population->nombre; i>3; i-=4)
  {
    pers = &population->liste[i];

    if(pers->malade==0 && covid->nbVaccines[t]<VACCINATION)
    {
      pers->malade = 3;
      covid->nbVaccines[t] += 1;
    }
  }
}


/* Ecrit dans un fichier les différents états (sain, infecté, malade, immunisé ou mort) pour chaque individu à tous les temps */
void ecriture_evolution(Population_t* pop, int tab[NB_POPULATION][DUREE], char* fichier)
{
  FILE* f;

  f = fopen(fichier, "w");
  if (f == NULL)
  {
    printf("Problème ouverture fichier dans ecriture_evolution\n");
    exit(EXIT_FAILURE);
  }

  fprintf(f, "Evolution de la maladie pour chaque individu\n");
  fprintf(f, "\t - '.' : individu sain\n");
  fprintf(f, "\t - 'I' : individu infecté\n");
  fprintf(f, "\t - 'M' : individu malade\n");
  fprintf(f, "\t - 'P' : individu immunisé\n");
  fprintf(f, "\t - 'D' : individu décédé\n\n");

  fprintf(f, "• Population totale : %d\n", pop->nombre);
  fprintf(f, "• Durée de la simulation : %d jours\n", DUREE);
  fprintf(f, "• Proportion de population malade au temps 0 : %.1lf %% \n", PROBA*100);
  fprintf(f, "• Nombre de lits totals : %d\n", (int)NB_LITS);
  fprintf(f, "• Nombre de vaccination par jour : %d\n", (int)VACCINATION);
  fprintf(f, "• Taux de transmission en famille : %.2lf\n", TAUX_TRANS_FAMILLE);
  fprintf(f, "• Taux de transmission entre amis : %.2lf\n", TAUX_TRANS_AMIS);
  fprintf(f, "• Temps d'incubation : %d jours\n", TPS_INCUBATION-1);
  fprintf(f, "• Temps de la maladie : %d jours\n\n\n", TPS_MALADE-1);

  for (int i=0; i<pop->nombre; i++)
  {
    Personne_t* pers = &pop->liste[i];

    if (i < 9)
      fprintf(f, "(%d)      :   ", i+1);

    if (i>=9 && i<99)
      fprintf(f, "(%d)     :   ", i+1);

    if (i>=99 && i<999)
      fprintf(f, "(%d)    :   ", i+1);

    if (i>=999 && i<9999)
      fprintf(f, "(%d)   :   ", i+1);

    if (i>=9999 && i<99999)
      fprintf(f, "(%d)  :   ", i+1);

    char sexe;
    if (pers->sexe == 0)
      sexe = 'F';

    else
      sexe = 'H';

    fprintf(f, "%c ", sexe);

    if (pers->age < 10)
      fprintf(f, "%d   ", pers->age);

    else if (pers->age>=10 && pers->age < 100)
      fprintf(f, "%d  ", pers->age);

    else
      fprintf(f, "%d ", pers->age);

    fprintf(f, "%lf   →   ", pers->sante);

    for (int j=0; j<DUREE; j++)
    {
      if (tab[i][j] == -1)
        fprintf(f, "D");

      if (tab[i][j] == 0)
        fprintf(f, ".");

      if (tab[i][j] == 1)
        fprintf(f, "I");

      if (tab[i][j] == 2)
        fprintf(f, "M");

      if (tab[i][j] == 3)
        fprintf(f, "P");
    }
    fprintf(f, "\n");
  }

  fclose(f);
}


/* Ecrit dans un fichier les chiffres (nbMalades, nbMorts, nbVaccines, nbLitsOccupes) à chaque temps pour tracer les courbes avec gnuplot */
void ecriture_chiffres(Covid_t* covid, char* fichier)
{
  FILE* f;

  f = fopen(fichier, "w");
  if (f == NULL)
  {
    printf("Problème ouverture fichier dans ecriture_chiffres\n");
    exit(EXIT_FAILURE);
  }

  for (int i=0; i<covid->temps; i++)
  {
    fprintf(f, "%d ", i);
    fprintf(f, "%d ", covid->nbMalades[i]);
    fprintf(f, "%d ", covid->nbMorts[i]);
    fprintf(f, "%d ", covid->nbVaccines[i]);
    fprintf(f, "%d\n", covid->nbLitsOccupes[i]);
  }

  fclose(f);
}


/* Affiche sur le terminal les chiffres (nbMalades, nbMorts, nbVaccines, nbLitsOccupes) à chaque temps */
void affichage_chiffres(Covid_t* covid, Population_t* pop)
{
  int malades = 0, morts = 0, vaccines = 0, lits = 0;
  int sains = 0;

  for(int i=0; i<covid->temps; i++)
  {
    printf("Jour %d :\n", i+1);
    printf("   Nouveaux Malades : %d\n", covid->nbMalades[i]);
    printf("   Nouveaux Morts : %d\n", covid->nbMorts[i]);
    printf("   Nouveaux Vaccinés : %d\n", covid->nbVaccines[i]);
    printf("   Nouveaux Lits Occupés : %d\n\n", covid->nbLitsOccupes[i]);

    malades += covid->nbMalades[i];
    morts += covid->nbMorts[i];
    vaccines += covid->nbVaccines[i];
    lits += covid->nbLitsOccupes[i];
  }

  for (int i=0; i<pop->nombre; i++)
  {
    if (pop->liste[i].malade == 0)
      sains += 1;
  }

  printf("\n• Taille population : %d\n", NB_POPULATION);
  printf("• Durée de la simulation : %d jours\n", DUREE);
  printf("• Total de malades : %d\n", malades);
  printf("• Total de sains : %d\n", sains);
  printf("• Total de morts : %d\n", morts);
  printf("• Total de vaccinés : %d\n", vaccines);
  printf("• Total de lits occupés : %d\n", lits);
}


/* Permet de tracer les différenes courbes (nbMalades, nbMorts, nbVaccines, nbLitsOccupes) au cours du temps avec gnuplot */
void tracage_gnuplot(char* fichier)
{
  char touche = 'a';
  FILE* f;

  f = popen("gnuplot", "w");
  if (f == NULL)
  {
    printf("Problème ouverture fichier dans tracage_gnuplot\n");
    exit(EXIT_FAILURE);
  }

  fprintf(f, "plot \"%s\" u 1:2 w lines title \"Nb malades\", \"%s\" u 1:3 w lines title \"Nb morts\", \"%s\" u 1:4 w lines title \"Nb vaccinés\", \"%s\" u 1:5 w lines title \"Nb lits occupés\"\n", fichier, fichier, fichier, fichier);
  fflush(f);

  // Ferme la fenêtre dès qu'on rentre la touche 'Q' dans le terminal
  while (touche != 'Q' && touche != 'q')
    scanf("%c", &touche);

  pclose(f);
}


/* Fonction principale : permet de simuler l'évolution du covid dans le temps */
void simulation()
{
  Covid_t covid;
  Arbre_t* attente = NULL;

  char* fichier1 = "evolution.txt";
  char* fichier2 = "chiffres.txt";

  int tab[NB_POPULATION][DUREE];

  Population_t population;
  int duree = 0;
  initialisation_population(&population, NB_POPULATION, PROBA);
  // afficher_population(&population);

  Graphe_t* GrFamille = familles(&population);
  Graphe_t* GrAmis = amis(&population);
  // afficher_graphe(GrFamille, &population);
  // afficher_graphe(GrAmis, &population);

  initialisation_covid(&covid, &population);
  attente = initialisation_hopital(attente, &population, &covid, duree);
  stockage_nb_malades(&population, (int*)tab, duree);

  if (attente)
    attente = retirer_attente(attente, &population, &covid, duree);

  for (duree=1; duree<DUREE; duree++)
  {
    contamination(GrFamille, GrAmis, &covid, &population, duree);

    attente = retirer_incubation(attente, &population, &covid, duree);
    attente = retirer_attente(attente, &population, &covid, duree);
    retirer_hopital(&covid, &population, duree);

    vaccination(&population, &covid, duree);
    temps_malade(&population);
    stockage_nb_malades(&population, (int*)tab, duree);
  }

  supprimer_graphe(GrFamille);
  supprimer_graphe(GrAmis);

  if (attente)
  {
    // afficher_arbre(attente);
    supprimer_arbre(&attente);
  }

  ecriture_evolution(&population, tab, fichier1);
  ecriture_chiffres(&covid, fichier2);
  affichage_chiffres(&covid, &population);

  // Mettre en commentaire la ligne suivante si vous n'avez pas Gnuplot
  tracage_gnuplot(fichier2);

  supprimer_population(&population);
  supprimer_covid(&covid);
}
