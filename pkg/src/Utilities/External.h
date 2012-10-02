#ifndef UTILITIES_EXTERNAL_H
#define UTILITIES_EXTERNAL_H

namespace Utilities
{
	template <class TYPE>
	class External
	{
	private:
		TYPE * ptr;

	public:
		explicit External(TYPE * ptr) : ptr(ptr) {}
		operator TYPE *() {return this->ptr;}
	};
}

#endif
