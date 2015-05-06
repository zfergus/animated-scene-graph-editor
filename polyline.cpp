/*
 * polyline.cpp
 * Created by Zachary Ferguson
 * Source file for the polyline class, a class for creating and drawing 
 * polylines
 */

/* Include necessary types */
#include "polyline.h"

/* Constructor that takes a list of vectors for the vertices and the color */
polyline::polyline(std::list<vec3>* vertices, float red, float blue, 
	float green)
{
	this->vertices = vertices;
	(this->color)[0] = red;
	(this->color)[1] = green;
	(this->color)[2] = blue;
}

/* Destructor for polylines */
polyline::~polyline()
{
	delete this->vertices;
	this->vertices = NULL;
}

/* Compares if too given polylines are equal */
bool operator==(const polyline& p1, const polyline& p2)
{
	std::list<vec3>::const_iterator it1 = (p1.vertices)->begin();
	std::list<vec3>::const_iterator it2 = (p2.vertices)->begin();

	while (it1 != (p1.vertices)->end() && it2 != (p2.vertices)->end())
	{
		if (*it1 != *it2)
		{
			return false;
		}
		it1++;
		it2++;
	}
	if (it1 != (p1.vertices)->end() || it2 != (p2.vertices)->end())
		return false;
	return true;
}

/* Compares if too given polylines are not equal */
bool operator!=(const polyline& p1, const polyline& p2)
{
	return !(p1 == p2);
}

/* Set the color of this polygon to the given hex code. */
void polyline::setColor(const float newRed, const float newGreen,
	const float newBlue)
{
	(this->color)[0] = newRed;
	(this->color)[1] = newGreen;
	(this->color)[2] = newBlue;
}

/* Returns the RGB color value of this geometry as a vector of floats */
const std::vector<float> polyline::getColor() const
{
	return {this->color[0], this->color[1], this->color[2]};
}

/* Draws this polyline */
void polyline::draw(mat3 transformation) const
{
	/* Set the color */
	glColor3f((this->color)[0], (this->color)[1], (this->color)[2]);
	/* Draw the vertices */
	glBegin(GL_LINE_STRIP);
	for (std::list<vec3>::const_iterator it = (this->vertices)->cbegin(); it !=
		(this->vertices)->end(); ++it)
	{
		vec3 vertex = transformation * (*it);
		glVertex2f(vertex[0], vertex[1]);
	}
	glEnd();
}

/* Returns the list of vertices */
const std::list<vec3>* polyline::getVertices() const
{
	return this->vertices;
}

/* Sets the list of vertices to the new one */
void polyline::setVertices(std::list<vec3>* newVertices)
{
	this->vertices = newVertices;
}