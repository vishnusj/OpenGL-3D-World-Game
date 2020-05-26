//
//  Main.cpp
//

#include <cassert>
#include <cstdlib>
#include <sstream>

#include "GetGlut.h"
#include "Sleep.h"
#include "Random.h"
#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/SpriteFont.h"
#include "World.h"
#include "PhysicsFrameLength.h"
#include "Game.h"
#include "Main.h"

#include "Nodes.h"

using namespace std;
using namespace ObjLibrary;
namespace
{
	const double FIELD_OF_VIEW_Y     = 60.0;
	const double CLIPPING_PLANE_NEAR = 0.01;
	const double CLIPPING_PLANE_FAR  = 1000.0;

	int g_window_width  = 640;
	int g_window_height = 480;

	const unsigned int KEY_COUNT       = 260;
	const unsigned int KEY_ARROW_LEFT  = 256;
	const unsigned int KEY_ARROW_UP    = 257;
	const unsigned int KEY_ARROW_RIGHT = 258;
	const unsigned int KEY_ARROW_DOWN  = 259;

	bool ga_is_pressed[KEY_COUNT];

	const Vector3 OVERVIEW_POSITION(100.0, 200.0, 10.0);
	const Vector3 OVERVIEW_LOOK_AT (  0.0,   0.0,  0.0);
	const Vector3 OVERVIEW_UP      (  0.0,   1.0,  0.0);

	Game g_game;

	double g_start_time;
	unsigned int g_update_count;
	double g_last_display_time;
	double g_display_fps;
	const double DISPLAY_FPS_SMOOTHING_FACTOR = 0.02;

	SpriteFont g_font;



	const bool DEBUGGING_FRAME_RATE = false;

}  // end of anonymous namespace



int main (int argc, char* argv[])
{
	glutInitWindowSize(g_window_width, g_window_height);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Assignment 5");
	glutKeyboardFunc(&keyboardDown);
	glutKeyboardUpFunc(&keyboardUp);
	glutSpecialFunc(&specialDown);
	glutSpecialUpFunc(&specialUp);
	glutIdleFunc(&update);
	glutReshapeFunc(&reshape);
	glutDisplayFunc(&display);

	seedRandom();
	initDisplay();
	Game::loadModels();
	g_game.init("Worlds/Small.txt");
	

	g_font.load("FontTimesNewRoman16.bmp");

	// one update now, because first frame is always slow
	g_game.update();

	g_start_time        = glutGet(GLUT_ELAPSED_TIME) * 0.001;
	g_update_count      = 0;
	g_last_display_time = g_start_time;
	assert(PHYSICS_FRAME_LENGTH > 0.0);
	g_display_fps       = 1.0 / PHYSICS_FRAME_LENGTH;

	glutMainLoop();

	return 1;
}

void initDisplay ()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);

	glutPostRedisplay();
}



void keyboardDown (unsigned char key, int x, int y)
{
	if(key >= 'A' && key <= 'Z')
		key = key - 'A' + 'a';
	ga_is_pressed[key] = true;

	switch (key)
	{
	case 27: // on [ESC]
		exit(0); // normal exit
		break;
	}
}

void keyboardUp (unsigned char key, int x, int y)
{
	if(key >= 'A' && key <= 'Z')
		key = key - 'A' + 'a';
	ga_is_pressed[key] = false;
}

void specialDown (int special_key, int x, int y)
{
	switch(special_key)
	{
	case GLUT_KEY_LEFT:
		ga_is_pressed[KEY_ARROW_LEFT]  = true;
		break;
	case GLUT_KEY_RIGHT:
		ga_is_pressed[KEY_ARROW_RIGHT] = true;
		break;
	case GLUT_KEY_UP:
		ga_is_pressed[KEY_ARROW_UP]    = true;
		break;
	case GLUT_KEY_DOWN:
		ga_is_pressed[KEY_ARROW_DOWN]  = true;
		break;
	}
}

void specialUp (int special_key, int x, int y)
{
	switch(special_key)
	{
	case GLUT_KEY_LEFT:
		ga_is_pressed[KEY_ARROW_LEFT]  = false;
		break;
	case GLUT_KEY_RIGHT:
		ga_is_pressed[KEY_ARROW_RIGHT] = false;
		break;
	case GLUT_KEY_UP:
		ga_is_pressed[KEY_ARROW_UP]    = false;
		break;
	case GLUT_KEY_DOWN:
		ga_is_pressed[KEY_ARROW_DOWN]  = false;
		break;
	}
}



void update ()
{
	double current_time    = glutGet(GLUT_ELAPSED_TIME) * 0.001;
	double next_frame_time = g_start_time + g_update_count * PHYSICS_FRAME_LENGTH;
	if(next_frame_time > current_time)
	{
		double sleep_time = next_frame_time - current_time;
		if(sleep_time > 0.0f)
			sleep(sleep_time);
		current_time = glutGet(GLUT_ELAPSED_TIME) * 0.001;
	}

	if(DEBUGGING_FRAME_RATE)
		cout << "time = " << current_time << " / " << next_frame_time;

	unsigned int sanity = 0;
	while(next_frame_time <= current_time && sanity < 10)
	{
		if(!g_game.isGameOver())
			handleKeyboard();
		g_game.update();
		g_update_count++;

		current_time    = glutGet(GLUT_ELAPSED_TIME) * 0.001;
		next_frame_time = g_start_time + g_update_count * PHYSICS_FRAME_LENGTH;
		sanity++;

		if(DEBUGGING_FRAME_RATE)
			cout << " +";
	}
	if(DEBUGGING_FRAME_RATE)
		cout << endl;

	if(sanity > 0)
		glutPostRedisplay();
}

void handleKeyboard ()
{
	if(ga_is_pressed[KEY_ARROW_LEFT])
		g_game.playerTurnLeft();
	if(ga_is_pressed[KEY_ARROW_RIGHT])
		g_game.playerTurnRight();

	if(!g_game.isPlayerJumping())
	{
		if(ga_is_pressed['w'] || ga_is_pressed[KEY_ARROW_UP])
			g_game.playerAccelerateForward();
		if(ga_is_pressed['s'] || ga_is_pressed[KEY_ARROW_DOWN])
			g_game.playerAccelerateBackward();
		if(ga_is_pressed['a'])
			g_game.playerAccelerateLeft();
		if(ga_is_pressed['d'])
			g_game.playerAccelerateRight();
		if(ga_is_pressed[' '])
			g_game.playerJump();
	}
}



void reshape (int w, int h)
{
	glViewport (0, 0, w, h);

	g_window_width  = w;
	g_window_height = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW_Y,
	               (GLdouble)w / (GLdouble)h,
	               CLIPPING_PLANE_NEAR,
	               CLIPPING_PLANE_FAR);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void display ()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	bool is_overview = ga_is_pressed['o'];
	g_game.over = is_overview;
	if(is_overview)
	{
		gluLookAt(OVERVIEW_POSITION.x, OVERVIEW_POSITION.y, OVERVIEW_POSITION.z,
		          OVERVIEW_LOOK_AT .x, OVERVIEW_LOOK_AT .y, OVERVIEW_LOOK_AT .z,
		          OVERVIEW_UP      .x, OVERVIEW_UP      .y, OVERVIEW_UP      .z);


	}
	else
	{
		g_game.setupCamera();
		g_game.drawSkybox();  // must be drawn first
	}

	g_game.draw();

	

	if(!is_overview)
		drawOverlays();  // must be drawn last

	glutSwapBuffers();
}

void drawOverlays ()
{
	double current_time = glutGet(GLUT_ELAPSED_TIME) * 0.001;

	stringstream score_ss;
	score_ss << "Score: " << g_game.getScore();

	double elapsed_time = current_time - g_start_time;
	int    update_rate  = (int)(g_update_count / elapsed_time + 0.5);
	stringstream update_rate_ss;
	update_rate_ss << "Updates: " << update_rate << " / s";

	double display_time = current_time - g_last_display_time;
	int    current_fps = 1000;
	if(display_time > 0.0)
		current_fps = (int)(1.0 / display_time);
	g_display_fps += (current_fps - g_display_fps) * DISPLAY_FPS_SMOOTHING_FACTOR;
	stringstream display_rate_ss;
	display_rate_ss << "Display: " << (int)(g_display_fps + 0.5) << " / s";

	SpriteFont::setUp2dView(g_window_width, g_window_height);
		g_font.draw(score_ss       .str(), g_window_width - 160, 16);
		g_font.draw(update_rate_ss .str(), g_window_width - 160, 40);
		g_font.draw(display_rate_ss.str(), g_window_width - 160, 64);

		if(g_game.isGameOver())
		{
			static const string GAME_OVER_TEXT = "Game Over";
			float game_over_x = g_window_width  * 0.5f  - g_font.getWidth(GAME_OVER_TEXT) * 0.5f;
			float game_over_y = g_window_height * 0.22f - g_font.getHeight()              * 0.5f;
			g_font.draw(GAME_OVER_TEXT, game_over_x, game_over_y);
		}
	SpriteFont::unsetUp2dView();

	g_last_display_time = current_time;
}
