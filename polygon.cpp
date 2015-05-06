/*
 * polygon.cpp
 * Created by Zachary Ferguson
 * Source file for the polygon class, a class for creating and drawing polygons
 */

/* Include necessary types */
#include "polygon.h"

/* Constructor that takes a list of vectors for the vertices and the color */
polygon::polygon(std::list<vec3>* vertices, float red, float green, 
	float blue) : polyline(vertices, red, green, blue){}

polygon::~polygon()
{
	delete this->vertices;
	this->vertices = NULL;
}

/* Draws this polygon */
void polygon::draw(mat3 transformation) const
{
	/* Set the color */
	glColor3f((this->color)[0], (this->color)[1], (this->color)[2]);
	/* Draw the vertices */
	glBegin(GL_POLYGON);
	for (std::list<vec3>::const_iterator it = (this->vertices)->begin(); it != 
		(this->vertices)->end(); ++it)
	{
		vec3 vertex = transformation * (*it);
		glVertex2f(vertex[0], vertex[1]);
	}
	glEnd();
}

/* Sets the list of vertices to the new one */
void polygon::setVertices(std::list<vec3>* newVertices)
{
	this->vertices = newVertices;
}
