/*
 * GLWindow.h
 * Created by Zachary Ferguson
 * Source file for the GLWindow class, a class for creating an openGL Window
 */

#include "GLWindow.h"

/* Constructor for a GLWindow that takes the int aspects */
/* Initializes the root to NULL                          */
GLWindow::GLWindow(int x, int y, int w, int h, const char* c)
	: Fl_Gl_Window(x, y, w, h, c)
{
	this->root = NULL;
}

/* Constructor for a GLWindow that takes the int aspects and the root Node */
GLWindow::GLWindow(int x, int y, int w, int h, const char* c, Node* root)
	: Fl_Gl_Window(x, y, w, h, c)
{
	this->root = root;
}

/* Destructor for this GLWindow, deletes the root. */
GLWindow::~GLWindow()
{
	delete root;
	this->root = NULL;
}

/* Sets this GLWindows root Node to the given Node */
/* Deletes the old root Node                       */
void GLWindow::setSceneGraph(Node* newRoot)
{
	if ((this->root) != NULL)
	{
		/* Free memory */
		delete this->root;
	}
	/* Assign new root */
	this->root = newRoot;
}

/* Draws this scene graph out to the screen */
void GLWindow::draw()
{
	/* FLTK housekeeping */
	if (!valid())
	{
		valid(1);
		
		/* glViewport specifies the affine transformation of x and y from */
		/* normalized device coordinates to window coordinates.           */
		glViewport(0, 0, this->w(), this->h());

		/* glOrtho describes a transformation that produces a */
		/* parallel projection.                               */
		glOrtho(-10, 10, -10, 10, -1, 1);
	}

	/* Clear the GLWindow first before drawing */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Traverse the scene graph */
	Node::traverseSceneGraph(*(this -> root), mat3::identity(), this->transformNum);
}

/* Get the transform "frame" number. */
unsigned int GLWindow::getTransformNum() const
{
	return this->transformNum;
}

/* Set the transform "frame" number. */
void GLWindow::setTransformNum(unsigned int newTransformNum)
{
	this->transformNum = newTransformNum;
}