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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __INTERNAL_PK_COMPRESS_H__
#define __INTERNAL_PK_COMPRESS_H__
////////////////////////////////////////////////////////////////////////////////

// Algorithm: fixed bitrate by PKZIP 1.10

#define MATCH_CONNECT_SIZE  (0x200)
#define MAX_MATCH_RANGE     (0x2000)

// encoded length table
extern byte g_EncodeLenLenTable[];
extern ushort g_EncodeLenBitsTable[];
// encoded position table (high byte)
extern byte g_EncodePosLenTable[];
extern byte g_EncodePosBitsTable[];

/*
if matching length > 9, output 010, output 1 byte to represent actual length.
0 -- 10, 1 -- 11, ..., 252 -- 262. (106H)
253, 254, 255 are used for special flags.
*/
#define LEN_LAST_ELEMENT  (9 - 1)   // (numofarray(g_EncodeLenBitsTable) - 1)
#define VL_THRESHOLD      (9 + 1)   // (numofarray(g_EncodeLenBitsTable) + 1)

#define MAX_MATCH_LEN     (252 + VL_THRESHOLD)

/*
The maximum value of high-order position byte is 1FH,
then the maximum position is 1FFFH, i.e., 2000H - 1, MAX_MATCH_RANGE - 1
*/

// pkzip_buffer

/*
working buffer layout:
----------
RawBuf     --- processed data, size=0x2200, the maximum matching range 0x2000 + step transition area 0x200
CodingBuf  --- encoding data, at least 0x2000, the maximum matching range
Extra      --- extension area for reading input file
----------
CodedBuf   --- encoded data, >=0x400

RawBufSize + CodingBufSize >=0x4200 for zip search table
CodingBufSize : CodingBuf + Extra

After encoded, data of CodingBuf is moved to RawBuf, data of extra area is moved to CodingBuf
*/

struct pkzip_buffer
{
	enum { EXTRA_SIZE = 0x9600 };
	//0xD800
	byte data_buffer[MAX_MATCH_RANGE + MATCH_CONNECT_SIZE  //RawBuf
					+ MAX_MATCH_RANGE  //CodingBuf
					+ EXTRA_SIZE];  //ExtraBuf
	byte* pRawBuf = data_buffer;
	byte* pCodingBuf = &data_buffer[MAX_MATCH_RANGE + MATCH_CONNECT_SIZE];  //0x2200
	ushort uCodingBufSize = MAX_MATCH_RANGE + EXTRA_SIZE;

	byte code_buffer[0xD800];
	byte* pCodedBuf = code_buffer;
	ushort uCodedBufSize = 0xD800 - 0x10;

	ushort uCodeStoreOfs; //next position for storing data (encode: in CodedBuf, decode: in RawBuf)
	ushort uBytesReadInBuf; //the number of bytes of input data (encode: in CodingBuf, decode: in CodedBuf)
	ushort uNextSrcDataOfs; //the offset of next source data (encode: in CodingBuf, decode: in CodedBuf)
};

// pkzip_putbuf

struct pkzip_putbuf
{
	//flag & data output buffer
	//2: ushort, flag buffer
	//16: flag buffer has 16 bits, data buffer must have 16 bytes for non-encoded characters.
	//    it may be 1 byte if length > 9 and 1 low byte of position for encoded characters.
	byte put_buffer[2 + 16 * 2];
	byte uPutLen;  //left bits in Flag Buffer (ushort), 0---16
	ushort uPutBufIdx;  //index of output data buffer
	byte* pPutBuf = put_buffer;
	ushort* pFlagBuf = (ushort*)put_buffer;
};

// pkzip_compressor

class pkzip_compressor
{
private:
	//hash bucket (pointer group array)
	// byte offsets for search table (5+4)*0x100 = 256*9
	enum { ZIP_SYMB_GROUPS = 0x900 };
	ushort m_ZipGroupOfsTable[ZIP_SYMB_GROUPS];
	// offsets for input buffer 0x4200
	ushort m_ZipSearchTable[2 * MAX_MATCH_RANGE + MATCH_CONNECT_SIZE];

	//buffers
	pkzip_buffer m_Buf;
	pkzip_putbuf m_Put;

	ushort m_uCodingBufIdx;   //the index of position for next input data moved to CodingBuf [0 -- MAX_MATCH_RANGE]
	ushort m_uNextEncodeOfs;  //the offset of next encoding string base on RawBuf
	ushort m_uEncodeMatchEndOfs;
	ushort m_uSearchEndOfs;

	bool m_bDataMoved;

	bool m_bNeedSearch;
	ushort m_uMatchLen;
	ushort m_uMatchRelaOfs;

	//step
	byte m_uEncodeStep;    //step
	byte m_uEndOfSrcFill;  //end flag

	bool m_bLongestSearch;

private:
	ushort& group_ofs_to_buffer_ofs(ushort uGroupOfs) noexcept
	{
		return *((ushort*)((byte*)m_ZipSearchTable + uGroupOfs));
	}

	//(uSrcEndOfs - uSrcOfs) must be <= 65535 / 2
	//uSrcOfs: the start offset of input data (RawBuf)
	//uSrcEndOfs: the end offset of input data, the character pointed to is valid (RawBuf)
	void build_zip_search_table(ushort uSrcOfs, ushort uSrcEndOfs) noexcept
	{
		ushort size = uSrcEndOfs - uSrcOfs;  //size: buffer_size-1, must be >= 1
		mem_zero(m_ZipGroupOfsTable, sizeof(m_ZipGroupOfsTable));
		ushort i, j;
		byte   x, y;
		const byte* pSrc;
		//calculate bytes for each bucket element
		pSrc = m_Buf.pRawBuf + uSrcOfs;
		j = size;
		x = *pSrc ++;
		do {
			y = *pSrc ++;
			m_ZipGroupOfsTable[((ushort)x + (ushort)y) * 4 + (ushort)x] += 2;  //hash value: 5*x+4*y
			x = y;
		} while( --j );
		//each bucket element points to the end of region of search table
		i = 0;
		j = 0;
		do {
			m_ZipGroupOfsTable[j] = (i += m_ZipGroupOfsTable[j]);
		} while( ++j < ZIP_SYMB_GROUPS );  // 900H
		//fill reversely
		//each bucket element points to the start of region of search table
		//offsets are in incremental order in region
		pSrc = m_Buf.pRawBuf + uSrcEndOfs;
		i = uSrcEndOfs;
		j = size;
		y = *pSrc --;
		do {
			i --;
			x = *pSrc --;
			group_ofs_to_buffer_ofs(m_ZipGroupOfsTable[(((ushort)x + (ushort)y) << 2) + (ushort)x] -= 2) = i;
			y = x;
		} while( --j );
	}

	//only encode string if the matching length >= 2
	ushort zip_search(ushort uSrcOfs, ushort uSrcEndOfs, ushort& uMatchRelaOfs) noexcept
	{
		ushort size = uSrcEndOfs - uSrcOfs;  // >=1
		byte x, y;
		x = *(m_Buf.pRawBuf + uSrcOfs);
		y = *(m_Buf.pRawBuf + uSrcOfs + 1);
		ushort uGroupOfsTblIdx = (((ushort)x + (ushort)y) << 2) + (ushort)x;
		ushort uGroupOfs = m_ZipGroupOfsTable[uGroupOfsTblIdx];
		//no match after uSrcOfs
		if( group_ofs_to_buffer_ofs(uGroupOfs) >= uSrcOfs ) return 0;
		//current maximum matching length, must be > 2
		ushort uMatchLen = 2;  //match the last two characters, assume matching at least two characters
		uMatchRelaOfs = 0;  //initialize
		//the beginning offset for searching, uSrcOfs - uMatchBeginOfs < MAX_MATCH_RANGE
		//  before uSrcOfs
		ushort uMatchBeginOfs = uSrcOfs - (MAX_MATCH_RANGE - 1);
		//check searching range from bucket start, revise group offset table
		if( group_ofs_to_buffer_ofs(uGroupOfs) < uMatchBeginOfs ) {
			do {
				uGroupOfs += 2;  //ushort*
			} while( group_ofs_to_buffer_ofs(uGroupOfs) < uMatchBeginOfs );
			m_ZipGroupOfsTable[uGroupOfsTblIdx] = uGroupOfs;  //change first position
		}
		//search
		for( ; ; uGroupOfs += 2 ) { //each offset in searching table
			ushort uCmpDataOfs = group_ofs_to_buffer_ofs(uGroupOfs);
			if( uSrcOfs <= uCmpDataOfs ) { //after
				if( uMatchRelaOfs != 0 ) return uMatchLen; //return matching length if offset is set
				else return 0; //no match
			}
			//compare the last two characters in matched string
			if( *((ushort*)(m_Buf.pRawBuf + uSrcOfs + uMatchLen - 2)) !=
				*((ushort*)(m_Buf.pRawBuf + uCmpDataOfs + uMatchLen - 2)) )
				continue;
			//compare the first character
			if( x != *(m_Buf.pRawBuf + uCmpDataOfs) )
				continue;
			//next characters from 2
			ushort i = 2;
			while( 1 ) {
				//maximum search length
				if( i >= (size + 1) )
					break;
				//compare
				if( *(m_Buf.pRawBuf + uSrcOfs + i) !=
					*(m_Buf.pRawBuf + uCmpDataOfs + i) )
					break;
				//maximum matching length
				if( ++i == MAX_MATCH_LEN ) {
					uMatchRelaOfs = uSrcOfs - uCmpDataOfs;
					return MAX_MATCH_LEN;
				}
			}
			//update if the length of this time is maximum
			//this time is also the nearest matched string to the uSrcOfs
			if( uMatchLen > i ) continue;
			uMatchLen = i;
			uMatchRelaOfs = uSrcOfs - uCmpDataOfs;
		}
	}

	//bits: output bit string, length or high byte of position, bit 0->15
	//len: the number of bits to write
	void put_bit(ushort bits, ushort len) noexcept
	{
		if( m_Put.uPutLen != 0 ) { //rest bits
			*(m_Put.pFlagBuf) |= (bits << (16 - m_Put.uPutLen));  //left shift
		}
		if( m_Put.uPutLen > len ) {
			m_Put.uPutLen -= len; //enough room
		}
		else {
			if( !byte_order_helper::IsBigEndianHost() ) *(m_Put.pFlagBuf) = byte_order_helper::Swap(*(m_Put.pFlagBuf));
			mem_copy(m_Put.pPutBuf, m_Put.uPutBufIdx, m_Buf.pCodedBuf + m_Buf.uCodeStoreOfs); //flag & data buffer are moved to CodedBuf
			m_Buf.uCodeStoreOfs += m_Put.uPutBufIdx;
			m_Put.uPutBufIdx = 2; //reset position of output data buffer
			*(m_Put.pFlagBuf) = bits >> m_Put.uPutLen; //rest bit string
			m_Put.uPutLen += (16 - len);
		}
	}

	//if len == 1, data is non-encoded byte.
	//if len > 1, data is position, len is length.
	void encode(ushort data, ushort len) noexcept
	{
		if( len == 1 ) { // output the byte stored in `data`
			put_bit(0, 1);  //write a bit `0` to FlagBuf
		}
		else {
			put_bit(1, 1);
			if( VL_THRESHOLD > len ) { //length range 2--9, using length encoded table
				put_bit(g_EncodeLenBitsTable[len - 2], g_EncodeLenLenTable[len - 2]);
			}
			else { //length > 9
				//Length > the number of items in encoding tables, variable length
				put_bit(g_EncodeLenBitsTable[LEN_LAST_ELEMENT], g_EncodeLenLenTable[LEN_LAST_ELEMENT]);
				m_Put.pPutBuf[m_Put.uPutBufIdx ++] = len - VL_THRESHOLD;
			}
			if( len != 2 ) { //high-order position byte, non-encoding for len == 2 (postion < 100H)
				//encode position
				put_bit(g_EncodePosBitsTable[data >> 8], g_EncodePosLenTable[data >> 8]);
			}
		}
		m_Put.pPutBuf[m_Put.uPutBufIdx ++] = (byte)data;  //low-order position byte
	}

	//output end flag
	void encode_end() noexcept
	{
		put_bit(1, 1);
		put_bit(g_EncodeLenBitsTable[LEN_LAST_ELEMENT], g_EncodeLenLenTable[LEN_LAST_ELEMENT]);  //length > 9
		m_Put.pPutBuf[m_Put.uPutBufIdx ++] = 0xFF;  //end flag
		//flush
		if( !byte_order_helper::IsBigEndianHost() ) *(m_Put.pFlagBuf) = byte_order_helper::Swap(*(m_Put.pFlagBuf));
		mem_copy(m_Put.pPutBuf, m_Put.uPutBufIdx, m_Buf.pCodedBuf + m_Buf.uCodeStoreOfs);
		m_Buf.uCodeStoreOfs += m_Put.uPutBufIdx;
	}

	//call this method after calling encode(...)
	bool is_output_full() const noexcept
	{
		//after calling encode(...), the number of pending bytes in tail is (1 * 0x22)
		//after calling encode_end(...), PutBuf may be flushed two times. So the number of pending bytes in tail is (2 * 0x22)
		// 2+16*2=0x22
		return m_Buf.uCodeStoreOfs > (m_Buf.uCodedBufSize - 2 * 0x22);  // m_Buf.uCodedBufSize - 0x32
	}

	bool need_input() const noexcept
	{
		return m_uCodingBufIdx < MAX_MATCH_RANGE;
	}
	//move data from extra buffer to CodingBuf
	void source_fill_coding() noexcept
	{
		m_uCodingBufIdx = 0;
		if( m_Buf.uBytesReadInBuf == 0 ) { //rest bytes in extra buffer
			return ;
		}
		ushort uBytesToMove;
		uBytesToMove = numeric_min<ushort>(MAX_MATCH_RANGE - m_uCodingBufIdx, m_Buf.uBytesReadInBuf);
		mem_copy(m_Buf.pCodingBuf + m_Buf.uNextSrcDataOfs, uBytesToMove, m_Buf.pCodingBuf + m_uCodingBufIdx);
		m_Buf.uBytesReadInBuf -= uBytesToMove; //adjust number of bytes in extra buffer
		m_Buf.uNextSrcDataOfs += uBytesToMove; //adjust source data offset in extra buffer
		m_uCodingBufIdx += uBytesToMove;
	}

	void zip_init() noexcept
	{
		m_uCodingBufIdx = 0;
		m_uNextEncodeOfs = MAX_MATCH_RANGE + MATCH_CONNECT_SIZE;  //next encoding string
		m_uEncodeMatchEndOfs = MAX_MATCH_RANGE + MATCH_CONNECT_SIZE;

		m_bDataMoved = false;

		m_bNeedSearch = true;

		m_uEndOfSrcFill = 0;
		m_uEncodeStep = 0;

		m_Buf.uCodeStoreOfs = 0;    //CodedBuf
		m_Buf.uBytesReadInBuf = 0;  //no data

		*(m_Put.pFlagBuf) = 0;  //flag buffer
		m_Put.uPutBufIdx = 2;    //data buffer
		m_Put.uPutLen = 16;      //counter of flag buffer

		m_bLongestSearch = true;
	}

	//return : 0 -- end, 1 -- need input, 2 -- output is full
	int zip_encode() noexcept
	{
		if( m_uEndOfSrcFill == 0 ) {
			if( need_input() ) return 1;
		}
		if( is_output_full() ) return 2;

		//loop
		while( 1 ) {
			//not in encoding loop
			if( m_uNextEncodeOfs >= m_uEncodeMatchEndOfs ) {
				if( m_uEndOfSrcFill == 0 && !m_bDataMoved ) {
					//move processed data
					if( m_uEncodeStep != 0 ) {
						mem_move(m_Buf.pRawBuf + MAX_MATCH_RANGE, MAX_MATCH_RANGE + MATCH_CONNECT_SIZE, m_Buf.pRawBuf); //move data from CodingBuf to RawBuf
						m_uNextEncodeOfs -= MAX_MATCH_RANGE;
						m_uEncodeMatchEndOfs = m_uNextEncodeOfs;
						m_uCodingBufIdx = 0;
					}
					//fill data
					if( m_uCodingBufIdx < MAX_MATCH_RANGE ) {
						source_fill_coding();
						m_bDataMoved = true;
						if( need_input() ) return 1;
					}
				}

				m_bDataMoved = false;
				m_uEncodeMatchEndOfs = MAX_MATCH_RANGE + MATCH_CONNECT_SIZE + m_uCodingBufIdx;

				//step 4
				ushort size = m_uEncodeMatchEndOfs - m_uNextEncodeOfs;
				if( m_uEndOfSrcFill != 0 && size < 2 ) { // 0 or 1
					if( size == 1 ) {
						encode(*(m_Buf.pRawBuf + m_uNextEncodeOfs), 1);  //non-encoding
						m_uNextEncodeOfs ++;
						if( is_output_full() ) return 2;
					}
					//output end flag
					encode_end();
					return 0;
				}

				m_uSearchEndOfs = m_uEncodeMatchEndOfs - 1;
				if( m_uEndOfSrcFill == 0 ) m_uEncodeMatchEndOfs -= MAX_MATCH_LEN; //not step 4

				//steps
/*
Maximum length of search table: RawBuf + CodingBuf, 0x4200
0: beginning of CodingBuf -> m_uEncodeMatchEndOfs - 1
1: last non-encoding position - 0x2000 + MAX_MATCH_LEN -> m_uEncodeMatchEndOfs - 1, after data moved to RawBuf
2: last non-encoding position - 0x2000 -> m_uEncodeMatchEndOfs - 1, after data moved to RawBuf
3: start position of any previous step -> position of data end in CodingBuf, when m_uEndOfSrcFill == 1
*/
				if( m_uEncodeStep == 2 ) {
					build_zip_search_table(m_uNextEncodeOfs - MAX_MATCH_RANGE, m_uEncodeMatchEndOfs - 1);
				}
				else if( m_uEncodeStep == 1 ) {
					build_zip_search_table(m_uNextEncodeOfs - MAX_MATCH_RANGE + MAX_MATCH_LEN, m_uEncodeMatchEndOfs - 1);
					m_uEncodeStep ++;
				}
				else { //==0
					build_zip_search_table(m_uNextEncodeOfs, m_uEncodeMatchEndOfs - 1);
					m_uEncodeStep ++;
				}
			}

			//compressing loop
			do {
				//step 4, last byte
				if( m_uEndOfSrcFill != 0 && m_uEncodeMatchEndOfs - m_uNextEncodeOfs < 2 ) {
					assert( m_uEncodeMatchEndOfs - m_uNextEncodeOfs == 1 );
 					encode(*(m_Buf.pRawBuf + m_uNextEncodeOfs), 1);  //non-encoding
					m_uNextEncodeOfs ++;
					if( is_output_full() ) return 2;
					continue;
				}

				if( m_bNeedSearch ) {
					if( (m_uMatchLen = zip_search(m_uNextEncodeOfs, m_uSearchEndOfs, m_uMatchRelaOfs)) == 0 ) { //search matching string
						encode(*(m_Buf.pRawBuf + m_uNextEncodeOfs), 1);  //non-encoding
						m_uNextEncodeOfs ++;
						if( is_output_full() ) return 2;
						continue;
					}
				}

				//maximum length loop
				do {
					m_bNeedSearch = true;

					if( m_uMatchLen == 2 && m_uMatchRelaOfs >= 0x100 ) {
						encode(*(m_Buf.pRawBuf + m_uNextEncodeOfs), 1);  //non-encoding
						m_uNextEncodeOfs ++;
						if( is_output_full() ) return 2;
						break;
					}

					if( m_bLongestSearch && (
						(m_uEndOfSrcFill == 0 && m_uNextEncodeOfs < (m_uEncodeMatchEndOfs - 1))
						|| (m_uEndOfSrcFill != 0 && m_uNextEncodeOfs < (m_uEncodeMatchEndOfs - m_uMatchLen - 1)) //step 4, check if the matching string exceed m_uEncodeMatchEndOfs?
						)
						) {
						ushort uSaveMatchRelaOfs = m_uMatchRelaOfs;
						ushort uSaveMatchLen = m_uMatchLen; // save position and length, search maximum length string from next byte
						if( ((m_uMatchLen = zip_search(m_uNextEncodeOfs + 1, m_uSearchEndOfs, m_uMatchRelaOfs)) > uSaveMatchLen) &&
							(uSaveMatchLen + 1 < m_uMatchLen || uSaveMatchRelaOfs > 0x80) ) {
							m_bNeedSearch = false;
							encode(*(m_Buf.pRawBuf + m_uNextEncodeOfs), 1); //non-encoding
							m_uNextEncodeOfs ++;
							if( is_output_full() ) return 2;
							continue;
						}
						encode(uSaveMatchRelaOfs, uSaveMatchLen); //not longer than the first one
						m_uNextEncodeOfs += uSaveMatchLen;
						if( is_output_full() ) return 2;
						break;
					}
					//the match string length exceed m_uEncodeMatchEndOfs
					encode(m_uMatchRelaOfs, m_uMatchLen); //length position code
					m_uNextEncodeOfs += m_uMatchLen;
					if( is_output_full() ) return 2;
					break;
				} while ( 1 );
			} while( m_uNextEncodeOfs < m_uEncodeMatchEndOfs );

			if( m_uEndOfSrcFill != 0 ) break; //end
		}

		//end flag
		encode_end();
		return 0;
	}

public:
	pkzip_compressor() noexcept
	{
		Initialize();
	}
	~pkzip_compressor() noexcept
	{
	}

	void Initialize() noexcept
	{
		zip_init();
	}
	void SetLongestSearch(bool b) noexcept
	{
		m_bLongestSearch = b;
	}

	void GetInputBuffer(byte*& pBuffer, ushort& uLength) noexcept
	{
		pBuffer = m_Buf.pCodingBuf + m_uCodingBufIdx;
		uLength = m_Buf.uCodingBufSize - m_uCodingBufIdx;
	}
	void SetInputSize(ushort uSize) noexcept
	{
		assert( uSize <= m_Buf.uCodingBufSize - m_uCodingBufIdx );
		m_uCodingBufIdx += uSize;
		if( m_uCodingBufIdx >= MAX_MATCH_RANGE ) {
			m_Buf.uBytesReadInBuf = m_uCodingBufIdx - MAX_MATCH_RANGE;
			m_uCodingBufIdx = MAX_MATCH_RANGE;
			m_Buf.uNextSrcDataOfs = MAX_MATCH_RANGE;
		}
	}
	void GetOutputBuffer(byte*& pBuffer, ushort& uLength) noexcept
	{
		pBuffer = m_Buf.pCodedBuf;
		uLength = m_Buf.uCodeStoreOfs;
	}
	void ResetOutputBuffer() noexcept
	{
		m_Buf.uCodeStoreOfs = 0;
	}

	//return : 1 -- need input, 2 -- output is full
	int Encode() noexcept
	{
		return zip_encode();
	}

	void StartFinish() noexcept
	{
		m_uEndOfSrcFill = 1;
	}
	//return : 0 -- end, 2 -- output is full
	int ProcessFinish() noexcept
	{
		return zip_encode();
	}
};

//decoding matrix of matching length
extern byte g_DecodeLenTable[];
//decoding matrix of high-order byte of position
extern byte g_DecodePosTable[];

// pkzip_decompressor

class pkzip_decompressor
{
private:
	//flag buffer
	byte   m_uBitsInGetBuf;  //rest bits in flag buffer
	ushort m_uGetBuf;        //flag buffer

	//current bit & byte
	byte   m_bit_num;
	ushort m_bit;
	ushort m_len;
	ushort m_pos;

	pkzip_buffer m_Buf;

	ushort m_uCodeStoreEndOfs;

	byte m_step;  //InLenBits InLenLen InPosBits InPosLen

private:
	//uRequest : 1 or 2
	bool check_request_bytes(byte uRequest) noexcept
	{
		if( m_Buf.uBytesReadInBuf >= uRequest ) return true;
		if( m_Buf.uBytesReadInBuf == 0 ) { //no data left in buffer
			m_Buf.uNextSrcDataOfs = 0;
			return false;
		}
		if( m_Buf.uNextSrcDataOfs != 0 ) {
			//move to head
			mem_move(m_Buf.pCodedBuf + m_Buf.uNextSrcDataOfs, m_Buf.uBytesReadInBuf, m_Buf.pCodedBuf);
			m_Buf.uNextSrcDataOfs = 0;
		}
		return false;
	}
	//num : 1 or 2
	ushort zip_get_bytes(byte num) noexcept
	{
		m_Buf.uBytesReadInBuf -= num;
		ushort data = *((ushort*)(m_Buf.pCodedBuf + m_Buf.uNextSrcDataOfs));
		m_Buf.uNextSrcDataOfs += num;
		return data;
	}

	//flag buffer
	ushort get_bit() noexcept
	{
		ushort bit = m_uGetBuf & 1; //bit 0
		-- m_uBitsInGetBuf;
		m_uGetBuf >>= 1; //remove bit 0
		return bit;
	}
	bool check_flag_buffer() noexcept
	{
		//process empty flag buffer
		if( m_uBitsInGetBuf == 0 ) { //empty
			if( !check_request_bytes(2) ) return false;
			m_uGetBuf = zip_get_bytes(2);
			if( !byte_order_helper::IsBigEndianHost() ) m_uGetBuf = byte_order_helper::Swap(m_uGetBuf);
			m_uBitsInGetBuf = 16;
		}
		return true;
	}

	bool is_output_full() const noexcept
	{
		return m_Buf.uCodeStoreOfs >= m_uCodeStoreEndOfs;
	}

	void init() noexcept
	{
		//flag buffer
		m_uBitsInGetBuf = 0;

		//current
		m_bit_num = 0;

		//buffer
		m_Buf.uBytesReadInBuf = 0;
		m_Buf.uNextSrcDataOfs = 0;
		m_Buf.uCodeStoreOfs = MAX_MATCH_RANGE;

		//output
		m_uCodeStoreEndOfs = MAX_MATCH_RANGE + MATCH_CONNECT_SIZE + m_Buf.uCodingBufSize - 0x125; //uCodingBufSize + 0x1EDB (0x1EDB = <RawBuf size (0x2000)> - 0x125), if only 0x125 > 0x106

		//step
		m_step = 0; //InLenBits
	}

	// return 0 -- end  1 -- need input  2 -- output is full
	int zip_decode() noexcept
	{
		if( !check_flag_buffer() ) return 1;
		if( is_output_full() ) return 2;

		do {
			//InLenBits or InPosBits
			if( m_step == 0 || m_step == 2 ) {
				while( 1 ) {
					//process bit
					if( m_bit_num != 0 ) {
						if( m_step == 0 ) {
							if( m_bit_num == 1 ) {
								if( m_bit == 0 ) { //non-encoding
									m_len = 1;
									m_step = 1;  //InLenLen
									break;
								}
							}
							else if( m_bit_num == 2 ) {
								m_len = m_bit;
							}
							else {
								m_len = (m_len << 1) | m_bit;
								if( m_bit_num == 3 && m_len == 0 ) { //read 2 bits, length = 3
									m_len = g_DecodeLenTable[m_len];
									m_step = 2; //InPosBits
									break;
								}
								else if( m_bit_num == 4 && m_len <= 5 ) { //read 3 bits, 011,101,100,010, -> 2,4,5,>9
									m_len = g_DecodeLenTable[m_len];
									m_step = 1; //InLenLen
									break;
								}
								else if( m_bit_num == 5 ) {
									m_len = g_DecodeLenTable[m_len];
									m_step = 1; //InLenLen
									break;
								}
							}
						}
						else {
							if( m_bit_num == 1 ) {
								if( m_bit != 0 ) {
									m_pos = 0;
									m_step = 3;  //InPosLen
									break;
								}
							}
							else if( m_bit_num == 2 ) {
								m_pos = m_bit;
							}
							else {
								m_pos = (m_pos << 1) | m_bit;
								if( m_bit_num == 4 && m_pos <= 1 ) { //read 3 bits, 000 or 001, -> 1 or 2 (high byte)
									m_pos = (ushort)g_DecodePosTable[m_pos] << 8;
									m_step = 3; //InPosLen
									break;
								}
								else if( m_bit_num == 5 && m_pos <= 7 ) { //read 4 bits, if < 8, -> 3,4,5,6 (high byte)
									m_pos = (ushort)g_DecodePosTable[m_pos] << 8;
									m_step = 3; //InPosLen
									break;
								}
								else if( m_bit_num == 6 && m_pos <= 0x16 ) { //read 5 bits
									m_pos = (ushort)g_DecodePosTable[m_pos] << 8;
									m_step = 3; //InPosLen
									break;
								}
								else if( m_bit_num == 7 ) { //read 6 bits, remove high bits
									m_pos = (m_pos & 0xDF) << 8;
									m_step = 3; //InPosLen
									break;
								}
							}
						} //end if m_step
					} //end if m_bit_num != 0

					//get bit
					m_bit = get_bit();
					m_bit_num ++;

					//process empty flag buffer
					if( !check_flag_buffer() ) return 1;
				}

				m_bit_num = 0; //reset
			}

			//bytes
			if( m_step == 1 ) { //InLenLen
				if( m_len == 1 ) {
					if( !check_request_bytes(1) ) return 1;
					*(m_Buf.pRawBuf + m_Buf.uCodeStoreOfs) = (byte)zip_get_bytes(1);
					m_Buf.uCodeStoreOfs ++;
					m_step = 0; //InLenBits
					if( is_output_full() ) return 2;
				}
				else if( m_len == 10 ) { //length>=10, read a byte, v+10
					if( !check_request_bytes(1) ) return 1;
					if( (m_len += (ushort)(byte)zip_get_bytes(1)) == 0x109 ) break; //0xFF, end
					m_step = 2; //InPosBits
				}
				else if( m_len == 2 ) {
					m_pos = 0;
					m_step = 3; //InPosLen
				}
				else {
					m_step = 2; //InPosBits
				}
			}
			else if( m_step == 3 ) { //InPosLen
				if( !check_request_bytes(1) ) return 1;
				m_pos |= (ushort)(byte)zip_get_bytes(1); //low byte
				ushort i = m_Buf.uCodeStoreOfs - m_pos; //copy string
				do {
					*(m_Buf.pRawBuf + m_Buf.uCodeStoreOfs) = *(m_Buf.pRawBuf + i);
					i ++;
					m_Buf.uCodeStoreOfs ++;
				} while( --m_len );
				m_step = 0; //InLenBits
				if( is_output_full() ) return 2;
			}
		} while( 1 );

		//end
		return 0;
	}

public:
	pkzip_decompressor() noexcept
	{
		Initialize();
	}
	~pkzip_decompressor() noexcept
	{
	}

	void Initialize() noexcept
	{
		init();
	}

	void GetInputBuffer(byte*& pBuffer, ushort& uLength) noexcept
	{
		ushort uPos = m_Buf.uNextSrcDataOfs + m_Buf.uBytesReadInBuf;
		pBuffer = m_Buf.pCodedBuf + uPos;
		uLength = m_Buf.uCodedBufSize - uPos;
	}
	void SetInputSize(ushort uSize) noexcept
	{
		assert( uSize <= m_Buf.uCodedBufSize - (m_Buf.uNextSrcDataOfs + m_Buf.uBytesReadInBuf) );
		m_Buf.uBytesReadInBuf += uSize;
	}
	void GetOutputBuffer(byte*& pBuffer, ushort& uLength) noexcept
	{
		pBuffer = m_Buf.pRawBuf + MAX_MATCH_RANGE;
		uLength = m_Buf.uCodeStoreOfs - MAX_MATCH_RANGE;
	}
	void ResetOutputBuffer() noexcept
	{
		mem_copy(m_Buf.pRawBuf + m_Buf.uCodeStoreOfs - MAX_MATCH_RANGE, MAX_MATCH_RANGE, m_Buf.pRawBuf); //move previous encoded data (0x2000) to RawBuf
		m_Buf.uCodeStoreOfs = MAX_MATCH_RANGE; //reset output offset
	}

	// return 0 -- end  1 -- need input  2 -- output is full
	int Decode() noexcept
	{
		return zip_decode();
	}
};

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
