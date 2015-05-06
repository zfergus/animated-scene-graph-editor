/*
 * Node.h
 * Created by Zachary Ferguson
 * Header file for the SceneGraph class, a class for creating a Scene Graph
 */

#ifndef NODE_H
#define NODE_H

/* Include necessary types */
#include <list>
#include <vector>
#include <assert.h>
#include "polyline.h"
#include "frame.h"

class Node
{
	private:

		/* Vector of Transformations matrices for this node. */
		std::vector<Frame*>* frames;

		/* List of pointers to Node children */
		std::list<Node*>* children;
	
		/* A constant polyline */
		polyline* geometry;

		/* Pointer to the parent of this Node */
		Node* parent;

	public:

		/* Constructor for a Node that takes three mat3's for the */
		/* transformations.                                       */
		Node(mat3 scale, mat3 rotation, mat3 translation);
	
		/* Constructor for a Node that takes three mat3's for the */
		/* transformations and the geometry to point.             */
		Node(mat3 scale, mat3 rotation, mat3 translation, polyline* geometry);

		/* Destructor for this node, deletes all of the children Nodes. */
		virtual ~Node();
	
		/* Compares if too given Nodes are equal */
		friend bool operator==(const Node& n1, const Node& n2);

		/* Add the given Node to the list of children */
		void addChild(Node* newNode);
	
		/* Removes the given Node from the list of children */
		void removeChild(Node* rNode);
	
		/* Returns the first transform mat3 */
		const mat3 getTransformation() const;

		/* Returns the nth transform mat3 */
		const mat3 getTransformation(unsigned int n) const;
	
		/* Sets the first transform mat3 */
		void setTransformation(mat3 scale, mat3 rotation, mat3 translation);

		/* Sets the nth transform mat3 */
		void setTransformation(mat3 scale, mat3 rotation, mat3 translation, 
			unsigned int n);

		/* Returns a constant reference to the the geometry */
		const polyline* getGeometry() const;

		/* Sets the geometry */
		void setGeometry(polyline* newGeometry);

		/* Method for getting the colors of the stored geometry. */
		/* Returns a vector of all zeros if geometry == NULL.    */
		const std::vector<float> getColors() const;	

		/* Method for getting the colors of the stored geometry. */
		/* Returns a vector of all zeros if geometry == NULL.    */
		const std::vector<float> getColors(unsigned int frameNum) const;	
		
		/* Sets the geometry color */
		void setGeometryColor(const float newRed, const float newGreen, const 
			float newBlue);

		/* Sets the geometry color */
		void setGeometryColor(const float newRed, const float newGreen, const 
			float newBlue, unsigned int frameNum);

		/* Returns the list of children Node pointers */
		const std::list<Node*>* getChildren() const;

		/* Returns a pointer to the parent Node */
		Node* getParent();

		/** Methods for accessing the individual transformation values. **/
		/* Default frameNum to 0. */
		const float getScaleX() const;
		const float getScaleY() const;
		const float getRotation() const;
		const float getTranslationX() const;
		const float getTranslationY() const;
		/* Must send frame number. */
		const float getScaleX(unsigned int frameNum) const;
		const float getScaleY(unsigned int frameNum) const;
		const float getRotation(unsigned int frameNum) const;
		const float getTranslationX(unsigned int frameNum) const;
		const float getTranslationY(unsigned int frameNum) const;



		/* Traverse the scene graph applying transforms and drawing geometry. */
		/* Must give a Node and the accumalated transformation.               */
		static void traverseSceneGraph(const Node& n, mat3 transformation);

		/* Traverse the scene graph applying transforms and drawing geometry. */
		/* Must give a Node, the accumalated transformation, and the          */
		/* transformation "frame" number.                                     */
		static void traverseSceneGraph(const Node& n, mat3 transformation, 
			unsigned int transformNum);

		/* Copies the nth transform out to until the transforms vector equals */
		/* the given size. Must send the index of the transform to be copied  */
		/* and the new size of the transforms vector.                         */
		void expandTransforms(unsigned int frameNum, unsigned int size);

		/* Shrinks the transforms vector down to the size given. Must send the */
		/* new size of the transforms vector.                                  */
		void shrinkTransforms(unsigned int size);

		/* Copies the nth transformation over till the end of the transforms */
		/* vector. Must send the index of the starting transform (where to   */
		/*start copying and where to copy from).                             */
		void copyTransforms(unsigned int startIndex);

		/* Makes the given transform a key frame and it children have the same */
		/* keyframe.                                                           */
		void makeKeyframe(unsigned int frameNum);

		/* Interpolate frames between key frames. */
		void linearlyInterpolate();
		/* Reset interpolated frames between key frames. */
		void unInterpolate();

};

#endif