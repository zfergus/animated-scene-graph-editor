/*
 *  vec3.h
 */

#ifndef VEC3_H
#define VEC3_H

class vec3{
private:
	float data[3];
public:
	///----------------------------------------------------------------------
	/// Constructors
	///----------------------------------------------------------------------
	vec3();
	vec3(float x, float y, float z);
	
	///----------------------------------------------------------------------
	/// Getters/Setters
	///----------------------------------------------------------------------		
	/// Returns the value at index
	float operator[](unsigned int index) const;
	
	/// Returns a reference to the value at index
	float& operator[](unsigned int index);

	/// Prints the vector to standard output in a nice format
	void print();
	
	///----------------------------------------------------------------------
	/// Vector Operations
	///----------------------------------------------------------------------
	/// Returns the geometric length of the vector
	float length() const;
		
	///----------------------------------------------------------------------
	/// Friend Functions
	///----------------------------------------------------------------------		
	/// Checks if v1 == v2
	friend bool operator==(const vec3& v1, const vec3& v2);
	
	/// Checks if v1 != v2
	friend bool operator!=(const vec3& v1, const vec3& v2);
	
	/// Vector Addition (v1 + v2)
	friend vec3 operator+ (const vec3& v1, const vec3& v2);
	
	/// Vector Subtraction (v1 - v2)
	friend vec3 operator- (const vec3& v1, const vec3& v2);
	
	/// Scalar Multiplication (v * c)
	friend vec3 operator* (const vec3& v, float c);	
	
	/// Scalar Multiplication (c * v)
	friend vec3 operator* (float c, const vec3& v);
	
	/// Scalar Division (v/c)
	friend vec3 operator/ (const vec3& v, float c);
	
	/// Dot Product (v1 * v2)
	friend float operator* (const vec3& v1, const vec3& v2);
	
	/// Cross Product (v1 % v2)
	friend vec3 operator% (const vec3& v1, const vec3& v2);
};

#endif