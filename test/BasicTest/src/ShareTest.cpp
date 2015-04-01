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
	GKC_TEST_ASSERT_TRUE( iShareCount == 1 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 1 );

	//case 2
	GKC::SharedPtr<_Inner> spTest1(spTest);
	GKC::SharedPtrBlock* spb1 = GKC::SharedPtrHelper::GetBlockPointer(spTest1);
	iShareCount = spb1->GetShareCount();
	iWeakCount  = spb1->GetWeakCount();
	GKC_TEST_ASSERT_TRUE( iShareCount == 2 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 1 );

	//case 3
	GKC::WeakPtr<_Inner> wpTest = GKC::SharedPtrHelper::ToWeakPtr(spTest);
	GKC::SharedPtrBlock* spb2 = GKC::SharedPtrHelper::GetBlockPointer(wpTest);
	iShareCount = spb2->GetShareCount();
	iWeakCount = spb2->GetWeakCount();
	GKC_TEST_ASSERT_TRUE( iShareCount == 2 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 2 );

	//case 4
	GKC::SharedPtr<_Inner> spTest2 = GKC::SharedPtrHelper::ToSharedPtr(wpTest);
	GKC::SharedPtrBlock* spb3 = GKC::SharedPtrHelper::GetBlockPointer(spTest2);
	iShareCount = spb3->GetShareCount();
	iWeakCount = spb3->GetWeakCount();
	GKC_TEST_ASSERT_TRUE(iShareCount == 3);
	GKC_TEST_ASSERT_TRUE(iWeakCount == 2);

	//case 5
	GKC_BEGIN_TEST_BLOCK
	{
		GKC::SharedPtr<_Inner> spR(GKC::SharedPtrHelper::MakeSharedPtr<_Inner>(GKC::MemoryHelper::GetCrtMemoryManager(), GKC::RefPtr<GKC::ITypeProcess>(&tp)));
		GKC::SharedPtr<_Inner> spR1(spR);
		GKC::WeakPtr<_Inner> wpR = GKC::SharedPtrHelper::ToWeakPtr(spR);
		GKC::SharedPtr<_Inner> spR2 = GKC::SharedPtrHelper::ToSharedPtr(wpR);
		spR1.Release();
		spR.Release();
	}
	GKC_END_TEST_BLOCK
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
	GKC_BEGIN_TEST_EXCEPTION
	{
		//1T memory
		uintptr uElements = 1000ULL* 1000ULL * 1000ULL * 1000ULL;
		arrTest.SetCount(uElements, 0);
	}
	GKC_END_TEST_EXCEPTION

	//case 3
	GKC::SharedArrayBlock* sab = GKC::SharedArrayHelper::GetBlockPointer(arrTest);
	iShareCount = sab->GetShareCount();
	iWeakCount  = sab->GetWeakCount();
	GKC_TEST_ASSERT_TRUE( iShareCount == 1 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 1 );

	//case 4
	GKC::SharedArray<uintptr> arrTest1 = arrTest;
	GKC::SharedArrayBlock* sab1 = GKC::SharedArrayHelper::GetBlockPointer(arrTest1);
	iShareCount = sab1->GetShareCount();
	iWeakCount  = sab1->GetWeakCount();
	GKC_TEST_ASSERT_TRUE( iShareCount == 2 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 1 );

	//case 5
	GKC::WeakArray<uintptr> warrTest = GKC::SharedArrayHelper::ToWeakArray(arrTest);
	GKC::SharedArrayBlock* sab2 = GKC::SharedArrayHelper::GetBlockPointer(warrTest);
	iShareCount = sab2->GetShareCount();
	iWeakCount = sab2->GetWeakCount();
	GKC_TEST_ASSERT_TRUE( iShareCount == 2 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 2 );

	//case 6
	GKC::SharedArray<uintptr> arrTest2 = GKC::SharedArrayHelper::ToSharedArray(warrTest);
	GKC::SharedArrayBlock* sab3 = GKC::SharedArrayHelper::GetBlockPointer(arrTest2);
	iShareCount = sab3->GetShareCount();
	iWeakCount = sab3->GetWeakCount();
	GKC_TEST_ASSERT_TRUE(iShareCount == 3);
	GKC_TEST_ASSERT_TRUE(iWeakCount == 2);

	//case 7
	GKC_BEGIN_TEST_BLOCK
	{
		GKC::SharedArray<uintptr> arrR(GKC::SharedArrayHelper::MakeSharedArray<uintptr>(GKC::MemoryHelper::GetCrtMemoryManager()));
		arrR.SetCount(2000, 0);
		GKC::SharedArray<uintptr> arrR1 = arrR;
		GKC::WeakArray<uintptr> warrR = GKC::SharedArrayHelper::ToWeakArray(arrR);
		GKC::SharedArray<uintptr> arrR2 = GKC::SharedArrayHelper::ToSharedArray(warrR);
		arrR1.Release();
		arrR2.RemoveAll();
		arrR.Release();
	}
	GKC_END_TEST_BLOCK
}
GKC_END_TEST

////////////////////////////////////////////////////////////////////////////////
