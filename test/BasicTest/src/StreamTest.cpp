/*
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
This file contains unit test functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// MemoryStreamTest

GKC_BEGIN_TEST(MemoryStreamTest)
{
	CallResult cr;

	int iShareCount;
	int iWeakCount;

	//case 1
	ShareCom<IByteStream> spTest;
	cr = StreamHelper::CreateMemoryStream(spTest);
	GKC_TEST_ASSERT_TRUE( cr.IsSucceeded() );
	iShareCount = ShareComHelper::test_get_share_count(spTest);
	iWeakCount  = ShareComHelper::test_get_weak_count(spTest);
	GKC_TEST_ASSERT_TRUE( iShareCount == 1 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 1 );

	//case 2
	ShareCom<IByteStream> spTest1(spTest);
	iShareCount = ShareComHelper::test_get_share_count(spTest1);
	iWeakCount  = ShareComHelper::test_get_weak_count(spTest1);
	GKC_TEST_ASSERT_TRUE( iShareCount == 2 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 1 );

	//case 3
	WeakCom<IByteStream> wpTest = ShareComHelper::ToWeakCom(spTest);
	iShareCount = ShareComHelper::test_get_share_count(wpTest);
	iWeakCount  = ShareComHelper::test_get_weak_count(wpTest);
	GKC_TEST_ASSERT_TRUE( iShareCount == 2 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 2 );

	//case 4
	ShareCom<IByteStream> spTest2 = ShareComHelper::ToShareCom(wpTest);
	iShareCount = ShareComHelper::test_get_share_count(spTest2);
	iWeakCount  = ShareComHelper::test_get_weak_count(spTest2);
	GKC_TEST_ASSERT_TRUE( iShareCount == 3 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 2 );

	//case 5
	ShareCom<IMemoryUtility> spM = CALL_COM_TYPECAST(spTest, IByteStream, IMemoryUtility);
	GKC_TEST_ASSERT_TRUE( !spM.IsBlockNull() );
	iShareCount = ShareComHelper::test_get_share_count(spM);
	iWeakCount  = ShareComHelper::test_get_weak_count(spM);
	GKC_TEST_ASSERT_TRUE( iShareCount == 4 );
	GKC_TEST_ASSERT_TRUE( iWeakCount == 2 );

	//case 6
	GKC_BEGIN_TEST_BLOCK
	{
		ShareCom<IByteStream> spR;
		cr = StreamHelper::CreateMemoryStream(spR);
		GKC_TEST_ASSERT_TRUE( cr.IsSucceeded() );
		ShareCom<IByteStream> spR1(spR);
		WeakCom<IByteStream> wpR = ShareComHelper::ToWeakCom(spR);
		ShareCom<IByteStream> spR2 = ShareComHelper::ToShareCom(wpR);
		spR1.Release();
		spR.Release();
	}
	GKC_END_TEST_BLOCK
}
GKC_END_TEST

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
