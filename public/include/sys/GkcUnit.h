/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
This file contains macros and helper functions to make it easier to write tests.

Author: Lijuan Mei
*/

/*
These are the basic macros you will use for creating tests and doing setup/teardown.
 
Example:

In unit test cpp file:

// define static variables for this file
class XXXTest
{
public:
	static int l_myNumber;
};
int XXXTest::l_myNumber = 0;

GKC_FIXTURE(MyFixture);  // declares a fixture

GKC_SETUP(MyFixture)
{
    // Do some stuff here that you want done before a test starts. It can include accessing static variables.
	// An example would be creating a file (and then deleting it in TEARDOWN)
    XXXTest::l_myNumber = 5;
}

GKC_TEARDOWN(MyFixture)
{
    // Undo whatever you did in the SETUP.
    XXXTest::l_myNumber = 0;
}

// This test uses a fixture (the second argument).
// Setup and Teardown will be run before and after the body of the test, respectively.
GKC_BEGIN_TESTF(MyTestWithFixture, MyFixture)
{
    // Do some tests.
	// You can assume l_myNumber is 5.
    GKC_ASSERT(5 == XXXTest::l_myNumber);
}
GKC_END_TESTF

GKC_BEGIN_TEST(MyTestWithoutFixtures)
{
    // Do some tests.
	// Do not rely on any variables set in the setup or teardown.
}
GKC_END_TEST
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_UNIT_H__
#define __GKC_UNIT_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_COLL_H__
	#error GkcUnit.h requires GkcColl.h to be included first.
#endif

#ifndef __GKC_STRING_H__
	#error GkcUnit.h requires GkcString.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_UNIT_H__
////////////////////////////////////////////////////////////////////////////////
