/*
 * triangle.h
 * Created by Zachary Ferguson
 * Header file for the triangle class, a class for creating and drawing 
 * triangles
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

/* Include necessary types */
#include "polygon.h"
#include <assert.h>

class triangle : public polygon
{
	public:
	
		/* Constructor that takes a list of vectors for the vertices and the */
		/* color.                                                            */
		triangle(std::list<vec3>* vertices, float red, float green, float blue);
		
		/* Destructor for polylines */
		virtual ~triangle();
		
		/* Draws this polyline */
		virtual void draw(mat3 transformation) const;
		
		/* Sets the list of vertices to the new one */
		virtual void setVertices(std::list<vec3>* newVertices);

};
#endif