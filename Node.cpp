/*
 * Node.cpp
 * Created by Zachary Ferguson
 * Source file for the SceneGraph class, a class for creating a Scene Graph
 */

#include "Node.h"

/* Constructor for a Node that takes three mat3's for the transformations */
Node::Node(mat3 scale, mat3 rotation, mat3 translation) :
	Node(scale, rotation, translation, (polyline*)NULL){}

/* Constructor for a Node that takes three mat3's for the transformations */
/* and the geometry to point.                                             */
Node::Node(mat3 scale, mat3 rotation, mat3 translation, polyline* geometry)
{
	/* Initialize the children to a new list */
	this->children = new std::list<Node*>();

	/* Initialize the geometry to the given value */
	this->geometry = geometry;

	/* Initialize the parent Node to NULL */
	this->parent = NULL;	
	
	/* Initialize the transform to the product of the individual */
	/* transformations.                                          */
	this->frames = new std::vector<Frame*>();
	if(this->geometry != NULL)
	{
		std::vector<float> colors = this->geometry->getColor();
		this->frames->push_back(new Frame(scale, rotation, translation,
			colors[0], colors[1], colors[2], true));
	}
	else
	{
		this->frames->push_back(new Frame(scale, rotation, translation, true));
	}
}

/* Destructor for this node.          */
/* Deletes all of the children Nodes. */
Node::~Node()
{
	/* Deletes the contents of the vector of Frames */
	this->frames->clear();
	/* Delete the vector of Frames */
	delete this->frames;
	/* Deletes all the children, calling the destructor for each child node. */
	(this->children)->clear();
	///* Delete the geometry pointer */
	if (this->geometry != NULL && this->geometry->getVertices() != NULL)
	{
		delete this->geometry;
		this->geometry = NULL;
	}
}

/* Compares if too given Nodes are equal. Compares by geometry and transform. */
bool operator==(const Node& n1, const Node& n2)
{
	/* Compare geometry and transforms vector size */
	if(n1.geometry != n2.geometry || (n1.frames)->size() != (n2.frames)->size())
	{
		return false;
	}

	/* Compare transform matrices */
	for (unsigned int i = 0; i < (n1.frames)->size(); i++)
	{
		if((*(n1.frames))[i] != (*(n2.frames))[i])
		{
			return false;
		}
	}

	return true;
}

/* Add the given Node to the list of children */
void Node::addChild(Node* newNode)
{
	if (newNode != NULL)
	{
		newNode->parent = this;
		(this->children)->push_back(newNode);
	}
}

/* Removes the given Node from the list of children */
void Node::removeChild(Node* rNode)
{
	for (std::list<Node*>::iterator it = this->children->begin(); it != 
		this->children->end(); ++it)
	{
		if (*(*it) == *rNode)
		{
			(this->children)->erase(it);
			return;
		}
	}
}

/* Returns the first transform mat3 */
const mat3 Node::getTransformation() const
{
	return this->getTransformation(0);
}

/* Returns the nth transform mat3 */
const mat3 Node::getTransformation(unsigned int n) const
{
	/* Check the bounds of the transforms vector */
	assert(n < this->frames->size());
	
	return ((*(this->frames))[n])->getTransformation();
}

/* Sets the first transform mat3 */
void Node::setTransformation(mat3 scale, mat3 rotation, mat3 translation)
{
	this->setTransformation(scale, rotation, translation, 0);
}

/* Sets the nth transform mat3 */
void Node::setTransformation(mat3 scale, mat3 rotation, mat3 translation, unsigned 
	int n)
{
	/* Check the bounds of the transforms vector */
	assert(n < this->frames->size());

	std::vector<float> colors = this->getColors(n);
	Frame* newFrame = new Frame(
		scale, rotation, translation,
		colors[0], colors[1], colors[2],
		true
	);

	(*(this->frames))[n] = newFrame;
}

/* Returns a constant reference to the the geometry */
const polyline* Node::getGeometry() const
{
	if(this->geometry != NULL)
		return this->geometry;
	return (polyline*)NULL;
}

/* Sets the geometry */
void Node::setGeometry(polyline* newGeometry)
{
	this->geometry = newGeometry;
}

/* Method for getting the colors of the stored geometry. */
/* Returns a vector of all zeros if geometry == NULL.    */
const std::vector<float> Node::getColors() const
{
	return this->getColors(0);
}

/* Method for getting the colors of the stored geometry. */
/* Returns a vector of all zeros if geometry == NULL.    */
const std::vector<float> Node::getColors(unsigned int frameNum) const
{
	assert(frameNum < this->frames->size());
	return this->frames->at(frameNum)->getColors();
}

/* Sets the geometry color */
void Node::setGeometryColor(const float newRed, const float newGreen, const 
	float newBlue)
{
	this->setGeometryColor(newRed, newGreen, newBlue, 0);
}

/* Sets the geometry color */
void Node::setGeometryColor(const float newRed, const float newGreen, const 
	float newBlue, unsigned int frameNum)
{
	assert(frameNum < this->frames->size());
	if(this->geometry != NULL)
	{
		/* Create a copy of the startIndex frame that is not a keyframe */
		Frame* newFrame = new Frame(
			mat3::scale2D(this->getScaleX(frameNum), this->getScaleY(frameNum)),

			mat3::rotation2D(this->getRotation(frameNum)),

			mat3::translation2D(this->getTranslationX(frameNum), 
				this->getTranslationY(frameNum)),

			newRed, newGreen, newBlue,

			true
		);

		(*(this->frames))[frameNum] = newFrame;
	}
}

/* Returns the list of children Node pointers */
const std::list<Node*>* Node::getChildren() const
{
	return this->children;
}

/* Returns a pointer to the parent Node */
Node* Node::getParent()
{
	return this->parent;
}

/* Methods for accessing the individual transformation values. */
/* Default frameNum to 0.                                      */
const float Node::getScaleX() const
{
	return this->getScaleX(0);
}
const float Node::getScaleY() const
{
	return this->getScaleY(0);
}
const float Node::getRotation() const
{
	return this->getRotation(0);
}
const float Node::getTranslationX() const
{
	return this->getTranslationX(0);
}
const float Node::getTranslationY() const
{
	return this->getTranslationY(0);
}

/* Methods for accessing the individual transformations */
const float Node::getScaleX(unsigned int  frameNum) const
{
	assert(frameNum < this->frames->size());
	return this->frames->at(frameNum)->getScaleX();
}

const float Node::getScaleY(unsigned int  frameNum) const
{
	assert(frameNum < this->frames->size());
	return this->frames->at(frameNum)->getScaleY();
}

const float Node::getRotation(unsigned int  frameNum) const
{
	assert(frameNum < this->frames->size());
	return this->frames->at(frameNum)->getRotation();
}

const float Node::getTranslationX(unsigned int  frameNum) const
{
	assert(frameNum < this->frames->size());
	return this->frames->at(frameNum)->getTranslationX();
}

const float Node::getTranslationY(unsigned int  frameNum) const
{
	assert(frameNum < this->frames->size());
	return this->frames->at(frameNum)->getTranslationY();
}



/* Traverse the scene graph applying transforms and drawing geometry. */
/* Must give a Node and the accumalated transformation.               */
void Node::traverseSceneGraph(const Node& n, mat3 transformation)
{
	traverseSceneGraph(n, transformation, 0);
}

/* Traverse the scene graph applying transforms and drawing geometry. */
/* Must give a Node, the accumalated transformation, and the          */
/* transformation "frame" number.                                     */
void Node::traverseSceneGraph(const Node& n, mat3 transformation, unsigned int 
	transformNum)
{
	/* Checks that the given Node is not Null */
	if(&n != NULL)
	{
		/* Multiplies the given transform and the Nodes transform mat3 */
		transformation = transformation * n.getTransformation(transformNum);

		/* If the Node points to geometry draw it */
		if(n.getGeometry() != NULL)
		{
			/* Draw the geometry */
			std::vector<float> colors = n.getColors(transformNum);
			n.geometry->setColor(colors[0], colors[1], colors[2]);
			n.geometry->draw(transformation);
		}

		/* Iterate over the children Nodes and traverse them */
		for (std::list<Node*>::const_iterator it = (n.getChildren())->cbegin(); 
			it != (n.getChildren())->end(); ++it)
		{
			/* Recursive call to traverse the child Node */
			traverseSceneGraph(*(*it), transformation, transformNum);
		}
	}
}

/* Copies the nth transform out to until the transforms vector equals */
/* the given size. Must send the index of the transform to be copied  */
/* and the new size of the transforms vector.                         */
void Node::expandTransforms(unsigned int frameNum, unsigned int size)
{
	/* Ensure the transformIndex is valid. */
	assert(frameNum < this->frames->size());

	/* Create a copy of the startIndex frame that is not a keyframe */
	std::vector<float> colors = this->getColors(frameNum);
	Frame* copy = new Frame(
		mat3::scale2D(this->getScaleX(frameNum), this->getScaleY(frameNum)),

		mat3::rotation2D(this->getRotation(frameNum)),

		mat3::translation2D(this->getTranslationX(frameNum), 
			this->getTranslationY(frameNum)),

		colors[0], colors[1], colors[2]
	);

	/* Copy the choosen mat3 to all the new transformations. */
	for (unsigned int i = this->frames->size()-1; i < size; i++)
	{
		this->frames->push_back(copy);
	}

	/* Expand children's transforms vector. */
	for (std::list<Node*>::const_iterator it = this->children->cbegin(); 
			it != this->children->end(); it++)
	{
		(*it)->expandTransforms(frameNum, size);
	}
}

/* Shrinks the transforms vector down to the size given. Must send the */
/* new size of the transforms vector.                                  */
void Node::shrinkTransforms(unsigned int size)
{
	/* Remove transform until the sizes are equal */
	while(this->frames->size() != size)
	{
		this->frames->pop_back();
	}

	/* Shrink children's transforms vector. */
	for (std::list<Node*>::const_iterator it = this->children->cbegin(); 
			it != this->children->end(); it++)
	{
		(*it)->shrinkTransforms(size);
	}
}

/* Copies the nth transformation up to the next keyframe. Must send the index */
/* of the starting transform (where to start copying and where to copy from). */
void Node::copyTransforms(unsigned int startIndex)
{
	/* Ensure the transformIndex is valid. */
	assert(startIndex < this->frames->size());

	
	/* Create a copy of the startIndex frame that is not a keyframe */
	std::vector<float> colors = this->getColors(startIndex);
	Frame* copy = new Frame(
		mat3::scale2D(this->getScaleX(startIndex), this->getScaleY(startIndex)),

		mat3::rotation2D(this->getRotation(startIndex)),

		mat3::translation2D(this->getTranslationX(startIndex), 
			this->getTranslationY(startIndex)),

		colors[0], colors[1], colors[2]
	);

	/* Copy the choosen mat3 to all frames up till the next keyframe. */
	for (unsigned int i = startIndex+1; i < this->frames->size() && 
		!(this->frames->at(i)->getIsKeyframe()); i++)
	{
		(*(this->frames))[i] = copy;
	}
}

void Node::makeKeyframe(unsigned int frameNum)
{
	/* Ensure the transformIndex is valid. */
	assert(frameNum < this->frames->size());

	/* Create a copy of the startIndex frame that is not a keyframe */
	std::vector<float> colors = this->getColors(frameNum);
	Frame* frame = new Frame(
		mat3::scale2D(this->getScaleX(frameNum), this->getScaleY(frameNum)),

		mat3::rotation2D(this->getRotation(frameNum)),

		mat3::translation2D(this->getTranslationX(frameNum), 
			this->getTranslationY(frameNum)),

		colors[0], colors[1], colors[2],

		true
	);

	(*(this->frames))[frameNum] = frame;

	/* Expand children's transforms vector. */
	for (std::list<Node*>::const_iterator it = this->children->cbegin(); 
			it != this->children->end(); it++)
	{
		(*it)->makeKeyframe(frameNum);
	}
}


/* Interpolate frames between key frames */
void Node::linearlyInterpolate()
{
	std::vector<int> keyframes = std::vector<int>();

	/* Fill the vector of key frame indeces */
	for(unsigned int i = 0; i < this->frames->size(); i++)
	{
		if(this->frames->at(i)->getIsKeyframe())
			keyframes.push_back(i);
	}

	/* Interpolate all the frame values for non keyframes */
	for(unsigned int k = 0; k < keyframes.size()-1; k++)
	{

		int i = keyframes[k], f = keyframes[k+1];
		int indexDifference = f-i;
		Frame *iframe = this->frames->at(i), *fframe = this->frames->at(f);
	
		float scaleXConversionFactor = (fframe->getScaleX()-iframe->getScaleX())/
			indexDifference;
		float scaleYConversionFactor = (fframe->getScaleY()-iframe->getScaleY())/
			indexDifference;
	
		float rotationConversionFactor = (fframe->getRotation()-iframe->
			getRotation())/indexDifference;
	
		float translationXConversionFactor = (fframe->getTranslationX()-iframe->
			getTranslationX())/indexDifference;
		float translationYConversionFactor = (fframe->getTranslationY()-iframe->
			getTranslationY())/indexDifference;
	
		std::vector<float> icolors = iframe->getColors(), fcolors = fframe->
			getColors(); 
	
		float redConversionFactor = (fcolors[0]-icolors[0])/indexDifference;
		float greenConversionFactor = (fcolors[1]-icolors[1])/indexDifference;
		float blueConversionFactor = (fcolors[2]-icolors[2])/indexDifference;
	
		for(int n = keyframes[k]+1; n < keyframes[k+1]; n++)
		{
			(*(this->frames))[n] = new Frame( 
				mat3::scale2D
				(
					scaleXConversionFactor*(n-i)+iframe->getScaleX(),
					scaleYConversionFactor*(n-i)+iframe->getScaleY()
				),
				mat3::rotation2D
				(
					rotationConversionFactor*(n-i)+iframe->getRotation()
				),
				mat3::translation2D
				(
					translationXConversionFactor*(n-i)+iframe->getTranslationX(),
					translationYConversionFactor*(n-i)+iframe->getTranslationY()
				),
				redConversionFactor*(n-i)+icolors[0],
				greenConversionFactor*(n-i)+icolors[1],
				blueConversionFactor*(n-i)+icolors[2]
			);
		}
	}

	/* Interpolate children Nodes. */
	for (std::list<Node*>::const_iterator it = this->children->cbegin(); 
			it != this->children->end(); it++)
	{
		(*it)->linearlyInterpolate();
	}
}

/* Interpolate frames between key frames */
void Node::unInterpolate()
{
	std::vector<int> keyframes = std::vector<int>();

	/* Fill the vector of key frame indeces */
	for(unsigned int i = 0; i < this->frames->size(); i++)
	{
		if(this->frames->at(i)->getIsKeyframe())
			keyframes.push_back(i);
	}

	/* Copy over the keyframes */
	for(unsigned int k = 0; k < keyframes.size(); k++)
	{
		this->copyTransforms(keyframes[k]);
	}

	/* unInterpolate children Nodes. */
	for (std::list<Node*>::const_iterator it = this->children->cbegin(); 
			it != this->children->end(); it++)
	{
		(*it)->unInterpolate();
	}
}
