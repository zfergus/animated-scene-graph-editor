/*
 * quad.cpp
 * Created by Zachary Ferguson
 * Source file for the quadrilateral class, a class for creating and drawing 
 * quadrilaterals
 */

/* Include necessary header */
#include "quad.h"

/* Constructor that takes a list of vectors for the vertices and the color */
quad::quad(std::list<vec3>* vertices, float red, float green, float blue) :
	polygon(vertices, red, green, blue)
{
	assert(vertices->size() == 4);
}

quad::~quad()
{
	delete this->vertices;
	this->vertices = NULL;
}

/* Draws this polyline */
void quad::draw(mat3 transformation) const
{
	/* Set the color */
	glColor3f((polyline::color)[0], (this->color)[1], (this->color)[2]);
	/* Draw the vertices */
	glBegin(GL_QUADS);
	for (std::list<vec3>::const_iterator it = (this->vertices)->begin(); it != 
		(this->vertices)->end(); ++it)
	{
		vec3 vertex = transformation * (*it);
		glVertex2f(vertex[0], vertex[1]);
	}
	glEnd();
}

/* Sets the list of vertices to the new one */
void quad::setVertices(std::list<vec3>* newVertices)
{
	assert(newVertices->size() == 4);
	this->vertices = newVertices;
}
