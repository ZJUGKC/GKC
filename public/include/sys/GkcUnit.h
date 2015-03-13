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

The base/GkcBase.cpp must be included in cpp file.

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

// This is the max buffer length for the message assigned to an assert exception.
#define _GKC_MAX_ASSERT_MESSAGE_LENGTH  (1024)

// _UnitTestMessgeBuffer

typedef FixedStringT<CharS, _GKC_MAX_ASSERT_MESSAGE_LENGTH>  _UnitTestMessageBuffer;

// unit test function prototype

typedef bool (*_UnitTestFunc)(_UnitTestMessageBuffer& buffer);

// _UnitTestMap

class _UnitTestMap
{
private:
	typedef HashMap<StringS, _UnitTestFunc, StringHashTrait<StringS>, StringCompareTrait<StringS>>  mapClass;

public:
	struct ItemInfo
	{
		StringS             strName;  //test name
		_UnitTestFunc       pFunc;    //function
		mapClass::Iterator  iter;
	};

public:
	_UnitTestMap() throw() : m_map(MemoryHelper::GetCrtMemoryManager())
	{
	}

//methods
	void AddUnitTest(const ConstStringS& strName, _UnitTestFunc pFunc)
	{
		StringS strM;
		StringUtilHelper::MakeString(strName, strM);
		m_map.Insert(strM, pFunc);
	}

	uintptr GetCount() const throw()
	{
		return m_map.GetCount();
	}

	//enum
	bool EnumFirst(ItemInfo& info) const throw()
	{
		info.iter = m_map.GetBegin();
		return EnumNext(info);
	}
	bool EnumNext(ItemInfo& info) const throw()
	{
		if( info.iter == m_map.GetEnd() )
			return false;
		info.strName = info.iter.get_Value().get_First();
		info.pFunc   = info.iter.get_Value().get_Second();
		info.iter.MoveNext();
		return true;
	}

	//find
	_UnitTestFunc Find(const ConstStringS& strName) const
	{
		StringS strM;
		StringUtilHelper::MakeString(strName, strM);
		auto iter = m_map.Find(strM);
		if( iter == m_map.GetEnd() )
			return NULL;
		return iter.get_Value().get_Second();
	}

private:
	mapClass  m_map;
};

// _UnitTestMapHelper

class _UnitTestMapHelper
{
public:
	//get map
	BEGIN_NOINLINE
	static _UnitTestMap*& GetUnitTestMap()
	END_NOINLINE
	{
		static _UnitTestMap* l_unit_test_map = NULL;

		if( l_unit_test_map == NULL )
			l_unit_test_map = new _UnitTestMap;
		return l_unit_test_map;
	}
	//free
	static void FreeUnitTestMap()
	{
		_UnitTestMap*& map = GetUnitTestMap();
		if( map != NULL ) {
			delete map;
			map = NULL;
		}
	}
};

// _UnitTestMainHelper

class _UnitTestMainHelper
{
public:
	static int MainProcess(const GKC::ConstArray<GKC::ConstStringS>& args)
	{
		_UnitTestMessageBuffer buffer;

		_UnitTestMap* pMap = _UnitTestMapHelper::GetUnitTestMap();

		if( args.GetCount() <= 1 ) {
			//all tests
			_UnitTestMap::ItemInfo info;
			bool bContinue = pMap->EnumFirst(info);
			while( bContinue ) {
				if( !info.pFunc(buffer) ) {

				}
				bContinue = pMap->EnumNext(info);
			}
		}
		else {
			//specified tests
			for( auto iter = args.GetBegin(); iter != args.GetEnd(); iter.MoveNext() ) {
				_UnitTestFunc pFunc = pMap->Find(iter.get_Value());
				if( pFunc == NULL ) {
				}
				else {
					if( !pFunc(buffer) ) {

					}
				}
			}
		} //end if

		//free map
		_UnitTestMapHelper::FreeUnitTestMap();

		return 0;
	}
};

// for main function
#define UNIT_TEST_MAIN_PROCESS(args)  GKC::_UnitTestMainHelper::MainProcess(args)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_UNIT_H__
////////////////////////////////////////////////////////////////////////////////
