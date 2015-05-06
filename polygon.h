/*
 * polygon.h
 * Created by Zachary Ferguson
 * Header file for the polyline class, a class for creating and drawing polygons
 */

#ifndef POLYGON_H
#define POLYGON_H

/* Include necessary types */
#include "polyline.h"

class polygon: public polyline
{
	public:
		
		/* Constructor that takes a list of vectors for the vertices and the */
		/* color.                                                            */
		polygon(std::list<vec3>* vertices, float red, float green, float blue);
		
		/* Destructor for polylines */
		virtual ~polygon();
		
		/* Draws this polyline */
		virtual void draw(mat3 transformation) const;
		
		/* Sets the list of vertices to the new one */
		virtual void setVertices(std::list<vec3>* newVertices);
};
#endif