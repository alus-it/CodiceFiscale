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

int main(int argc, char *argv[]) {
	char *codfisc, ch, nomeComune[MAXP+3], cogn[MAXP+3], nome[MAXP+3], valid = 0;
	int gg, mm, aaaa, retVal = EXIT_FAILURE;
	printf("                    Calcolo del codice fiscale\n");
	
	// Con piu' di un argomento ci aspettiamo i dati da linea di comando
	if (argc > 1) {
		char allArgs[MAXP * 3];
		strcpy(allArgs, argv[1]);
		for (int i = 2; i < argc; ++i) {
  			strcat(allArgs, " ");
			strcat(allArgs, argv[i]);
		}
		char *token = NULL;
		token = strtok(allArgs, ",");
		if (token != NULL) {
			valid = strlen(token) > 0;
			if (valid) {
				strcpy(cogn, token);
				printf("Cognome: %s\n", cogn);
				token = strtok(NULL, ",");
			} else printf("ERRORE: Cognome vuoto.\n");
		}
		if (valid && token != NULL) {
			valid = strlen(token) > 0;
			if (valid) {
				strcpy(nome, token);
				printf("Nome: %s\n", nome);
				token = strtok(NULL, ",");
			} else printf("ERRORE: Nome vuoto.\n");
		}
		if (valid && token != NULL) {
			valid = sscanf(token, "%d/%d/%d", &gg, &mm, &aaaa) == 3 && gg >= 1 && gg <= 31 && mm >= 1 && mm <= 12 && aaaa >= 0;
			if (valid) {
				printf("Data di nascita: %02d/%02d/%4d\n", gg, mm, aaaa);
				token = strtok(NULL, ",");
			} else printf("ERRORE: Data di nascita non valida.\n");
		}
		if (valid && token != NULL) {
			valid = sscanf(token, "%c", &ch) == 1 && (ch == 'm' || ch == 'M' || ch == 'f' || ch == 'F');
			if (valid) {
				printf("Sesso: %c\n", ch);
				token = strtok (NULL, ",");
			} else printf("ERRORE: Sesso non valido: inserire solo il carattere 'M' o 'F'\n");
		}
		if (valid && token != NULL) {
			valid = strlen(token) > 0;
			if (valid) {
				strcpy(nomeComune, token);
				printf("Luogo di nascita: %s\n", nomeComune);
			} else printf("ERRORE: Luogo di nascita vuoto.\n");
		}
	}

	// Questo e' il momento di caricare l'elenco dei luoghi di nascita ...
	if (argc == 1 || (argc > 1 && valid)) { // da evitare se l'argomento contiene dati non validi
		printf("Lettura comuni in corso....");
		if (!carica("comuni.txt")) {
			printf(" FALLITO\nERRORE: File 'comuni.txt' non trovato o danneggiato!\n");
			return EXIT_FAILURE;
		}
		printf(" OK\n\n\n");
	}
	
	// Processa i dati da argomento se presenti e validi
	if (argc > 1) {
		if (valid) {
			comune luogoNascita = ricerca(nomeComune);
			if (luogoNascita != NULL) {
				codfisc = cf(cogn, nome, gg, mm, aaaa, ch, luogoNascita);
				printf("Codice Fiscale: %s\n", codfisc);
#ifdef _WIN32
				copyToClipboard(codfisc);
#endif
				retVal = EXIT_SUCCESS;
			} else printf("ERRORE: Comune non trovato.\n");
		} else {
			printf("Il programma puo' essere usato da linea di comando passando i parametri nel modo seguente: \n");
			printf("cf Rossi,Mario,15/05/1972,M,Roma\n");
			printf("Non sono ammessi altri formati.\n");
			printf("Su Windows il codice fiscale sara' copiato anche negli appunti pronto per essere incollato.\n");
		}
	}

	// Oppure continua con il normale inserimento dati manuale
	char fine = argc==1 ? 0 : 1;
	while (!fine) {
		comune luogoNascita;
		printf("\n\n");
		printf("Cognome: ");
		fgets(cogn, MAXP+3, stdin);
		printf("Nome: ");
		fgets(nome, MAXP+3, stdin);
		printf("Inserire la data di nascita:\n");
		do {
			printf("Giorno di nascita (gg): ");
			scanf("%d", &gg);
			if (gg < 1 || gg > 31) printf("ERRORE: Giorno di nascita non valido.\n");
		} while (gg < 1 || gg > 31);
		do {
			printf("Mese di nascita (mm): ");
			scanf("%d", &mm);
			if (mm < 1 || mm > 12) printf("ERRORE: Mese di nascita non valido.\n");
		} while(mm < 1 || mm > 12);
		do {
			printf("Anno di nascita (aaaa): ");
			scanf("%d", &aaaa);
			if (aaaa < 0) printf("ERRORE: Anno di nascita non valido.\n");
		} while (aaaa < 0);
		getchar();				
		do {
			printf("Sesso (M/F): ");
			scanf("%c", &ch);
			getchar();
			if (ch != 'm' && ch != 'M' && ch != 'f' && ch != 'F') printf("ERRORE: Inserire solo il carattere 'M' o 'F'.\n");
		} while (ch != 'm' && ch != 'M' && ch != 'f' && ch != 'F');
		do {
			printf("Comune di nascita: ");
			fgets(nomeComune, MAXP+3, stdin);
			luogoNascita = ricerca(nomeComune);
			if(luogoNascita == NULL) printf("ERRORE: Comune non trovato.\n");
		} while (luogoNascita == NULL);
		codfisc = cf(cogn, nome, gg, mm, aaaa, ch, luogoNascita);
		printf("Codice fiscale: %s\n\n", codfisc);
#ifdef _WIN32
		copyToClipboard(codfisc);
#endif
		retVal = EXIT_SUCCESS;
		printf("Vuoi calcolarne un altro?  (S/N): ");
		scanf("%c", &ch);
		if(ch != 's' && ch != 'S') fine=1;
		else {
			scanf("%c", &ch);
			retVal = EXIT_FAILURE;
		}
	}

	pulisci();
	return retVal;
}
