# CodiceFiscale
Software dimostrativo per calcolare codici fiscali italiani.

## Intento
Questo programma era nato per dimostrare l'uso di liked-list e relativa funzione di hash in C per ritrovare dati.  
In questo caso si gestisce L'elenco, con piu' di 10000 comuni italiani e stati esteri; ad ognuno dei quali e' assegnato un codice identificativo, usato appunto nel codice fiscale per indicare il luogo di nascita.

## Attenzione!!!
Oggigiorno sarebbe ovviamente meglio usare le funzioni hashmap gia' disponibili nella C++ standard library.  
Questo programma usava i caratteri accentati usando la codifica ASCII estesa (da 128 a 255); ovviamente in UTF-8 non funzionano piu.  
Quindi questo software, al momento, e' utilizzabile solo omettendo tutti gli accenti dai nomi, cognomi e nomi dei luoghi di nascita!

## Versione a linea di comando
La versione a linea di comando *cf.exe* richiede i sorgenti: _cf.h_, _cf.c_ e _main.c_ e puo' funzionare in due modi:
* ricevendo i dati tramite argomenti sulla line di comando
* oppure chiedendo all'utente di inserire i dati

I parametri della persona a cui si vuole calcolare il codice fiscale possono essere passati come un singolo argomento nel modo seguente:  

    cf Rossi,Mario,15/05/1972,M,Roma  

Gli spazi sono ammessi nel cognome, nel nome e nel nome del luogo di nascita, non sono ammessi altri formati.  
Su Windows il codice fiscale sara' copiato anche negli appunti pronto per essere incollato laddove richiesto.  
Se non viene passato alcun parametro il programma chiedera' all'utente di inserire i dati.

## Versione con interfaccia grafica per Windows
La versione Win32 _CodiceFiscale.exe_ con interfaccia grafica richiede i sorgenti: _cf.h_, _cf.c_ e _CodiceFiscale.cpp_ e presenta un'interfaccia grafica in cui inserire i dati. Anche in questo caso, il codice fiscale appena calcolato, sara' copiato anche negli appunti pronto per essere incollato altrove.  

## Compilazione su Linux
La versione a linea di comando di questo programma, inizialmente nato su Linux, puo essere compilato su Linux grazie al Makefile in questo repository invocando:  
    make all

## Compilazione su Windows
Questo progetto e' ancora in grado di compilare su Windows 11 utilizzando Dev-C++.
Si consiglia l'uso della variante "Orwell C++" disponibile su SourceForge a questo link:  
https://sourceforge.net/projects/orwelldevcpp/  
Nel repository sono disponibili i file di progetto per Dev-C++, sia per compilare la versione a linea di comando (_cf.dev_), che quella con interfaccia grafica (_CodiceFiscale.dev_).
