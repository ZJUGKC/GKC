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
    // Do some tests.
	// You can assume l_myNumber is 5.
	bool bCheck = (5 == XXXTest::l_myNumber);
    GKC_ASSERT_TRUE(bCheck);
GKC_END_TESTF

GKC_BEGIN_TEST(MyTestWithoutFixtures)
    // Do some tests.
	// Do not rely on any variables set in the setup or teardown.
GKC_END_TEST
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_UNIT_H__
#define __GKC_UNIT_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_COLL_H__
	#error GkcUnit.h requires GkcColl.h to be included first.
#endif

#ifndef __GKC_CONSOLE_H__
	#error GkcUnit.h requires GkcConsole.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// This is the max buffer length for the message assigned to an assert exception.
#define _GKC_MAX_ASSERT_MESSAGE_LENGTH  (16*1024)

// _UnitTestMessgeBuffer

typedef FixedStringT<CharS, _GKC_MAX_ASSERT_MESSAGE_LENGTH>  _UnitTestMessageBuffer;

// _UnitTestAssertException

class _UnitTestAssertException
{
public:
	_UnitTestMessageBuffer& GetMessageBuffer() throw()
	{
		return m_buffer;
	}

private:
	_UnitTestMessageBuffer  m_buffer;
};

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

//_UnitTestReg

class _UnitTestReg
{
public:
	_UnitTestReg(const CharS* szName, _UnitTestFunc pFunc)
	{
		ConstStringS strName;
		ConstHelper::SetInternalPointer(szName, calc_string_length(szName), strName);
		_UnitTestMap* pMap = _UnitTestMapHelper::GetUnitTestMap();
		pMap->AddUnitTest(strName, pFunc);
	}
};

// _UnitTestMainHelper

class _UnitTestMainHelper
{
public:
	static int MainProcess(const GKC::ConstArray<GKC::ConstStringS>& args)
	{
		const CharS* l_szSep1   = _S("==========================");
		const CharS* l_szSep2   = _S("**************************");
		const CharS* l_szColon  = _S(":");
		const CharS* l_szNoTest = _S("ERROR: NO SUCH TEST!");

		_UnitTestMessageBuffer buffer;

		//map
		_UnitTestMap* pMap = _UnitTestMapHelper::GetUnitTestMap();
		uintptr uTotal  = 0;
		uintptr uFailed = 0;

		if( args.GetCount() <= 1 ) {
			//all tests
			_UnitTestMap::ItemInfo info;
			bool bContinue = pMap->EnumFirst(info);
			while( bContinue ) {
				//current test
				Console::WriteLine(ConstStringS(l_szSep1, 0));
				Console::Write(info.strName);
				Console::WriteLine(ConstStringS(l_szColon, 0));
				if( !info.pFunc(buffer) ) {
					Console::WriteLine(buffer);
					uFailed ++;
				}
				Console::WriteLine(ConstStringS(l_szSep2, 0));
				uTotal ++;
				bContinue = pMap->EnumNext(info);
			}
		}
		else {
			//specified tests
			auto iter = args.GetBegin();
			assert( iter != args.GetEnd() );
			iter.MoveNext();  //from 1
			for( ; iter != args.GetEnd(); iter.MoveNext() ) {
				//current test
				Console::WriteLine(ConstStringS(l_szSep1, 0));
				Console::Write(iter.get_Value());
				Console::WriteLine(ConstStringS(l_szColon, 0));
				_UnitTestFunc pFunc = pMap->Find(iter.get_Value());
				if( pFunc == NULL ) {
					Console::WriteLine(ConstStringS(l_szNoTest, 0));
					uFailed ++;
				}
				else {
					if( !pFunc(buffer) ) {
						Console::WriteLine(buffer);
						uFailed ++;
					}
				}
				Console::WriteLine(ConstStringS(l_szSep2, 0));
				uTotal ++;
			}
		} //end if

		//total information

		//free map
		_UnitTestMapHelper::FreeUnitTestMap();

		return 0;
	}
};

// for main function
#define UNIT_TEST_MAIN_PROCESS(args)  GKC::_UnitTestMainHelper::MainProcess(args)

// in cpp file

// _UnitTestBodyHelper

class _UnitTestBodyHelper
{
public:
	static void FormatFixtureErrorByCallResult(const CallResult& cr, _UnitTestMessageBuffer& buffer) throw()
	{
		const CharS*  l_szFixture   = _S("[fixture]");
		const uintptr l_iFixtureLen = calc_string_length(l_szFixture);
		result_to_string(cr, FixedArrayHelper::GetInternalPointer(buffer), _UnitTestMessageBuffer::c_size);
		StringUtilHelper::Append(ConstStringS(l_szFixture, l_iFixtureLen), buffer);
	}
	static void IsTrue(bool bExpressionValue, const CharS* szFileName, int iLineNumber)
	{
		if( bExpressionValue )
			return ;
		_UnitTestAssertException exception;
		value_to_string(FixedArrayHelper::GetInternalPointer(exception.GetMessageBuffer()), _UnitTestMessageBuffer::c_size,
						_S("%s(%d): error : ASSERT_TRUE failed."), szFileName, iLineNumber);
		throw exception;
	}
};

// define error message
#define _GKC_FORMAT_ERROR(...)  \
	value_to_string(FixedArrayHelper::GetInternalPointer(buffer), _UnitTestMessageBuffer::c_size,  \
					__VA_ARGS__);

// define function
#define _GKC_BEGIN_TEST_FUNC(x)  \
	bool _GKC_TEST_##x(_UnitTestMessageBuffer& buffer);	\
	_UnitTestReg g_gkc_test_##x(_S(#x), &GKC_TEST_##x);	\
	bool GKC_TEST_##x(_UnitTestMessageBuffer& buffer) {

#define _GKC_END_TEST_FUNC    return true; }

// define block
#define _GKC_BEGIN_TEST_BLOCK  try {

#define _GKC_END_TEST_BLOCK  \
	} catch(_UnitTestAssertException& e) {  \
		_GKC_FORMAT_ERROR(_S("%s"), FixedArrayHelper::GetInternalPointer(e.GetMessageBuffer()));  \
		return false; }

// define fixture block
#define _GKC_BEGIN_FIXTURE_BLOCK(fixtureName)  \
	try { _GKC_FIXTURE_##fixtureName __fixture_##fixtureName;

#define _GKC_END_FIXTURE_BLOCK  \
	} catch(Exception& e) {  \
		_UnitTestBodyHelper::FormatFixtureErrorByCallResult(e.GetResult(), buffer);  \
		return false; }

// define TEST

#define GKC_BEGIN_TEST(x)      _GKC_BEGIN_TEST_FUNC(x)  _GKC_BEGIN_TEST_BLOCK
#define GKC_END_TEST           _GKC_END_TEST_BLOCK      _GKC_END_TEST_FUNC
#define GKC_BEGIN_TESTF(x, f)  _GKC_BEGIN_TEST_FUNC(x)  _GKC_BEGIN_FIXTURE_BLOCK(f)  _GKC_BEGIN_TEST_BLOCK
#define GKC_END_TESTF          _GKC_END_TEST_BLOCK      _GKC_END_FIXTURE_BLOCK       _GKC_END_TEST_FUNC

// GKC_ASSERT_TRUE

#define GKC_ASSERT_TRUE(expression)  \
	_UnitTestBodyHelper::IsTrue(((expression) ? true : false), __SFILE__, __LINE__)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_UNIT_H__
////////////////////////////////////////////////////////////////////////////////
