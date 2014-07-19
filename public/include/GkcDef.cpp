/*
** Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
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
This file contains global variables for header file.
*/

////////////////////////////////////////////////////////////////////////////////

#include "GkcDef.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//static variables

const float Limits<float>::Lowest = -FLT_MAX;
const float Limits<float>::Min = FLT_MIN;
const float Limits<float>::Max = FLT_MAX;

const double Limits<double>::Lowest = -DBL_MAX;
const double Limits<double>::Min = DBL_MIN;
const double Limits<double>::Max = DBL_MAX;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
