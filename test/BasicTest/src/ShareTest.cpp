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
This file contains unit test functions.

Author : Lijuan Mei
*/

////////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

////////////////////////////////////////////////////////////////////////////////

// SharedPtr Test

GKC_BEGIN_TEST(SharedPtrTest)
{
	class _Inner
	{
	public:
		_Inner() throw()
		{
		}
		~_Inner() throw()
		{
		}

	private:
		uintptr m_uTest;
	};
	class _TypeProcess : public GKC::ITypeProcess
	{
	public:
		virtual uintptr TypeCast(const guid& clsid) throw()
		{
			return 0;
		}
		virtual void Destruction(const uintptr& p) throw()
		{
			((_Inner*)p)->~_Inner();
		}
	};
	_TypeProcess tp;

	int iShareCount;
	int iWeakCount;

	//case 1
	GKC::SharedPtr<_Inner> spTest(GKC::SharedPtrHelper::MakeSharedPtr<_Inner>(GKC::MemoryHelper::GetCrtMemoryManager(), GKC::RefPtr<GKC::ITypeProcess>(&tp)));
	GKC::SharedPtrBlock* spb = GKC::SharedPtrHelper::GetBlockPointer(spTest);
	iShareCount = spb->GetShareCount();
	iWeakCount  = spb->GetWeakCount();
	GKC_ASSERT_TRUE( iShareCount == 1 );
	GKC_ASSERT_TRUE( iWeakCount == 1 );

	//case 2
	GKC::SharedPtr<_Inner> spTest1(spTest);
	GKC::SharedPtrBlock* spb1 = GKC::SharedPtrHelper::GetBlockPointer(spTest1);
	iShareCount = spb1->GetShareCount();
	iWeakCount  = spb1->GetWeakCount();
	GKC_ASSERT_TRUE( iShareCount == 2 );
	GKC_ASSERT_TRUE( iWeakCount == 1 );
}
GKC_END_TEST

// SharedArrayTest

GKC_BEGIN_TEST(SharedArrayTest)
{
	int iShareCount;
	int iWeakCount;

	GKC::SharedArray<uintptr> arrTest(GKC::SharedArrayHelper::MakeSharedArray<uintptr>(GKC::MemoryHelper::GetCrtMemoryManager()));

	//case 1
	GKC_BEGIN_TEST_BLOCK
	{
		arrTest.Add(1);
	}
	GKC_END_TEST_BLOCK

	//case 2
	GKC::SharedArrayBlock* sab = GKC::SharedArrayHelper::GetBlockPointer(arrTest);
	iShareCount = sab->GetShareCount();
	iWeakCount  = sab->GetWeakCount();
	GKC_ASSERT_TRUE( iShareCount == 1 );
	GKC_ASSERT_TRUE( iWeakCount == 1 );

	//case 3
	GKC::SharedArray<uintptr> arrTest1 = arrTest;
	GKC::SharedArrayBlock* sab1 = GKC::SharedArrayHelper::GetBlockPointer(arrTest1);
	iShareCount = sab1->GetShareCount();
	iWeakCount  = sab1->GetWeakCount();
	GKC_ASSERT_TRUE( iShareCount == 2 );
	GKC_ASSERT_TRUE( iWeakCount == 1 );
}
GKC_END_TEST

////////////////////////////////////////////////////////////////////////////////
