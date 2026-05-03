/*============================================================================
 * Name        : cf.h
 * Version     : 1.5
 * Since       : 2004
 * Author      : Alberto Realis-Luc <alberto.realisluc@gmail.com>
 * Web         : http://www.alus.it/pubs/CodiceFiscale
 * Copyright   : (C) 2004 Alberto Realis-Luc
 * License     : GNU GPL v2
 * Description : Header of cf module to generate Italian fiscal codes
 *============================================================================*/

#include <stdio.h>
#include <string.h>

#define MAXP 30 /*numero massimo caratteri del comune*/

typedef struct comunale {
	const char *nome;
	char alfa;
	int num;
	struct comunale *nx;
} *comune;

int carica(const char *filecomuni);
comune ricerca(const char *nomeComune);
char *cf(const char *cog, const char *nome, int gg, int mm, int aaaa, char sex, comune comuneNascita);
void pulisci(void);
