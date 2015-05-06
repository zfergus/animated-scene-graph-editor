/*
 * SceneGraphWindow.h
 * Created by Zachary Ferguson
 * Header file for the SceneGraphWindow class, a class for creating a GUI 
 * for controlling and manipulating a Scene Graph. Uses a GLWindow to display 
 * the scene graph.
 */

#ifndef SCENEGRAPHWINDOW_H
#define SCENEGRAPHWINDOW_H

#include <iostream>	
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Input.H>
#include "GLWindow.h"
#include "polyline.h"
#include "polygon.h"
#include "quad.h"
#include "triangle.h"

#define TREEVIEWX 10
#define TREEVIEWY 20
#define MAXTRANSLATION 10
#define MINTRANSLATION -10
#define MINSCALE 0.1
#define MAXSCALE 5

class SceneGraphWindow : public Fl_Window
{

	protected:

		/* Pointer to the GLWindow to display the SceneGraph. */
		GLWindow* glWin;
		/* The scene graph that is being displayed. */
		Node* sceneGraph;
		/* The active Node selected that is being modified. */
		Node* activeNode;
		/* The active tree item selected that is being modified. */
		Fl_Tree_Item* activeItem;
		/* A Pointer to the exit button. */
		Fl_Button* exitB;
		/* A Pointer to the reset/"magic" button. */
		Fl_Button* resetB;		
		/* A Pointer to the remove Node button. */
		Fl_Button* removeB;
		/* A Pointer to the add Node button. */
		Fl_Button* addB;
		/* Group of the transformation widgets. */
		Fl_Group* transformationG;
		/* Pointers to the translation sliders. */
		Fl_Value_Slider* translateXSlider, *translateYSlider,
		/* Pointers to the scale sliders. */
			           *scaleXSlider, *scaleYSlider;
		/* A pointer to the rotation dial. */
		Fl_Dial* rotationDial;
		/* Tree view of the scene graph. */
		Fl_Tree* treeView;
		/* Color wheel to pick geometry colors. */
		Fl_Color_Chooser* colorChooser;
		/* Input for renaming the active Item */
		Fl_Input* itemNameInput;


		/* Callback function for the exit button. */
		static void exitCB(Fl_Widget *w, void *data);
		/* Callback function for the reset button. */
		static void resetCB(Fl_Widget *w, void *data);
		/* Callback function for the color chooser. */
		static void colorCB(Fl_Widget *w, void *data);
		/* Callback function for the remove node button. */
		static void removeCB(Fl_Widget *w, void *data);
		/* Callback function for the add node button. */
		static void addWindowCB(Fl_Widget *w, void *data);
		/* Callback function for adding a transformation node. */
		static void addTransformNodeCB(Fl_Widget *w, void *data);
		/* Callback function for adding a polyline node. */
		static void addPolylineNodeCB(Fl_Widget *w, void *data);
		/* Callback function for getting the number of sides of the new */
		/* polygon.                                                     */
		static void getNSides(Fl_Widget *w, void *data);
		/* Callback function for adding a n-sided polygon node. */
		static void addPolygonNodeCB(Fl_Widget *w, void *data);
		/* Callback function for adding a quadrilateral node. */
		static void addQuadNodeCB(Fl_Widget *w, void *data);
		/* Callback function for adding a triangle node. */
		static void addTriangleNodeCB(Fl_Widget *w, void *data);
		/* Callback function for the tree view of the scene graph. */
		static void treeCB(Fl_Widget *w, void *data);
		/* Callback function for the entire transformation group. */
		static void transformCB(Fl_Widget *w, void *data);
		/* Calback function for changing the active items label. */
		static void changeItemLabelCB(Fl_Widget *w, void *data);
		
		/* Create the original animal scene graph */
		static Node* createAnimalSceneGraph();

		/* Creates the FL Group of translation sliders. */
		Fl_Group* makeTranslationG(const int x, const int y);
		/* Creates the FL Group of scale sliders. */
		Fl_Group* makeScaleG(const int x, const int y);
		/* Creates the group of transformations. */
		Fl_Group* makeTransformG(const int x, const int y);

		/* Sets the values of the transformation widgets to */
		/* the activeNode's values.                         */
		void setTransformationG();
		/* Sets the values of the color selection widgets to */
		/* the activeNode's color values.                    */
		void setColorChooser();
		/* Sets the value of the rename item input to the */
		/* active item's label.                           */
		void setItemNameInput();

		/* Creates the tree view of the scene graph. */
		Fl_Tree* makeTree(const int x, const int y);
		/* Adds the children to the tree view. */
		void addChildrenToTree();

		/* Adds the given Node to the Scene Graph and tree view with the */
		/* given label.                                                  */
		void addNode(Node* n, const char* label);

	public:

		/* Constructor for a SceneGraphWindow that takes the x,y coordinates, */
		/* the width, the height, and the window label as a char*.            */
		SceneGraphWindow(int x, int y, int w, int h, const char* c);

		/* Destructor for this SceneGraphWindow. */
		virtual ~SceneGraphWindow();

};

#endif