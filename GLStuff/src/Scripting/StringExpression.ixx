export module StringExpression;
import Viewport;

int pow(int num, int pow)
{
	int total = 1;
	for (size_t i = 0; i < pow; i++)
		total *= num;
	return total;
}

export class StringExpression
{
	public:
	StringExpression(const char* src)
	{
		Parse(src);
	}

	private:
	static inline const Viewport& view = Viewport::Get();
	double total = 0;

	private:
	const int ParseNum(const char* str, const size_t maxLen = 4, const char delim = ' ') const
	{
		int sum = 0;
		for (size_t i = 0; i < maxLen; i++)
		{
			if (*str == delim || *str == 0) break;
			const char ch = *str;
			const int num = ch - 48;
			const int step = pow(10, i);
			sum += num * step;
			--str;
		}
		return sum;
	}

	void ParsePct(const char* src)
	{
		--src; // First char is %
		total += ParseNum(src) / static_cast<double>(100);
	}

	double ParseOp(const char* src) const
	{
		bool isPixels = false;
		++src; // First char is +
		if (*src == ' ') ++src;
		// Continue until a delimiter or EOS has been found.
		while (*src)
		{
			if (!isPixels && *src == 'p' && *(src + 1) == 'x')
			{
				isPixels = true;
				continue;
			}
			if (*src == ' ')
				break;
			++src;
		}
		src += isPixels ? -3 : -1; // Go back once to end of num.
		const auto num = ParseNum(src);
		return isPixels ? num / static_cast<double>(view.width) : num;
	}

	void ParseAdd(const char* src)
	{
		total += ParseOp(src);
	}

	void ParseSub(const char* src)
	{
		total -= ParseOp(src);
	}

	void ParseDiv(const char* src)
	{
		total /= ParseOp(src);
	}

	void ParseMul(const char* src)
	{
		total *= ParseOp(src);
	}

	void Parse(const char* src)
	{
		while (*src)
		{
			switch (*src)
			{
				case '%':
					ParsePct(src);
					break;
				case '+':
					ParseAdd(src);
					break;
				case '-':
					ParseSub(src);
					break;
				case '/':
					ParseDiv(src);
					break;
				case '*':
					ParseMul(src);
					break;
				default: [[likely]]
					break;
			}
			++src;
		}
	}

	public:
	double GetNumericValue() const
	{
		return total;
	}
};