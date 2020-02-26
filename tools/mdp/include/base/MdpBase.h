/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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
#ifndef __MDP_BASE_H__
#define __MDP_BASE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// ProjectInfo

class ProjectInfo
{
private:
	//states
	enum {
		STATE_START = 1,
		STATE_BASE,
		STATE_LANG,
		STATE_COMMANDS,
		STATE_READING_ORDER,
		STATE_LINE_LAYOUT,
		STATE_TREE,
		STATE_TREE_IN_NODE,
		STATE_TREE_IN_GUIDE
	};

public:
	ProjectInfo() throw() : m_uLCID(0), m_bRTLorder(false), m_bVerticalLine(false),
							m_tree(MemoryHelper::GetCrtMemoryManager())
	{
	}
	~ProjectInfo() throw()
	{
	}

	const StringA& GetProjectName() const throw()
	{
		return m_strProjectName;
	}
	const StringA& GetDocumentDirectory() const throw()
	{
		return m_strDocumentDirectory;
	}
	uint GetLCID() const throw()
	{
		return m_uLCID;
	}
	const ShareArray<StringA>& GetCommandArray() const throw()
	{
		return m_arrCommand;
	}
	const StringA& GetCoverImageFile() const throw()
	{
		return m_strCoverImageFile;
	}
	const StringA& GetCoverName() const throw()
	{
		return m_strCoverName;
	}
	const StringA& GetTopic() const throw()
	{
		return m_strTopic;
	}
	const StringA& GetIdentifier() const throw()
	{
		return m_strIdentifier;
	}
	const MultiwayTree<Pair<StringA, StringA>>& GetFileTree() const throw()
	{
		return m_tree;
	}
	const StringA& GetAuthor() const throw()
	{
		return m_strAuthor;
	}
	const StringA& GetDescription() const throw()
	{
		return m_strDescription;
	}
	const StringA& GetDate() const throw()
	{
		return m_strDate;
	}
	const StringA& GetContributor() const throw()
	{
		return m_strContributor;
	}
	const StringA& GetPublisher() const throw()
	{
		return m_strPublisher;
	}
	const StringA& GetSubject() const throw()
	{
		return m_strSubject;
	}
	const StringA& GetRights() const throw()
	{
		return m_strRights;
	}
	bool IsRTLorder() const throw()
	{
		return m_bRTLorder;
	}
	bool IsVerticalLine() const throw()
	{
		return m_bVerticalLine;
	}

	bool GetTitleFile(StringA& strName, StringA& strFile) const throw()
	{
		return get_guide_file(m_iterTitle, strName, strFile);
	}
	bool GetContentFile(StringA& strName, StringA& strFile) const throw()
	{
		return get_guide_file(m_iterContent, strName, strFile);
	}
	bool GetCopyrightFile(StringA& strName, StringA& strFile) const throw()
	{
		return get_guide_file(m_iterCopyright, strName, strFile);
	}

	//load project file with WON format
	bool Load(const ConstStringS& strFile)
	{
		CallResult cr;

		ShareCom<ITextStream> spText;
		{
			ShareCom<IByteStream> spStream;
			cr = StreamHelper::CreateFileStream(strFile, FileOpenTypes::Read, 0, spStream);
			if( cr.IsFailed() )
				return false;
			cr = StreamHelper::CreateTextStream(spText);
			if( cr.IsFailed() )
				return false;
			spText.Deref().SetStream(spStream);
			// BOM
			int iType;
			cr = spText.Deref().CheckBOM(iType);
			if( cr.IsFailed() )
				return false;
			if( iType != BOMTypes::UTF8 )
				return false;
		} //end block

		ShareCom<IWonParser> spParser;
		{
			ShareCom<IWonUtility> spU;
			cr = ParserHelper::CreateWonUtility(spU);
			if( cr.IsFailed() )
				return false;
			cr = spU.Deref().Initialize();
			if( cr.IsFailed() )
				return false;
			cr = ParserHelper::CreateWonParser(spParser);
			if( cr.IsFailed() )
				return false;
			cr = spParser.Deref().Initialize(spU);
			if( cr.IsFailed() )
				return false;
		} //end block

		spParser.Deref().SetInput(spText);
		cr = spParser.Deref().Start();
		if( cr.IsFailed() )
			return false;

		StringA strToken;
		RefPtr<StringA> refString;
		MultiwayTree<Pair<StringA, StringA>>::Iterator iterParent;
		MultiwayTree<Pair<StringA, StringA>>::Iterator iterAfter;

		m_iterTitle = MultiwayTree<Pair<StringA, StringA>>::Iterator();
		m_iterContent = MultiwayTree<Pair<StringA, StringA>>::Iterator();
		m_iterCopyright = MultiwayTree<Pair<StringA, StringA>>::Iterator();

		int iState = STATE_START;
		//loop
		while( true ) {
			int iType;
			cr = spParser.Deref().GetNextToken(iType, strToken);
			if( cr.IsFailed() )
				return false;
			if( cr.GetResult() == SystemCallResults::S_False )
				break;
			//types
			switch( iType ) {
			case WonTokenTypes::BeginObject:
				if( iState == STATE_START ) {
					iState = STATE_BASE;
				}
				else if( iState == STATE_TREE ) {
					iterAfter = m_tree.Insert(iterParent, iterAfter);  //may throw
				}
				else {
					return false;
				}
				break;
			case WonTokenTypes::EndObject:
				if( iState == STATE_BASE ) {
					iState = STATE_START;
				}
				else if( iState == STATE_TREE ) {
					;
				}
				else {
					return false;
				}
				break;
			case WonTokenTypes::BeginArray:
				if( iState == STATE_COMMANDS ) {
					;
				}
				else if( iState == STATE_TREE ) {
					;
				}
				else {
					return false;
				}
				break;
			case WonTokenTypes::EndArray:
				if( iState == STATE_COMMANDS ) {
					;
				}
				else if( iState == STATE_TREE ) {
					;
				}
				else {
					return false;
				}
				break;
			case WonTokenTypes::Key:
				if( iState == STATE_BASE ) {
					if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"project-name\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strProjectName);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"document-directory\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strDocumentDirectory);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"language\"")) ) {
						iState = STATE_LANG;
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"command\"")) ) {
						if( m_arrCommand.IsBlockNull() )
							m_arrCommand = ShareArrayHelper::MakeShareArray<StringA>(MemoryHelper::GetCrtMemoryManager());  //may throw
						m_arrCommand.RemoveAll();
						iState = STATE_COMMANDS;
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"cover\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strCoverImageFile);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"cover-name\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strCoverName);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"topic\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strTopic);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"identifier\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strIdentifier);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"tree\"")) ) {
						m_tree.RemoveAll();
						iState = STATE_TREE;
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"author\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strAuthor);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"description\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strDescription);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"date\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strDate);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"contributor\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strContributor);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"publisher\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strPublisher);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"subject\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strSubject);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"rights\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(m_strRights);
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"reading-order\"")) ) {
						iState = STATE_READING_ORDER;
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"line-layout\"")) ) {
						iState = STATE_LINE_LAYOUT;
					}
					else {
						return false;
					}
				}
				else if( iState == STATE_TREE ) {
					if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"tree\"")) ) {
						//enter the next level
						iterParent = iterAfter;
						iterAfter = MultiwayTree<Pair<StringA, StringA>>::Iterator();
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"name\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(iterAfter.get_Value().get_First());
						iState = STATE_TREE_IN_NODE;
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"file\"")) ) {
						refString = RefPtrHelper::MakeRefPtr<StringA>(iterAfter.get_Value().get_Second());
						iState = STATE_TREE_IN_NODE;
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "\"guide\"")) ) {
						iState = STATE_TREE_IN_GUIDE;
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
				break;
			case WonTokenTypes::EndPair:
				if( iState == STATE_BASE ) {
					;
				}
				else if( iState == STATE_COMMANDS ) {
					iState = STATE_BASE;
				}
				else if( iState == STATE_TREE_IN_NODE ) {
					iState = STATE_TREE;
				}
				else if( iState == STATE_TREE_IN_GUIDE ) {
					iState = STATE_TREE;
				}
				else if( iState == STATE_TREE ) {
					if( !iterParent.IsNull() && iterParent != m_tree.GetRoot() ) {
						//back to parent
						iterAfter = iterParent;
						iterParent = iterAfter;
						iterParent.MoveParent();
					}
					else {
						iState = STATE_BASE;
					}
				}
				else {
					return false;
				}
				break;
			case WonTokenTypes::ValueString:
				delete_quotes(strToken);
				if( iState == STATE_BASE ) {
					refString.Deref() = strToken;
				}
				else if( iState == STATE_TREE_IN_NODE ) {
					refString.Deref() = strToken;
				}
				else if( iState == STATE_TREE_IN_GUIDE ) {
					if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "title")) ) {
						if( !m_iterTitle.IsNull() )
							return false;
						m_iterTitle = iterAfter;
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "content")) ) {
						if( !m_iterContent.IsNull() )
							return false;
						m_iterContent = iterAfter;
					}
					else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "copyright")) ) {
						if( !m_iterCopyright.IsNull() )
							return false;
						m_iterCopyright = iterAfter;
					}
					else {
						return false;
					}
				}
				else if( iState == STATE_COMMANDS ) {
					m_arrCommand.Add(strToken);  //may throw
				}
				else if( iState == STATE_READING_ORDER ) {
					if( !strToken.IsEmpty() ) {
						if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "rtl")) ) {
							m_bRTLorder = true;
						}
						else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "ltr")) ) {
							m_bRTLorder = false;
						}
						else {
							return false;
						}
					}
					iState = STATE_BASE;
				}
				else if( iState == STATE_LINE_LAYOUT ) {
					if( !strToken.IsEmpty() ) {
						if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "v")) ) {
							m_bVerticalLine = true;
						}
						else if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strToken), DECLARE_TEMP_CONST_STRING(ConstStringA, "h")) ) {
							m_bVerticalLine = false;
						}
						else {
							return false;
						}
					}
					iState = STATE_BASE;
				}
				else {
					return false;
				}
				break;
			case WonTokenTypes::ValueNumberInteger:
				if( iState == STATE_LANG ) {
					bool bOK;
					string_to_value(ShareArrayHelper::GetInternalPointer(strToken), 10, m_uLCID, bOK);  //no check
					assert( bOK );
					if( m_uLCID == 0 )
						m_uLCID = 1033;
					iState = STATE_BASE;
				}
				else {
					return false;
				}
				break;
			default:
				return false;
				break;
			} //end switch
		} //end while

		assert( iState == STATE_START );

		return true;
	}

private:
	static bool get_guide_file(const MultiwayTree<Pair<StringA, StringA>>::Iterator& iter, StringA& strName, StringA& strFile) throw()
	{
		if( iter.IsNull() )
			return false;
		strName = iter.get_Value().get_First();
		strFile = iter.get_Value().get_Second();
		return true;
	}

	void delete_quotes(StringA& str) throw()
	{
		assert( str.GetLength() >= 2 );
		StringHelper::Delete(str.GetLength() - 1, 1, str);
		StringHelper::Delete(0, 1, str);
	}

private:
	StringA m_strProjectName;
	StringA m_strDocumentDirectory;
	uint    m_uLCID;
	ShareArray<StringA>  m_arrCommand;
	StringA m_strCoverImageFile;
	StringA m_strCoverName;
	StringA m_strTopic;
	StringA m_strIdentifier;
	MultiwayTree<Pair<StringA, StringA>>  m_tree;
//optional
	StringA m_strAuthor;
	StringA m_strDescription;
	StringA m_strDate;
	StringA m_strContributor;
	StringA m_strPublisher;
	StringA m_strSubject;
	StringA m_strRights;
	bool m_bRTLorder;
	bool m_bVerticalLine;

	MultiwayTree<Pair<StringA, StringA>>::Iterator m_iterTitle,
		m_iterContent,
		m_iterCopyright;

private:
	//noncopyable
	ProjectInfo(const ProjectInfo&) throw();
	ProjectInfo& operator=(const ProjectInfo&) throw();
};

// FileTreeEnumerator

class FileTreeEnumerator
{
public:
	FileTreeEnumerator(const MultiwayTree<Pair<StringA, StringA>>& tree) throw() : m_tree(tree), m_bHaveChild(false), m_uLevel(0), m_uDelta(0), m_bNegative(false)
	{
	}
	~FileTreeEnumerator() throw()
	{
	}

	bool FindFirst() throw()
	{
		m_bHaveChild = false;
		m_uLevel = 1;
		m_uDelta = 0;
		m_bNegative = false;
		m_iter = m_tree.GetRoot();
		if( m_iter.IsNull() )
			return false;
		m_iter.MoveChild();
		if( m_iter.IsNull() )
			return false;
		check_child();
		return true;
	}
	bool FindNext() throw()
	{
		auto iter(m_iter);
		m_bNegative = false;
		//child
		iter.MoveChild();
		if( !iter.IsNull() ) {
			m_iter = iter;
			check_child();
			m_uLevel ++;
			m_uDelta = 1;
			return true;
		}
		//next
		iter = m_iter;
		iter.MoveNext();
		if( !iter.IsNull() ) {
			m_iter = iter;
			check_child();
			m_uDelta = 0;
			return true;
		}
		//parent
		iter = m_iter;
		m_uDelta = 0;
		m_bNegative = true;
		while( true ) {
			iter.MoveParent();
			m_uLevel --;
			m_uDelta ++;
			if( iter.IsNull() || iter == m_tree.GetRoot() ) {
				-- m_uDelta;
				break;
			}
			iter.MoveNext();
			if( !iter.IsNull() ) {
				m_iter = iter;
				check_child();
				return true;
			}
		} //end while
		if( m_uDelta == 0 )
			m_bNegative = false;
		return false;
	}

	ConstStringA GetName() const throw()
	{
		return StringUtilHelper::To_ConstString(m_iter.get_Value().get_First());
	}
	ConstStringA GetFile() const throw()
	{
		return StringUtilHelper::To_ConstString(GetFileString());
	}
	StringA GetFileString() const throw()
	{
		return m_iter.get_Value().get_Second();
	}
	bool IsLeaf() const throw()
	{
		return !m_bHaveChild;
	}
	uintptr GetLevel() const throw()
	{
		return m_uLevel;
	}
	uintptr GetDelta(bool& bNegative) const throw()
	{
		bNegative = m_bNegative;
		return m_uDelta;
	}

private:
	void check_child() throw()
	{
		auto iter(m_iter);
		iter.MoveChild();
		m_bHaveChild = !iter.IsNull();
	}

private:
	const MultiwayTree<Pair<StringA, StringA>>& m_tree;
	MultiwayTree<Pair<StringA, StringA>>::Iterator m_iter;
	bool m_bHaveChild;
	uintptr m_uLevel;
	uintptr m_uDelta;
	bool m_bNegative;

private:
	//noncopyable
	FileTreeEnumerator(const FileTreeEnumerator&) throw();
	FileTreeEnumerator& operator=(const FileTreeEnumerator&) throw();
};

// FileListT<TString>

template <class TString>
class FileListT
{
private:
	typedef HashList<TString, StringHashTrait<TString>, StringCompareTrait<TString>>  HashListClass;

public:
	FileListT() throw() : m_hashList(MemoryHelper::GetCrtMemoryManager())
	{
	}
	~FileListT() throw()
	{
	}

	uintptr GetCount() const throw()
	{
		return m_arrFile.GetCount();
	}
	const TString& GetAt(uintptr uIndex) const throw()
	{
		assert( uIndex < GetCount() );
		return m_arrFile.GetAt(uIndex).get_Value().get_Value();
	}
	bool Find(const TString& str) const throw()
	{
		return m_hashList.Find(str) != m_hashList.GetEnd();
	}

	void Clear() throw()
	{
		m_hashList.RemoveAll();
		if( !m_arrFile.IsBlockNull() )
			m_arrFile.RemoveAll();
	}
	//add a file
	void AddFile(const TString& str)
	{
		if( Find(str) )
			return ;
		auto iter(m_hashList.InsertWithoutFind(str));  //may throw
		if( m_arrFile.IsBlockNull() )
			m_arrFile = ShareArrayHelper::MakeShareArray<typename HashListClass::Iterator>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_arrFile.Add(iter);  //may throw
	}

private:
	HashListClass  m_hashList;
	ShareArray<typename HashListClass::Iterator>  m_arrFile;

private:
	//noncopyable
	FileListT(const FileListT&) throw();
	FileListT& operator=(const FileListT&) throw();
};

// DirFileList
typedef FileListT<StringS>  DirFileList;

// FileListInfo
typedef FileListT<StringA>  FileListInfo;

// _Generate_Project_FileList
inline void _Generate_Project_FileList(const ProjectInfo& info, FileListInfo& flInfo)
{
	FileTreeEnumerator ftEnum(info.GetFileTree());
	bool bFound = ftEnum.FindFirst();
	while( bFound ) {
		ConstStringA strFile(ftEnum.GetFile());
		StringA strV(ftEnum.GetFileString());
		auto iter(ConstStringHelper::FindLast(strFile, '#', 0));
		if( !iter.IsNull() ) {
			StringA strTemp(StringHelper::Clone(strV));  //may throw
			uintptr uPos = iter.CalcDelta(strFile.GetBegin());
			StringHelper::Delete(uPos, strFile.GetCount() - uPos, strTemp);
			strV = strTemp;
		}
		flInfo.AddFile(strV);  //may throw
		bFound = ftEnum.FindNext();
	} //end while
}

//tools

inline
bool _CheckFileExtension(const ConstStringS& str, const ConstStringS& strExt, uintptr& uPos) throw()
{
	if( !FsPathHelper::FindExtensionStart(str, uPos) )
		return false;
	return ConstStringCompareTrait<ConstStringS>::IsEQ(
		ConstStringS(ConstArrayHelper::GetInternalPointer(str) + uPos, str.GetCount() - uPos),
		strExt);
}

//copy file
inline bool _Copy_File(const ConstStringS& strSrc, const ConstStringS& strDest) throw()
{
	CallResult cr;
	ShareCom<IByteStream> spSrc;
	cr = StreamHelper::CreateFileStream(strSrc, FileOpenTypes::Read, 0, spSrc);
	if( cr.IsFailed() )
		return false;
	ShareCom<IByteStream> spDest;
	cr = StreamHelper::CreateFileStream(strDest, FileOpenTypes::Write, FileCreationTypes::Create, spDest);
	if( cr.IsFailed() )
		return false;
	cr = StreamHelper::Copy(spSrc, spDest);
	if( cr.IsFailed() )
		return false;
	return true;
}
inline bool _Copy_MD(const FileListInfo& flInfo, const ConstStringA& strExt,
					const ConstStringS& strSrc, const ConstStringS& strDest)
{
	CallResult cr;

	ShareCom<ITextStream> spText;
	cr = StreamHelper::CreateTextStream(spText);
	if( cr.IsFailed() )
		return false;

	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw

	//load
	{
		ShareCom<IByteStream> spSrc;
		cr = StreamHelper::CreateFileStream(strSrc, FileOpenTypes::Read, 0, spSrc);
		if( cr.IsFailed() )
			return false;
		spText.Deref().SetStream(spSrc);
		// BOM
		int iType;
		cr = spText.Deref().CheckBOM(iType);
		if( cr.IsFailed() )
			return false;
		if( iType != BOMTypes::UTF8 )
			return false;
		cr = spText.Deref().GetAllStringA(strContent);
		if( cr.IsFailed() )
			return false;
	} //end block

	//replace
	uintptr uCount = flInfo.GetCount();
	for( uintptr i = 0; i < uCount; i ++ ) {
		ConstStringA strFile(StringUtilHelper::To_ConstString(flInfo.GetAt(i)));
		uintptr uStart = FsPathHelper::FindFilePartStart(strFile);
		ConstArrayHelper::SetInternalPointer(ConstArrayHelper::GetInternalPointer(strFile) + uStart, strFile.GetCount() - uStart, strFile);
		uintptr uPos = 0;
		while( true ) {
			//find
			auto iter(StringUtilHelper::Find(strContent, strFile, uPos));
			if( iter.IsNull() )
				break;
			uPos = iter.CalcDelta(strContent.GetBegin());
			//.md -> .xxx...
			uPos += (strFile.GetCount() - 3);
			uPos = StringUtilHelper::Replace(uPos, 3, strExt, strContent);  //may throw
		}
	} //end for

	//write
	{
		ShareCom<IByteStream> spDest;
		cr = StreamHelper::CreateFileStream(strDest, FileOpenTypes::Write, FileCreationTypes::Create, spDest);
		if( cr.IsFailed() )
			return false;
		spText.Deref().SetStream(spDest);
		spText.Deref().Reset();
	} //end block
	spText.Deref().SetBOM(BOMTypes::UTF8);
	cr = spText.Deref().WriteBOM();
	if( cr.IsFailed() )
		return false;
	cr = spText.Deref().PutStringA(StringUtilHelper::To_ConstString(strContent));
	if( cr.IsFailed() )
		return false;

	return true;
}

//scan file tree
inline bool _Scan_FileTree(const ConstStringS& strSrcRoot, const ConstStringS& strDestRoot,
						DirFileList& flMd, DirFileList& flAux)
{
	StringS strDir(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringS strRelative(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//tasks
	List<StringS> listTask(MemoryHelper::GetCrtMemoryManager());
	//add
	listTask.AddTail(strRelative);  //may throw
	//loop
	StringS strTemp;
	FileSearcher fs;
	while( !listTask.IsEmpty() ) {
		//head
		strRelative = listTask.GetBegin().get_Value();
		listTask.RemoveHead();
		//destination
		if( !strRelative.IsEmpty() ) {
			StringUtilHelper::MakeString(strDestRoot, strDir);  //may throw
			FsPathHelper::AppendSeparator(strDir);  //may throw
			StringUtilHelper::Append(strRelative, strDir);  //may throw
			FsPathHelper::ConvertPathStringToPlatform(strDir);
			if( !FileManagementHelper::CheckDirectoryExists(StringUtilHelper::To_ConstString(strDir))
				&& !FileManagementHelper::CreateDirectory(StringUtilHelper::To_ConstString(strDir))
				)
				return false;
		}
		//source
		StringUtilHelper::MakeString(strSrcRoot, strDir);  //may throw
		FsPathHelper::AppendSeparator(strDir);  //may throw
		StringUtilHelper::Append(strRelative, strDir);  //may throw
		FsPathHelper::AppendSeparator(strDir);  //may throw
		StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("*")), strDir);  //may throw
		FsPathHelper::ConvertPathStringToPlatform(strDir);
		//find
		bool bFound = fs.Find(StringUtilHelper::To_ConstString(strDir));
		while( bFound ) {
			if( !fs.IsDots() ) {
				ConstStringS strFile(fs.GetFileName());
				strTemp = StringHelper::Clone(strRelative);  //may throw
				if( !strTemp.IsEmpty() )
					StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("/")), strTemp);  //may throw
				StringUtilHelper::Append(strFile, strTemp);  //may throw
				if( fs.IsDirectory() ) {
					//add to list
					listTask.AddTail(strTemp);  //may throw
				}
				else {
					//ext
					uintptr uPos;
					if( _CheckFileExtension(strFile, DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".md")), uPos) ) {
						flMd.AddFile(strTemp);  //may throw
					}
					else {
						flAux.AddFile(strTemp);  //may throw
					}
				}
			}
			bFound = fs.FindNext();
		} //end while
		fs.Close();
	} //end while
	return true;
}

//copy aux files
inline bool _Copy_Aux_Files(const ConstStringS& strSrcRoot, const ConstStringS& strDestRoot,
							const DirFileList& flAux)
{
	StringS strDir(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringS strOutput(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//loop
	uintptr uCount = flAux.GetCount();
	for( uintptr i = 0; i < uCount; i ++ ) {
		ConstStringS strFile(StringUtilHelper::To_ConstString(flAux.GetAt(i)));
		//src
		StringUtilHelper::MakeString(strSrcRoot, strDir);  //may throw
		FsPathHelper::AppendSeparator(strDir);  //may throw
		StringUtilHelper::Append(strFile, strDir);  //may throw
		FsPathHelper::ConvertPathStringToPlatform(strDir);
		//dest
		StringUtilHelper::MakeString(strDestRoot, strOutput);  //may throw
		FsPathHelper::AppendSeparator(strOutput);  //may throw
		StringUtilHelper::Append(strFile, strOutput);  //may throw
		FsPathHelper::ConvertPathStringToPlatform(strOutput);
		//copy
		if( !_Copy_File(StringUtilHelper::To_ConstString(strDir), StringUtilHelper::To_ConstString(strOutput)) )
			return false;
	} //end for
	return true;
}

//copy md files
inline bool _Copy_MD_Files(const ConstStringA& strExt, const ProjectInfo& info,
						const ConstStringS& strSrcRoot, const ConstStringS& strDestRoot,
						const FileListInfo& flInfo, const DirFileList& flMd)
{
	StringS strDir(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringS strOutput(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw

	//loop
	uintptr uCount = flMd.GetCount();
	for( uintptr i = 0; i < uCount; i ++ ) {
		ConstStringS strFile(StringUtilHelper::To_ConstString(flMd.GetAt(i)));
		//src
		StringUtilHelper::MakeString(strSrcRoot, strDir);  //may throw
		FsPathHelper::AppendSeparator(strDir);  //may throw
		StringUtilHelper::Append(strFile, strDir);  //may throw
		FsPathHelper::ConvertPathStringToPlatform(strDir);
		//dest
		StringUtilHelper::MakeString(strDestRoot, strOutput);  //may throw
		FsPathHelper::AppendSeparator(strOutput);  //may throw
		StringUtilHelper::Append(strFile, strOutput);  //may throw
		FsPathHelper::ConvertPathStringToPlatform(strOutput);
		//copy this file
		if( !_Copy_MD(flInfo, strExt, StringUtilHelper::To_ConstString(strDir), StringUtilHelper::To_ConstString(strOutput)) )  //may throw
			return false;
	} //end for

	return true;
}

//generate fix content file
inline bool _Generate_Fix_Content_File(const ConstStringS& strFile, const ConstStringA& strContent) throw()
{
	CallResult cr;
	ShareCom<IByteStream> spDest;
	cr = StreamHelper::CreateFileStream(strFile, FileOpenTypes::Write, FileCreationTypes::Create, spDest);
	if( cr.IsFailed() )
		return false;
	ShareCom<ITextStream> spText;
	cr = StreamHelper::CreateTextStream(spText);
	if( cr.IsFailed() )
		return false;
	spText.Deref().SetStream(spDest);
	//save
	cr = spText.Deref().PutStringA(strContent);
	if( cr.IsFailed() )
		return false;
	return true;
}

//generate cover html file
inline bool _Generate_Cover_Html_File(const ConstStringS& strFile, const ConstStringA& strCoverImageFile)
{
	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_cover_htm_body::GetAddress(), g_cover_htm_body::GetCount()), strContent);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$COVER$$"), strCoverImageFile, strContent);  //may throw
	return _Generate_Fix_Content_File(strFile, StringUtilHelper::To_ConstString(strContent));
}

//generate file-url string
inline void _Generate_FileUrl_String(const ConstStringA& strFile, const ConstStringA& strExt, StringA& strUrl)
{
	uintptr uPos = strFile.GetCount() - 3;
	auto iter(ConstStringHelper::FindLast(strFile, '#', 0));
	if( !iter.IsNull() )
		uPos = iter.CalcDelta(strFile.GetBegin()) - 3;
	StringUtilHelper::MakeString(strFile, strUrl);  //may throw
	StringUtilHelper::Replace(uPos, 3, strExt, strUrl);  //may throw
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
