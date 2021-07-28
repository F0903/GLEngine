export module Vector;

export template<const size_t N = 3, class T = float>
struct Vector
{
	T values[N];
};