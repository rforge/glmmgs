#ifndef UTILITIES_VECTORBUILDER_H
#define UTILITIES_VECTORBUILDER_H

#include "Exceptions/Assertions.h"
#include "Container.h"
#include "Vector.h"

namespace Utilities
{
	// VectorBuilder
	template <class TYPE> 
	class VectorBuilder : public Container<TYPE>
	{
	private:
		// Fields
		int size;

	public:
		// Construction
		VectorBuilder();
		explicit VectorBuilder(int size);

		// Attributes
		int Size() const;
		Vector<TYPE> ToVector() const;

		// Element access
		TYPE & operator ()(int index);
		const TYPE & operator ()(int index) const;
		
		// Assignment
		const VectorBuilder<TYPE> & operator =(const TYPE & x);

		// Memory
		void Size(int size);
		void Resize(int size);
		void Free();
		void FreeExtra();

		// Manipulation
		void Add(const TYPE & x);
		void Remove();
		void RemoveAll();
	};

	// Construction
	template <class TYPE> inline
	VectorBuilder<TYPE>::VectorBuilder()
		: size(0)
	{
	}

	template <class TYPE> inline
	VectorBuilder<TYPE>::VectorBuilder(int size)
		: Container<TYPE>(size), size(size)
	{
	}

	// Attributes
	template <class TYPE> inline
	int VectorBuilder<TYPE>::Size() const
	{
		return this->size;
	}

	template <class TYPE> inline
	Vector<TYPE> VectorBuilder<TYPE>::ToVector() const
	{
		Vector<TYPE> v(this->size);
		for (int i = 0; i < this->size; ++i)
			v(i) = (*this)(i);
		return v;
	}

	// Element access
	template <class TYPE> inline
	TYPE & VectorBuilder<TYPE>::operator ()(int index)
	{
		_ASSERT(index >= 0 && index < this->size);
		return this->ptr[index];
	}

	template <class TYPE> inline
	const TYPE & VectorBuilder<TYPE>::operator ()(int index) const
	{
		_ASSERT(index >= 0 && index < this->size);
		return this->ptr[index];
	}

	// Assignment
	template <class TYPE>
	const VectorBuilder<TYPE> & VectorBuilder<TYPE>::operator =(const TYPE & x)
	{
		for (int i = 0; i < this->size; ++i)
			this->ptr[i] = x;
		return *this;
	}

	// Memory
	template <class TYPE>
	void VectorBuilder<TYPE>::Size(int size)
	{
		Container<TYPE>::Allocate(size);
		this->size = size;
	}

	template <class TYPE>
	void VectorBuilder<TYPE>::Resize(int size)
	{
		Container<TYPE>::Reallocate(size);
		this->size = size;
	}
	
	template <class TYPE>
	void VectorBuilder<TYPE>::Free()
	{
		Container<TYPE>::Free();
		this->size = 0;
	}

	template <class TYPE>
	void VectorBuilder<TYPE>::FreeExtra()
	{
		Container<TYPE>::Reallocate(this->size);
	}

	// Manipulation
	template <class TYPE>
	void VectorBuilder<TYPE>::Add(const TYPE & x)
	{
		if (this->size == this->capacity)
		{
			const TYPE y = x; // Copy in case x points to an element of the vector
			Container<TYPE>::Grow();
			this->ptr[this->size] = y;
			++this->size;
		}
		else
		{
			this->ptr[this->size] = x;
			++this->size;
		}
	}

	template <class TYPE>
	void VectorBuilder<TYPE>::Remove()
	{
		if (this->size > 0)
			--this->size;
	}

	template <class TYPE>
	void VectorBuilder<TYPE>::RemoveAll()
	{
		this->size = 0;
	}
}

#endif
