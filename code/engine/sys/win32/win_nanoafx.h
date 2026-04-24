#ifndef __WIN_NANOAFX_HPP__
#define __WIN_NANOAFX_HPP__

#define _T( n ) L##n

/*!
	\class CComBSTR
	\brief A class for handling wide character strings with automatic memory management.
*/
class CComBSTR : public _bstr_t
{
public:
	//! Constructs a CComBSTR object from a wide character string.
	inline CComBSTR( const wchar_t* str ) :
		_bstr_t( str )
	{
	}

	//! Constructs a CComBSTR object from a null-terminated UTF-8 string.
	inline CComBSTR( const char* str ) :
		_bstr_t( str )
	{
	}
};

/*!
	\class CComVariant
	\brief CComVariant provides a COM-compatible variant type for handling mixed data types.
*/
class CComVariant : public tagVARIANT
{
};

/*!
	\class CComPtr
	\brief A smart pointer implementation for COM-style reference counted objects.

	This class provides a COM-style smart pointer that manages the lifetime of reference-counted objects. It automatically handles reference counting by incrementing the count when a new pointer is
   assigned and decrementing it when the pointer is released. The class supports conversion to and from raw pointers, pointer dereferencing, and equality comparison operations. It is designed to
   simplify memory management for objects that follow COM reference counting semantics, ensuring proper cleanup when the smart pointer goes out of scope or is assigned a new value.

*/
template<class T>
class CComPtr
{
private:
	T* _ptr;

public:
	//! Initializes a CComPtr object with a null pointer.
	inline CComPtr() { _ptr = NULL; }

	//! Initializes the smart pointer with a raw pointer, incrementing the reference count of the referenced object.
	inline CComPtr( T* ptr )
	{
		if( ptr ) {
			ptr->AddRef();
			_ptr = ptr;
		}
	}

	//! Releases the COM reference and resets the internal pointer.
	inline ~CComPtr()
	{
		if( _ptr ) { _ptr->Release(); }
		_ptr = NULL;
	}

	//! Assigns a pointer to the COM smart pointer object
	inline CComPtr& operator=( T* ptr )
	{
		if( ptr ) {
			ptr->AddRef();
			_ptr = ptr;
		}
	}

	//! Compares the managed pointer with another pointer for equality.
	inline bool operator==( T* ptr ) { return _ptr == ptr; }

	//! Returns the managed pointer.
	inline T*	operator->() { return _ptr; }

	//! Returns a pointer to the internal pointer variable.
	inline T**	operator&() { return &_ptr; }

	//! Converts the smart pointer to a raw pointer.
	inline		operator T*() { return _ptr; }
};

#endif
