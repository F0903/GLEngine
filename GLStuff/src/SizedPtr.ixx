module;
#include <initializer_list>
export module SizedPtr;

export template<class T>
struct SizedPtr
{
	SizedPtr(std::initializer_list<T> tList) : ptr(tList.begin()), ptrSize(tList.size())
	{}

	const T* ptr;
	size_t ptrSize;
};