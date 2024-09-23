/*

  Projet de Clément Rouanet
            MACS2 11702635

*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "covid.h"


int main()
{
  srand(time(NULL));  // Pour avoir les fonction rand() vraiment aléatoire

  simulation(); // Simule l'évolution du covid dans la population en fonction du temps
}
