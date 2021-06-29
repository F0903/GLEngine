export module RenderSize;
import Viewport;

export enum class NormalizationContext
{
	Width,
	Height
};

enum class ValueType
{
	Pixels,
	WidthPercentage,
	HeightPercentage,
	GenericPercentage
};

export class RenderSize
{
	private:
	RenderSize(float value, ValueType valueType) : value(value), valueType(valueType)
	{}

	private:
	float value;
	ValueType valueType;

	float Normalize(Viewport view, NormalizationContext context)
	{
		switch (valueType)
		{
			case ValueType::Pixels:
				switch (context)
				{
					case NormalizationContext::Width:
						return value / view.width;
					case NormalizationContext::Height:
						return value / view.height;
				}
				throw "Invalid context in RenderSize.";
			case ValueType::GenericPercentage:
				switch (context)
				{
					case NormalizationContext::Width:
						return (value / 100 * view.width) / view.width;
					case NormalizationContext::Height:
						return (value / 100 * view.height) / view.height;
				}
				throw "Invalid context in RenderSize.";
			case ValueType::WidthPercentage:
				return (value / 100 * view.width) / view.width;
			case ValueType::HeightPercentage:
				return (value / 100 * view.height) / view.height;
		}
		throw "Error when normalizing RenderSize.";
	}

	public:
	float Get(Viewport view, NormalizationContext context)
	{
		return Normalize(view, context);
	}

	friend RenderSize operator ""_vw(long double num);
	friend RenderSize operator ""_vh(long double num);
	friend RenderSize operator ""_px(long double num);
	friend RenderSize operator ""_pct(long double num);
};

export RenderSize operator ""_vw(long double num)
{
	return RenderSize(num, ValueType::WidthPercentage);
}

export RenderSize operator ""_vh(long double num)
{
	return RenderSize(num, ValueType::HeightPercentage);
}

export RenderSize operator ""_px(long double num)
{
	return RenderSize(num, ValueType::Pixels);
}

export RenderSize operator ""_pct(long double num)
{
	return RenderSize(num, ValueType::GenericPercentage);
}