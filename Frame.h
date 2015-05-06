/*
 * Frame.h
 * Created by Zachary Ferguson
 * Header file for the Frame class, a class for holding frame data
 */

#ifndef FRAME_H
#define FRAME_H

/* Include necessary types */
#include "mat3.h"
#include <vector>
#include <cmath> /* Included for atan2 function */

#define PI 3.1415926535897f /* Math constant for angle conversions. */

class Frame
{
	private:
	
		/* The 3x3 matrix for this frames transformation */
		mat3 transformation;
		
		/* Boolean value for if this frame is a key frame */
		bool isKeyframe;
		
		/* Numerical values for the transformation.                     */
		/* These values are only stores never used in any computations. */
		float scaleX, scaleY, rotationAngle, translationX, translationY;

		/* Determine all of the transformation values from the given mat3's. */
		void extractTransformValues(mat3 scale, mat3 rotation, mat3 translation);

		/* Array of float values for the RGB of this frame's geometry. */
		float color[3];

	public:

		/* Constructor for a Frame that takes a mat3 for the transformation */
		/* and set the isKeyframe boolean value to false.                   */
		Frame(mat3 scale, mat3 rotation, mat3 translation);

		/* Constructor for a Frame that takes a mat3 for the transformation */
		/* and a boolean for if this frame is a keyframe.                   */
		Frame(mat3 scale, mat3 rotation, mat3 translation, bool isKeyframe);

		/* Constructor for a Frame that takes a mat3 for the transformation */
		/* and the RGB values for the color of the frame.                   */
		Frame(mat3 scale, mat3 rotation, mat3 translation, float red, float green,
			float blue);

		/* Constructor for a Frame that takes a mat3 for the transformation, */
		/* the RGB values for the color of the frame, and a boolean for if   */
		/* this frame is a keyframe.                                         */
		Frame(mat3 scale, mat3 rotation, mat3 translation, float red, float green,
			float blue, bool isKeyframe);

		/* Destructor for the Frame */
		virtual ~Frame();

		/* Returns the mat3 transformation used for this frame. */
		const mat3 getTransformation() const;
		
		/* Sets this frames transformation mat3 to the product of the three  */
		/* different transformation mat3's given.                            */
		/* Must send a scale mat3, rotation mat3, and a translation mat3, in */
		/* that order, for the new transformation.                           */
		void setTransformation(mat3 scale, mat3 rotation, mat3 translation);

		/* Returns if this frame is a keyframe. */
		bool getIsKeyframe() const;
		
		/* Sets wether or not this frame is a keyframe. */
		void setIsKeyframe(bool isKeyframe);

		/* Methods for accessing the individual transformation values */
		const float getScaleX() const;
		const float getScaleY() const;
		const float getRotation() const;
		const float getTranslationX() const;
		const float getTranslationY() const;

		/* Method for getting the colors of the frame's geometry. */
		/* Returns a vector of all ones if geometry == NULL.    */
		const std::vector<float> getColors() const;	
		
		/* Sets the geometry color to the new RGB values */
		void setGeometryColor(const float newRed, const float newGreen, const 
			float newBlue);

};

#endif