// Copyright (C) 2003 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_

#include "Common.h"

#if !defined(ARM) && !defined(MIPS) && !defined(_XBOX)
#define USE_SSE
#endif

#ifdef USE_SSE
#include <xmmintrin.h>
#endif

#include <vector>

namespace MathUtil
{

static const u64 DOUBLE_SIGN = 0x8000000000000000ULL,
	DOUBLE_EXP  = 0x7FF0000000000000ULL,
	DOUBLE_FRAC = 0x000FFFFFFFFFFFFFULL,
	DOUBLE_ZERO = 0x0000000000000000ULL;

static const u32 FLOAT_SIGN = 0x80000000,
	FLOAT_EXP  = 0x7F800000,
	FLOAT_FRAC = 0x007FFFFF,
	FLOAT_ZERO = 0x00000000;

union IntDouble {
	double d;
	u64 i;
};
union IntFloat {
	float f;
	u32 i;
};

inline bool IsNAN(double d)
{
	IntDouble x; x.d = d;
	return ( ((x.i & DOUBLE_EXP) == DOUBLE_EXP) &&
			 ((x.i & DOUBLE_FRAC) != DOUBLE_ZERO) );
}

inline bool IsQNAN(double d)
{
	IntDouble x; x.d = d;
	return ( ((x.i & DOUBLE_EXP) == DOUBLE_EXP) &&
		     ((x.i & 0x0007fffffffffffULL) == 0x000000000000000ULL) &&
		     ((x.i & 0x000800000000000ULL) == 0x000800000000000ULL) );
}

inline bool IsSNAN(double d)
{
	IntDouble x; x.d = d;
	return( ((x.i & DOUBLE_EXP) == DOUBLE_EXP) &&
			((x.i & DOUBLE_FRAC) != DOUBLE_ZERO) &&
			((x.i & 0x0008000000000000ULL) == DOUBLE_ZERO) );
}

inline float FlushToZero(float f)
{
	IntFloat x; x.f = f;
	if ((x.i & FLOAT_EXP) == 0)
		x.i &= FLOAT_SIGN;  // turn into signed zero
	return x.f;
}

inline double FlushToZeroAsFloat(double d)
{
	IntDouble x; x.d = d;
	if ((x.i & DOUBLE_EXP) < 0x3800000000000000ULL)
		x.i &= DOUBLE_SIGN;  // turn into signed zero
	return x.d;
}

enum PPCFpClass
{
	PPC_FPCLASS_QNAN = 0x11,
	PPC_FPCLASS_NINF = 0x9,
	PPC_FPCLASS_NN   = 0x8,
	PPC_FPCLASS_ND   = 0x18,
	PPC_FPCLASS_NZ   = 0x12,
	PPC_FPCLASS_PZ   = 0x2,
	PPC_FPCLASS_PD   = 0x14,
	PPC_FPCLASS_PN   = 0x4,
	PPC_FPCLASS_PINF = 0x5,
};

// Uses PowerPC conventions for the return value, so it can be easily
// used directly in CPU emulation.
u32 ClassifyDouble(double dvalue);
// More efficient float version.
u32 ClassifyFloat(float fvalue);

template<class T>
struct Rectangle
{
	T left;
	T top;
	T right;
	T bottom;

	Rectangle()
	{ }

	Rectangle(T theLeft, T theTop, T theRight, T theBottom)
		: left(theLeft), top(theTop), right(theRight), bottom(theBottom)
	{ }

	T GetWidth() const { return abs(right - left); }
	T GetHeight() const { return abs(bottom - top); }

	// If the rectangle is in a coordinate system with a lower-left origin, use
	// this Clamp.
	void ClampLL(T x1, T y1, T x2, T y2)
	{
		if (left < x1) left = x1;
		if (right > x2) right = x2;
		if (top > y1) top = y1;
		if (bottom < y2) bottom = y2;
	}

	// If the rectangle is in a coordinate system with an upper-left origin,
	// use this Clamp.
	void ClampUL(T x1, T y1, T x2, T y2) 
	{
		if (left < x1) left = x1;
		if (right > x2) right = x2;
		if (top < y1) top = y1;
		if (bottom > y2) bottom = y2;
	}
};

}  // namespace MathUtil

inline float pow2f(float x) {return x * x;}
inline double pow2(double x) {return x * x;}
int Pow2roundup(int x);
int GetPow2(int x);

#endif // _MATH_UTIL_H_