/*
 * GLWindow.h
 * Created by Zachary Ferguson
 * Header file for the GLWindow class, a class for creating an openGL Window
 */

#ifndef GLWINDOW_H
#define GLWINDOW_H

/* Include necessary types */
#include "Node.h"

/* Child class of FL_GL_Window */
class GLWindow : public Fl_Gl_Window
{
	private:
		
		/* The transform "frame" number. */
		unsigned int transformNum;

		/* The SceneGrpah that will be drawn */
		Node *root;

		/* Method in FL_GL_Window class for drawing the window */
		/* Draws this scene graph out to the screen            */
		void draw();
	
	public:

		/* Constructor for a GLWindow that takes the int aspects */
		/* Initializes the root to NULL                          */
		GLWindow(int x, int y, int w, int h, const char* c);
		
		/* Constructor for a GLWindow that takes the int aspects and the root */ 
		/* Node.                                                              */
		GLWindow(int x, int y, int w, int h, const char* c, Node* root);
		
		/* Destructor for this GLWindow, deletes the root. */
		virtual ~GLWindow();

		/* Sets this GLWindows root Node to the given Node */
		/* Deletes the old root Node                       */
		void setSceneGraph(Node* newRoot);

		/* Get the transform "frame" number. */
		unsigned int getTransformNum() const;

		/* Set the transform "frame" number. */
		void setTransformNum(unsigned int newTransformNum);
};

#endif