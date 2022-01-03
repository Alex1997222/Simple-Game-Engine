
#pragma once

#include "ECommon.h"

namespace E3D
{
	template <class T> 
	const T& Min ( const T& a, const T& b ) {
		return (a<b)?a:b;
	}

	template <class T> 
	const T& Max ( const T& a, const T& b ) {
		return (b<a)?a:b;
	}


	// set the value of val between minV and maxV
	template<typename T>
	const T& Clamp(const T& val, const T& minV, const T& maxV)
	{
		return Min(Max(val, maxV), minV);
	}

	// Get translation transformation matrix
	void GetTranslateMatrix44(EMatrix44 &mat, EFloat x, EFloat y, EFloat z);

	// Get zoom transformation matrix
	void GetScaleMatrix44(EMatrix44 &mat, EFloat x, EFloat y, EFloat z);

	// Get the X axis rotation transformation matrix
    // The rotation parameter is angle, not radian
	void GetRotateMatrix44X(EMatrix44 &mat, EFloat degree);
	// Get the Y axis rotation transformation matrix
    // The rotation parameter is angle, not radian
	void GetRotateMatrix44Y(EMatrix44 &mat, EFloat degree);
	// Get the Z axis rotation transformation matrix
    // The rotation parameter is angle, not radian
	void GetRotateMatrix44Z(EMatrix44 &mat, EFloat degree);
	
	// Rotate along any axis
	// The rotation parameter is angle, not radian
	void GetRotateMatrix44(EMatrix44 &mat, const EVector4D &normal, EFloat degree);

	void GetVertex4DAddVector4D(const EVertex4D &v, const EVector4D& p, EVertex4D &result);

	// 3D vector mul 44 matrix
	void GetVector3DMulMatrix44(const EVector3D& vec, const EMatrix44&mat, EVector3D &result);
	// 4D vector mul 44 matrix
	void GetVector4DMulMatrix44(const EVector4D& vec, const EMatrix44&mat, EVector4D &result);

	void GetVertex4DMulMatrix44(const EVertex4D& vec, const EMatrix44&mat, EVertex4D &result);
}