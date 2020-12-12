/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
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

// PkzTest

GKC_BEGIN_TEST(PkzTest)
{
	byte buffer1[0x2000 + 0x500];
	byte buffer2[0x2000 + 0x500];
	byte buffer3[0x2000 + 0x500];

	uint uSize1, uSize2, uSize3;

	CallResult cr;

	UniqueCom upCompressor;
	cr = CompressionHelper::CreatePkzCompressor(upCompressor);
	GKC_TEST_ASSERT_TRUE( cr.IsSucceeded() );
	UniqueCom upDecompressor;
	cr = CompressionHelper::CreatePkzDecompressor(upDecompressor);
	GKC_TEST_ASSERT_TRUE( cr.IsSucceeded() );

	RefPtr<IPkCompressor> refCompressor;
	_UNIQUECOM_INSTANCE_INTERFACE(IPkCompressor, upCompressor, refCompressor, cr);
	GKC_TEST_ASSERT_TRUE( cr.IsSucceeded() );
	RefPtr<IPkDecompressor> refDecompressor;
	_UNIQUECOM_INSTANCE_INTERFACE(IPkDecompressor, upDecompressor, refDecompressor, cr);
	GKC_TEST_ASSERT_TRUE( cr.IsSucceeded() );

	PkInputBuffer bufIn;
	PkOutputBuffer bufOut;

	RandHelper::Init();

	int res;

	//case 1
	refCompressor.Deref().Initialize();
	refCompressor.Deref().SetInputSize(0);
	res = refCompressor.Deref().Encode();
	GKC_TEST_ASSERT_TRUE( res == 1 );
	refCompressor.Deref().StartFinish();
	res = refCompressor.Deref().ProcessFinish();
	GKC_TEST_ASSERT_TRUE( res == 0 );
	refCompressor.Deref().GetOutputBuffer(bufOut);
	GKC_TEST_ASSERT_TRUE( bufOut.uLength == 2 + 1 );
	mem_copy((void*)bufOut.pBuffer, bufOut.uLength, buffer2);
	uSize2 = bufOut.uLength;
	refDecompressor.Deref().Initialize();
	refDecompressor.Deref().SetInputSize(0);
	res = refDecompressor.Deref().Decode();
	GKC_TEST_ASSERT_TRUE( res == 1 );
	refDecompressor.Deref().GetInputBuffer(bufIn);
	mem_copy(buffer2, uSize2, (void*)bufIn.pBuffer);
	refDecompressor.Deref().SetInputSize(uSize2);
	res = refDecompressor.Deref().Decode();
	GKC_TEST_ASSERT_TRUE( res == 0 );
	refDecompressor.Deref().GetOutputBuffer(bufOut);
	GKC_TEST_ASSERT_TRUE( bufOut.uLength == 0 );

	//case 2
	refCompressor.Deref().Initialize();
	refCompressor.Deref().SetInputSize(0);
	res = refCompressor.Deref().Encode();
	GKC_TEST_ASSERT_TRUE( res == 1 );
	refCompressor.Deref().GetInputBuffer(bufIn);
	buffer1[0] = (byte)(RandHelper::Next() * 2 / 255);
	mem_copy(buffer1, 1, (void*)bufIn.pBuffer);
	refCompressor.Deref().SetInputSize(1);
	res = refCompressor.Deref().Encode();
	GKC_TEST_ASSERT_TRUE( res == 1 );
	refCompressor.Deref().StartFinish();
	res = refCompressor.Deref().ProcessFinish();
	GKC_TEST_ASSERT_TRUE( res == 0 );
	refCompressor.Deref().GetOutputBuffer(bufOut);
	GKC_TEST_ASSERT_TRUE( bufOut.uLength == 2 + 2 );
	mem_copy((void*)bufOut.pBuffer, bufOut.uLength, buffer2);
	uSize2 = bufOut.uLength;
	refDecompressor.Deref().Initialize();
	refDecompressor.Deref().SetInputSize(0);
	res = refDecompressor.Deref().Decode();
	GKC_TEST_ASSERT_TRUE( res == 1 );
	refDecompressor.Deref().GetInputBuffer(bufIn);
	mem_copy(buffer2, uSize2, (void*)bufIn.pBuffer);
	refDecompressor.Deref().SetInputSize(uSize2);
	res = refDecompressor.Deref().Decode();
	GKC_TEST_ASSERT_TRUE( res == 0 );
	refDecompressor.Deref().GetOutputBuffer(bufOut);
	GKC_TEST_ASSERT_TRUE( bufOut.uLength == 1 );
	mem_copy((void*)bufOut.pBuffer, bufOut.uLength, buffer3);
	GKC_TEST_ASSERT_TRUE( buffer1[0] == buffer3[0] );

	//case 3
	refCompressor.Deref().Initialize();
	refCompressor.Deref().GetInputBuffer(bufIn);
	buffer1[0] = (byte)(RandHelper::Next() * 2 / 255);
	buffer1[1] = buffer1[0];
	mem_copy(buffer1, 2, (void*)bufIn.pBuffer);
	refCompressor.Deref().SetInputSize(2);
	res = refCompressor.Deref().Encode();
	GKC_TEST_ASSERT_TRUE( res == 1 );
	refCompressor.Deref().StartFinish();
	res = refCompressor.Deref().ProcessFinish();
	GKC_TEST_ASSERT_TRUE( res == 0 );
	refCompressor.Deref().GetOutputBuffer(bufOut);
	GKC_TEST_ASSERT_TRUE( bufOut.uLength == 2 + 3 );
	mem_copy((void*)bufOut.pBuffer, bufOut.uLength, buffer2);
	uSize2 = bufOut.uLength;
	refDecompressor.Deref().Initialize();
	refDecompressor.Deref().GetInputBuffer(bufIn);
	mem_copy(buffer2, uSize2, (void*)bufIn.pBuffer);
	refDecompressor.Deref().SetInputSize(uSize2);
	res = refDecompressor.Deref().Decode();
	GKC_TEST_ASSERT_TRUE( res == 0 );
	refDecompressor.Deref().GetOutputBuffer(bufOut);
	GKC_TEST_ASSERT_TRUE( bufOut.uLength == 2 );
	mem_copy((void*)bufOut.pBuffer, bufOut.uLength, buffer3);
	GKC_TEST_ASSERT_TRUE( mem_compare(buffer1, buffer3, 2) == 0 );

	uint len_array[] = { 3, 4, 5, 6, 0x1200, 0xB000, 0x400000 };

	//case 4
	for ( int i = 0; i < 5; i ++ ) {
		refCompressor.Deref().Initialize();
		uSize1 = len_array[i];
		refCompressor.Deref().GetInputBuffer(bufIn);
		buffer1[0] = (byte)(RandHelper::Next() * 2 / 255);
		for( uint j = 1; j < uSize1; j ++ )
			buffer1[j] = buffer1[0];
		mem_copy(buffer1, uSize1, (void*)bufIn.pBuffer);
		refCompressor.Deref().SetInputSize(uSize1);
		res = refCompressor.Deref().Encode();
		GKC_TEST_ASSERT_TRUE( res == 1 );
		refCompressor.Deref().StartFinish();
		res = refCompressor.Deref().ProcessFinish();
		GKC_TEST_ASSERT_TRUE( res == 0 );
		refCompressor.Deref().GetOutputBuffer(bufOut);
		uSize2 = bufOut.uLength;
		mem_copy((void*)bufOut.pBuffer, bufOut.uLength, buffer2);
		GKC_TEST_OUTPUT(_S("source: %u, dest: %u"), uSize1, uSize2);
		refDecompressor.Deref().Initialize();
		refDecompressor.Deref().GetInputBuffer(bufIn);
		mem_copy(buffer2, uSize2, (void*)bufIn.pBuffer);
		refDecompressor.Deref().SetInputSize(uSize2);
		res = refDecompressor.Deref().Decode();
		GKC_TEST_ASSERT_TRUE( res == 0 );
		refDecompressor.Deref().GetOutputBuffer(bufOut);
		uSize3 = bufOut.uLength;
		GKC_TEST_ASSERT_TRUE( uSize1 == uSize3 );
		mem_copy((void*)bufOut.pBuffer, bufOut.uLength, buffer3);
		GKC_TEST_ASSERT_TRUE( mem_compare(buffer1, buffer3, uSize3) == 0 );
	}

	const uint c_size = 0x500000;
	UniqueArray<byte> arr;
	arr.SetCount(c_size * 3);
	byte* pB1 = UniqueArrayHelper::GetInternalPointer(arr);
	byte* pB2 = pB1 + c_size;
	byte* pB3 = pB2 + c_size;

	GKC_BEGIN_TEST_FUNCTOR(fnLargeData, bool bLongestSearch)
	{
		refCompressor.Deref().Initialize();
		refCompressor.Deref().SetLongestSearch(bLongestSearch);
		uint uRest = uSize1;
		uSize2 = 0;
		/*compress*/
		GKC_BEGIN_TEST_TIME
		{
			do {
				res = refCompressor.Deref().Encode();
				if ( res == 1 ) {
					uint uInput = uRest;
					if ( uInput == 0 )
						break;
					refCompressor.Deref().GetInputBuffer(bufIn);
					if ( bufIn.uLength < uInput )
						uInput = bufIn.uLength;
					mem_copy(pB1 + uSize1 - uRest, uInput, (void*)bufIn.pBuffer);
					refCompressor.Deref().SetInputSize(uInput);
					uRest -= uInput;
				}
				else {
					refCompressor.Deref().GetOutputBuffer(bufOut);
					mem_copy((void*)bufOut.pBuffer, bufOut.uLength, pB2 + uSize2);
					uSize2 += bufOut.uLength;
					refCompressor.Deref().ResetOutputBuffer();
				}
			} while( 1 );
			refCompressor.Deref().StartFinish();
			do {
				res = refCompressor.Deref().ProcessFinish();
				refCompressor.Deref().GetOutputBuffer(bufOut);
				mem_copy((void*)bufOut.pBuffer, bufOut.uLength, pB2 + uSize2);
				uSize2 += bufOut.uLength;
				refCompressor.Deref().ResetOutputBuffer();
				if ( res == 0 )
					break;
			} while( 1 );
		}
		GKC_END_TEST_TIME
		GKC_TEST_OUTPUT(_S("source: %u, dest: %u"), uSize1, uSize2);
		refDecompressor.Deref().Initialize();
		uRest = uSize2;
		uSize3 = 0;
		/*decompress*/
		GKC_BEGIN_TEST_TIME
		{
			do {
				res = refDecompressor.Deref().Decode();
				if ( res == 1 ) {
					uint uInput = uRest;
					if ( uInput == 0 )
						GKC_TEST_ASSERT_TRUE( false );
					refDecompressor.Deref().GetInputBuffer(bufIn);
					if ( bufIn.uLength < uInput )
						uInput = bufIn.uLength;
					mem_copy(pB2 + uSize2 - uRest, uInput, (void*)bufIn.pBuffer);
					refDecompressor.Deref().SetInputSize(uInput);
					uRest -= uInput;
				}
				else {
					refDecompressor.Deref().GetOutputBuffer(bufOut);
					mem_copy((void*)bufOut.pBuffer, bufOut.uLength, pB3 + uSize3);
					uSize3 += bufOut.uLength;
					refDecompressor.Deref().ResetOutputBuffer();
					if ( res == 0 )
						break;
				}
			} while( 1 );
		}
		GKC_END_TEST_TIME
		GKC_TEST_ASSERT_TRUE( uSize1 == uSize3 );
		GKC_TEST_ASSERT_TRUE( mem_compare(pB1, pB3, uSize3) == 0 );
	}
	GKC_END_TEST_FUNCTOR

	//case 5
	for ( int i = 4; i < 7; i ++ ) {
		uSize1 = len_array[i];
		for( uint j = 0; j < uSize1; j ++ )
			pB1[j] = (byte)(RandHelper::Next() * 2 / 255);
		fnLargeData(true);
	}
}
GKC_END_TEST

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
