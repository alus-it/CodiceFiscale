/*============================================================================
 * Name        : main.cpp
 * Version     : 0.2
 * Since       : 2004
 * Author      : Alberto Realis-Luc <alberto.realisluc@gmail.com>
 * Web         : http://www.alus.it/pubs/CodiceFiscale
 * Copyright   : (C) 2004 Alberto Realis-Luc
 * License     : GNU GPL v2
 * Description : Win32 application to calculate Italian fiscal codes
 *============================================================================*/

//TODO:
	//reset CF se si cambia qualcosa

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include "resource.h"
#include "cf.h"

#define ID_L_NOME 1001
#define ID_L_COGN 1002
#define ID_L_TITL 1003
#define ID_L_DATA 1004
#define ID_L_SESS 1005
#define ID_L_COMN 1006
#define ID_L_CODF 1007
#define ID_NOME 1012
#define	ID_COGN 1011
#define ID_GIOR 1013
#define ID_MESE 1014
#define ID_ANNO 1015
#define ID_COMN 1016
#define ID_CODF 1017
#define ID_MASC 1018
#define ID_FEMM 1019
#define ID_CALC 1020
#define ID_RSET 1021
#define ID_HELP 1022

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	const char *Mesi[]={"Gennaio","Febbraio","Marzo","Aprile","Maggio","Giugno","Luglio","Agosto","Settembre","Ottobre","Novembre","Dicembre"};
	const short giorniMesi[]={31 ,29        ,31     ,30      ,31      ,30      ,31      ,31      ,30         ,31       ,30        ,31};
	
	static HWND nomeLabel;
	static HWND cognLabel;
	static HWND titlLabel;
	static HWND dataLabel;
	static HWND sessLabel;
	static HWND comnLabel;
	static HWND codfLabel;
	static HWND	hEditCogn;
	static HWND	hEditNome;
	static HWND hComboDay;
	static HWND hCmbMonth;
	static HWND hEditYear;
	static HWND RButtMasc;
	static HWND RButtFemm;
	static HWND hEditComn;
	static HWND hEditCodF;
	static HWND calcButtn;
	static HWND rsetButtn;
	static HWND helpButtn;
	//static LRESULT res = 0;
	
	switch(Message) {
		case WM_CREATE: {
			if(carica((char*)"comuni.txt")==0) {
				MessageBox(hwnd,"File 'comuni.txt' non trovato!","Errore",MB_OK|MB_ICONSTOP);
				DestroyWindow(hwnd);
				break;
			}
			
			titlLabel = CreateWindow("STATIC", "CALCOLO CODICE FISCALE", WS_CHILD | WS_VISIBLE, 40, 5, 200, 30, hwnd, (HMENU)ID_L_TITL, GetModuleHandle(NULL), NULL);
			cognLabel = CreateWindow("STATIC", "Cognome:", WS_CHILD | WS_VISIBLE, 5, 30, 120, 30, hwnd, (HMENU)ID_L_COGN, GetModuleHandle(NULL), NULL);
			hEditCogn = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT", 0, WS_VISIBLE  | WS_CHILD | ES_LEFT | WS_TABSTOP, 125, 30, 190, 20, hwnd, (HMENU)ID_COGN, GetModuleHandle(NULL), NULL);
			nomeLabel = CreateWindow("STATIC", "Nome:", WS_CHILD | WS_VISIBLE, 5, 60, 120, 30, hwnd, (HMENU)ID_L_NOME, GetModuleHandle(NULL), NULL);
			hEditNome = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT", 0, WS_VISIBLE  | WS_CHILD | ES_LEFT | WS_TABSTOP, 125, 60, 190, 20, hwnd, (HMENU)ID_NOME, GetModuleHandle(NULL), NULL);
			dataLabel = CreateWindow("STATIC", "Data di nascita:", WS_CHILD | WS_VISIBLE, 5, 90, 120, 30, hwnd, (HMENU)ID_L_DATA, GetModuleHandle(NULL), NULL);
			hComboDay = CreateWindow("COMBOBOX","NULL",WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST,125,90,45,300,hwnd,(HMENU)ID_GIOR,GetModuleHandle(NULL), NULL);
			hCmbMonth = CreateWindow("COMBOBOX","NULL",WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST,175,90,95,300,hwnd,(HMENU)ID_MESE,GetModuleHandle(NULL), NULL);
			hEditYear = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT", 0, WS_VISIBLE  | WS_CHILD | ES_LEFT | ES_NUMBER | WS_TABSTOP,275,90,40,22, hwnd,(HMENU)ID_ANNO,GetModuleHandle(NULL), NULL);
			sessLabel = CreateWindow("STATIC", "Sesso:", WS_CHILD | WS_VISIBLE, 5, 120, 120, 30, hwnd, (HMENU)ID_L_SESS, GetModuleHandle(NULL), NULL);
			RButtMasc = CreateWindow("BUTTON","Maschile", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 125, 120, 80, 30, hwnd, (HMENU)ID_MASC,GetModuleHandle(NULL), NULL);
			RButtFemm = CreateWindow("BUTTON","Femminile",WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 220, 120, 90, 30, hwnd, (HMENU)ID_FEMM,GetModuleHandle(NULL), NULL);
			comnLabel = CreateWindow("STATIC", "Luogo di nascita:", WS_CHILD | WS_VISIBLE, 5, 150, 120, 30, hwnd, (HMENU)ID_L_COMN, GetModuleHandle(NULL), NULL);
			hEditComn = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT", 0, WS_VISIBLE  | WS_CHILD | ES_LEFT | WS_TABSTOP, 125, 150, 190, 20, hwnd, (HMENU)ID_COMN, GetModuleHandle(NULL), NULL);
			codfLabel = CreateWindow("STATIC", "Codice Fiscale:", WS_CHILD | WS_VISIBLE, 5, 180, 120, 30, hwnd, (HMENU)ID_L_CODF, GetModuleHandle(NULL), NULL);
			hEditCodF = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT", 0, WS_VISIBLE | WS_CHILD | ES_LEFT | ES_READONLY, 125, 180, 190, 20, hwnd, (HMENU)ID_CODF, GetModuleHandle(NULL), NULL);
			calcButtn = CreateWindow("BUTTON", "CALCOLA", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,125,210,190,30,hwnd, (HMENU)ID_CALC, GetModuleHandle(NULL), NULL);
			rsetButtn = CreateWindow("BUTTON","Reimposta", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  5,210,110,30,hwnd, (HMENU)ID_RSET, GetModuleHandle(NULL), NULL);
			rsetButtn = CreateWindow("BUTTON","Aiuto", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  260,5,55,20,hwnd, (HMENU)ID_HELP, GetModuleHandle(NULL), NULL);
			
			for(int i=1; i<=31; i++) {
				char buf[3];
				sprintf(buf,"%d",i);
				SendMessage(hComboDay,CB_ADDSTRING,0,reinterpret_cast<LPARAM>((LPCTSTR)buf));
			} 
			
			for(int i=0; i<12; i++) SendMessage(hCmbMonth,CB_ADDSTRING,0,reinterpret_cast<LPARAM>((LPCTSTR)Mesi[i]));
		
			break;
		}
	
		case WM_CLOSE: { /* trap the WM_CLOSE (clicking X) message, and actually tell the window to close */
			pulisci();
			DestroyWindow(titlLabel);
			DestroyWindow(cognLabel);
			DestroyWindow(hEditCogn);
			DestroyWindow(nomeLabel);
			DestroyWindow(hEditNome);
			DestroyWindow(dataLabel);
			DestroyWindow(hComboDay);
			DestroyWindow(hCmbMonth);
			DestroyWindow(hEditYear);
			DestroyWindow(RButtMasc);
			DestroyWindow(RButtFemm);
			DestroyWindow(sessLabel);
			DestroyWindow(comnLabel);
			DestroyWindow(codfLabel);
			DestroyWindow(hEditComn);
			DestroyWindow(hEditCodF);
			DestroyWindow(calcButtn);
			DestroyWindow(rsetButtn);
			DestroyWindow(helpButtn);
			DestroyWindow(hwnd);
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		case WM_COMMAND: {
    		//wmId    = LOWORD(wParam);
    		//wmEvent = HIWORD(wParam);
			switch (LOWORD(wParam)) {
				case ID_MASC: {
                    if(HIWORD(wParam)==BN_CLICKED) {
						if(SendDlgItemMessage(hwnd, ID_MASC, BM_GETCHECK, 0, 0) == 0) {
                            SendDlgItemMessage(hwnd, ID_MASC, BM_SETCHECK, 1, 0);
                            SendDlgItemMessage(hwnd, ID_FEMM, BM_SETCHECK, 0, 0);
                        } 
                    }
					break;
				}
                case ID_FEMM: {
					if(HIWORD(wParam)==BN_CLICKED) {
                        if (SendDlgItemMessage(hwnd, ID_FEMM, BM_GETCHECK, 0, 0) == 0) {
                            SendDlgItemMessage(hwnd, ID_FEMM, BM_SETCHECK, 1, 0);
                            SendDlgItemMessage(hwnd, ID_MASC, BM_SETCHECK, 0, 0);
                        } 
                    }
					break;
				}
				case ID_RSET: {
					SetDlgItemText(hwnd, ID_COGN, "");
					SetDlgItemText(hwnd, ID_NOME, "");
					SendMessage(hComboDay, CB_SETCURSEL, -1, 0);
					SendMessage(hCmbMonth, CB_SETCURSEL, -1, 0);
					SetDlgItemText(hwnd, ID_ANNO, "");
					SendDlgItemMessage(hwnd, ID_MASC, BM_SETCHECK, 0, 0);
					SendDlgItemMessage(hwnd, ID_FEMM, BM_SETCHECK, 0, 0);
					SetDlgItemText(hwnd, ID_COMN, "");
					SetDlgItemText(hwnd, ID_CODF, "");
					break;
				}
				case ID_CALC: {
					char *cogn;
					int len = SendMessage(hEditCogn, WM_GETTEXTLENGTH, 0, 0);
					if(len>0) {
						cogn = new char[len];
    					SendMessage(hEditCogn, WM_GETTEXT, (WPARAM)len+1, (LPARAM)cogn);
					} else {
						MessageBox(hwnd,"Nessun cognome inserito!","Attenzione",MB_OK|MB_ICONWARNING);
						break;
					}
					char *nome;
    				len = SendMessage(hEditNome, WM_GETTEXTLENGTH, 0, 0);
					if(len>0) {
						nome = new char[len];
    					SendMessage(hEditNome, WM_GETTEXT, (WPARAM)len+1, (LPARAM)nome);
					} else {
						MessageBox(hwnd,"Nessun nome inserito!","Attenzione",MB_OK|MB_ICONWARNING);
						break;
					}
					int gg = SendMessage(hComboDay,CB_GETCURSEL, 0,0)+1;
					if(gg==0) {
						MessageBox(hwnd,"Nessun giorno di nascita inserito!","Attenzione",MB_OK|MB_ICONWARNING);
						break;
					}
					int mm = SendMessage(hCmbMonth,CB_GETCURSEL, 0,0)+1;
					if(mm==0) {
						MessageBox(hwnd,"Nessun mese di nascita inserito!","Attenzione",MB_OK|MB_ICONWARNING);
						break;
					}
					if(gg>giorniMesi[mm-1]) {
						char buffer[40];
						sprintf(buffer,"%s non può avere %d giorni!",Mesi[mm-1],gg);
						MessageBox(hwnd,buffer,"Attenzione",MB_OK|MB_ICONWARNING);
						SendMessage(hComboDay, CB_SETCURSEL, gg-2, 0);
						break;
					}
					int aaaa;
					len = SendMessage(hEditYear, WM_GETTEXTLENGTH, 0, 0);
					if(len>0) {
						char* buffer = new char[len];
    					SendMessage(hEditYear, WM_GETTEXT, (WPARAM)len+1, (LPARAM)buffer);
    					aaaa=atoi(buffer);
					} else {
						MessageBox(hwnd,"Nessun anno di nscita inserito!","Attenzione",MB_OK|MB_ICONWARNING);
						break;
					}
					if(mm==2 && gg==29 && !(aaaa%4==0 && (aaaa%100!=0 || aaaa%400==0))) {
						MessageBox(hwnd,"L'anno specificato non è bisestile.\nQuindi febbraio non può avere 29 giorni!","Attenzione",MB_OK|MB_ICONWARNING);
						SendMessage(hComboDay, CB_SETCURSEL, gg-2, 0);
						break;
					}
					char sex;
					if(SendDlgItemMessage(hwnd,ID_MASC,BM_GETCHECK,0,0)==0 && SendDlgItemMessage(hwnd,ID_FEMM,BM_GETCHECK,0,0)==0) {
						MessageBox(hwnd,"Sesso non specificato!","Attenzione",MB_OK|MB_ICONWARNING);
						break;
					} else {
						if(SendDlgItemMessage(hwnd,ID_FEMM,BM_GETCHECK,0,0)==0) sex='m';
						else sex='f';
					}
					char *comn;
    				len = SendMessage(hEditComn, WM_GETTEXTLENGTH, 0, 0);
					if(len>0) {
						comn = new char[len];
    					SendMessage(hEditComn, WM_GETTEXT, (WPARAM)len+1, (LPARAM)comn);
					} else {
						MessageBox(hwnd,"Nessun luogo di nascita inserito!","Attenzione",MB_OK|MB_ICONWARNING);
						break;
					}
					comune luogoNascita=ricerca(comn);
					if(luogoNascita==NULL) {
						MessageBox(hwnd,"Luogo di nascita non trovato!","Attenzione",MB_OK|MB_ICONWARNING);
						break;
					} else {
						char *codfisc=cf(cogn,nome,gg,mm,aaaa,sex,luogoNascita);
						SetDlgItemText(hwnd, ID_CODF, codfisc);
						HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,17);
						memcpy(GlobalLock(hMem),codfisc,17);
						GlobalUnlock(hMem);
						OpenClipboard(0);
						EmptyClipboard();
						SetClipboardData(CF_TEXT, hMem);
						CloseClipboard();
					}
                    break;
                }
                case ID_HELP: {
					ShellExecute(hwnd, "open", "http://www.alus.it/pubs/CodiceFiscale",NULL, NULL, SW_SHOWNORMAL);
					break;
				}
			}
			default:
				return DefWindowProc(hwnd, Message, wParam, lParam);
			
		}
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG Msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW/*+1*/);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MYICON)); // Load The Icon
	wc.hIconSm		 = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MYICON));

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","CodiceFiscale",WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		330, /* width */
		280, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*  This is the heart of our program where all input is processed and sent to WndProc.
		Note that GetMessage blocks code flow until it receives something, so this loop will not produre unreasonably CPU usage */
	while(GetMessage(&Msg, NULL, 0, 0) > 0) { /* If no error is received... */
		if (!IsDialogMessage(hwnd, &Msg)) { /* this is to make it usabe with the TAB button like in a dialog*/
			TranslateMessage(&Msg); /* Translate keycodes to chars if present */
			DispatchMessage(&Msg); /* Send it to WndProc */
		}	
	}
	return Msg.wParam;
}

