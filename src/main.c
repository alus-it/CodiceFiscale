/*============================================================================
 * Name        : cf.h
 * Version     : 1.5
 * Since       : 2004
 * Author      : Alberto Realis-Luc <alberto.realisluc@gmail.com>
 * Web         : http://www.alus.it/pubs/CodiceFiscale
 * Repository  : https://github.com/alus-it/CodiceFiscale
 * Copyright   : (C) 2004 Alberto Realis-Luc
 * License     : GNU GPL v2
 * Description : Program to generate Italian fiscal codes
 *============================================================================*/

#include "cf.h"
#include <stdlib.h>
#define FILE_COMUNI "comuni.txt"

#ifdef _WIN32
#include <windows.h>

void clearClipboard() {
	OpenClipboard(0);
	EmptyClipboard();
	CloseClipboard();
}

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
	printf("                    Calcolo del codice fiscale\n");

	// Con piu' di un argomento ci aspettiamo il percorso del file luoghi di nascita ed i dati da linea di comando
	printf("File luoghi di nascita: %s - Lettura in corso....", argc > 1 ? argv[1] : FILE_COMUNI);
	if (!carica(argc > 1 ? argv[1] : FILE_COMUNI)) {
		printf(" FALLITO\nERRORE: File luoghi di nascita non trovato o danneggiato!\n");
		return EXIT_FAILURE;
	}
	printf(" OK\n");

	char *codfisc, ch, nomeComune[MAXP + 3], cogn[MAXP + 3], nome[MAXP + 3], valid = 1;
	int gg, mm, aaaa, retVal = EXIT_FAILURE;

	// Con piu' di 2 argomenti dal secondo in poi ci aspettiamo i dati della persona a cui calcolare il codice fiscale
	if (argc > 2) {
		char allArgs[MAXP * 3];
		strcpy(allArgs, argv[2]);
		for (int i = 3; i < argc; i++) { // Se ci sono spazi nei nomi
			strcat(allArgs, " ");
			strcat(allArgs, argv[i]);
		}
		char *token = NULL;
		token = strtok(allArgs, ",");
		if ((valid = (token != NULL))) {
			valid = strlen(token) > 0;
			if (valid) {
				strcpy(cogn, token);
				printf("Cognome: %s\n", cogn);
				token = strtok(NULL, ",");
			} else printf("ERRORE: Cognome vuoto.\n");
		}
		if ((valid = (valid && token != NULL))) {
			valid = strlen(token) > 0;
			if (valid) {
				strcpy(nome, token);
				printf("Nome: %s\n", nome);
				token = strtok(NULL, ",");
			} else printf("ERRORE: Nome vuoto.\n");
		}
		if ((valid = (valid && token != NULL))) {
			valid = sscanf(token, "%d/%d/%d", &gg, &mm, &aaaa) == 3 && gg >= 1 && gg <= 31 && mm >= 1 && mm <= 12 && aaaa >= 0;
			if (valid) {
				printf("Data di nascita: %02d/%02d/%4d\n", gg, mm, aaaa);
				token = strtok(NULL, ",");
			} else printf("ERRORE: Data di nascita non valida.\n");
		}
		if ((valid = (valid && token != NULL))) {
			valid = sscanf(token, "%c", &ch) == 1 && (ch == 'm' || ch == 'M' || ch == 'f' || ch == 'F');
			if (valid) {
				printf("Sesso: %c\n", ch);
				token = strtok (NULL, ",");
			} else printf("ERRORE: Sesso non valido: inserire solo il carattere 'M' o 'F'\n");
		}
		if ((valid = (valid && token != NULL))) {
			valid = strlen(token) > 0;
			if (valid) {
				strcpy(nomeComune, token);
				printf("Luogo di nascita: %s\n", nomeComune);
			} else printf("ERRORE: Luogo di nascita vuoto.\n");
		}
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
			printf("ERRORE: Dati incompleti o non validi.\n");
			printf("Il programma puo' essere usato da linea di comando passando i parametri nel modo seguente: \n");
			printf("cf comuni.txt Rossi,Mario,15/05/1972,M,Roma\n");
			printf("Non sono ammessi altri formati. Non usare lettere accentate: omettere gli accenti.\n");
		}
#ifdef _WIN32
		if (retVal == EXIT_FAILURE) clearClipboard(); // Se non ha funzionato rimuovi il (precedente) CF dagli appunti
#endif
	}

	// Oppure continua con il normale inserimento dati manuale
	else do {
		printf("\n\nCognome: ");
		if (!fgets(cogn, MAXP + 3, stdin)) break;
		printf("Nome: ");
		if (!fgets(nome, MAXP + 3, stdin)) break;
		printf("Inserire la data di nascita:\n");
		do {
			printf("Giorno di nascita (gg): ");
			if (!scanf("%d", &gg) || gg < 1 || gg > 31) printf("ERRORE: Giorno di nascita non valido.\n");
		} while (gg < 1 || gg > 31);
		do {
			printf("Mese di nascita (mm): ");
			if (!scanf("%d", &mm) || mm < 1 || mm > 12) printf("ERRORE: Mese di nascita non valido.\n");
		} while(mm < 1 || mm > 12);
		do {
			printf("Anno di nascita (aaaa): ");
			if (!scanf("%d", &aaaa) || aaaa < 0) printf("ERRORE: Anno di nascita non valido.\n");
		} while (aaaa < 0);
		getchar();
		do {
			printf("Sesso (M/F): ");
			if (!scanf("%c", &ch) || ( ch != 'm' && ch != 'M' && ch != 'f' && ch != 'F')) printf("ERRORE: Inserire solo il carattere 'M' o 'F'.\n");
			getchar();
		} while (ch != 'm' && ch != 'M' && ch != 'f' && ch != 'F');
		comune luogoNascita = NULL;
		do {
			printf("Luogo di nascita: ");
			if (!fgets(nomeComune, MAXP+3, stdin)) printf("ERRORE: nel leggere il luogo di nascita.\n");
			else {
				luogoNascita = ricerca(nomeComune);
				if (luogoNascita == NULL) printf("ERRORE: Comune o stato estero non trovato.\n");
			}
		} while (luogoNascita == NULL);
		codfisc = cf(cogn, nome, gg, mm, aaaa, ch, luogoNascita);
		printf("Codice fiscale: %s\n\n", codfisc);
#ifdef _WIN32
		copyToClipboard(codfisc);
#endif
		retVal = EXIT_SUCCESS;
		do {
			printf("Vuoi calcolarne un altro?  (S/N): ");
			if (!scanf("%c", &ch) || ( ch != 's' && ch != 'S' && ch != 'n' && ch != 'N')) printf("ERRORE: Inserire solo il carattere 'S' o 'N'.\n");
			getchar();
			if (ch == 's' && ch == 'S') retVal = EXIT_FAILURE;
		} while (ch != 's' && ch != 'S' && ch != 'n' && ch != 'N');
	} while (ch != 'n' && ch != 'N');

	pulisci();
	return retVal;
}
