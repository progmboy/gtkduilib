#ifndef __DUI_REF_H__
#define __DUI_REF_H__

//#include "DUIDef.h"

//T's base class for CRefPtr<T>
class CRefBase
{
public:
	CRefBase()
	{
		m_nRefCount = 0;
	}

	int GetRef() const
	{
		return m_nRefCount;
	}

	int AddRef()
	{
		return ++m_nRefCount;
	}

	int Release()
	{
        if(!--m_nRefCount){
            delete this;
        }
        return m_nRefCount;
	}

	VOID Reset()
	{
		m_nRefCount = 0;
	}


private:
	int	m_nRefCount;
};

template<typename T>
class CRefPtr
{
public:
	T* operator->() const
	{
		return m_pRawObj;
	}

	T& operator()() const
	{
		return *m_pRawObj;
	}

	T& operator*() const
	{
		return *m_pRawObj;
	}

	T* GetPtr() const
	{
		return m_pRawObj;
	}

	bool IsNull() const
	{
		return m_pRawObj == NULL;
	}

	CRefPtr()
	{
		m_pRawObj = NULL;
	}

	CRefPtr(T* p)
	{
		m_pRawObj = p;
		if(p != NULL)
		{
			p->AddRef();
		}
	}

	CRefPtr(const CRefPtr& ref)
	{
		m_pRawObj = ref.m_pRawObj;
		if(m_pRawObj != NULL)
		{
			m_pRawObj->AddRef();
		}
	}

	~CRefPtr()
	{
		if(m_pRawObj != NULL){
			m_pRawObj->Release();
		}
	}

	CRefPtr& operator = (const CRefPtr& ref)
	{
		if(this != &ref)
		{
			if(m_pRawObj != NULL){
				m_pRawObj->Release();
			}

			m_pRawObj = ref.m_pRawObj;

			if(m_pRawObj != NULL){
				m_pRawObj->AddRef();
			}
		}

		return *this;
	}

	bool operator == (const CRefPtr& ref) const
	{
		return m_pRawObj == ref.m_pRawObj;
	}

	bool operator != (const CRefPtr& ref) const
	{
		return m_pRawObj != ref.m_pRawObj;
	}

private:
	T* m_pRawObj;
};

#endif //__DUI_REF_H__
