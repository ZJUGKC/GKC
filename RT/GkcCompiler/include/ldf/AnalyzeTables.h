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
This file contains classes for Tables of Language Definition File.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __LDF_TABLE_H__
#define __LDF_TABLE_H__
////////////////////////////////////////////////////////////////////////////////

#include "parser.h"

#include "_ldf_base.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// LdfTableAnalyzer

class LdfTableAnalyzer
{
public:
	LdfTableAnalyzer() throw()
	{
	}
	~LdfTableAnalyzer() throw()
	{
	}

	//called only once
	void Init()
	{
		m_lexer.Init();

	}

	CallResult Process(IN const ConstStringS& strLexFile, IN const ConstStringS& strGraFile,
					OUT TokenTable& tokenTable, OUT FsaTableInPool& fsa,
					OUT TokenTable& actionTable, OUT PdaTableInPool& pda
					)
	{
		CallResult cr;

		cr = process_lex_file(strLexFile, tokenTable, fsa);
		if( cr.IsFailed() )
			return cr;

		return cr;
	}

private:
	CallResult process_lex_file(IN const ConstStringS& strLexFile, OUT TokenTable& tokenTable, OUT FsaTableInPool& fsa)
	{
		CallResult cr;

		//stream
		FileCharStream stream;
		cr = stream.Initialize(StringUtilHelper::To_C_Style(strLexFile));
		if( cr.IsFailed() )
			return cr;

		//lexer parser
		m_lexer.SetStream(RefPtrHelper::TypeCast<FileCharStream, ICharStream>(RefPtr<FileCharStream>(stream)));

		//grammar parser
		_ldf_helper::_LDF_lex_GrammarParser parser;
		parser.SetLexerParser(m_lexer.GetLexerParser());
		// grammar table
		_ldf_helper::_LDF_lex_GrammarTable graTable;
		graTable.Init();
		parser.SetGrammarTable(graTable.GetGrammarTable());
		// data
		_ldf_helper::_LDF_lex_GrammarData data(tokenTable);
		// action
		_ldf_helper::_LDF_lex_GrammarAction action(data);
		action.Apply(parser.GetGrammarParser());

		//execute
		cr = parser.Execute();
		if( cr.IsFailed() )
			return cr;

		//expand macros
		data.ExpandTokenMacros();

		//analyze regular expressions
		SharedArray<StringA>& regex(data.GetTokenRegex());
		_ldf_helper::_LDF_regex_AST ast;
		cr = ast.Generate(regex);
		if( cr.IsFailed() )
			return cr;

		cr.SetResult(SystemCallResults::OK);
		return cr;
	}

private:
	//lexer parser
	_ldf_helper::_LDF_LexerParser m_lexer;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__LDF_TABLE_H__
////////////////////////////////////////////////////////////////////////////////
