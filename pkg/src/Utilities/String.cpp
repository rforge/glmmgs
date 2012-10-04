#include <string.h>
#include "External.h"
#include "Array.h"
#include "String.h"

namespace Utilities
{
	const char ImmutableString::empty_pcstr[] = "";

	// Construction
	ImmutableString::ImmutableString(const char * str, int len)
		: length(0)
	{
		if (str != NULL)
		{
			len = (len >= 0) ? len : static_cast<int>(strlen(str));
			if (len >= 0)
			{
				this->array = Array<char>(len + 1);
				for (int i = 0; i < len; ++i)
					this->array[i] = str[i];
				this->array[len] = '\0';
				this->length = len;
			}
		}
	}

	ImmutableString::ImmutableString(External<char> ext)
		: length(0)
	{
		const char * str = ext;
		if (str != NULL)
		{
			const int len = static_cast<int>(strlen(str));
			if (len >= 0)
				this->array = Array<char>(ext, len + 1);
		}
	}
}
