/*============================================================================
 * Name        : cf.h
 * Version     : 1.5
 * Since       : 2004
 * Author      : Alberto Realis-Luc <alberto.realisluc@gmail.com>
 * Web         : http://www.alus.it/pubs/CodiceFiscale
 * Copyright   : (C) 2004 Alberto Realis-Luc
 * License     : GNU GPL v2
 * Description : Program to generate Italian fiscal codes
 *============================================================================*/

#include "cf.h"
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

void copyToClipboard(const char* text) {
	const size_t length = strlen(text) + 1;
	if (length <= 1) return;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, length);
	memcpy(GlobalLock(hMem), text, length);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}
#endif

int main(void) {
	char *codfisc,fine=0,alfa,nomeComune[MAXP+3],cogn[MAXP+3],nome[MAXP+3];
	int gg,mm,aaaa;

	printf("                    Calcolo del codice fiscale\n");
	printf("Lettura comuni in corso....");
	if(carica("comuni.txt")==0) {
		printf(" FALLITO\nERRORE: File 'comuni.txt' non trovato o danneggiato!\n");
		return EXIT_FAILURE;
	}
	printf(" OK\n");
	while(!fine) {
		comune luogoNascita;
		printf("\n\n\n");
		printf("Cognome: ");
		fgets(cogn,MAXP+3,stdin);
		printf("Nome: ");
		fgets(nome,MAXP+3,stdin);
		printf("Inserire la data di nascita:\n");
		do {
			printf("Giorno di nascita (gg): ");
			scanf("%d",&gg);
			if(gg<1 || gg>31) printf("ERRORE: Giorno di nascita non valido.\n");
		} while(gg<1 || gg>31);
		do {
			printf("Mese di nascita (mm): ");
			scanf("%d",&mm);
			if(mm<1 || mm>12) printf("ERRORE: Mese di nascita non valido.\n");
		} while(mm<1 || mm>12);
		printf("Anno di nascita (aaaa): ");
		scanf("%d",&aaaa);
		getchar();				
		do {
			printf("Sesso (M/F): ");
			scanf("%c",&alfa);
			getchar();
			if(alfa!='m' && alfa!='M' && alfa!='f' && alfa!='F') printf("ERRORE: Inserire solo il carattere 'M' o 'F'.\n");
		} while(alfa!='m' && alfa!='M' && alfa!='f' && alfa!='F');
		do {
			printf("Comune di nascita: ");
			fgets(nomeComune,MAXP+3,stdin);
			luogoNascita=ricerca(nomeComune);
			if(luogoNascita==NULL) printf("ERRORE: Comune non trovato.\n");
		} while(luogoNascita==NULL);
		codfisc=cf(cogn,nome,gg,mm,aaaa,alfa,luogoNascita);
		printf("Codice fiscale: %s\n\n",codfisc);
#ifdef _WIN32
		copyToClipboard(codfisc);
#endif
		printf("Vuoi calcolarne un altro?  (S/N): ");
		scanf("%c",&alfa);
		if(alfa!='s'&&alfa!='S') fine=1;
		else scanf("%c",&alfa);
	}/*chiude il while di uscita*/
	pulisci();
	return EXIT_SUCCESS;
} /*chiude il main*/
