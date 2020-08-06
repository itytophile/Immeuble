#include <fxlib.h>
#include "MonochromeLib.h"
#include "simpleTime.h"
#include "simpleTool.h"
#include "timer.h"
#include "fonts.h"

#define NBIM 5 //pas la peine d'augmenter, c'est pour un souci d'affichage et de game design. En dessous de 3 le programme ne peut pas marcher.
#define FRAMERATE 15
#define PUISSANCE_SAUT 3.7
#define VITESSE 3

typedef struct Immeuble {
	float x;
	float y;
	int taille;
} Immeuble;

float x, y, taille, t;
int saut, bloc, over, score, i, a;
unsigned int key;
Immeuble im[NBIM];
char *s = NULL;

int strlen(char *str) {
	int i = 0;
	while(str[i]) i++;
	return i;
}

void callback() {
	ML_clear_vram();
	//input
	if(key && key != KEY_CTRL_EXIT) {
		if(key == KEY_CTRL_RIGHT) a = 1;
		else if(saut) saut = 0; //pour la chute rapide
		else if(!t) saut = 1; //On vérifie que le bloc subit pas la gravité i.e le bloc est sur un sol
		key = 0; //utile dans le contexte du timer
	}
	//gestion saut
	if(saut) y -= PUISSANCE_SAUT;
	//gravité
	if(y+taille+t < im[0].y-1 || x > im[0].x+im[0].taille || bloc) { //y+taille+t pour éviter au bloc de s'enfoncer dans le sol
		y += t;
		t += 0.5;
	}
	else {
		saut = 0;
		y = im[0].y-1-taille;
		t = 0;
	}
	//le carré qu'on contrôle
	ML_rectangle(x, y, x+taille, y+taille, 1, ML_BLACK, ML_BLACK);
	//On déssine les immeubles
	for (i = 0; i < NBIM; i++) ML_rectangle(im[i].x, im[i].y, im[i].x+im[i].taille, ML_SCREEN_HEIGHT, 1, ML_BLACK, ML_TRANSPARENT);
	//Application du déplacement sur les immeubles
	for (i = 0; a && !bloc && i < NBIM; i++) im[i].x -= VITESSE;
	//On change la propriété des immeubles à l'écran
	if (x >= im[1].x) {
		im[NBIM-1].x = im[0].x; //le dernier immeuble de la liste ne fait que de prendre le relai du premier pour qu'il reste à l'écran
		im[NBIM-1].y = im[0].y; 
		im[NBIM-1].taille = im[0].taille;
		for(i = 0; i < NBIM-2; ++i) {
			im[i].x = im[i+1].x;
			im[i].y = im[i+1].y;
			im[i].taille = im[i+1].taille;
		}
		im[NBIM-2].x = im[NBIM-3].x+im[NBIM-3].taille+7+rand()%27;
		im[NBIM-2].y = 15+rand()%10;
		im[NBIM-2].taille = 20+rand()%10;
		if (y > im[0].y) { //on vérifie si le cube se prend le mur de l'immeuble d'en face
			bloc = 1;
			x = im[0].x - taille;
		} else ++score;
	}
	//on affiche le score :
	intToString(score, &s);
	dispStr((unsigned char *)s, 0, ML_SCREEN_HEIGHT-6);
	//on vérifie si le cube tombe trop bas
	over = y > ML_SCREEN_HEIGHT;
	if(over) {
		KillTimer(ID_USER_TIMER1);
		ML_clear_vram();
		dispStr((unsigned char *)"[MENU]", 0, 0);
		dispStr((unsigned char *)"Perdu", ML_SCREEN_WIDTH/2-10, ML_SCREEN_HEIGHT/2-13);
		dispStr((unsigned char *)"Votre score :", ML_SCREEN_WIDTH/2-23, ML_SCREEN_HEIGHT/2-6);
		dispStr((unsigned char *)s, ML_SCREEN_WIDTH/2-(2*(strlen(s))), ML_SCREEN_HEIGHT/2+1);
		dispStr((unsigned char *)"Par ityt - www.planete-casio.fr", 0, 56);
	}
	
	Bdisp_PutDisp_DD();
}

int main() {
	srand(GetTicks());
	ML_clear_vram();
	dispStr((unsigned char *)"[?] pour sauter !", 0, 0);
	dispStr((unsigned char *)"[?] pour faire une chute rapide !", 0, 7);
	dispStr((unsigned char *)"[EXIT] pour la pause !", 0, 14);
	dispStr((unsigned char *)"[RIGHT] pour FONCER !!!!!!!!!!!", 0, 21);
	dispStr((unsigned char *)"(Il faut appuyer qu'une fois...)", 0, 28);
	dispStr((unsigned char *)"Utilisez [MENU] seulement en", 0, 42);
	dispStr((unsigned char *)"pause ! C'est important.", 0, 49);
	dispStr((unsigned char *)"Appuyez pour continuer...", 0, 56);
	GetKey(&key);
	while(1) {
		key = score = over = bloc = t = y = a = saut = 0;
		x = ML_SCREEN_WIDTH/4;
		taille = 2;
		
		im[0].x = 10;
		im[0].y = 20;
		im[0].taille = 30;
		im[NBIM-1].x = -1;
		im[NBIM-1].y = 0;
		im[NBIM-1].taille = 0;
		for(i = 1; i < NBIM-1; ++i) {
			im[i].x = im[i-1].x+im[i-1].taille+5+rand()%27;
			im[i].y = 20;
			im[i].taille = 30;
		}
		
		//création du timer
		SetTimer(ID_USER_TIMER1, 1000/FRAMERATE, callback);
		while(!over) {
			GetKey(&key);
			if(key == KEY_CTRL_EXIT && !over) {
				KillTimer(ID_USER_TIMER1);
				dispStr((unsigned char *)"[MENU]", 0, 0);
				ML_rectangle(ML_SCREEN_WIDTH/3, ML_SCREEN_HEIGHT/3, ML_SCREEN_WIDTH/3*2, ML_SCREEN_HEIGHT/3*2, 1, ML_WHITE, ML_WHITE);
				dispStr((unsigned char *)"Pause", ML_SCREEN_WIDTH/2-10, ML_SCREEN_HEIGHT/2-3);
				Bdisp_PutDisp_DD();
				GetKey(&key);
				key = 0;
				SetTimer(ID_USER_TIMER1, 1000/FRAMERATE, callback);
			}
		}
	}
	return 1;
}