/*
 * SceneGraphWindow.cpp
 * Created by Zachary Ferguson
 * Source code for the SceneGraphWindow class, a class for creating a GUI
 * for controlling and manipulating a Scene Graph. Uses a GLWindow to display the
 * scene graph.
 */

#include "SceneGraphWindow.h"

/* Constructor for a SceneGraphWindow that takes the x,y coordinates, */
/* the width, the height, and the window label as a char*.            */
SceneGraphWindow::SceneGraphWindow(int x, int y, int w, int h, const char* c) : 
	Fl_Window(x, y, w, h, c)
{
	this->sceneGraph = createAnimalSceneGraph();

	/* Make the original GLWindow */
	this->glWin = new GLWindow(210, 10, 400, 400, "GLWindow", sceneGraph);

	/* Make the tree view of the scene graph */
	this->treeView = this->makeTree(TREEVIEWX, TREEVIEWY);
	this->addChildrenToTree();

	/* Exit button */
	this->exitB = new Fl_Button(w - 60, h - 30, 50, 20, "Exit");
	this->exitB->box(FL_PLASTIC_UP_BOX);
	this->exitB->color(FL_RED);
	this->exitB->callback(SceneGraphWindow::exitCB, NULL);

	/* Reset button */
	this->resetB = new Fl_Button(w - 120, h - 30, 50, 20, "Reset");
	this->resetB->box(FL_PLASTIC_UP_BOX);
	this->resetB->color(FL_YELLOW);
	this->resetB->callback(SceneGraphWindow::resetCB, this);

	/* Add Child node to active node */
	this->addB = new Fl_Button(10, h - 30, 90, 20, "Add Child");
	this->addB->box(FL_PLASTIC_UP_BOX);
	this->addB->callback(SceneGraphWindow::addWindowCB, this);

	/* Remove active Node */
	this->removeB = new Fl_Button(110, h - 30, 110, 20, "Remove Node");
	this->removeB->box(FL_PLASTIC_UP_BOX);
	this->removeB->callback(SceneGraphWindow::removeCB, this);

	/* Color Chooser */
	this->colorChooser = new Fl_Color_Chooser(w-190, h-130, 170, 90, "Color");
	this->colorChooser->rgb(1.0, 1.0, 1.0); /* Sets initial value */
	this->colorChooser->callback(SceneGraphWindow::colorCB, this);

	/* Add the Item Rename Input */
	this->itemNameInput = new Fl_Input(this->treeView->x(), 
		this->treeView->y()+this->treeView->h()+20, 
		this->treeView->w(), 20, "Rename Node:");
	this->itemNameInput->value(this->activeItem->label());
	this->itemNameInput->when(FL_WHEN_ENTER_KEY);
	this->itemNameInput->callback(SceneGraphWindow::changeItemLabelCB, this);
	this->itemNameInput->align(Fl_Align(FL_ALIGN_TOP));
	this->itemNameInput->box(FL_PLASTIC_DOWN_BOX);

	/* Create the transformation group for transforming Nodes */
	this->transformationG = this->makeTransformG(w-380, 20);

	//this->end();

	/* Set the widgets to the active Nodes values */
	this->setTransformationG();
	this->setColorChooser();
}

/* Destructor for this SceneGraphWindow */
SceneGraphWindow::~SceneGraphWindow()
{
	/* Remove all child widgets */
	delete this->glWin;
	delete this->sceneGraph;
	delete this->activeNode;
	delete this->activeItem;
	delete this->exitB;
	delete this->resetB;
	delete this->removeB;
	delete this->addB;
	delete this->transformationG;
	delete this->translateXSlider;
	delete this->translateYSlider;
	delete this->scaleXSlider;
	delete this->scaleYSlider;
	delete this->rotationDial;
	delete this->treeView;
	delete this->itemNameInput;
	delete this->colorChooser;
}

/* Callback function for the exit button */
void SceneGraphWindow::exitCB(Fl_Widget *w, void *data)
{
	delete data;
	exit(0);
}

/* Callback function for the reset button */
void SceneGraphWindow::resetCB(Fl_Widget *w, void *data)
{ 
	SceneGraphWindow* sgWin = (SceneGraphWindow*)data;
	
	/* Creates the animal scene graph */
	sgWin->sceneGraph = createAnimalSceneGraph();
	sgWin->activeNode = sgWin->sceneGraph;

	/* Deletes sgWin->sceneGraph too */
	sgWin->glWin->setSceneGraph(sgWin->sceneGraph);

	/* Rebuild the tree view */
	sgWin->treeView->clear_children(sgWin->treeView->root());
	sgWin->addChildrenToTree();

	/* Set the transformation widget values */
	sgWin->setTransformationG();
	sgWin->setColorChooser();
	sgWin->setItemNameInput();
	
	sgWin->glWin->redraw();

	sgWin->redraw();

	std::cout << "Scene Graph Reset" << std::endl;
}

/* Callback function for the color chooser */
void SceneGraphWindow::colorCB(Fl_Widget *w, void *data)
{
	SceneGraphWindow* sgWin = (SceneGraphWindow*)data;
	Fl_Color_Chooser* color = (Fl_Color_Chooser*)w;
	if(sgWin->activeNode != NULL)
	{
		sgWin->activeNode->setGeometryColor((float)(color->r()), 
			(float)(color->g()), (float)(color->b()));
	}
	sgWin->glWin->redraw();
}

/* Callback function for the color chooser */
void SceneGraphWindow::removeCB(Fl_Widget *w, void *data)
{
	SceneGraphWindow* sgWin = (SceneGraphWindow*)data;
	/* Make sure the active item and node are not NULL */
	if(sgWin -> activeItem != NULL && sgWin->activeNode != NULL)
	{
		/* Get the parent node of the active one */
		Node* parentNode = sgWin->activeNode->getParent();
		/* Get the parent item of the active one */
		Fl_Tree_Item* parentItem = sgWin->activeItem->parent();

		/* Make sure the parent item ans node is not NULL */
		if(parentItem != NULL)
		{
			sgWin->treeView->remove(sgWin->activeItem);
			sgWin->activeItem = parentItem;
			/* If the parent Node is not NULL */
			if (parentNode != NULL)
			{
				sgWin->activeItem->select(1);
				parentNode->removeChild(sgWin->activeNode);
				sgWin->activeNode = parentNode;
				sgWin->setTransformationG();
				sgWin->setColorChooser();
				sgWin->setItemNameInput();
				std::cout << "Removed Node successfully" << std::endl;
			}
			/* Else if only the parent item is not NULL */
			else
			{
				sgWin->glWin->setSceneGraph(NULL);
				sgWin->activeNode = NULL;
				sgWin->sceneGraph = NULL;
				sgWin->transformationG->deactivate();
				sgWin->colorChooser->deactivate();
				sgWin->itemNameInput->deactivate();
				std::cout << "Removed Node successfully" << std::endl;
			}
		}

		sgWin->glWin->redraw();
		sgWin->redraw();
	}
}

/* Callback function for the color chooser */
void SceneGraphWindow::addWindowCB(Fl_Widget *w, void *data)
{
	Fl_Window* win = new Fl_Window(550, 300, 153, 57);
  
	Fl_Choice* choice = new Fl_Choice(10, 25, 140, 25, "Type of Node");
	choice->down_box(FL_BORDER_BOX);
	choice->align(Fl_Align(FL_ALIGN_TOP));
	choice->add("Transform Node (No Geometry)", 0, addTransformNodeCB, data);
	choice->add("Polyline", 0, addPolylineNodeCB, data);
	choice->add("Polygon", 0, getNSides, data);
	choice->add("Quadrilateral", 0, addQuadNodeCB, data);
	choice->add("Triangle", 0, addTriangleNodeCB, data);

	win->show();
}

/* Callback function for adding a transformation node */
void SceneGraphWindow::addTransformNodeCB(Fl_Widget* w, void* data)
{
	delete w->parent();
	SceneGraphWindow* sgWin = (SceneGraphWindow*)data;

	Node* transformNode = new Node(mat3::identity(), mat3::identity(),
								   mat3::identity());
	
	/* Add the node to the sgWin */
	((SceneGraphWindow*)data)->addNode(transformNode, "Transform");
}

/* Callback function for adding a polyline node */
void SceneGraphWindow::addPolylineNodeCB(Fl_Widget *w, void *data)
{
	delete w->parent();
	SceneGraphWindow* sgWin = (SceneGraphWindow*)data;

	std::list<vec3>* vecs = new std::list<vec3>();
	vecs->push_back(vec3(0, 0, 1));
	vecs->push_back(vec3(-0.5, 0, 1));
	vecs->push_back(vec3(-0.5, 0.5, 1));
	vecs->push_back(vec3(-1, 0.5, 1));
	polyline* line = new polyline(vecs, 1.0, 0.0, 0.0);
	
	Node* polylineNode = new Node(mat3::identity(), mat3::identity(),
								  mat3::identity(), line);
	
	/* Add the node to the sgWin */
	((SceneGraphWindow*)data)->addNode(polylineNode, "Polyline");
}

/* Callback function for getting the number of sides of the new polygon */
void SceneGraphWindow::getNSides(Fl_Widget *w, void *data)
{
	delete w->parent(); 

	Fl_Window* window = new Fl_Window(537, 300, 175, 57);

	/* Create a value input for the number of sides of the new polygon */
	Fl_Value_Input* nSidesInput = new Fl_Value_Input(25, 23, 130, 24, 
		"Number of Sides [3, 100]:");
	//nSidesInput->minimum(3);
	nSidesInput->maximum(100);
	nSidesInput->step(1);
	//nSidesInput->value(3);
	nSidesInput->when(FL_WHEN_ENTER_KEY);
	nSidesInput->callback(SceneGraphWindow::addPolygonNodeCB, data);
	nSidesInput->align(Fl_Align(FL_ALIGN_TOP));

	window->end();
	window->show();
}

/* Callback function for adding a n-sided polygon node */
void SceneGraphWindow::addPolygonNodeCB(Fl_Widget* w, void* data)
{
	SceneGraphWindow* sgWin = (SceneGraphWindow*)data;
	std::list<vec3>* vecs = new std::list<vec3>();

	int n = (int)(((Fl_Value_Input*)w)->value());
	/* Check that n is in the range [3, 100] */
	if (n < 3 || n > 100)
		return;
	else
		delete w->parent();
	
	/* Create an N sided polygon */
	for (int i = 0; i < n; i++)
	{
		float angle = (2 * PI * i) / n;
		vecs->push_back(vec3(cos(angle), sin(angle), 1));
	}
	polygon* nGon = new polygon(vecs, 1.0, 0.0, 0.0);
	
	Node* polygonNode = new Node(mat3::identity(), mat3::identity(),
								mat3::identity(), nGon);
	
	/* Add the node to the sgWin */
	((SceneGraphWindow*)data)->addNode(polygonNode, "Polygon");
}

/* Callback function for adding a quadrilateral node */
void SceneGraphWindow::addQuadNodeCB(Fl_Widget* w, void* data)
{
	/* Delete the selection window */
	delete w->parent();

	/* Create a square */
	std::list<vec3>* vecs = new std::list<vec3>();
	vecs->push_back(vec3( 0.5, 1, 1));
	vecs->push_back(vec3( 0.5, 0, 1));
	vecs->push_back(vec3(-0.5, 0, 1));
	vecs->push_back(vec3(-0.5, 1, 1));
	quad* square = new quad(vecs, 1.0, 0.0, 0.0);

	Node* quadNode = new Node(mat3::identity(), mat3::identity(),
								   mat3::identity(), square);
	
	/* Add the node to the sgWin */
	((SceneGraphWindow*)data)->addNode(quadNode, "Quadrilateral");
}

/* Callback function for adding a triangle node */
void SceneGraphWindow::addTriangleNodeCB(Fl_Widget* w, void* data)
{
	/* Delete the selection window */
	delete w->parent();

	/* Create a square */
	std::list<vec3>* vecs = new std::list<vec3>();
	vecs->push_back(vec3( 0.5, 0, 1));
	vecs->push_back(vec3(-0.5, 0, 1));
	vecs->push_back(vec3(   0, 1, 1));
	triangle* tri = new triangle(vecs, 1.0, 0.0, 0.0);

	Node* triangleNode = new Node(mat3::identity(), mat3::identity(),
								  mat3::identity(), tri);
	
	/* Add the node to the sgWin */
	((SceneGraphWindow*)data)->addNode(triangleNode, "Triangle");
}

/* Adds the given Node to the Scene Graph and tree view with the given */
/* label.                                                              */
void SceneGraphWindow::addNode(Node* n, const char* label)
{	
	/* De-select the active Node */
	this->activeItem->select(0);
	/* If the root Node has not been removed */
	if(this->activeNode != NULL)
	{
		/* Add the new node as a child of the active node */
		this->activeNode->addChild(n);
		this->activeNode = n;
	}
	else
	{
		/* Create a new scene graph with the new node as the root */
		this->sceneGraph = n;
		this->glWin->setSceneGraph(this->sceneGraph);
		this->activeNode = this->sceneGraph;
	}
	/* Make a new Tree Item with the active node as the user data */
	this->activeItem = this->treeView->add(this->activeItem, label);
	this->activeItem->user_data(this->activeNode);
	/* Select the new active item */
	this->activeItem->select(1);

	/* Set the transform widgets to the active nodes transforms */
	this->setTransformationG();
	/* Set the color choosers value */
	this->setColorChooser();
	/* Set the item name input */
	this->setItemNameInput();

	/* Redraw the glWin and the this */
	this->glWin->redraw();
	this->redraw();
}

/* Creates the FL Group of translation sliders */
Fl_Group* SceneGraphWindow::makeTranslationG(const int x, const int y)
{
	/* Create the translation group */
	Fl_Group* translateG = new Fl_Group(x, y, 225, 170, "Translation");
	translateG->box(FL_PLASTIC_DOWN_FRAME);
	translateG->color(FL_GRAY0);

	/* Create the translate x slider */
	this->translateXSlider = new Fl_Value_Slider(x+5, y+85, 145, 
		30, "Translation X");
	this->translateXSlider->type(1);
	this->translateXSlider->minimum(MINTRANSLATION);
	this->translateXSlider->maximum(MAXTRANSLATION);
	this->translateXSlider->box(FL_PLASTIC_DOWN_BOX);
	this->translateXSlider->color((Fl_Color)30);
	this->translateXSlider->step(0.01);
	this->translateXSlider->align(Fl_Align(FL_ALIGN_TOP));
	this->translateXSlider->callback(SceneGraphWindow::transformCB, this);
	
	/* Create the translate y slider */
	this->translateYSlider = new Fl_Value_Slider(x+160, y+20, 30, 
		145, "Translation Y");
	this->translateYSlider->minimum(MAXTRANSLATION);
	this->translateYSlider->maximum(MINTRANSLATION);
	this->translateYSlider->box(FL_PLASTIC_DOWN_BOX);
	this->translateYSlider->color((Fl_Color)30);
	this->translateYSlider->step(0.01);
	this->translateYSlider->align(Fl_Align(FL_ALIGN_TOP));
	this->translateYSlider->callback(SceneGraphWindow::transformCB, this);

	translateG->end();
	return translateG;
}

/* Creates the FL Group of scale sliders */
Fl_Group* SceneGraphWindow::makeScaleG(const int x, const int y)
{
	/* Create the scale group */
	Fl_Group* scaleG = new Fl_Group(x, y, 225, 170, "Scale");
	scaleG->box(FL_PLASTIC_DOWN_FRAME);
	scaleG->color(FL_GRAY0);
	
	/* Create the scale x slider */
	this->scaleXSlider = new Fl_Value_Slider(x+5, y+85, 145, 30, 
		"Scale X");
	this->scaleXSlider->type(1);
	this->scaleXSlider->minimum(MINSCALE);
	this->scaleXSlider->maximum(MAXSCALE);
	this->scaleXSlider->box(FL_PLASTIC_DOWN_BOX);
	this->scaleXSlider->color((Fl_Color)30);
	this->scaleXSlider->step(0.1);
	this->scaleXSlider->align(Fl_Align(FL_ALIGN_TOP));
	this->scaleXSlider->callback(SceneGraphWindow::transformCB, this);

	/* Create the scale y slider */
	this->scaleYSlider = new Fl_Value_Slider(x+160, y+20, 30, 145, 
		"Scale Y");
	this->scaleYSlider->minimum(MAXSCALE);
	this->scaleYSlider->maximum(MINSCALE);
	this->scaleYSlider->box(FL_PLASTIC_DOWN_BOX);
	this->scaleYSlider->color((Fl_Color)30);
	this->scaleYSlider->step(0.1);
	this->scaleYSlider->align(Fl_Align(FL_ALIGN_TOP));
	this->scaleYSlider->callback(SceneGraphWindow::transformCB, this);

	scaleG->end();
	return scaleG;
}

/* Creates the group of transformations */
Fl_Group* SceneGraphWindow::makeTransformG(const int x, const int y)
{
	/* Make a new group for the transformation widgets */
	Fl_Group* transformG = new Fl_Group(x, y, 370, 390, "Transformations");
	transformG->box(FL_PLASTIC_DOWN_FRAME);
	transformG->color(FL_GRAY0);

	/* Make the scale group */
	Fl_Group* scaleG = makeScaleG(x + 10, y + 210);

	/* Make the Rotation dial */
	this->rotationDial = new Fl_Dial(x+250, y+150, 105, 105, "Rotation");
	this->rotationDial->align(Fl_Align(FL_ALIGN_TOP));
	this->rotationDial->callback(SceneGraphWindow::transformCB, this);
	this->rotationDial->minimum(0);
	this->rotationDial->maximum(360);
	this->rotationDial->angle1(270);
	this->rotationDial->angle2(-90);
	this->rotationDial->step(1);

	/* Make the translation group */
	Fl_Group* translateG = makeTranslationG(x + 10, y + 20);
    
	/* Return the new group of transformation widgets */
	transformG->end();
	return transformG;
}

/* Callback function for the entire transformation group */
void SceneGraphWindow::transformCB(Fl_Widget *w, void *data)
{
	SceneGraphWindow* sgWin = (SceneGraphWindow*)data;

	/* Create a scale matrix from the x and y sliders' values */
	mat3 scale = mat3::scale2D(
		(float)(sgWin->scaleXSlider->value()), 
		(float)(sgWin->scaleYSlider->value()));
	
	/* Create a rotation matrix from the dials' value */
	mat3 rotate = mat3::rotation2D((float)(sgWin->rotationDial->value()));
	
	/* Create a translation matrix from the x and y sliders' values */
	mat3 translate = mat3::translation2D(
		(float)(sgWin->translateXSlider->value()), 
		(float)(sgWin->translateYSlider->value()));
	
	sgWin->activeNode->setTransformation(scale, rotate, translate);

	sgWin->glWin->redraw();
}

/* Calback function for changing the active items label. */
void SceneGraphWindow::changeItemLabelCB(Fl_Widget *w, void *data)
{
	const char* new_label = ((Fl_Input*)w)->value();
	((SceneGraphWindow*)data)->activeItem->label(new_label);
	((SceneGraphWindow*)data)->treeView->redraw();
}

/* Creates the tree view of the scene graph */
Fl_Tree* SceneGraphWindow::makeTree(const int x, const int y)
{
	Fl_Tree* tree = new Fl_Tree(x, y, 190, 350, "Nodes");
	tree->showroot(0);
	
	tree->color((Fl_Color)55);
	tree->selection_color((Fl_Color)183);
	tree->callback(SceneGraphWindow::treeCB, this);
	tree->box(FL_PLASTIC_DOWN_BOX);
	
	return tree;
}

/* Adds the children to the tree view */
void SceneGraphWindow::addChildrenToTree()
{
	Fl_Tree_Item* temp;
	std::list<Node*>::const_iterator it = ((this->sceneGraph)->getChildren())->
	cbegin(); 

	temp = this->treeView->add("Body");
	temp->select(1);
	temp->user_data(this->sceneGraph);
	this->activeItem = temp;
	this->activeNode = this->sceneGraph;

	temp = this->treeView->add("Body/Head");
	temp->user_data(*it++);
	
	temp = this->treeView->add("Body/Limb1Upper");
	temp->user_data(*it);
	temp = this->treeView->add("Body/Limb1Upper/Limb1Lower");
	temp->user_data((*it++)->getChildren()->front());
	
	temp = this->treeView->add("Body/Limb2Upper");
	temp->user_data(*it);
	temp = this->treeView->add("Body/Limb2Upper/Limb2Lower");
	temp->user_data((*it++)->getChildren()->front());
	
	temp = this->treeView->add("Body/Limb3Upper");
	temp->user_data(*it);
	temp = this->treeView->add("Body/Limb3Upper/Limb3Lower");
	temp->user_data((*it++)->getChildren()->front());
	
	temp = this->treeView->add("Body/Limb4Upper");
	temp->user_data(*it);
	temp = this->treeView->add("Body/Limb4Upper/Limb4Lower");
	temp->user_data((*it++)->getChildren()->front());
	
	temp = this->treeView->add("Body/Tail");
	temp->user_data(*it++);
}

/* Callback function for the tree view of the scene graph */
void SceneGraphWindow::treeCB(Fl_Widget *w, void *data)
{
	SceneGraphWindow* sgWin = (SceneGraphWindow*)data;

	sgWin->activeItem = ((Fl_Tree*)w)->callback_item();
	sgWin->activeNode = (Node*)((((Fl_Tree*)w)->callback_item())->user_data());

	sgWin->setItemNameInput();
	sgWin->setTransformationG();
	sgWin->setColorChooser();

	sgWin->transformationG->redraw();
	sgWin->colorChooser->redraw();
	sgWin->itemNameInput->redraw();
}

/* Sets the values of the transformation widgets to */
/* the activeNode's values.                         */
void SceneGraphWindow::setTransformationG()
{
	/* Activate the transformationG */
	this->transformationG->activate();

	/* Set the X scale slider */
	this->scaleXSlider->value(this->activeNode->getScaleX());
	/* Set the Y scale slider */
	this->scaleYSlider->value(this->activeNode->getScaleY());

	/* Set the values of the rotation dial */
	float activeRoation = this->activeNode->getRotation();
	this->rotationDial->value(activeRoation >= 0 ? activeRoation:activeRoation+360);

	/* Set the X scale slider */
	this->translateXSlider->value(this->activeNode->getTranslationX());
	/* Set the Y scale slider */
	this->translateYSlider->value(this->activeNode->getTranslationY());
}

/* Sets the values of the color selection widgets to */
/* the activeNode's color values.                    */
void SceneGraphWindow::setColorChooser()
{
	if(this->activeNode->getGeometry() != NULL)
	{
		this->colorChooser->activate();
		const std::vector<float> activeColor = this->activeNode->getColors();
		this->colorChooser->rgb(activeColor[0], activeColor[1], activeColor[2]);
	}
	else
	{
		this->colorChooser->deactivate();
	}
}

/* Sets the value of the rename item input to the */
/* active item's label.                           */
void SceneGraphWindow::setItemNameInput()
{
	this->itemNameInput->activate();
	this->itemNameInput->value(this->activeItem->label());
}

/* Creates a scene graph of a kitten */
Node* SceneGraphWindow::createAnimalSceneGraph()
{
	/********************/
	/********BODY********/
	/********************/

	std::list<vec3> *bVertices = new std::list<vec3>();
	bVertices->push_back(vec3(   1,    0, 1));
	bVertices->push_back(vec3( 0.5,   -1, 1));
	bVertices->push_back(vec3(-0.5,   -1, 1));
	bVertices->push_back(vec3(  -1,    0, 1));
	bVertices->push_back(vec3(-0.5, 0.25, 1));
	bVertices->push_back(vec3( 0.5, 0.25, 1));

	polygon* body = new polygon(bVertices, 1, 0, 0.647f);
	/* Root Scale */
	mat3 bScale = mat3::scale2D(2, 1.65f);
	/* Root Rotation */
	mat3 bRotation = mat3::rotation2D(30);
	/* Root Translate */
	mat3 bTranslate = mat3::translation2D(-1, 4);
	Node* root = new Node(bScale, bRotation,
						  bTranslate, body);

	/********************/
	/********HEAD********/
	/********************/
	std::list<vec3> *hVertices = new std::list<vec3>();
	hVertices->push_back(vec3(0.5, 0, 1));
	hVertices->push_back(vec3(  0, 0, 1));
	hVertices->push_back(vec3(  0, 1, 1));

	triangle* head = new triangle(hVertices, 1.0, 0.0, 1.0);

	/* Head Scale */
	mat3 hScale = mat3::scale2D(2, .75);
	/* Head Rotation */
	mat3 hRotation = mat3::rotation2D(-15);
	/* Head Translate */
	mat3 hTranslate = mat3::translation2D(1, 0);
	Node* hNode = new Node(hScale, hRotation,
						   hTranslate, head);


	/***************************/
	/********UPPER LIMBS********/
	/***************************/
	std::list<vec3>* upperLimb = new std::list<vec3>();
	upperLimb->push_back(vec3( 0.25,  0, 1));
	upperLimb->push_back(vec3( 0.25, -1, 1));
	upperLimb->push_back(vec3(-0.25, -1, 1));
	upperLimb->push_back(vec3(-0.25,  0, 1));

	/*** Limb 1 Upper ***/
	quad* limb1upper = new quad(upperLimb, 1.0, 0.0, 0.0);
	mat3 limb1upperScale = mat3::scale2D(.5, 1);
	/* Limb 1 Upper Rotation */
	mat3 limb1upperRotate = mat3::rotation2D(60);
	mat3 limb1upperTranslate = mat3::translation2D(0.75, -0.5);
	Node* frontOutterLeg = new Node(limb1upperScale, limb1upperRotate,
									limb1upperTranslate, limb1upper);

	/*** Limb 2 Upper ***/
	quad* limb2upper = new quad(upperLimb, 1.0, 0.0, 0.0);
	mat3 limb2upperScale = mat3::scale2D(.5, 1);
	/* Limb 2 Upper Rotation */
	mat3 limb2upperRotate = mat3::rotation2D(-60);
	mat3 limb2upperTranslate = mat3::translation2D(-0.75, -0.5);
	Node* backOutterLeg = new Node(limb2upperScale, limb2upperRotate,
								   limb2upperTranslate, limb2upper);

	/*** Limb 3 Upper ***/
	quad* limb3upper = new quad(upperLimb, 0.0, 0.0, 1.0);
	mat3 limb3upperScale = mat3::scale2D(.5, 1);
	/* Limb 3 Upper Rotation */
	mat3 limb3upperRotate = mat3::rotation2D(45);
	mat3 limb3upperTranslate = mat3::translation2D(0.625, -0.625);
	Node* frontInnerLeg = new Node(limb3upperScale, limb3upperRotate,
								   limb3upperTranslate, limb3upper);

	/*** Limb 4 Upper ***/
	quad* limb4upper = new quad(upperLimb, 0.0, 0.0, 1.0);
	mat3 limb4upperScale = mat3::scale2D(.5, 1);
	/* Limb 4 Upper Rotation */
	mat3 limb4upperRotate = mat3::rotation2D(-45);
	mat3 limb4upperTranslate = mat3::translation2D(-0.625, -0.625);
	Node* backInnerLeg = new Node(limb4upperScale, limb4upperRotate,
								  limb4upperTranslate, limb4upper);


	/***************************/
	/********LOWER LIMBS********/
	/***************************/
	std::list<vec3>* lowerLimb = new std::list<vec3>();
	lowerLimb->push_back(vec3( 0.25,    0, 1));
	lowerLimb->push_back(vec3( 0.25, -0.5, 1));
	lowerLimb->push_back(vec3(-0.25, -0.5, 1));
	lowerLimb->push_back(vec3(-0.25,    0, 1));

	/*** Limb 1 Lower ***/
	quad* limb1lower = new quad(lowerLimb, 1.0, 1.0, 0.0);
	mat3 limb1lowerScale = mat3::scale2D(.5, 1.5);
	/* Limb 1 Lower Rotation */
	mat3 limb1lowerRotate = mat3::rotation2D(60);
	mat3 limb1lowerTranslate = mat3::translation2D(0, -1);
	Node* frontOutterFoot = new Node(limb1lowerScale, limb1lowerRotate,
									 limb1lowerTranslate, limb1lower);
	(*frontOutterLeg).addChild(frontOutterFoot);

	/*** Limb 2 Lower ***/
	quad* limb2lower = new quad(lowerLimb, 1.0, 1.0, 0.0);
	mat3 limb2lowerScale = mat3::scale2D(.5, 2);
	/* Limb 2 Lower Rotation */
	mat3 limb2lowerRotate = mat3::rotation2D(90);
	mat3 limb2lowerTranslate = mat3::translation2D(0, -1);
	Node* backOutterFoot = new Node(limb2lowerScale, limb2lowerRotate,
									limb2lowerTranslate, limb2lower);
	(*backOutterLeg).addChild(backOutterFoot);

	/*** Limb 3 Lower ***/
	quad* limb3lower = new quad(lowerLimb, 0.0, 1.0, 1.0);
	mat3 limb3lowerScale = mat3::scale2D(.5, 1.5);
	/* Limb 3 Lower Rotation */
	mat3 limb3lowerRotate = mat3::rotation2D(60);
	mat3 limb3lowerTranslate = mat3::translation2D(0, -1);
	Node* frontInnerFoot = new Node(limb3lowerScale, limb3lowerRotate,
									limb3lowerTranslate, limb3lower);
	(*frontInnerLeg).addChild(frontInnerFoot);

	/*** Limb 4 Lower ***/
	quad* limb4lower = new quad(lowerLimb, 0.0, 1.0, 1.0);
	mat3 limb4lowerScale = mat3::scale2D(.5, 2);
	/* Limb 4 Lower Rotation */
	mat3 limb4lowerRotate = mat3::rotation2D(90);
	mat3 limb4lowerTranslate = mat3::translation2D(0, -1);
	Node* backInnerFoot = new Node(limb4lowerScale, limb4lowerRotate,
								   limb4lowerTranslate, limb4lower);
	(*backInnerLeg).addChild(backInnerFoot);

	/********************/
	/********TAIL********/
	/********************/

	std::list<vec3>* tVertices = new std::list<vec3>();
	tVertices->push_back(vec3(   0,   0, 1));
	tVertices->push_back(vec3(-0.5,   0, 1));
	tVertices->push_back(vec3(-0.5, 0.5, 1));
	tVertices->push_back(vec3(  -1, 0.5, 1));

	polyline* tail = new polyline(tVertices, 0.647f, 0.1647f, 0.1647f);

	/* Tail Scale */
	mat3 tScale = mat3::scale2D(1, 1);
	/* Tail Rotation */
	mat3 tRotation = mat3::rotation2D(-30);
	/* Tail Translate */
	mat3 tTranslate = mat3::translation2D(-1, 0);
	Node* tNode = new Node(tScale, tRotation,
						   tTranslate, tail);

	/* Add parts to the body */
	(*root).addChild(hNode);
	(*root).addChild(frontInnerLeg);
	(*root).addChild(frontOutterLeg);
	(*root).addChild(backInnerLeg);
	(*root).addChild(backOutterLeg);
	(*root).addChild(tNode);

	return root;
}