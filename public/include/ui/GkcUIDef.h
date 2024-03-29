﻿/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

/*
This file contains preprocessor directives used to help programming
for cross-platform.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_UI_DEF_H__
#define __GKC_UI_DEF_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_DEF_H__
	#error GkcUIDef.h requires GkcDef.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////

//basic
#include "system/basic_types.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//ColorQuad
/*! \brief A type for color quad.

A type for color quad.
\note The format of color is ARGB.
\verbatim
   ARGB
MSB    LSB
Little-endian : BGRA
Big-endian : ARGB
\endverbatim
*/
typedef color_quad  ColorQuad;

//------------------------------------------------------------------------------

//UiSize
typedef ui_size  UiSize;

//UiPoint
typedef ui_point  UiPoint;

//UiRect
typedef ui_rect  UiRect;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_UI_DEF_H__
////////////////////////////////////////////////////////////////////////////////
