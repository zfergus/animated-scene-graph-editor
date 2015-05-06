/*
 * main.cpp
 * Created by Zachary Ferguson
 * Main file for creating a animated scene graph.
 */

#include "AnimatedSGWindow.h"

/* Draw a Animated Scene Graph */
int main(int argc, char* const argv[])
{
	/***Draw the Animated Scene Graph***/

	AnimatedSGWindow *aSGWin = new AnimatedSGWindow(200, 50, 1000, 560, "Animated \
		Scene Graph Editor - Zachary Ferguson, zfergus2");
	aSGWin->show();
	return Fl::run();

}