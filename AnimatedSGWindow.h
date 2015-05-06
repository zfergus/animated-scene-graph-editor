/*
 * AnimatedSGWindow.h
 * Created by Zachary Ferguson
 * Header file for the AnimatedSGWindow class, a class for creating a GUI 
 * for controlling and manipulating a Scene Graph and Animating it. Uses a 
 * GLWindow to display the scene graph.
 */

#ifndef ANIMATEDSGWINDOW_H
#define ANIMATEDSGWINDOW_H

/* Allows Fl_Spinner.h to use sprintf */
#define _CRT_SECURE_NO_WARNINGS

#include "SceneGraphWindow.h"
#include <FL/Gl.H>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/Fl_Spinner.H>
#include <Fl/Fl_Light_Button.H>
#include <FL/Gl.H>
#include <sstream>
#include <iostream>
#include <list>
#include <jpeg/jpeglib.h>

/* Macro for converting a void pointer to an AnimatedSGWindow pointer. */
#define VOID_TO_ASGWIN(ptr) AnimatedSGWindow* aSGWin = (AnimatedSGWindow*)ptr

class AnimatedSGWindow : public SceneGraphWindow
{

	protected:

		/* A Pointer to the animate button. */
		Fl_Button* animateB;
		/* A Pointer to the render out button. */
		Fl_Button* renderB;
		/* Render button for rendering out the animation to the local directory. */
		Fl_Light_Button* interpolateB;
		/* A Pointer to the group of timeline and play controls. */
		Fl_Group* timelineG;
		/* A Pointer to the timeline slider */
		Fl_Value_Slider* timeline;
		/* Pointers to the timeline controles */
		Fl_Button *playB, *forwardB, *backB;
		Fl_Light_Button* loopB;
		/* A Pointer to the group of frame controls. */
		Fl_Group* frameControlG;
		/* Pointers to the frame controls */
		Fl_Spinner *framerateSpinner, *numFramesSpinner;
		/* Boolean for if the animation is playing. */
		bool isPlaying;

		/* Creates the group for the timeline and its controls and returns it. */
		Fl_Group* makeTimelineG(const int x, const int y);		
		/* Creates the group for the frame controls and returns it. */
		Fl_Group* makeFrameControlG(const int x, const int y);	
		
		/* Sets the values of the transformation widgets to */
		/* the activeNode's values.                         */
		void setTransformationG();
		/* Sets the values of the color selection widgets to */
		/* the activeNode's color values.                    */
		void setColorChooser();

		/* Callback function for the animate button. */
		static void animateCB(Fl_Widget *w, void *data);
		/* Callback function for the render out button. */
		static void renderCB(Fl_Widget *w, void *data);
		/* Callback function for the render out button. */
		static void backCB(Fl_Widget *w, void *data);
		/* Callback function for the render out button. */
		static void playbuttonCB(Fl_Widget *w, void *data);
		/* Callback function for the play timer. */
		static void playCB(void *data);
		/* Callback function for the render out button. */
		static void forwardCB(Fl_Widget *w, void *data);
		/* Callback function for the render out button. */
		static void timelineCB(Fl_Widget *w, void *data);
		/* Callback function for the entire transformation group. */
		static void multipleTransformsCB(Fl_Widget *w, void *data);
		/* Callback function for the color chooser. */
		static void multipleColorCB(Fl_Widget *w, void *data);
		/* Callback function for the number of frames spinner. */
		static void numFramesCB(Fl_Widget *w, void *data);
		/* Interpolate the scene graph frames. */
		static void interpolateCB(Fl_Widget *w, void *data);
		
	public:

		/* Constructor for a SceneGraphWindow that takes the x,y coordinates, */
		/* the width, the height, and the window label as a char*.            */
		AnimatedSGWindow(int x, int y, int w, int h, const char* label);

		/* Destructor for this SceneGraphWindow. */
		virtual ~AnimatedSGWindow();

};

#endif