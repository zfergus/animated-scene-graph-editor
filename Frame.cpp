/*
 * Frame.cpp
 * Created by Zachary Ferguson
 * Source file for the Frame class, a class for holding frame data
 */

#include "Frame.h"

/* Constructor for a Frame that takes a mat3 for the transformation */
/* and set the isKeyframe boolean value to false.                   */
Frame::Frame(mat3 scale, mat3 rotation, mat3 translation)
	: Frame(scale, rotation, translation, 1.0, 1.0, 1.0, false){}

/* Constructor for a Frame that takes a mat3 for the transformation */
/* and a boolean for if this frame is a keyframe.                   */
Frame::Frame(mat3 scale, mat3 rotation, mat3 translation, bool isKeyframe)
	: Frame(scale, rotation, translation, 1.0, 1.0, 1.0, isKeyframe){}

/* Constructor for a Frame that takes a mat3 for the transformation */
/* and the RGB values for the color of the frame.                   */
Frame::Frame(mat3 scale, mat3 rotation, mat3 translation, float red, float green,
	float blue) : Frame(scale, rotation, translation, red, green, blue, false){}

/* Constructor for a Frame that takes a mat3 for the transformation, the RGB */
/* values for the color of the frame, and a boolean for if this frame is a   */
/* keyframe.                                                                 */
Frame::Frame(mat3 scale, mat3 rotation, mat3 translation, float red, float green,
	float blue, bool isKeyframe)
{
	this->transformation =  translation * rotation * scale;
	this->extractTransformValues(scale, rotation, translation);
	this->isKeyframe = isKeyframe;
	this->color[0] = red;
	this->color[1] = green;
	this->color[2] = blue;
}

/* Destructor for the Frame */
Frame::~Frame()
{
	
}

/* Returns the mat3 transformation used for this frame. */
const mat3 Frame::getTransformation() const
{
	return this->transformation;
}

/* Sets this frames transformation mat3 to the product of the three  */
/* different transformation mat3's given.                            */
/* Must send a scale mat3, rotation mat3, and a translation mat3, in */
/* that order, for the new transformation.                           */
void Frame::setTransformation(mat3 scale, mat3 rotation, mat3 translation)
{
	this->transformation =  translation * rotation * scale;
	this->extractTransformValues(scale, rotation, translation);
}

/* Returns if this frame is a keyframe. */
bool Frame::getIsKeyframe() const
{
	return this->isKeyframe;
}

/* Sets wether or not this frame is a keyframe. */
void Frame::setIsKeyframe(bool isKeyframe)
{
	this->isKeyframe = isKeyframe;
}

/* Determine all of the transformation values from the given mat3's. */
void Frame::extractTransformValues(mat3 scale, mat3 rotation, mat3 translation)
{
	/* Determine all of the transformation values */
	this->scaleX = scale[0][0];
	this->scaleY = scale[1][1];
	this->rotationAngle = (180 * atan2(rotation[1][0], rotation[0][0]))/PI;
	this->translationX = translation[0][2];
	this->translationY = translation[1][2];
}

/** Methods for accessing the individual transformation values **/
const float Frame::getScaleX() const
{
	return this->scaleX;
}

const float Frame::getScaleY() const
{
	return this->scaleY;
}

const float Frame::getRotation() const
{
	return this->rotationAngle;
}

const float Frame::getTranslationX() const
{
	return this->translationX;
}

const float Frame::getTranslationY() const
{
	return this->translationY;
}

/* Method for getting the colors of the frame's geometry. */
/* Returns a vector of all ones if geometry == NULL.    */
const std::vector<float> Frame::getColors() const
{
	return {this->color[0], this->color[1], this->color[2]};
}
		
/* Sets the geometry color to the new RGB values */
void Frame::setGeometryColor(const float newRed, const float newGreen, const 
	float newBlue)
{
	(this->color)[0] = newRed;
	(this->color)[1] = newGreen;
	(this->color)[2] = newBlue;
}