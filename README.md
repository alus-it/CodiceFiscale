# CodiceFiscale
Software dimostrativo per calcolare codici fiscali italiani.

## Intento
Questo programma era nato per dimostrare l'uso di liked-list e relativa funzione di hash in C per ritrovare dati.  
In questo caso si gestisce L'elenco, con piu' di 10000 comuni italiani e stati esteri; ad ognuno dei quali e' assegnato un codice identificativo, usato appunto nel codice fiscale per indicare il luogo di nascita.

## Attenzione!!!
Sarebbe ovviamente meglio usare le funzioni hashmap gia' disponibili nella standard library.  
Questo programma usava i caratteri accentati usando la codifica ISO 8859-1: (caratteri ASCII da 128 a 255); ovviamente in UTF-8 non funzionano piu'.  
Quindi questo software, al momento, e' utilizzabile solo omettendo tutti gli accenti dai nomi, cognomi e dai nomi dei luoghi di nascita!

## Variante a linea di comando
La variante a linea di comando *cf* (o su Windows *cf.exe*) puo' funzionare in tre modi:
* senza argomenti: usando il file *comuni.txt* nella stessa directory come elenco luoghi di nascita
* specificando il percorso del file con l'elenco dei luoghi di nascita da usare come unico argomento
* specificando il file del luoghi di nascita come primo argomento ed i dati della persona come argomenti successivi

Esempi:  
* `./cf`  
* `./cf ./dati/elencoComuni.txt`  
* `./cf comuni.txt Rossi,Mario,15/05/1972,M,Roma`  

Gli spazi sono ammessi nel cognome, nel nome e nel nome del luogo di nascita, non sono ammessi altri formati. Non usare caratteri accentati, omettere gli accenti.  
Su Windows il codice fiscale sara' copiato anche negli appunti pronto per essere incollato laddove richiesto.  

## Variante con interfaccia grafica per Windows
L'eseguibile _CodiceFiscale.exe_ presenta un'interfaccia grafica Win32 in cui inserire i dati. Anche in questo caso, il codice fiscale appena calcolato, sara' copiato anche negli appunti pronto per essere incollato altrove.

## Compilazione

### Su Linux
La variante a linea di comando di questo programma richiede i sorgenti nella cartella _src_ e puo' essere compilato su Linux grazie al Makefile in questo repository invocando:  
    `make all`

### Su Windows
Per semplicita' si consiglia l'uso della variante "_Orwell C++_" di Dev-C++ disponibile su SourceForge a questo link:  
https://sourceforge.net/projects/orwelldevcpp/  
Nella cartella _Windows_ sono disponibili i file di progetto per Dev-C++, sia per compilare la variante a linea di comando (_cf.dev_), che quella con interfaccia grafica (_CodiceFiscale.dev_).
