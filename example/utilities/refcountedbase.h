
#ifndef REF_COUNTED_BASE_H_
#define REF_COUNTED_BASE_H_

#include <stddef.h>
#include <assert.h>

class ReferenceCountedBase
{
protected:
	unsigned int	m_nReferenceCount;
protected:
	virtual 		~ReferenceCountedBase(void){};
public:
	unsigned int	AddRef(){return(++m_nReferenceCount);}
	unsigned int	Release(){
								unsigned int nRetVal(0x00);
								//assert(m_nReferenceCount>=1);

								nRetVal=--m_nReferenceCount;
								if(m_nReferenceCount==0x00)
								{
									delete this;
								}

								return(nRetVal);
								}
					ReferenceCountedBase(void):m_nReferenceCount(1){};
};



template <class T> class SmartPointer
{

protected:
	T* wrapped_pointer;
public:
	SmartPointer(): wrapped_pointer(NULL)
	{

	}

	SmartPointer(T* const ptr_to_wrap): wrapped_pointer( ptr_to_wrap )
	{
		if( wrapped_pointer != NULL )
		{
			wrapped_pointer->AddRef();
		}
	}

	SmartPointer(const SmartPointer<T>& ptr_to_wrap): wrapped_pointer( ptr_to_wrap.wrapped_pointer )
	{
		if( wrapped_pointer != NULL )
		{
			wrapped_pointer->AddRef();
		}
	}

	~SmartPointer()
	{
		if( wrapped_pointer != NULL )
		{
			wrapped_pointer->Release();
		}
	}

	T* operator() () const
	{
		return wrapped_pointer;
	}


	bool operator == (const T* lhs) const
	{

			return wrapped_pointer==lhs;
	}

	SmartPointer<T>& operator = (const SmartPointer<T>& ptr)
	{

		if(this!=&ptr)
		{
			if( wrapped_pointer != ptr.wrapped_pointer )
			{
				reset();

				wrapped_pointer = ptr.wrapped_pointer;

				if(wrapped_pointer)
					wrapped_pointer->AddRef();
			}
		}
		else
		{
			assert(0);
		}
		return *this;
	}

	T* operator-> () const
	{
		return wrapped_pointer;
	}

	void reset ()
	{

		if(wrapped_pointer)
		{
			if(wrapped_pointer)
				wrapped_pointer->Release();
			wrapped_pointer=NULL;
		}
	}
	bool isSet()
	{
		return ((wrapped_pointer!=NULL)?true:false);
	}

};

#endif
