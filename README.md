# CodiceFiscale
Software dimostrativo per calcolare codici fiscali italiani.

## Intento
Questo programma era nato per dimostrare l'uso di liked-list e relativa funzione di hash in C per ritrovare dati.  
In questo caso si gestisce L'elenco, con piu' di 10000 comuni italiani e stati esteri; ad ognuno dei quali e' assegnato un codice identificativo, usato appunto nel codice fiscale per indicare il luogo di nascita.

## Attenzione!!!
Oggigiorno sarebbe ovviamente meglio usare le funzioni hashmap gia' disponibili nella C++ standard library.  
Questo programma usava i caratteri accentati usando la codifica ASCII estesa (da 128 a 255); ovviamente in UTF-8 non funzionano piu'.  
Quindi questo software, al momento, e' utilizzabile solo omettendo tutti gli accenti dai nomi, cognomi e dai nomi dei luoghi di nascita!

## Variante a linea di comando
La versione a linea di comando *cf.exe* richiede i sorgenti nella cartella _src_ e puo' funzionare in due modi:
* ricevendo i dati tramite argomenti sulla line di comando
* oppure chiedendo all'utente di inserire i dati

I parametri della persona a cui si vuole calcolare il codice fiscale possono essere passati come un singolo argomento nel modo seguente:  

    cf Rossi,Mario,15/05/1972,M,Roma  

Gli spazi sono ammessi nel cognome, nel nome e nel nome del luogo di nascita, non sono ammessi altri formati.  
Su Windows il codice fiscale sara' copiato anche negli appunti pronto per essere incollato laddove richiesto.  
Se non viene passato alcun parametro il programma chiedera' all'utente di inserire i dati.

## Variante con interfaccia grafica per Windows
L'eseguibile _CodiceFiscale.exe_ presenta un'interfaccia grafica Win32 in cui inserire i dati. Anche in questo caso, il codice fiscale appena calcolato, sara' copiato anche negli appunti pronto per essere incollato altrove.

## Compilazione su Linux
La variante a linea di comando di questo programma, puo' essere compilato su Linux grazie al Makefile in questo repository invocando:  
    make all

## Compilazione su Windows
Per semplicita' si consiglia l'uso della variante "Orwell C++" di Dev-C++ disponibile su SourceForge a questo link:  
https://sourceforge.net/projects/orwelldevcpp/  
Nella cartella _Windows_ sono disponibili i file di progetto per Dev-C++, sia per compilare la variante a linea di comando (_cf.dev_), che quella con interfaccia grafica (_CodiceFiscale.dev_).
