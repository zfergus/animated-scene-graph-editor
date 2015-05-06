/*
 * quad.h
 * Created by Zachary Ferguson
 * Header file for the quadrilateral class, a class for creating and drawing 
 * quadrilaterals
 */

#ifndef QUAD_H
#define QUAD_H

/* Include necessary types */
#include "polygon.h"
#include <assert.h>

class quad : public polygon
{
	public:
	
		/* Constructor that takes a list of vectors for the vertices and the */
		/* color.                                                            */
		quad(std::list<vec3>* vertices, float red, float green, float blue);
	
		/* Destructor for polylines */
		virtual ~quad();
		
		/* Draws this polyline */
		virtual void draw(mat3 transformation) const;
		
		/* Sets the list of vertices to the new one */
		virtual void setVertices(std::list<vec3>* newVertices);

};
#endif