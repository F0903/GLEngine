module;
#include <initializer_list>
export module SizedPtr;

export template<class T>
struct SizedPtr
{
	SizedPtr(std::initializer_list<T> tList) : ptr(tList.begin()), length(tList.size())
	{}

	const T* ptr;
	size_t length;

	T& operator[](size_t i)
	{
		return ptr + i;
	}
};