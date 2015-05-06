/*
 * polyline.h
 * Created by Zachary Ferguson
 * Header file for the polyline class, a class for creating and drawing 
 * polylines
 */

#ifndef POLYLINE_H
#define POLYLINE_H

/* Include necessary types */
#include <list>
#include <vector>
#include "vec3.h"
#include "mat3.h"
#include <FL/Fl.H>
#include <FL/Gl.H>
#include <Fl/Fl_Gl_Window.H>

class polyline
{
	protected:
		
		/* List of the polygon's vertices */
		std::list<vec3> *vertices;
		
		/* Array of float values for RGB color of the polyline */
		float color[3];
	
	public:

		/* Constructor that takes a list of vectors for the vertices and the */
		/* color.                                                            */
		polyline(std::list<vec3>* vertices, float red, float green, float blue);
	
		/* Destructor for polylines */
		virtual ~polyline();
		
		/* Compares if too given polylines are equal */
		friend bool operator==(const polyline& p1, const polyline& p2);

		/* Compares if too given polylines are not equal */
		friend bool operator!=(const polyline& p1, const polyline& p2);

		/* Set the color of this polygon to the given hex code. */
		void setColor(const float newRed, const float newGreen,
			const float newBlue);

		/* Returns the RGB color value of this geometry as a vector of floats */
		const std::vector<float> getColor() const;
		
		/* Draws this polyline */
		virtual void draw(mat3 transformation) const;
		
		/* Returns the list of vertices */
		const std::list<vec3>* getVertices() const;
		
		/* Sets the list of vertices to the new one */
		virtual void setVertices(std::list<vec3>* newVertices);

};

#endif