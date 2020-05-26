//
//  Main.h
//

#ifndef MAIN_H
#define MAIN_H
#include "overview.h"


int main (int argc, char* argv[]);
void initDisplay ();

void keyboardDown (unsigned char key, int x, int y);
void keyboardUp (unsigned char key, int x, int y);
void specialDown (int special_key, int x, int y);
void specialUp (int special_key, int x, int y);

void update ();
void handleKeyboard ();

void reshape (int w, int h);
void display ();
void drawOverlays ();



#endif

