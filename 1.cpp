#include <windows.h>
#include "resource.h" 
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include<malloc.h>
HWND hwnd;
HWND hwnd2;
const char g_szClassName[] = "FereastraMea";
HDC hDC;
int p=0, q=0, l=0;
HBRUSH newbrush;
PAINTSTRUCT Ps;
HPEN hPen;
int ok = 0;
int ok2,ok3;

typedef struct tip_nod
{
	int cheie;
	struct tip_nod *stg;
	struct tip_nod *dr;
	struct tip_nod *parinte;

	int h_stanga;
	int h_dreapta;
}TIP_NOD;

TIP_NOD *rad=NULL;
TIP_NOD *aux;

int print=1;
int idx=0;
double timp_inserare, timp_cautare;

void display_out_cautare(TIP_NOD *nod)
{

	
	SelectObject(hDC,newbrush);
	Ellipse(hDC, 648, 80, 698, 130);		// nod parinte
	MoveToEx( hDC,673, 130, NULL);
	LineTo(hDC, 673, 230);

	MoveToEx( hDC,673, 280, NULL);		
	LineTo(hDC, 823, 380);
	Ellipse(hDC, 798, 355, 848, 405);	// fiu drept

	MoveToEx( hDC,673, 280, NULL);
	LineTo(hDC, 548, 380);
	Ellipse(hDC, 543, 355, 593, 405);	// fiu stang

	newbrush = CreateSolidBrush(RGB(255,0,0));	// nod cheie
	SelectObject(hDC, newbrush);		
	Ellipse(hDC,646, 228, 700, 282);
	DeleteObject(newbrush);

	newbrush = CreateSolidBrush(RGB(255,255,255));	
	SelectObject(hDC, newbrush);		
	Ellipse(hDC,648, 230, 698, 280);
	DeleteObject(newbrush);

	char s[5];		// printare nod cheie
	itoa(nod->cheie,s,10);
	TextOut(hDC,664,248,s,strlen(s));

	if(nod->parinte)	// printare parinte
	{
		itoa(nod->parinte->cheie,s,10);
		TextOut(hDC,664,98,s,strlen(s));
	}
	else
	{
		strcpy(s,"---");
		TextOut(hDC,664,98,s,strlen(s));
	}

	if(nod->stg)		// printare copil stanga
	{
		itoa(nod->stg->cheie,s,10);
		TextOut(hDC,561,373,s,strlen(s));
	}
	else
	{
		strcpy(s,"---");
		TextOut(hDC,561,373,s,strlen(s));
	}

	if(nod->dr)		// printare copil dreapta
	{
		itoa(nod->dr->cheie,s,10);
		TextOut(hDC,816,373,s,strlen(s));
	}
	else
	{
		strcpy(s,"---");
		TextOut(hDC,816,373,s,strlen(s));
	}
	
}

void display_out(tip_nod *rad,int p,int q,int l,int level)
{
	if(rad==NULL)
		return;

	if(level>4)
	{
		print=0;
		return;
	}
	if(rad!=NULL)
	{
			SelectObject(hDC,newbrush);
			Ellipse(hDC, p, q, p+50, q+50);
			DeleteObject(newbrush);
			char s[5];
			itoa(rad->cheie,s,10);
			TextOut(hDC,p+18,q+18,s,strlen(s));
	}
	else return;
	if(rad->stg)
	{
		MoveToEx(hDC, p+25, q+50, NULL);
		LineTo(hDC, p-150+l, q+100);
		display_out(rad->stg,p-150+l-25,q+100,l+90,level+1);
	}
	if(rad->dr)
	{
		MoveToEx(hDC, p+25, q+50, NULL);
		LineTo(hDC, p+200-l, q+100);
		display_out(rad->dr,p+200-l-25,q+100,l+90,level+1);
	}
}
void preordine(TIP_NOD *p, int nivel)
{
  int i;
  if (p!=0){
         	for(i=0;i<=nivel;i++) printf("  ");
         	printf("%2d\n",p->cheie);
         	preordine(p->stg,nivel+1);
         	preordine(p->dr,nivel+1);
         }
}
void inordine(TIP_NOD *p, int nivel)
{
  int i;
  if (p!=0){
         	inordine(p->stg,nivel+1);
         	for(i=0;i<=nivel;i++) printf("  ");
         	printf("%2d\n",p->cheie);
         	inordine(p->dr,nivel+1);
         }
}
void postordine(TIP_NOD *p, int nivel)
{
  int i;
  if (p!=0){
         	postordine(p->stg,nivel+1);
         	postordine(p->dr,nivel+1);
         	for(i=0;i<=nivel;i++) printf("  ");
         	printf("%2d\n",p->cheie);
         }
}

 TIP_NOD * cautare(TIP_NOD *rad, int key)
{
  TIP_NOD *p;
 
  if(rad==NULL) 
	  return NULL;/*arborele este vid */

  p=rad;
  while(p != 0)
            {
         	 if(p->cheie == key) return p;/* s-a gasit nodul */
         	 else if(key < p->cheie) p=p->stg;
         	        else p=p->dr;
         	}
  return 0; /* nu exista nod de cheie key */
   }

 void retus_inaltime(TIP_NOD *nod)
 {
	 if(nod==NULL)
		 return;

	 while(nod->parinte!=NULL)
	 {
		 if(nod->parinte->stg==nod)
		 {
			 int h=max(nod->h_dreapta,nod->h_stanga)+1;
			 if(nod->parinte->h_stanga != h)
				 nod->parinte->h_stanga = h;
			 else
				 return;
		 }
		 else
		 {
			 int h=max(nod->h_dreapta,nod->h_stanga)+1;
			 if(nod->parinte->h_dreapta != h)
				 nod->parinte->h_dreapta = h;
			 else
				 return;
		 }

		 nod=nod->parinte;
	 }
 }

 void rotire_stanga(TIP_NOD *aux)
{
	if(aux==rad)
		rad=aux->stg;

	if(aux->parinte)
	{
			if(aux->parinte->stg==aux)
				aux->parinte->stg=aux->stg;
			else
				aux->parinte->dr=aux->stg;
	}

	aux->stg->parinte=aux->parinte;

	aux->parinte=aux->stg;

	aux->stg=aux->stg->dr;
	
	if(aux->stg)
		aux->stg->parinte=aux;

	aux->parinte->dr=aux;

	aux->h_stanga=aux->parinte->h_dreapta;
	if(aux->h_stanga>aux->h_dreapta)
		aux->parinte->h_dreapta=aux->h_stanga+1;
	else
		aux->parinte->h_dreapta=aux->h_dreapta+1;

	retus_inaltime(aux->parinte);
}

void rotire_preliminara_stanga(TIP_NOD *aux)
{
	aux->dr->parinte=aux->parinte;
	aux->parinte->stg=aux->dr;
	
	aux->dr=aux->dr->stg;
	if(aux->dr)
		aux->dr->parinte=aux;
	
	aux->parinte->stg->stg=aux;
	aux->parinte=aux->parinte->stg;

	aux->h_dreapta=aux->parinte->h_dreapta;
	
	if(aux->h_stanga>aux->h_dreapta)
		aux->parinte->h_stanga=aux->h_stanga+1;
	else
		aux->parinte->h_stanga=aux->h_dreapta+1;
	
	if(aux->parinte->h_stanga>aux->parinte->h_dreapta)
		aux->parinte->parinte->h_stanga=aux->parinte->h_stanga+1;
	else
		aux->parinte->parinte->h_stanga=aux->parinte->h_dreapta+1;
	
}

void rotire_dreapta(TIP_NOD *aux)
{
	if(aux==rad)
		rad=aux->dr;

	if(aux->parinte)
	{
			if(aux->parinte->dr==aux)
				aux->parinte->dr=aux->dr;
			else
				aux->parinte->stg=aux->dr;
	}

	aux->dr->parinte=aux->parinte;

	aux->parinte=aux->dr;

	aux->dr=aux->dr->stg;
	
	if(aux->dr)
		aux->dr->parinte=aux;

	aux->parinte->stg=aux;

	aux->h_dreapta=aux->parinte->h_stanga;
	if(aux->h_dreapta>aux->h_stanga)
		aux->parinte->h_stanga=aux->h_dreapta+1;
	else
		aux->parinte->h_stanga=aux->h_stanga+1;

	retus_inaltime(aux->parinte);
}

void rotire_preliminara_dreapta(TIP_NOD *aux)
{
	aux->stg->parinte=aux->parinte;
	aux->parinte->dr=aux->stg;
	
	aux->stg=aux->stg->dr;
	if(aux->stg)
		aux->stg->parinte=aux;
	
	aux->parinte->dr->dr=aux;
	aux->parinte=aux->parinte->dr;

	aux->h_stanga=aux->parinte->h_stanga;
	
	if(aux->h_dreapta>aux->h_stanga)
		aux->parinte->h_dreapta=aux->h_dreapta+1;
	else
		aux->parinte->h_dreapta=aux->h_stanga+1;
	
	if(aux->parinte->h_dreapta>aux->parinte->h_stanga)
		aux->parinte->parinte->h_dreapta=aux->parinte->h_dreapta+1;
	else
		aux->parinte->parinte->h_dreapta=aux->parinte->h_stanga+1;
	
}

void echilibrare_arbore(TIP_NOD *aux)
{
	if((aux->h_dreapta-aux->h_stanga)==2)
	{
		if((aux->dr->h_dreapta-aux->dr->h_stanga)==-1)
			rotire_preliminara_dreapta(aux->dr);
		rotire_dreapta(aux);
		return;	
	}

	if((aux->h_dreapta-aux->h_stanga)==-2)
	{
		if((aux->stg->h_stanga-aux->stg->h_dreapta)==-1)
			rotire_preliminara_stanga(aux->stg);
		rotire_stanga(aux);
		return;
	}

	if(aux->parinte==NULL)
			return;

	echilibrare_arbore(aux->parinte);
}

void inaltime_stergere(TIP_NOD *nod,bool s_d)
{
	if(nod==NULL)
		return;

	if(s_d)
	{
		int h;
		if(nod->dr==NULL)
			return;
		if(nod->dr->h_dreapta > nod->dr->h_stanga)
			h=nod->dr->h_dreapta +1 ;
		else
			h=nod->dr->h_stanga +1 ;

		if(nod->h_dreapta != h)
		{
			nod->h_dreapta = h;
			inaltime_stergere(nod->parinte,s_d);
		}
		else
			return;
	}
	else
	{
		int h;
		if(nod->stg==NULL)
			return;
		if(nod->stg->h_dreapta > nod->stg->h_stanga)
			h=nod->stg->h_dreapta +1 ;
		else
			h=nod->stg->h_stanga +1 ;

		if(nod->h_stanga != h)
		{
			nod->h_stanga = h;
			inaltime_stergere(nod->parinte,s_d);
		}
		else
			return;
	}
}

void stergere(TIP_NOD *nod)
 {
	 if(nod->dr==NULL && nod->stg==NULL)		// cazul in care nodul nu are copii
	 {
		 if(nod->parinte==NULL)
		 {
			rad=NULL;
			free(nod);
			return;
		 }
		 else
			 if(nod->parinte->stg==nod)
			 {
				 nod->parinte->stg=NULL;
				 nod->parinte->h_stanga--;
				 inaltime_stergere(nod->parinte->parinte,0);
			 }
			 else
			 {
				 nod->parinte->dr=NULL;
				 nod->parinte->h_dreapta--;
				 inaltime_stergere(nod->parinte->parinte,1);
			 }
		 
		 echilibrare_arbore(nod->parinte);
		 free(nod);
		 return;
	 }

	 if( nod->stg==NULL && nod->dr!=NULL )	// cazul in care are un singur copil ( in dreapta)
	 {
		 nod->dr->parinte=nod->parinte;

		 if(nod->parinte==NULL)
		 {
			 rad=nod->dr;
			 free(nod);
			 echilibrare_arbore(rad);
			 return;
		 }
		 		
		 if(nod->parinte->dr==nod)
		 {
			 nod->parinte->dr = nod->dr;
			 nod->parinte->h_dreapta = max(nod->dr->h_dreapta,nod->dr->h_stanga)+1;
			 inaltime_stergere(nod->parinte->parinte,1);
		 }
		 else
		 {
			 nod->parinte->stg = nod->dr;
			 nod->parinte->h_stanga = max(nod->dr->h_dreapta,nod->dr->h_stanga)+1;
		     inaltime_stergere(nod->parinte->parinte,0);
		 }
		
		 echilibrare_arbore(nod->parinte);
		 free(nod);
		 return;
	 }
	 else
		 if( nod->dr==NULL && nod->stg!=NULL)		// cazul in care are un singur copil ( in stanga)
		 {
			 nod->stg->parinte=nod->parinte;

			 if(nod->parinte==NULL)
			 {
				 rad=nod->stg;
				 free(nod);
				 echilibrare_arbore(rad);
				 return;
			 }

			 if(nod->parinte->dr==nod)
			 {
				 nod->parinte->dr = nod->stg;
				 nod->parinte->h_dreapta = max(nod->stg->h_dreapta,nod->stg->h_stanga)+1;
				 inaltime_stergere(nod->parinte->parinte,1);
			 }
			 else
			 {
				 nod->parinte->stg = nod->stg;
				 nod->parinte->h_stanga = max(nod->stg->h_dreapta,nod->stg->h_stanga)+1;
				 inaltime_stergere(nod->parinte->parinte,0);
			 }

			 echilibrare_arbore(nod->parinte);
			 free(nod);
			 return;
		 }
		 else                                  // cazul in care are 2 copii
		 {
			 if(nod->stg->dr==NULL)			// cazul in care copilul din stanga nu are copil in dreapta
			 {
				 nod->dr->parinte = nod->stg;
				 nod->stg->dr = nod->dr;
				 if(nod->dr->h_dreapta > nod->dr->h_stanga)
					 nod->stg->h_dreapta=nod->dr->h_dreapta+1;
				 else
					 nod->stg->h_dreapta=nod->dr->h_stanga+1;

				 nod->stg->parinte=nod->parinte;

				 if(nod->parinte==NULL)
				 {
					 rad=nod->stg;
					 free(nod);
					 echilibrare_arbore(rad);
					 return;
				 }

				 if(nod->parinte->dr==nod)
				 {
					 nod->parinte->dr=nod->stg;
					 nod->parinte->h_dreapta = max(nod->stg->h_dreapta,nod->stg->h_stanga)+1;
					inaltime_stergere(nod->parinte->parinte,1);
				 }
				 else
				 {
					 nod->parinte->stg=nod->stg;
					 nod->parinte->h_stanga = max(nod->stg->h_dreapta,nod->stg->h_stanga)+1;
					 inaltime_stergere(nod->parinte->parinte,0);
				 }

				 echilibrare_arbore(nod->stg);
				 free(nod);
				 return;
			 }		 
			 
			 if(nod->dr->stg==NULL)		// cazul in care copilul din dreapta nu are copil in stanga
			 {
				 nod->stg->parinte=nod->dr;
				 nod->dr->stg=nod->stg;
				 if(nod->stg->h_dreapta > nod->stg->h_stanga)
					 nod->dr->h_stanga = nod->stg->h_dreapta+1;
				 else
					 nod->dr->h_stanga = nod->stg->h_stanga+1;

				 nod->dr->parinte=nod->parinte;

				 if(nod->parinte==NULL)
				 {
					 rad=nod->dr;
					 free(nod);
					 echilibrare_arbore(rad);
					 return;
				 }

				 if(nod->parinte->dr==nod)
				 {
					 nod->parinte->dr=nod->dr;
					 nod->parinte->h_dreapta = max(nod->dr->h_dreapta,nod->dr->h_stanga)+1;
					 inaltime_stergere(nod->parinte->parinte,1);
				 }
				 else
				 {
					 nod->parinte->stg=nod->dr;
					 nod->parinte->h_stanga = max(nod->dr->h_dreapta,nod->dr->h_stanga)+1;
					 inaltime_stergere(nod->parinte->parinte,0);
				 }

				 echilibrare_arbore(nod->dr);
				 free(nod);
				 return;
			 }

			 TIP_NOD *aux=nod->dr;				// in acest caz luam cel mai din stanga copil al nodului din dreapta
			 while(aux->stg!=NULL)
				 aux=aux->stg;
			 TIP_NOD *aux2=aux->parinte;	// memoram parintele nodului gasit pentru a incepe echilibrarea de la acesta

			 int h;
			 if( aux->parinte->h_stanga > aux->parinte->h_dreapta )
				 h=nod->h_dreapta-1;
			 else
				 h=nod->h_dreapta;

			 aux->parinte->stg=aux->dr;
			 if(aux->dr!=NULL)
				 aux->dr->parinte=aux->parinte;
			 aux->parinte->h_stanga=aux->h_dreapta;

			 aux->stg=nod->stg;
			 aux->dr=nod->dr;
			 aux->h_stanga=nod->h_stanga;
			 aux->h_dreapta=h;

			 if(aux->dr==aux2)
				 aux2->parinte=aux;
			 if(aux->stg==aux2)
				 aux2->parinte=aux;

			 if(nod->parinte==NULL)
			 {
				 rad=aux;
				 aux->parinte=NULL;
			 }
			 else
			 {
				 aux->parinte=nod->parinte;
				 if( nod->parinte->stg == nod )
				 {
					 nod->parinte->stg = aux;
					 if(aux->h_dreapta > aux->h_stanga )
						 aux->parinte->h_stanga = aux->h_dreapta + 1;
					 else
						 aux->parinte->h_stanga = aux->h_stanga + 1;

					 inaltime_stergere(nod->parinte->parinte,0);
				 }
				 else
				 {
					 nod->parinte->dr = aux;
					 if(aux->h_dreapta > aux->h_stanga )
						 aux->parinte->h_dreapta = aux->h_dreapta + 1;
					 else
						 aux->parinte->h_dreapta = aux->h_stanga + 1;

					 inaltime_stergere(nod->parinte->parinte,1);
				 }
			 }

			 echilibrare_arbore(aux2);
			 free(nod);
			 return;
		}
 }

bool pregatire_stergere(TIP_NOD *rad,int key)
 {
	 while(rad!=NULL)
	 {
		 if(rad->cheie==key)
		 {
			 stergere(rad);
			 return 1;
		 }
		 else
			 if(rad->cheie>key)
				 rad=rad->stg;
			 else
				 rad=rad->dr;
	 }
	 return 0;
 }

 bool inserare_rec(TIP_NOD **rad,int key)
{
	if(cautare(*rad,key)!=NULL)
		return 0;
	

	TIP_NOD *nou=(TIP_NOD *)malloc(sizeof(TIP_NOD));
		nou->cheie=key; 		nou->parinte=NULL;
			nou->dr=NULL;		    nou->stg=NULL;
		nou->h_dreapta=0;		nou->h_stanga=0;


	if(*rad==NULL)
	{
		*rad=nou;
		return 1;
	}

	TIP_NOD *aux=*rad;

		while(1)
		{
			if(key<aux->cheie)
			{
				if(aux->stg==NULL)
				{
					aux->h_stanga++;
					aux->stg=nou;
					aux->stg->parinte=aux;
					if(aux->dr==NULL)
						retus_inaltime(aux);
					return 1;
				}
				else
					aux=aux->stg;
			}
			else
			{		
				if(aux->dr==NULL)
				{
					aux->dr=nou;
					aux->dr->parinte=aux;
					aux->h_dreapta++;
					if(aux->stg==NULL)
						retus_inaltime(aux);
					return 1;
				}
				else
					aux=aux->dr;
			}
		}
}

 void delete_grafic(HDC hdc, HWND hwnd)
{

	HDC hdc1=NULL;
	hdc1=GetDC(hwnd);
	HPEN Pen1=CreatePen(PS_SOLID,1,RGB(255,255,255));
	HBRUSH Brush1 = CreateSolidBrush(RGB(255,255,255));
	SelectObject(hdc1,Pen1);
	SelectObject(hdc1, Brush1);
	Rectangle(hdc1,335,599,550,100);
}

void desenare_grafic()
{
	HDC hdc=NULL;
	delete_grafic(hdc,hwnd);
	HPEN hPen = CreatePen(PS_SOLID,4,RGB(0,0,0));
	HBRUSH bBrush;
	hdc=hDC;
	
		SelectObject(hdc,hPen);

		char q[30];
			sprintf(q,"%f ",timp_inserare);
			strcat(q," s");
			TextOut(hdc,340,660,q,strlen(q));		// timpul inserarii
			strcpy(q,"Inserare");
			TextOut(hdc,340,70,q,strlen(q));

			sprintf(q,"%f ",timp_cautare);
			strcat(q," s");
			TextOut(hdc,470,660,q,strlen(q));
			strcpy(q,"Cautare");
			TextOut(hdc,470,70,q,strlen(q));

		double inaltime;		// reprezinta inaltimea graficului mai mic
		if(timp_inserare > timp_cautare)		// in acest caz timpul inserarii e mai mare
		{
			bBrush = CreateSolidBrush(RGB(255,0,0));
			SelectObject(hdc, bBrush);
			Rectangle(hdc,340,600,390,105);

			inaltime=(timp_cautare*495)/timp_inserare;

			bBrush = CreateSolidBrush(RGB(0,255,0));
			SelectObject(hdc, bBrush);
			Rectangle(hdc,470,600,520,600 - inaltime);
		}
		else		// in acest caz timpul cautarii este mai mare
		{
			bBrush = CreateSolidBrush(RGB(255,0,0));
			SelectObject(hdc, bBrush);
			Rectangle(hdc,470,600,520,105);

			inaltime=(timp_inserare*495)/timp_cautare;		

			bBrush = CreateSolidBrush(RGB(0,255,0));
			SelectObject(hdc, bBrush);
			Rectangle(hdc,340,600,390,600 - inaltime);
		}
}

void aleatoriu(int z)
{
//=============================== folosite la cronometrare ===============================//
	LARGE_INTEGER t1,t2;					
	LARGE_INTEGER tickPerSecond;	
	_int64   t_inserare=0, t_cautare=0;		
//=======================================================================================//
	int *vec=(int *)malloc(sizeof(int)*z);		// vectorul in care se memoreaza valoarile aleatorii



	for(int i=0; i<z; i++)		// se genereaza valorile
		vec[i]=rand();

		for(int i=0;i<z;i++)
		{
			 QueryPerformanceCounter(&t1);
			inserare_rec(&rad,vec[i]);
			 QueryPerformanceCounter(&t2);
			 t_inserare=t_inserare + (t2.QuadPart-t1.QuadPart);     
		}

	for(int i=0; i<z; i++)	// se cauta si se cronometreaza timpul de cautare
	{
		int x=rand()%z;

			 QueryPerformanceCounter(&t1);
			 cautare(rad,vec[x]);
			 QueryPerformanceCounter(&t2);
			 t_cautare=t_cautare + (t2.QuadPart-t1.QuadPart);     	
			
	}

	QueryPerformanceFrequency(&tickPerSecond);
	timp_inserare=(double)t_inserare/(1000*((double)tickPerSecond.QuadPart/1000)); 
	QueryPerformanceFrequency(&tickPerSecond);
	timp_cautare=(double)t_cautare/(1000*((double)tickPerSecond.QuadPart/1000));
}

BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName)
{
    HANDLE hFile;
    BOOL bSuccess = FALSE;

    hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if(hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwFileSize;

        dwFileSize = GetFileSize(hFile, NULL);
        if(dwFileSize != 0xFFFFFFFF)
        {
            LPSTR pszFileText;

            pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
            if(pszFileText != NULL)
            {
                DWORD dwRead;

                if(ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
                {
                    pszFileText[dwFileSize] = 0; // Adauga NULL in capat
                    if(SetWindowText(hEdit, pszFileText))
                        bSuccess = TRUE; // Merge
                }
                GlobalFree(pszFileText);
            }
        }
        CloseHandle(hFile);
    }
    return bSuccess;
}

BOOL CALLBACK DlgProc2(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	static HWND hList;

	switch(Message)
	{
		case WM_INITDIALOG:
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDC_INSERARE:
				{		
					BOOL bb;
					int z;
					char sir[3];
					sir[0] = NULL;
					z = GetDlgItemInt(hwnd, IDC_NUMBER, &bb, FALSE);
					if(!inserare_rec(&rad,z))
							MessageBox(hwnd, "Valoarea exista deja","Inserare", NULL );
					else
						echilibrare_arbore(cautare(rad,z));
					SetDlgItemText(hwnd, IDC_NUMBER, sir);
					char *P = new char;
					itoa(z, P, 10);
					UpdateWindow(hwnd2);
					ok=1;
					InvalidateRect(hwnd2, NULL, TRUE);
				}
				break;
			case IDC_EXIT:
				{
					EndDialog(hwnd, 0);
				}
				break;
			case IDC_BUTTON1:
				{
					BOOL bb;
					int z;
					char sir[3];
					sir[0] = NULL;
					SetDlgItemText(hwnd, IDC_NUMBER, sir);
					UpdateWindow(hwnd2);
					ok=1;
					InvalidateRect(hwnd2, NULL, TRUE);
				}
				break;
			case IDC_BUTTON2:
			{       
				    BOOL bb;
					int z;
					char sir[3];
					sir[0] = NULL;
					z = GetDlgItemInt(hwnd, IDC_EDIT1, &bb, FALSE);
					aleatoriu(z);
					ok3=1;
					SetDlgItemText(hwnd, IDC_NUMBER, sir);
					char *P = new char;
					itoa(z, P, 10);
					UpdateWindow(hwnd2);
					ok=1;
					InvalidateRect(hwnd2, NULL, TRUE);

					rad=NULL;
			}
			break;
			case IDC_STERGE:
				{
					BOOL bb;
					int z;		
					z = GetDlgItemInt(hwnd, IDC_NUMBER, &bb, FALSE);
						if ( cautare(rad, z) == NULL)
						{
							///*char sir[3];
							//sir[0] = NULL;
							//SetDlgItemText(hwnd,IDC_NUMBER,sir);*/
							//MessageBox(hwnd, "Valoare inexistenta!!!","Stergere", NULL );
							//break;
						}
						else
						{
							pregatire_stergere(rad,z);
							char *P = new char;
							itoa(z, P, 10);
							MessageBox(hwnd, "stergere cu succes","Stergere", NULL );
							char sir[3];
							sir[0] = NULL;
							SetDlgItemText(hwnd,IDC_NUMBER, sir);
							UpdateWindow(hwnd2);
							ok = 1;
							InvalidateRect(hwnd2, NULL, TRUE);	
						}
				}
				break;
			case IDC_CAUTA:
				{
					BOOL bb;
					int z;
					char sir[3];
					sir[0] = NULL;
					z = GetDlgItemInt(hwnd, IDC_NUMBER, &bb, FALSE);

					aux=cautare(rad,z);
					if(aux!=NULL)
					{
					ok2=1;
					SetDlgItemText(hwnd, IDC_NUMBER, sir);
					char *P = new char;
					itoa(z, P, 10);
					UpdateWindow(hwnd2);
					ok=1;
					InvalidateRect(hwnd2, NULL, TRUE);
					}
				}
				break;		
				case IDC_LIST:
					switch(HIWORD(wParam))
					{
						case LBN_SELCHANGE:
						{
							// numarul de noduri.

							HWND hList = GetDlgItem(hwnd, IDC_LIST);
							int count = SendMessage(hList, LB_GETSELCOUNT, 0, 0);
							if(count != LB_ERR)
							{
								// daca doar exista doar un nod vom continua
								// nod selectat

								if(count == 1)
								{
									//putem sa adaugam doar un nod / inserare
									// indexarea nu necesita alocarea unui vector

									int index;
									int err = SendMessage(hList, LB_GETSELITEMS, (WPARAM)1, (LPARAM)&index);
									if(err != LB_ERR)
									{
										// tine minte valoarea nodului de deasupra sa
										// tine minte de cate ori a fost adaugat

										int val = SendMessage(hList, LB_GETITEMDATA, (WPARAM)index, 0);

										SetDlgItemInt(hwnd, IDC_SHOWCOUNT, val, FALSE);
									}
									else 
									{
										MessageBoxA(hwnd, "eroare la selectarea nodului", "atentie", MB_OK);
									}
								}
								else 
								{
									// nici un nod selectat
									SetDlgItemText(hwnd, IDC_SHOWCOUNT, "-");
								}
							}
							else
							{
								MessageBoxA(hwnd, "Eroare la numerotare","atentie", MB_OK);
							}
						}
						break;
					}
				break;
					}
		break;
		case WM_CLOSE:
			EndDialog(hwnd, 0);
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd2, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND d2 = NULL;
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd2);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
		case WM_PAINT:
			{
				if  (ok == 1)
				{
					if(ok3)
					{
						hDC=BeginPaint(hwnd2, &Ps);
						desenare_grafic();
						EndPaint(hwnd2, &Ps);
						ok3=0;
						break;
					}
					if(ok2)
					{
						hDC=BeginPaint(hwnd2, &Ps);
					
						display_out_cautare(aux);
		
						EndPaint(hwnd2, &Ps);
						ok2=0;
						aux=NULL;
						break;
					}
					hDC=BeginPaint(hwnd2, &Ps);
					print=1;
					display_out(rad, 648,80, -40, 1);
					if(print==0)
						MessageBoxA(hwnd, "Inaltime prea mare => arbore incomplet","Atentie", MB_OK);
					EndPaint(hwnd2, &Ps);
				}
				ok=0;
				break;
			}
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDABORT:
				d2 = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAIN), hwnd2, DlgProc2);
				if (d2 != NULL)
					ShowWindow(d2, SW_SHOW);
				break;
			}
        break;
        default:
            return DefWindowProc(hwnd2, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)   //fereastra mare
{
	WNDCLASSEX wc;
	HWND hwnd2;
	MSG Msg;

//Step 1: Registering the Window Class

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(-1, -1, -1));
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU2);
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Fereastra nu a putut fi creeata!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

// Step 2: Creating the Window

	hwnd2 = CreateWindowEx(WS_EX_CLIENTEDGE,g_szClassName, "  Aplicatie Arbori Binari de cautare", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT, 1366,768,NULL, NULL, hInstance, NULL);
	if(hwnd2 == NULL)
	{
		MessageBox(NULL, "Fereastra nu a fost creeata !", "Eroare !",MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	ShowWindow(hwnd2, SW_NORMAL);
	UpdateWindow(hwnd2);

// Step 3: The Message Loop

while(GetMessage(&Msg, NULL, 0, 0) > 0)
{
	TranslateMessage(&Msg);
	DispatchMessage(&Msg);
}
return Msg.wParam;
}

