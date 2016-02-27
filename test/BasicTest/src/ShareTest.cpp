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

#include "PreComp.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// SharePtrTest

GKC_BEGIN_TEST(SharePtrTest)
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

	int iShareCount;
	int iWeakCount;

	//case 1
	SharePtr<_Inner> spTest(SharePtrHelper::MakeSharePtr<_Inner>(MemoryHelper::GetCrtMemoryManager()));
	iShareCount = SharePtrHelper::test_get_share_count(spTest);
	iWeakCount  = SharePtrHelper::test_get_weak_count(spTest);
	GKC_TEST_ASSERT_TRUE( iShareCount == 1 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 1 );

	//case 2
	SharePtr<_Inner> spTest1(spTest);
	iShareCount = SharePtrHelper::test_get_share_count(spTest1);
	iWeakCount  = SharePtrHelper::test_get_weak_count(spTest1);
	GKC_TEST_ASSERT_TRUE( iShareCount == 2 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 1 );

	//case 3
	WeakPtr<_Inner> wpTest = SharePtrHelper::ToWeakPtr(spTest);
	iShareCount = SharePtrHelper::test_get_share_count(wpTest);
	iWeakCount = SharePtrHelper::test_get_weak_count(wpTest);
	GKC_TEST_ASSERT_TRUE( iShareCount == 2 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 2 );

	//case 4
	SharePtr<_Inner> spTest2 = SharePtrHelper::ToSharePtr(wpTest);
	iShareCount = SharePtrHelper::test_get_share_count(spTest2);
	iWeakCount = SharePtrHelper::test_get_weak_count(spTest2);
	GKC_TEST_ASSERT_TRUE( iShareCount == 3 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 2 );

	//case 5
	GKC_BEGIN_TEST_BLOCK
	{
		SharePtr<_Inner> spR(SharePtrHelper::MakeSharePtr<_Inner>(MemoryHelper::GetCrtMemoryManager()));
		SharePtr<_Inner> spR1(spR);
		WeakPtr<_Inner> wpR = SharePtrHelper::ToWeakPtr(spR);
		SharePtr<_Inner> spR2 = SharePtrHelper::ToSharePtr(wpR);
		spR1.Release();
		spR.Release();
	}
	GKC_END_TEST_BLOCK
}
GKC_END_TEST

// ShareArrayTest

GKC_BEGIN_TEST(ShareArrayTest)
{
	int iShareCount;
	int iWeakCount;

	ShareArray<uintptr> arrTest(ShareArrayHelper::MakeShareArray<uintptr>(MemoryHelper::GetCrtMemoryManager()));

	//case 1
	GKC_BEGIN_TEST_BLOCK
	{
		arrTest.Add(1);
	}
	GKC_END_TEST_BLOCK

	//case 2
	GKC_BEGIN_TEST_EXCEPTION
	{
#if defined(CM_X64)
		//4T memory (1T elements)
		uintptr uElements = 1000ULL * 1000ULL * 1000ULL * 1000ULL;
#elif defined(CM_X86)
		//3G memory (750M elements)
		uintptr uElements = 3UL * 1000UL * 1000UL * 1000UL / sizeof(uintptr);
#else
	#error error machine type!
#endif

		arrTest.SetCount(uElements, 0);
	}
	GKC_END_TEST_EXCEPTION

	//case 3
	iShareCount = ShareArrayHelper::test_get_share_count(arrTest);
	iWeakCount  = ShareArrayHelper::test_get_weak_count(arrTest);
	GKC_TEST_ASSERT_TRUE( iShareCount == 1 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 1 );

	//case 4
	ShareArray<uintptr> arrTest1 = arrTest;
	iShareCount = ShareArrayHelper::test_get_share_count(arrTest1);
	iWeakCount  = ShareArrayHelper::test_get_weak_count(arrTest1);
	GKC_TEST_ASSERT_TRUE( iShareCount == 2 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 1 );

	//case 5
	WeakArray<uintptr> warrTest = ShareArrayHelper::ToWeakArray(arrTest);
	iShareCount = ShareArrayHelper::test_get_share_count(warrTest);
	iWeakCount = ShareArrayHelper::test_get_weak_count(warrTest);
	GKC_TEST_ASSERT_TRUE( iShareCount == 2 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 2 );

	//case 6
	ShareArray<uintptr> arrTest2 = ShareArrayHelper::ToShareArray(warrTest);
	iShareCount = ShareArrayHelper::test_get_share_count(arrTest2);
	iWeakCount = ShareArrayHelper::test_get_weak_count(arrTest2);
	GKC_TEST_ASSERT_TRUE( iShareCount == 3 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 2 );

	//case 7
	GKC_BEGIN_TEST_BLOCK
	{
		ShareArray<uintptr> arrR(ShareArrayHelper::MakeShareArray<uintptr>(MemoryHelper::GetCrtMemoryManager()));
		arrR.SetCount(2000, 0);
		ShareArray<uintptr> arrR1 = arrR;
		WeakArray<uintptr> warrR = ShareArrayHelper::ToWeakArray(arrR);
		ShareArray<uintptr> arrR2 = ShareArrayHelper::ToShareArray(warrR);
		arrR1.Release();
		arrR2.RemoveAll();
		arrR.Release();
	}
	GKC_END_TEST_BLOCK
}
GKC_END_TEST

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
