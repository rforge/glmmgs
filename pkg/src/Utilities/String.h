#ifndef UTILITIES_STRING_H
#define UTILITIES_STRING_H

#include "Exceptions/Assertions.h"
#include "External.h"
#include "Array.h"
#include "Cast.h"
#include <string.h>

namespace Utilities
{
	// ImmutableString
	class ImmutableString
	{
		template <class OTHER> friend class Cast;

	protected:
		static const char empty_pcstr[];

		// Fields
		Array<char> array;
		int length;

	public:
		// Construction
		ImmutableString();
		explicit ImmutableString(const char * ptr, int len = -1);
		explicit ImmutableString(External<char> ext);

		// Properties
		const char * PCSTR() const;
		int Length() const;
	};

	// Definition

	// Construction
	inline
	ImmutableString::ImmutableString()
		: length(0)
	{
	}

	// Properties
	inline
	int ImmutableString::Length() const
	{
		return this->length;
	}

	inline
	const char * ImmutableString::PCSTR() const
	{
		return this->array ? Cast<const char *>::Source(this->array) : empty_pcstr;
	}

	// Comparison operator
	inline
	bool operator ==(const ImmutableString & str1, const ImmutableString & str2)
	{
		return strcmp(str1.PCSTR(), str2.PCSTR()) == 0;
	}

	// Instant conversion from c-string to immutable String
	inline
	ImmutableString Str(const char * str)
	{
		return ImmutableString(External<char>(const_cast<char *>(str)));
	}
}

#endif
