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
This file contains SA cache for components.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __COM_SA_CACHE_H__
#define __COM_SA_CACHE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//functions in SA
typedef void (* _SA_Com_GetClassObject_Func)(const guid& cid, _ShareCom<_IComFactory>& sp, GKC::CallResult& cr);
typedef bool (* _SA_Com_CanUnloadNow_Func)();

// Com_SA_Item

class Com_SA_Item
{
public:
	Com_SA_Item() throw() : m_pGetClassObject(NULL), m_pCanUnloadNow(NULL)
	{
	}
	Com_SA_Item(Com_SA_Item&& src) throw()
	{
		m_hd.Attach(src.m_hd.Detach());
		m_pGetClassObject = src.m_pGetClassObject;
		m_pCanUnloadNow   = src.m_pCanUnloadNow;
	}
	~Com_SA_Item() throw()
	{
	}

	Com_SA_Item& operator=(Com_SA_Item&& src) throw()
	{
		if( this != &src ) {
			m_hd.Attach(src.m_hd.Detach());
			m_pGetClassObject = src.m_pGetClassObject;
			m_pCanUnloadNow   = src.m_pCanUnloadNow;
		}
		return *this;
	}

	bool Load(const CharS* szFile) throw()
	{
		bool bRet = m_hd.Load(szFile);
		if( bRet ) {
			m_pGetClassObject = (_SA_Com_GetClassObject_Func)m_hd.GetFunctionAddress("_SA_Com_GetClassObject");
			m_pCanUnloadNow   = (_SA_Com_CanUnloadNow_Func)m_hd.GetFunctionAddress("_SA_Com_CanUnloadNow");
			if( m_pGetClassObject == NULL || m_pCanUnloadNow == NULL ) {
				m_hd.Free();
				bRet = false;
			}
		}
		return bRet;
	}

	CallResult GetClassObject(const guid& cid, _ShareCom<_IComFactory>& sp) throw()
	{
		CallResult cr;
		if( m_hd.IsNull() ) {
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}
		m_pGetClassObject(cid, sp, cr);
		return cr;
	}
	bool CanUnloadNow() throw()
	{
		if( m_hd.IsNull() )
			return true;
		return m_pCanUnloadNow();
	}

private:
	sa_handle  m_hd;  //handle for SA
	_SA_Com_GetClassObject_Func  m_pGetClassObject;
	_SA_Com_CanUnloadNow_Func    m_pCanUnloadNow;

private:
	//noncopyable
	Com_SA_Item(const Com_SA_Item&) throw();
	Com_SA_Item& operator=(const Com_SA_Item&) throw();
};

// Com_SA_Cache

class Com_SA_Cache
{
private:
	typedef HashMap<_StringS, Com_SA_Item, _StringHashTrait<_StringS>, _StringCompareTrait<_StringS>>  mapClass;

public:
	Com_SA_Cache() throw()
	{
	}
	~Com_SA_Cache() throw()
	{
	}

	void Clear() throw()
	{
		m_map.RemoveAll();
	}

	void SetMemoryManager(const RefPtr<IMemoryManager>& mgr) throw()
	{
		m_map.SetMemoryManager(mgr);
	}
	void SetMutex(const RefPtr<Mutex>& mtx) throw()
	{
		m_mtx = mtx;
	}

	CallResult GetClassObject(const _StringS& strAssembly, const guid& cid, _ShareCom<_IComFactory>& sp) throw()
	{
		CallResult cr;
		//lock
		MutexLock lock(m_mtx.Deref());
		//find
		auto iter(m_map.Find(strAssembly));
		if( iter == m_map.GetEnd() ) {
			//load
			Com_SA_Item item;
			if( !item.Load(_ShareArrayHelper::GetInternalPointer(strAssembly)) ) {
				cr.SetResult(SystemCallResults::Fail);
				return cr;
			}
			try {
				iter = m_map.InsertWithoutFind(strAssembly);
			}
			catch(Exception& e) {
				cr = e.GetResult();
			}
			catch(...) {
				cr.SetResult(SystemCallResults::Fail);
			}
			if( cr.IsFailed() )
				return cr;
			iter.get_Value().get_Second() = rv_forward(item);
		}
		//call function
		Com_SA_Item& item = iter.get_Value().get_Second();
		_ShareCom<_IComFactory> spCF;
		cr = item.GetClassObject(cid, spCF);
		if( cr.IsFailed() )
			return cr;
		_ShareCom<_IComSA> spSA;
		_COMPONENT_INSTANCE_INTERFACE(_IComFactory, _IComSA, spCF, spSA, cr);
		if( cr.IsFailed() )
			return cr;
		//lock server
		spSA.Deref().LockServer(true);
		sp = spCF;
		return cr;
	}

	void FreeUnusedLibraries() throw()
	{
		//lock
		MutexLock lock(m_mtx.Deref());
		auto iter(m_map.GetBegin());
		while( iter != m_map.GetEnd() ) {
			auto iter1(iter);
			iter.MoveNext();
			Com_SA_Item& item = iter1.get_Value().get_Second();
			//call function
			if( item.CanUnloadNow() ) {
				//give up the current thread for completing the unlock server function in this SA by other threads.
				thread_sleep(10);
				//unload SA
				m_map.RemoveAt(iter1);
			}
		} //end while
	}

private:
	mapClass  m_map;
	RefPtr<Mutex>  m_mtx;

private:
	//noncopyable
	Com_SA_Cache(const Com_SA_Cache&) throw();
	Com_SA_Cache& operator=(const Com_SA_Cache&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
