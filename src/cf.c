/*============================================================================
 * Name        : cf.h
 * Version     : 1.5
 * Since       : 2004
 * Author      : Alberto Realis-Luc <alberto.realisluc@gmail.com>
 * Web         : http://www.alus.it/pubs/CodiceFiscale
 * Copyright   : (C) 2004 Alberto Realis-Luc
 * License     : GNU GPL v2
 * Description : cf module to generate Italian fiscal codes
 *============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cf.h"

#define MAXV 4000 // In linea con il valore massimo ottenibile dalla funzione di hash

comune hash[MAXV] = {NULL}; // tabella di hash
char codfisc[17];
char comuniCaricati = 0;

char *cfCod(const char *cog, const char *nome, int gg, int mm, int aaaa, char sex, const char *codcomune);
unsigned int h(const char *nomeLuogo); // funzione di hash
void inserisci(const char *nomeLuogo, char alfa, int num);
char *prepara(const char *stringa);
int vocale(char c);
int cfData(const char *cog, const char *nom, int gg, int mm, int aaaa, char sex);
char codiceControllo(void);

int carica(const char *filecomuni) {
	FILE *file = NULL;
	if ((file = fopen(filecomuni, "r")) == NULL) return 0;
	char nomeLuogo[MAXP], agg[2], alfa;
	int num;
	while (!feof(file)) {
		nomeLuogo[0]='\0';
		if (fscanf(file, "%c", &agg[0])) {
			agg[1] = '\0';
			do {
				strcat(nomeLuogo, agg);
				if (!fscanf(file, "%c", &agg[0])) agg[0] = '\0';
			} while (agg[0] != '\t');
			if (fscanf(file,"%c%d\n", &alfa, &num) == 2) inserisci(strdup(nomeLuogo), alfa, num);
		}
	}
	fclose(file);
	comuniCaricati = 1;
	return 1;
}

int cfData(const char *cog, const char *nom, int gg, int mm, int aaaa, char sex) {
	char cogn[MAXP], nome[MAXP], tre[4];
	unsigned int num, i, cont=0;
	strcpy(cogn, prepara(cog));
	for(i=0; i<strlen(cogn) && cont<=3; i++)
		if(vocale(cogn[i])==0) { // si prendono le prime 3 consonanti
			tre[cont]=cogn[i];
			cont++;
		}
	if(cont < 3) { // non basta? allora si prendono anche le vocali
		for(i=0; i<strlen(cogn) && cont<=3; i++)
			if(vocale(cogn[i])) {
				tre[cont] = cogn[i];
				cont++;
			}
		if(cont < 3) for(i=cont; i<3; i++)
			tre[i]='X'; // non basta? allora si aggiunge X
	}
	tre[3]='\0';
	strcpy(codfisc, tre);
	strcpy(nome, prepara(nom)); // si prendono la prima, la terza e la quarta consonante
	cont=0;
	num=0; // num conterà le consonanti
	for(i=0; i<strlen(nome) && cont<=3; i++)
		if(vocale(nome[i]) == 0) {
			num++;
			if(num != 2) { // non è la seconda vero?
				tre[cont] = nome[i];
				cont++;
			}
		}
	if(cont<3) { // non basta? allora si prendono le prime 3 consonanti come nel cognome
		cont=0;
		for(i = 0; i < strlen(nome) && cont <= 3; i++)
			if(vocale(nome[i]) == 0) {
				tre[cont] = nome[i];
				cont++;
			}
		if(cont<3) { // non basta? allora si prendono anche le vocali
			for(i=0; i<strlen(nome) && cont <= 3; i++)
				if(vocale(nome[i])) {
					tre[cont] = nome[i];
					cont++;
				}

			if(cont<3) for(i = cont; i < 3; i++)
				tre[i] = 'X'; // non basta? allora si aggiunge X
		}
	}
	tre[3] = '\0';
	strcat(codfisc,tre);
	if (gg < 1 || gg > 31 || mm < 1 || mm > 12) return (-1);
	aaaa=aaaa%100;
	if (aaaa < 10) strcat(codfisc, "0");
	sprintf(tre, "%d", aaaa);
	strcat(codfisc, tre);
	switch(mm){
		case 1:
			strcat(codfisc,"A");
			break;
		case 2:
			strcat(codfisc,"B");
			break;
		case 3:
			strcat(codfisc,"C");
			break;
		case 4:
			strcat(codfisc,"D");
			break;
		case 5:
			strcat(codfisc,"E");
			break;
		case 6:
			strcat(codfisc,"H");
			break;
		case 7:
			strcat(codfisc,"L");
			break;
		case 8:
			strcat(codfisc,"M");
			break;
		case 9:
			strcat(codfisc,"P");
			break;
		case 10:
			strcat(codfisc,"R");
			break;
		case 11:
			strcat(codfisc,"S");
			break;
		case 12:
			strcat(codfisc,"T");
			break;
	}
	if (sex != 'm' && sex != 'M' && sex!= 'f' && sex != 'F') return -2;
	switch(sex) {
		case 'm':
			if (gg < 10) strcat(codfisc,"0");
			sprintf(tre,"%d",gg);
			strcat(codfisc,tre);
			break;
		case 'M':
			if (gg < 10) strcat(codfisc,"0");
			sprintf(tre,"%d",gg);
			strcat(codfisc,tre);
			break;
		case 'f':
			sprintf(tre,"%d",gg+40);
			strcat(codfisc,tre);
			break;
		case 'F':
			sprintf(tre,"%d",gg+40);
			strcat(codfisc,tre);
			break;
	}
	return 1;
}

char codiceControllo(void) {
	int num = 0, i , cont = 0; //somma dei caratteri, indica quando sono in posizione pari
	const int disp[] = {1,0,5,7,9,13,15,17,19,21,2,4,18,20,11,3,6,8,12,14,16,10,22,25,24,23};
	char tre[4];
	tre[1]='\0';
	for(i=0;i<6;i++) { // prime 6 lettere
		if(cont) num=num+(codfisc[i]-65); // pari
		else num=num+disp[codfisc[i]-65]; // dispari
		cont=!cont;
	}
	for(;i<8;i++) { // 2 cifre dell'anno
		if(cont) {
			tre[0]=codfisc[i];
			num=num+atoi(tre);
		} // pari
		else {
			tre[0]=codfisc[i];
			num=num+disp[atoi(tre)];
		} // dispari
		cont=!cont;
	}
	// lettera del mese
	if(cont) num=num+(codfisc[i]-65); // pari
	else num=num+disp[codfisc[i]-65]; // dispari
	cont=!cont;
	i++;
	for(;i<11;i++) { // 2 cifre dell'eta e del sesso
		if(cont) {
			tre[0]=codfisc[i];
			num=num+atoi(tre);
		} // pari
		else {
			tre[0]=codfisc[i];
			num=num+disp[atoi(tre)];
		} // dispari
		cont=!cont;
	}
	// lettera del comune
	if(cont) num=num+(codfisc[i]-65); // pari
	else num=num+disp[codfisc[i]-65]; // dispari
	cont=!cont;
	i++;
	for(;i<15;i++) { // 3 cifre del comune
		if(cont) {
			tre[0]=codfisc[i];
			num=num+atoi(tre);
		} // pari
		else {
			tre[0]=codfisc[i];
			num=num+disp[atoi(tre)];
		} // dispari
		cont=!cont;
	}
	return ((num%26)+65);
}

char *cf(const char *cog, const char *nom, int gg, int mm, int aaaa, char sex, comune comun) {
	char tre[4];
	if(comuniCaricati==0) if(carica("comuni.txt")==0) return strdup("ERRORE FILE");
	if(!cfData(cog, nom, gg, mm, aaaa, sex)) return strdup("ERRORE DATI");
	if(comun!=NULL) {
		tre[0] = comun->alfa;
		tre[1] = '\0';
		strcat(codfisc,tre);
		sprintf(tre,"%3d",comun->num);
		if(comun->num < 100) {
			tre[0]='0';
			if(comun->num < 10) tre[1]='0';
		}
		tre[3] = '\0';
		strcat(codfisc,tre);
	} else return strdup("ERRORE COMUNE");
	codfisc[15] = codiceControllo();
	codfisc[16] = '\0';
	return codfisc;
}

char *cfCod(const char *cog, const char *nom, int gg, int mm, int aaaa, char sex, const char *codcomune) {
	if (!cfData(cog, nom, gg, mm, aaaa, sex)) return strdup("ERRORE DATI");
	strcat(codfisc, codcomune);
	codfisc[15] = codiceControllo();
	codfisc[16] = '\0';
	return codfisc;
}

unsigned int h(const char *nomeLuogo) { // funzione di hash
	unsigned int i, cod=0;
	const size_t len = strlen(nomeLuogo);
	for(i = 0; i < len; i++) {
		cod =+ nomeLuogo[i];
	}
	return cod;
}

void inserisci(const char *nomeLuogo, char alfa, int num) {
	comune inserendo;
	inserendo = (comune)malloc(sizeof(struct comunale));
	if(inserendo == NULL) return;
	inserendo->nome = nomeLuogo;
	inserendo->alfa = alfa;
	inserendo->num = num;
	const unsigned int pos = h(nomeLuogo);
	if (pos < MAXV) {
		if(hash[pos] == NULL) inserendo->nx = NULL;
		else inserendo->nx = hash[pos];
		hash[pos] = inserendo;
	} else printf("ERRORE: dimensioni tabella di hash insufficienti\n");
	
}

comune ricerca(const char *nomeLuogo) {
	comune retVal;
	char* luogo = prepara(nomeLuogo);
	const unsigned int pos = h(luogo);
	if (strcmp(hash[pos]->nome, luogo)==0) return hash[pos];
	else {
		retVal = hash[pos];
		while (strcmp(retVal->nome, luogo)!=0 && retVal->nx!=NULL) retVal=retVal->nx;
		if(strcmp(retVal->nome, luogo)!=0) retVal=NULL;
	}
	free(luogo);
	return retVal;
}

char *prepara(const char *stringa) {
	char uscita[MAXP+1]="\0", agg[3];
	unsigned char c;
	int i = 0;
	while (stringa[i] != '\0' && stringa[i] != '\n' && stringa[i] != '\r') { // questo perche' fgets() si prende anche il carattere invio
		agg[1] = '\0';
		c = stringa[i++];
		if((c >= 'A' && c <= 'Z') || c == ' ') agg[0] = c; // e' gia' maiuscola o e' uno spazio quindi non fare nulla
		else if(c >= 'a' && c <= 'z') agg[0] = c-32; // e' minuscola e va "maisuscolizzata"
		else agg[0] = '\0'; //FIXME: Non tiene conto degli accenti!!
			/* TODO: Questo non funziona piu' usando UTF-8: necessario usare wchar
			switch(c) { // casi particolari
			case 'à':
				agg[0]='A';
				agg[1]='\'';
				agg[2]='\0';
				break;
			case 'è':
				agg[0]='E';
				agg[1]='\'';
				agg[2]='\0';
				break;
			case 'é':
				agg[0]='E';
				agg[1]='\'';
				agg[2]='\0';
				break;
			case 'ì':
				agg[0]='I';
				agg[1]='\'';
				agg[2]='\0';
				break;
			case 'ò':
				agg[0]='O';
				agg[1]='\'';
				agg[2]='\0';
				break;
			case 'ù':
				agg[0]='U';
				agg[1]='\'';
				agg[2]='\0';
				break;
			case 'ä':
				agg[0]='A';
				agg[1]='\'';
				agg[2]='\0';
				break;
			case 'ö':
				agg[0]='O';
				agg[1]='\'';
				agg[2]='\0';
				break;
			case 'ü':
				agg[0]='U';
				agg[1]='\'';
				agg[2]='\0';
				break;
			default:
				agg[0]=c;
				agg[1]='\'';
				agg[2]='\0';
				break;
			}*/

		strcat(uscita, agg);
	}
	return strdup(uscita);
}

int vocale(char c) {
	switch(c){
		case 'A':
		case 'E':
		case 'I':
		case 'O':
		case 'U':
			return 1;
		default:
			if(c>65 && c<=90) return 0; //consonante
			else return -1; // altro carattere
	}
}

void pulisci(void) {
	if (!comuniCaricati) return;
	comune dato;
	for (unsigned int i = 0; i < MAXV; i++) {
		dato = hash[i];
		while (dato != NULL) {
			comune next = dato->nx;
			free(dato);
			dato = next;
		}
		hash[i] = NULL;
	}
}
