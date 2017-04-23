#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Numarul de ghisee
int length;

typedef struct ListNode
{
    struct ListNode *urmator;
    struct ListNode *anterior;
    int prioritate;
    char *nume;
} ListNode;

typedef struct List
{
    ListNode *primul;
    ListNode *ultimul;
} List;

int calculeaza_numar_oameni_ghiseu(List **ghisee, int numar_ghiseu)
{
 // Numaru ghiseu valid
  if(numar_ghiseu >= length)
    return -1;

	int i = numar_ghiseu;
	int numar_oameni_ghiseu = 0;

 // Nu sunt alocate ghiseele
	if(ghisee == NULL)
		return -1;

 // Ghiseul este gol
	if(ghisee[i] == NULL)
		return 0;
	if(ghisee[i]->primul == NULL)
		return 0;
	else
  // Are o singura persoana
		if(ghisee[i]->primul == ghisee[i]->ultimul)
			return 1;
	else
	{
  // Ghiseul are cel putin doua persoane
		ListNode *p;
		p = ghisee[i]->primul;
		while(p != NULL)
		{
			numar_oameni_ghiseu++;
			p = p->urmator;
		}
	}
	return numar_oameni_ghiseu;
}

int calculeaza_numar_total_oameni(List **ghisee)
{
	int i, numar_oameni_total = 0;
  for(i = 0; i < length; i++)
    numar_oameni_total += calculeaza_numar_oameni_ghiseu(ghisee, i);

	return numar_oameni_total;
}

void inchide_ghisee(List ***ghisee)
{
	int i;
	for(i = 0; i < length; i++)
	{
		ListNode *p;
		if((*ghisee)[i] != NULL)
		{
			p = (*ghisee)[i]->primul;
			while(p != NULL)
			{
				ListNode *aux;
				aux = p;
				p = p->urmator;
				if(aux->nume != NULL)
					free(aux->nume);
				free(aux);
			}
		if((*ghisee)[i] != NULL)
			free((*ghisee)[i]);
		}
	}

	free(*ghisee);
	length = 0;
	*ghisee = NULL;
	return ;
}

void adauga_persoana(List ***ghisee, char *nume, int prioritate,
		int numar_ghiseu)
{
 // Aloc memorie pentru noua persoana
	ListNode *persoana;
	persoana = (ListNode *) malloc(sizeof(ListNode));
	persoana->nume = (char *) malloc(sizeof(char) * strlen(nume) + 1);
	strcpy(persoana->nume, nume);
	persoana->prioritate = prioritate;
	persoana->urmator = NULL;
	persoana->anterior = NULL;

	int i, j;
 // Verific daca trebuie inserata prima (nu exista alte persoane)
  if((*ghisee)[numar_ghiseu]->primul == NULL)
	{
		(*ghisee)[numar_ghiseu]->primul = persoana;
		(*ghisee)[numar_ghiseu]->ultimul = persoana;
		return ;
	}

	ListNode *p;
	p = (*ghisee)[numar_ghiseu]->primul;

 // Caut prima persoana cu prioritate mai mica sau egala
	while(p != NULL)
	{
		if(p->prioritate <= persoana->prioritate)
			break;
		else
			p = p->urmator;
	}

 // Daca au prioritate egala, caut persoana in fata careia
 // trebuie sa o inserez, dupa nume
	if(p != NULL)
		if(p->prioritate == persoana->prioritate)
			while(strcmp(p->nume, persoana->nume) < 0   &&
				  p->prioritate == persoana->prioritate &&
			  	  p != NULL)
				{
					p = p->urmator;
					if(p == NULL)
						break;
				}
 // Daca am ajuns la sfarsit, o inserez ultima
	if(p == NULL)
	{
		persoana->anterior = (*ghisee)[numar_ghiseu]->ultimul;
		(*ghisee)[numar_ghiseu]->ultimul->urmator = persoana;
		(*ghisee)[numar_ghiseu]->ultimul = persoana;
		return ;
	}

 // Daca sunt pozitionat pe prima, o inserez prima
	if(p == (*ghisee)[numar_ghiseu]->primul)
	{
		persoana->urmator = (*ghisee)[numar_ghiseu]->primul;
		(*ghisee)[numar_ghiseu]->primul->anterior = persoana;
		(*ghisee)[numar_ghiseu]->primul = persoana;
		return ;
	}

 // Altfel, intre doua persoane
	p->anterior->urmator = persoana;
	persoana->anterior = p->anterior;
	persoana->urmator = p;
	p->anterior = persoana;
  return ;
}

void deschide_ghisee(List ***ghisee, int N)
{
	if(*ghisee == NULL && length == 0)
	{
 // Nu am niciun ghiseu deschis
		(*ghisee) = (List **) malloc(sizeof(List *) * N);
		int i;
		for(i = 0; i < N; i++)
		{
			((*ghisee)[i]) = (List *) malloc(sizeof(List));

			(*ghisee)[i]->primul = NULL;
			(*ghisee)[i]->ultimul = NULL;

		}
		length = N;
	}
	else
	{
 // Am cel putin un ghiseu deschis
		int i = 0, j =0;
		List **ghiseu_temp = NULL;
		ghiseu_temp = (List **) malloc(sizeof(List *) * 1);
		ghiseu_temp[0] = (List *) malloc(sizeof(List));
		ghiseu_temp[0]->primul = NULL;
		ghiseu_temp[0]->ultimul = NULL;

 // Adaug fiecare persoana in ghiseul temporar
		for(i = 0; i < length; i++)
		{
			ListNode *p;
			p = (*ghisee)[i]->primul;

			while(p != NULL)
			{
				adauga_persoana(&ghiseu_temp, p->nume, p->prioritate, 0);
				p = p->urmator;
			}
		}

 // Realoc memoria pentru ghisee
		inchide_ghisee(ghisee);
		length = 0;
		deschide_ghisee(ghisee, N);

		i = 0;
		ListNode *p = ghiseu_temp[0]->primul;

 // Adaug persoanele in noile ghisee
		while(p != NULL)
		{
			adauga_persoana(ghisee, p->nume, p->prioritate, i);
			if(i + 1 == length)
				i = -1;
			i++;
			p = p->urmator;
		}

 // Eliberez memoria pentru ghiseul temporar
		if(ghiseu_temp[0] != NULL)
		{
			p = ghiseu_temp[0]->primul;
			while(p != NULL)
			{
				ListNode *aux;
				aux = p;
				p = p->urmator;
				if(aux->nume != NULL)
					free(aux->nume);
				free(aux);
			}

    if(ghiseu_temp[0] != NULL)
			free(ghiseu_temp[0]);
		}

	  free(ghiseu_temp);
	  ghiseu_temp = NULL;
	}
  return ;
}

void sterge_persoana(List ***ghisee, char *nume, int prioritate,
		int numar_ghiseu)
{
 // Numar ghiseu valid
	if(numar_ghiseu >= length)
		return ;

	ListNode *p;
	p = (*ghisee)[numar_ghiseu]->primul;

 // Nu exista nicio persoana in ghiseu
	if(p == NULL)
		return ;

 // Cautare persoana
	while(p != NULL && strcmp(p->nume, nume) != 0 &&
        prioritate != p->prioritate)
		p = p->urmator;

	while(p != NULL && prioritate != p->prioritate)
		p = p->urmator;

	if(p == NULL)
 // Persoana nu exista
		return ;
	else
 // Persoana exista si trebuie stearsa
		if(strcmp(p->nume, nume) == 0 && prioritate == p->prioritate)
	{
		if(p == (*ghisee)[numar_ghiseu]->primul)
		{
			if(p == (*ghisee)[numar_ghiseu]->ultimul)
			{
        // Persoana este singura din sir
				(*ghisee)[numar_ghiseu]->primul  = NULL;
				(*ghisee)[numar_ghiseu]->ultimul = NULL;
				free(p->nume);
				free(p);
			}
			else
			{
        // Persoana este prima si nu este singura
				p->urmator->anterior = NULL;
				(*ghisee)[numar_ghiseu]->primul = p->urmator;
				free(p->nume);
				free(p);
			}
		}
		else
		if(p == (*ghisee)[numar_ghiseu]->ultimul)
		{
      // Persoana este ultima si nu este singura
			p->anterior->urmator = NULL;
			(*ghisee)[numar_ghiseu]->ultimul = p->anterior;
			free(p->nume);
			free(p);
		}
		else
		{
      // Persoana nu este nici prima, nici ultima
			p->anterior->urmator = p->urmator;
			p->urmator->anterior = p->anterior;
			free(p->nume);
			free(p);
		}
	}
  return ;
}

void sterge_primii_oameni(List ***ghisee)
{
	int i;
	for(i = 0; i < length; i++)
	{
		if((*ghisee)[i] != NULL)
			if((*ghisee)[i]->primul != NULL)
			{
				ListNode *p;
				p = (*ghisee)[i]->primul;

				if(p == (*ghisee)[i]->ultimul)
				{
					(*ghisee)[i]->primul = NULL;
					(*ghisee)[i]->ultimul = NULL;

					free(p->nume);
					free(p);
				}
				else
				{
					(*ghisee)[i]->primul = p->urmator;
					(*ghisee)[i]->primul->anterior = NULL;

					free(p->nume);
					free(p);
				}
		}
	}
}

int gaseste_pozitie_persoana (List **ghisee, char *nume, int prioritate,
		int numar_ghiseu)
{
 // Verificare ghiseu valid
	if(numar_ghiseu >= length)
		return -1;

	ListNode *p;
	p = ghisee[numar_ghiseu]->primul;

 // Cautare persoana
	int poz = 0;
	while(p != NULL)
		if(strcmp(p->nume, nume) == 0 && prioritate == p->prioritate)
			return poz;
		else
		{
			poz++;
			p = p->urmator;
		}

 // Persoana nu a fost gasita
	return -1;
}

char* gaseste_persoana_din_fata(List **ghisee, char *nume, int prioritate,
		int numar_ghiseu)
{
 // Nu exista ghiseul
  if(numar_ghiseu >= length)
    return "Nu exista.";

	ListNode *p;
	p = ghisee[numar_ghiseu]->primul;

 // Nu exista nicio persoana in la ghiseu
	if(p == NULL)
		return "Nu exista.";

 // Persoana cautata este prima
	if(strcmp(p->nume, nume) == 0 && prioritate == p->prioritate)
		return "Nu exista.";

 // Cautare persoana
	p = p->urmator;
	while(p != NULL)
	{
		if(strcmp(p->nume, nume) == 0 && prioritate == p->prioritate)
			return p->anterior->nume;
		p = p->urmator;
	}

 // Persoana cautata nu exista
	return "Nu exista.";
}

void afiseaza_rand_ghiseu(FILE *f, List **ghisee, int numar_ghiseu)
{
 // Numar ghiseu valid
	if(numar_ghiseu >= length)
		return ;

	ListNode *p;
	p = ghisee[numar_ghiseu]->primul;
	if(p != NULL)
	{
    // Ghiseul are cel putin o persoana
		while(p != NULL)
		{
			fprintf(f, "%s-%d;", p->nume, p->prioritate);
			p = p->urmator;
		}
		fprintf(f, "\n");
	}
	else
  // Ghiseul este gol
		fprintf(f, "Ghiseul %d nu are nicio persoana in asteptare.\n",
            numar_ghiseu);
}

#endif
