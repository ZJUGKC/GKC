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

GKC_TEST_FIXTURE(MyFixture);  // declares a fixture

GKC_TEST_SETUP(MyFixture)
{
    // Do some stuff here that you want done before a test starts. It can include accessing static variables.
	// An example would be creating a file (and then deleting it in TEARDOWN)
    XXXTest::l_myNumber = 5;
}

GKC_TEST_TEARDOWN(MyFixture)
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
	bool bCheck = (5 == XXXTest::l_myNumber);
    GKC_TEST_ASSERT_TRUE(bCheck);
}
GKC_END_TESTF

GKC_BEGIN_TEST(MyTestWithoutFixtures)
{
    // Do some tests.
	// Do not rely on any variables set in the setup or teardown.
}
GKC_END_TEST

These are macros which can be used in test body.

The GKC_TEST_ASSERT_TRUE macro can be used to verify condition which, if not true, indicates a test failure.
The GKC_BEGIN_TEST_BLOCK and GKC_END_TEST_BLOCK macros can be used to verify that no exceptions are thrown.
The GKC_BEGIN_TEST_EXCEPTION and GKC_END_TEST_EXCEPTION macros can be used to verify that some exceptions are thrown.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_UNIT_H__
#define __GKC_UNIT_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_COLLB_H__
	#error GkcUnit.h requires GkcCollb.h to be included first.
#endif

#ifndef __GKC_CONSOLE_H__
	#error GkcUnit.h requires GkcConsole.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// This is the max buffer length for the message assigned to an assert exception.
#define _GKC_TEST_MAX_ASSERT_MESSAGE_LENGTH  (16*1024)

// _UnitTestMessgeBuffer

typedef FixedStringT<CharS, _GKC_TEST_MAX_ASSERT_MESSAGE_LENGTH>  _UnitTestMessageBuffer;

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
		StringS strM(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
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
	_UnitTestFunc Find(const StringS& strName) const throw()
	{
		auto iter(m_map.Find(strName));
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
	static _UnitTestMap& GetUnitTestMap() throw()
	END_NOINLINE
	{
		static _UnitTestMap l_unit_test_map;
		return l_unit_test_map;
	}
};

//_UnitTestReg

class _UnitTestReg
{
public:
	_UnitTestReg(const CharS* szName, _UnitTestFunc pFunc)
	{
		ConstStringS strName;
		ConstArrayHelper::SetInternalPointer(szName, calc_string_length(szName), strName);
		_UnitTestMap& map = _UnitTestMapHelper::GetUnitTestMap();
		map.AddUnitTest(strName, pFunc);
	}
};

// _UnitTestMainHelper

class _UnitTestMainHelper
{
public:
	static int MainProcess(const ConstArray<ConstStringS>& args, _UnitTestMessageBuffer& buffer)
	{
		//const strings
		DECLARE_LOCAL_CONST_STRING(CharS, l_szSep1, l_uSep1Len, _S("=========================="))
		DECLARE_LOCAL_CONST_STRING(CharS, l_szSep2, l_uSep2Len, _S("**************************"))
		DECLARE_LOCAL_CONST_STRING(CharS, l_szSep3, l_uSep3Len, _S("--------------------------"))
		DECLARE_LOCAL_CONST_STRING(CharS, l_szColon, l_uColonLen, _S(":"))
		DECLARE_LOCAL_CONST_STRING(CharS, l_szNoTest, l_uNoTestLen, _S("ERROR: NO SUCH TEST!"))

		//map
		_UnitTestMap& map = _UnitTestMapHelper::GetUnitTestMap();
		uintptr uTotal  = 0;
		uintptr uFailed = 0;

		if( args.GetCount() <= 1 ) {
			//all tests
			_UnitTestMap::ItemInfo info;
			bool bContinue = map.EnumFirst(info);
			while( bContinue ) {
				//current test
				ConsoleHelper::WriteLine(ConstStringS(l_szSep1, l_uSep1Len));
				ConsoleHelper::Write(info.strName);
				ConsoleHelper::WriteLine(ConstStringS(l_szColon, l_uColonLen));
				if( !info.pFunc(buffer) ) {
					ConsoleHelper::WriteLine(buffer);
					uFailed ++;
				}
				ConsoleHelper::WriteLine(ConstStringS(l_szSep2, l_uSep2Len));
				uTotal ++;
				bContinue = map.EnumNext(info);
				//sleep
				thread_sleep(1);
			}
		}
		else {
			StringS strM(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
			//specified tests
			auto iter(args.GetBegin());
			assert( iter != args.GetEnd() );
			iter.MoveNext();  //from 1
			for( ; iter != args.GetEnd(); iter.MoveNext() ) {
				//current test
				ConsoleHelper::WriteLine(ConstStringS(l_szSep1, l_uSep1Len));
				ConsoleHelper::Write(iter.get_Value());
				ConsoleHelper::WriteLine(ConstStringS(l_szColon, l_uColonLen));
				//find
				StringUtilHelper::MakeString(iter.get_Value(), strM);
				_UnitTestFunc pFunc = map.Find(strM);
				if( pFunc == NULL ) {
					ConsoleHelper::WriteLine(ConstStringS(l_szNoTest, l_uNoTestLen));
					uFailed ++;
				}
				else {
					//run test
					if( !pFunc(buffer) ) {
						ConsoleHelper::WriteLine(buffer);
						uFailed ++;
					}
				}
				ConsoleHelper::WriteLine(ConstStringS(l_szSep2, l_uSep2Len));
				uTotal ++;
			}
		} //end if

		//total information
		{
			buffer.SetLength(0);
			int ret = value_to_string(FixedArrayHelper::GetInternalPointer(buffer), _UnitTestMessageBuffer::c_size,
									_S("Total (%Iu), Failed (%Iu)"), uTotal, uFailed);
			if( ret >= 0 )
				buffer.SetLength(ret);
			ConsoleHelper::WriteLine(ConstStringS(l_szSep3, l_uSep3Len));
			ConsoleHelper::WriteLine(buffer);
		} //end block

		return uFailed != 0 ? 1 : 0;
	}
};

// for main function
#define UNIT_TEST_MAIN_PROCESS(args)  \
	_UnitTestMessageBuffer g_buffer;  \
	return _UnitTestMainHelper::MainProcess(args, g_buffer);

// in cpp file

// _UnitTestBodyHelper

class _UnitTestBodyHelper
{
public:
	static void FormatErrorByCallResult(const CallResult& cr, const CharS* szFileName, int iLineNumber, bool bFixture, bool bError, _UnitTestMessageBuffer& buffer) throw()
	{
		//const strings
		DECLARE_LOCAL_CONST_STRING(CharS, l_szFixture, l_uFixtureLen, _S("[fixture]"))
		DECLARE_LOCAL_CONST_STRING(CharS, l_szError, l_uErrorLen, _S("error:"))
		DECLARE_LOCAL_CONST_STRING(CharS, l_szCorrect, l_uCorrectLen, _S("correct:"))
		//format
		_UnitTestMessageBuffer bufTemp;
		int ret = value_to_string(FixedArrayHelper::GetInternalPointer(bufTemp), _UnitTestMessageBuffer::c_size,
								_S("%s(%d) "), szFileName, iLineNumber);
		if( ret >= 0 )
			bufTemp.SetLength(ret);
		buffer.SetLength(0);
		StringUtilHelper::Append(bufTemp, buffer);
		if( bFixture ) {
			StringUtilHelper::Append(ConstStringS(l_szFixture, l_uFixtureLen), buffer);
			StringUtilHelper::Append(ConstStringS(_S(" "), 1), buffer);
		}
		if( bError )
			StringUtilHelper::Append(ConstStringS(l_szError, l_uErrorLen), buffer);
		else
			StringUtilHelper::Append(ConstStringS(l_szCorrect, l_uCorrectLen), buffer);
		result_to_string(cr, FixedArrayHelper::GetInternalPointer(bufTemp), _UnitTestMessageBuffer::c_size);
		bufTemp.RecalcLength();
		StringUtilHelper::Append(bufTemp, buffer);
	}
	static void IsTrue(bool bExpressionValue, const CharS* szFileName, int iLineNumber)
	{
		if( bExpressionValue )
			return ;
		_UnitTestAssertException exception;
		_UnitTestMessageBuffer&  buf = exception.GetMessageBuffer();
		int ret = value_to_string(FixedArrayHelper::GetInternalPointer(buf), _UnitTestMessageBuffer::c_size,
								_S("%s(%d): error : ASSERT_TRUE failed."), szFileName, iLineNumber);
		if( ret >= 0 )
			buf.SetLength(ret);
		throw exception;
	}
};

// define Fixture
#define GKC_TEST_FIXTURE(x)  \
	class GKC_TEST_FIXTURE_##x { public:  \
	void Setup(); void Teardown();  \
	public:  \
	GKC_TEST_FIXTURE_##x() { Setup(); }  \
	~GKC_TEST_FIXTURE_##x() { Teardown(); }  \
	};

#define GKC_TEST_SETUP(x)      void GKC_TEST_FIXTURE_##x::Setup()
#define GKC_TEST_TEARDOWN(x)   void GKC_TEST_FIXTURE_##x::Teardown()

// define string
#define _GKC_TEST_TO_STRING(x)  #x

// define error message
#define _GKC_TEST_FORMAT_ERROR(...)  \
	{ _gkc_utm_buffer.SetLength(0);  \
	int __ret = value_to_string(FixedArrayHelper::GetInternalPointer(_gkc_utm_buffer), _UnitTestMessageBuffer::c_size, __VA_ARGS__);  \
	if( __ret >= 0 ) _gkc_utm_buffer.SetLength(__ret); }

// define function
#define _GKC_BEGIN_TEST_FUNC(x)  \
	bool _GKC_TEST_##x(_UnitTestMessageBuffer& _gkc_utm_buffer);  \
	_UnitTestReg g_gkc_test_##x(_S(_GKC_TEST_TO_STRING(x)), &_GKC_TEST_##x);  \
	bool _GKC_TEST_##x(_UnitTestMessageBuffer& _gkc_utm_buffer) {

#define _GKC_END_TEST_FUNC  return true; }

// define fixture block
//   Teardown, i.e., the destructor of the "fixture object", may throw exceptions.
//   This could be a problem.
#define _GKC_BEGIN_FIXTURE_BLOCK(fixtureName)  \
	try { _GKC_TEST_FIXTURE_##fixtureName __fixture_##fixtureName;

#define _GKC_END_FIXTURE_BLOCK  \
	} catch(Exception& e) {  \
		_UnitTestBodyHelper::FormatErrorByCallResult(e.GetResult(), __SFILE__, __LINE__, true, true, _gkc_utm_buffer);  \
		return false; }  \
	catch(...) {  \
		_GKC_TEST_FORMAT_ERROR(_S("%s(%d) Other exception thrown in fixture."), __SFILE__, __LINE__);  \
		return false; }

// define block

#define GKC_BEGIN_TEST_BLOCK  try

#define GKC_END_TEST_BLOCK  \
	catch(_UnitTestAssertException& e) {  \
		_GKC_TEST_FORMAT_ERROR(_S("%s"), FixedArrayHelper::GetInternalPointer(e.GetMessageBuffer()));  \
		return false; }  \
	catch(Exception& e) {  \
		_UnitTestBodyHelper::FormatErrorByCallResult(e.GetResult(), __SFILE__, __LINE__, false, true, _gkc_utm_buffer);  \
		return false; }  \
	catch(...) {  \
		_GKC_TEST_FORMAT_ERROR(_S("%s(%d) Other exception thrown in block."), __SFILE__, __LINE__);  \
		return false; }

// define TEST

#define GKC_BEGIN_TEST(x)      _GKC_BEGIN_TEST_FUNC(x)  GKC_BEGIN_TEST_BLOCK
#define GKC_END_TEST           GKC_END_TEST_BLOCK       _GKC_END_TEST_FUNC
#define GKC_BEGIN_TESTF(x, f)  _GKC_BEGIN_TEST_FUNC(x)  _GKC_BEGIN_FIXTURE_BLOCK(f)  GKC_BEGIN_TEST_BLOCK
#define GKC_END_TESTF          GKC_END_TEST_BLOCK       _GKC_END_FIXTURE_BLOCK       _GKC_END_TEST_FUNC

// GKC_TEST_ASSERT_TRUE

#define GKC_TEST_ASSERT_TRUE(expression)  \
	_UnitTestBodyHelper::IsTrue(((expression) ? true : false), __SFILE__, __LINE__)

// define Exception Test (The GKC_TEST_ASSERT_TRUE should not be called in this body)

#define GKC_BEGIN_TEST_EXCEPTION  { bool __bException = false; CallResult __cr;  try

#define GKC_END_TEST_EXCEPTION  \
	catch(Exception& e) {  \
		__bException = true;  __cr = e.GetResult(); }  \
	catch(...) {  \
		__bException = true;  __cr.SetResult(0); }  \
	if( __bException ) {  \
		_UnitTestBodyHelper::FormatErrorByCallResult(__cr, __SFILE__, __LINE__, false, false, _gkc_utm_buffer);  \
		ConsoleHelper::WriteLine(_gkc_utm_buffer); }  \
	else { GKC_TEST_ASSERT_TRUE(false); } }

// GKC_TEST_OUTPUT

#define GKC_TEST_OUTPUT(format, ...)  \
	_GKC_TEST_FORMAT_ERROR(format, ##__VA_ARGS__);  \
	ConsoleHelper::WriteLine(_gkc_utm_buffer);

// test time

#define GKC_BEGIN_TEST_TIME  { TimeValue __tv1; TimeHelper::FetchCurrentTime(__tv1);

#define GKC_END_TEST_TIME  \
	TimeValue __tv2; TimeHelper::FetchCurrentTime(__tv2);  \
	TimeSpan  __ts = __tv2 - __tv1;  \
	_GKC_TEST_FORMAT_ERROR(_S("time : %I64u, %I64u"), __ts.get_Seconds(), __ts.get_Nanoseconds());  \
	ConsoleHelper::WriteLine(_gkc_utm_buffer); }

// test functor

#define GKC_BEGIN_TEST_FUNCTOR(fn_name, ...)  \
	auto fn_name = [&](__VA_ARGS__)->void

#define GKC_END_TEST_FUNCTOR  ;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_UNIT_H__
////////////////////////////////////////////////////////////////////////////////
