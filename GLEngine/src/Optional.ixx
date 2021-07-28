export module Optional;

export template <class T>
struct Optional
{
	public:
	Optional(void* null) : value{}, null(true)
	{}

	Optional(T&& val) : value(val), null(false)
	{}

	public:
	T value;
	bool null;
};