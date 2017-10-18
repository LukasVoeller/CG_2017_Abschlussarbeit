#include "vector.h"
#include <assert.h>
#include <math.h>

Vector::Vector( float x, float y, float z)
{
    X = x;
    Y = y;
    Z = z;
}

Vector::Vector()
{
    // Set all to 0
    //X = 0.0f;
    //Y = 0.0f;
    //Z = 0.0f;
}

/* angle between two vectors
 if result = 0: 90° (orthogonal) */
float Vector::dot(const Vector& v) const
{
    return X*v.X + Y*v.Y + Z*v.Z;;
}

Vector Vector::cross(const Vector& v) const
{
    float xCross = Y * v.Z - Z * v.Y;
    float yCross = Z * v.X - X * v.Z;
    float zCross = X * v.Y - Y * v.X;
    return Vector(xCross, yCross, zCross);
}


Vector Vector::operator+(const Vector& v) const
{
    return Vector(X+v.X, Y+v.Y, Z+v.Z);
}

Vector Vector::operator-(const Vector& v) const
{
    return Vector(X-v.X, Y-v.Y, Z-v.Z);
}

Vector Vector::operator*(float c) const
{
    return Vector(X*c, Y*c, Z*c);
}

Vector Vector::operator-() const
{
    
    return Vector(X*(-1), Y*(-1), Z*(-1)); // dummy (remove)
}

Vector& Vector::operator+=(const Vector& v)
{
    X += v.X;
    Y += v.Y;
    Z += v.Z;
    return *this;
}

/* length = 1 */
Vector& Vector::normalize()
{
    float len = this->length();
    X = X/len;
    Y = Y/len;
    Z = Z/len;
    return *this;
}

float Vector::length() const
{
    return sqrtf(lengthSquared());
}

float Vector::lengthSquared() const
{
    return X*X + Y*Y + Z*Z;
}

/* Vector normal is vertical to the reflection area
 * entry angle == reflection angle
 * reflection vector = entry vector - 2 skalarprod(entry vector, normal vector) * normal vector / lengthsquared(normal vector)
 */
Vector Vector::reflection( const Vector& normal) const
{
    float normal_lengthSquared = normal.lengthSquared();
    float dotProd = this->dot(normal);
    Vector reflection = *this - normal * (2* dotProd/normal_lengthSquared);
    return reflection;
}

/* Check for collision of ray with triangle
 * 1. find point of intersection with plain of triangle
 2. check, if point is inside of triangle */
bool Vector::triangleIntersection( const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
    // calculate plain (normal vector)
    Vector normal = ((b-a).cross(c-a));
    normal = normal.normalize();
    
    // calculate the distance s
    // do i need to try catch this
    // E:x = normal (dot) (x - a) = 0
    // with a = reference point
    s = normal.dot(a - *this)/normal.dot(d);
    
    // wrong direction -> no intersection
    if (s < 0){
        return false;
    }
    
    // step 2
    Vector intersectionPoint = *this + d*s;
    float areaABC = ((b-a).cross(c-a)).length()/2;
    float areaABP = ((b-a).cross(intersectionPoint-a)).length()/2;
    float areaACP = ((c-a).cross(intersectionPoint-a)).length()/2;
    float areaBCP = ((c-b).cross(intersectionPoint-b)).length()/2;
    
    double epsilon = 1e-6;
    
    if(areaABC + epsilon >= areaABP + areaACP + areaBCP)
    {
        return true;
    }
    return false;
}

void Vector::debugOutput()
{
    std::cout << X << " " << Y << " " << Z << std::endl;
}
