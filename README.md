# CodiceFiscale
Software dimostrativo per calcolare codici fiscali italiani.

## Intento
Questo programma era nato per dimostrare l'uso di liked-list e relativa funzione di hash in C per ritrovare dati.  
In questo caso si gestisce L'elenco, con piu' di 8000 comuni italiani; ad ognuno dei quali e' assegnato un codice, usato appunto nel codice fiscale per indicare il comune di nascita.

## Attenzione!!!
Oggigiorno sarebbe ovviamente meglio usare le funzioni hashmap gia' disponibili nella C++ standard library.  
Inoltre i caratteri con codice ASCII da 128 a 255 (dunque tutti i caratteri accentati) non funzionano piu' sulle moderne versioni di Windows come era inteso inizialmente in questo programma.  
Dunque su Windows questo software e' utilizzabile solo omettendo tutti gli accenti dai nomi, cognomi e nomi dei luoghi di nascita!

## Compilazione su Linux
Questo programma era inizialmente nato su Linux dunque non dovrebbero esserci particolari problemi anche se al momento non vie e' disponibile un Makefile.  

## Compilazione su Windows
Questo progetto e' ancora in grado di compilare su Windows 11 utilizzando Dev-C++.
Si consiglia l'uso della variante "Orwell C++" disponibile su SourceForge a questo link:  
https://sourceforge.net/projects/orwelldevcpp/  
Nel repository sono disponibili i file di progetto per Dev-C++.

## Versione a linea di comando
La versione a linea di comando *cf.exe* richiede i sorgenti: _cf.h_, _cf.c_ e _main.c_ e puo' funzionare in due modi:
* ricevendo i dati tramite argomenti sulla line di comando
* oppure chiedendo all'utente di inserire i dati

I parametri della persona a cui si vuole calcolare il codice fiscale possono essere passati come un singolo argomento (senza spazi) nel modo seguente:  

    cf Rossi,Mario,15/05/1972,M,Roma  

Non ci devono essere spazi tra i dati, non sono ammessi altri formati.
Su Windows il codice fiscale sara' copiato anche negli appunti pronto per essere incollato.  

Se non viene passato alcun parametro il programma chiedera' all'utente di inserire i dati.

## Versione con interfaccia grafica per Windows
La versione Win32 _CodiceFiscale.exe_ richiede i sorgenti: _cf.h_, _cf.c_ e _CodiceFiscale.cpp_ e presenta un'interfaccia grafica in cui inserire i dati. Anche in questo caso, il codice fiscale appena calcolato, sara' copiato anche negli appunti pronto per essere incollato altrove.  
