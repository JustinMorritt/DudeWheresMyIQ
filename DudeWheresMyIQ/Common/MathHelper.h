//***************************************************************************************
// MathHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper math class.
//***************************************************************************************

#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <Windows.h>
#include <xnamath.h>
#include "xnacollision.h"

class MathHelper
{
public:
	// Returns random float in [0, 1).
	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF()*(b-a);
	}

	static int RandI(float a, float b)
	{
		return (int)(a + RandF()*(b - a));
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	 
	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b-a)*t;
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x); 
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static XMMATRIX InverseTranspose(CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		XMMATRIX A = M;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		XMVECTOR det = XMMatrixDeterminant(A);
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}

	static XMVECTOR RandUnitVec3();
	static XMVECTOR RandHemisphereUnitVec3(XMVECTOR n);

	static const float Infinity;
	static const float Pi;

	static XMFLOAT3 AABBCollision(XNA::AxisAlignedBox& E1, XNA::AxisAlignedBox& E2)
	{
		XMFLOAT3 Correction = { 0.0f, 0.0f, 0.0f };
		//HALF WIDTHS
		XMFLOAT3 e1 = E1.Extents, e2 = E2.Extents;
		XMFLOAT3 c1 = E1.Center,  c2 = E2.Center;

		//DISTANCE
		float distX = abs(c1.x - c2.x);
		float distY = abs(c1.y - c2.y);
		float distZ = abs(c1.z - c2.z);

		//OVERLAP
		float olX = (e1.x + e2.x) - distX;
		float olY = (e1.y + e2.y) - distY;
		float olZ = (e1.z + e2.z) - distZ;

		//CHECK LEAST OVERLAP
		if (abs(olX) < abs(olY) && abs(olX) < abs(olZ))
		{
			if (c1.x > c2.x) //Hit Right Side
			{
				Correction.x = olX;		return Correction;
			}
			else //Hit Left
			{
				Correction.x = -olX;	return Correction;
			}
		}
		else if (abs(olY) < abs(olX) && abs(olY) < abs(olZ))
		{
			if (c1.y > c2.y) //Hit Top
			{
				Correction.y = olY;		return Correction;
			}
			else // Hit Bottom
			{
				Correction.y = -olY;	return Correction;
			}
		}
		else if (abs(olZ) < abs(olY) && abs(olZ) < abs(olX))
		{
			if (c1.z < c2.z) //Hit Back
			{
				Correction.z = -olZ;	return Correction;
			}
			else // Hit Front
			{
				Correction.z = olZ;		return Correction;
			}
		}

		return Correction;
	}
};

#endif // MATHHELPER_H