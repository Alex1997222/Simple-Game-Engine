#pragma once

#include "ECommon.h"
#include "EMath.h"
#include "EUtil.h"


namespace E3D
{
	// operation of vectors
	struct EVector2D
	{
		static EVector2D ZERO;
		static EVector2D UNIT_X;
		static EVector2D UNIT_Y;

		EFloat x;
		EFloat y;

		EVector2D() :x(0), y(0) {}
		EVector2D(EFloat x, EFloat y) :x(x), y(y) {}
		EVector2D(const EVector2D& v) :x(v.x), y(v.y) {}

		// normalize of vectors
		inline EFloat normalize()
		{
			EFloat len = length();
			if (len > EPSILON_E6)
			{
				EFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;

			}
			return len;
		}

		// dot product of two matrixes
		inline EFloat dotProduct(const EVector2D& right)const
		{
			return x * right.x + y * right.y;
		}

		//return the angle betweeb two products
		inline EFloat angleBetween(const EVector2D& right)const
		{
			EFloat lenProduct = length()* right.length();

			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;

			EFloat f = dotProduct(right) / lenProduct;
			f = Clamp(f, -1.0f, 1.0f);

			return acos(f);
		}

		// return the length of vector
		inline EFloat length()const 
		{
			return sqrt(x*x + y*y);
		}

		// return the square of vector length
		inline EFloat SquaredLength()const 
		{
			return x * x + y * y;
		}
		
		// multiply of the number of right
		inline EVector2D operator*(float right)const
		{
			return EVector2D(x*right, y*right);
		}

		// the dot product of two vectors
		inline EVector2D operator*(const EVector2D& right)const
		{
			return EVector2D(x*right.x, y *right.y);
		}
		
		// add of two vectors
		inline EVector2D operator+(const EVector2D& right)const
		{
			return EVector2D(x + right.x, y + right.y);
		}

		// substract of two vectors
		inline EVector2D operator-(const EVector2D& right)const
		{
			return EVector2D(x - right.x, y - right.y);
		}

		// reverse to negative
		inline EVector2D operator-()
		{
			x = -x;
			y = -y;
			return *this;
		}

		// reverse to positive
		inline EVector2D operator+()
		{
			x = +x;
			y = +y;
			return *this;
		}

		// judge whether two vectors are equal
		inline bool operator==(const EVector2D& right)const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y);
		}

		// vector assignment
		inline EVector2D operator=(const EVector2D& right)
		{
			x = right.x;
			y = right.y;
			return *this;
		}

	};


	// operation of 3d vector
	struct EVector3D
	{
		static EVector3D ZERO;
		static EVector3D UNIT_X;
		static EVector3D UNIT_Y;
		static EVector3D UNIT_Z;

		EFloat x;
		EFloat y;
		EFloat z;

		EVector3D() :x(0), y(0),z(0) {}
		EVector3D(EFloat x, EFloat y,EFloat z) :x(x), y(y),z(z) {}
		EVector3D(const EVector3D& v) :x(v.x), y(v.y),z(v.z) {}

		// dot product of 3d vector
		inline EFloat dotProduct(const EVector3D& right)const
		{
			return x * right.x+y*right.y+z*right.z;
		}

		// cross product of 3d vector
		inline EVector3D crossProduct(const EVector3D& right)const
		{
			return EVector3D(y*right.z - z * right.y,
							z*right.x - x * right.z,
							x*right.y - y * right.x);
		}


		// vector normalize
		inline EFloat normalize()
		{
			EFloat len = length();
			if (len > EPSILON_E6)
			{
				EFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
				z *= invLength;
			}
			return len;
		}

		// return the angle of two vectors
		inline EFloat angleBetween(const EVector3D& right)const
		{
			EFloat lenProduct = length()* right.length();

			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;

			EFloat f = dotProduct(right) / lenProduct;

			return acos(f);
		}



		// return the length of vector
		inline EFloat length()const
		{
			return sqrt(x*x + y * y+z*z);
		}

		// return the Squared length of vector
		inline EFloat SquaredLength()const
		{
			return x * x + y * y+z*z;
		}

		// multiply of the number of right
		inline EVector3D operator*(float right)const
		{
			return EVector3D(x*right, y*right,z*right);
		}

		// dot product of two 3d vectors
		inline EVector3D operator*(const EVector3D& right)const
		{
			return EVector3D(x*right.x, y *right.y,z*right.z);
		}

		// add of two 3d vectors
		inline EVector3D operator+(const EVector3D& right)const
		{
			return EVector3D(x + right.x, y + right.y,z+right.z);
		}

		// substract of two 3d vectors
		inline EVector3D operator-(const EVector3D& right)const
		{
			return EVector3D(x - right.x, y - right.y,z-right.z);
		}

		// reverse to negative
		inline EVector3D operator-()
		{
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}

		// reverse to postive
		inline EVector3D operator+()
		{
			x = +x;
			y = +y;
			z = +z;
			return *this;
		}

		// judge wether two vectors are equal
		inline bool operator==(const EVector3D& right)const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y)&&EqualFloat(z,right.z);
		}

		// vector assignment
		inline EVector3D operator=(const EVector3D& right)
		{
			x = right.x;
			y = right.y;
			z = right.z;

			return *this;
		}

	};


	// 4d vector operation
	struct EVector4D
	{
		static EVector4D ZERO;
		static EVector4D UNIT_X;
		static EVector4D UNIT_Y;
		static EVector4D UNIT_Z;

		EFloat x;
		EFloat y;
		EFloat z;
		EFloat w;

		EVector4D() :x(0), y(0), z(0), w(1) {}
		EVector4D(EFloat xf, EFloat yf, EFloat zf, EFloat wf = 1) :x(xf), y(yf), z(zf), w(wf) {}
		EVector4D(const EVector4D& v) :x(v.x), y(v.y), z(v.z), w(v.w) {}


		inline EFloat dotProduct(const EVector4D& right)const
		{
			return x * right.x + y * right.y + z * right.z;
		}

		inline EVector4D crossProduct(const EVector4D& right)const
		{
			return EVector4D(y*right.z - z * right.y,
				z*right.x - x * right.z,
				x*right.y - y * right.x, 1);
		}

		inline EFloat normalize()
		{
			EFloat len = length();
			if (len > EPSILON_E6)
			{
				EFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
				z *= invLength;
				w = 1;
			}
			return len;
		}

		inline bool isZero()const
		{
			return length() < EPSILON_E6;
		}

		inline EFloat angleBetween(const EVector4D& right)const
		{
			EFloat lenProduct = length()* right.length();

			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;

			EFloat f = dotProduct(right) / lenProduct;
			f = Clamp(f, -1.0f, 1.0f);

			return acos(f);
		}

		inline EFloat length()const
		{
			return sqrt(x*x + y * y + z * z);
		}

		inline EFloat SquaredLength()const
		{
			return x * x + y * y + z * z;
		}

		inline EVector4D operator*(float right)const
		{
			return EVector4D(x*right, y*right, z*right, 1);
		}

		inline EVector4D operator*(const EVector4D& right)const
		{
			return EVector4D(x*right.x, y *right.y, z*right.z, 1);
		}

		inline EVector4D operator+(const EVector4D& right)const
		{
			return EVector4D(x + right.x, y + right.y, z + right.z, 1);
		}

		inline EVector4D operator-(const EVector4D& right)const
		{
			return EVector4D(x - right.x, y - right.y, z - right.z, 1);
		}

		inline EVector4D operator-()
		{
			return EVector4D(-x, -y, -z, w);
		}

		inline bool operator==(const EVector4D& right)const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y) && EqualFloat(z, right.z) && EqualFloat(w, right.w);
		}

		inline EVector4D operator=(const EVector4D& right)
		{
			x = right.x;
			y = right.y;
			z = right.z;
			w = right.w;

			return *this;
		}

	};


}