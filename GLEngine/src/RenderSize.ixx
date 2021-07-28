export module RenderSize;
import Viewport;
import ScriptEngine;

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
	static inline const Viewport& view = Viewport::Get();
	float value;
	ValueType valueType;

	float Normalize(NormalizationContext context)
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
					default: throw "Invalid context in RenderSize.";
				}
			case ValueType::GenericPercentage:
				switch (context)
				{
					case NormalizationContext::Width:
						return (value * view.width) / view.width;
					case NormalizationContext::Height:
						return (value * view.height) / view.height;
					default: throw "Invalid context in RenderSize.";
				}
			case ValueType::WidthPercentage:
				return (value * view.width) / view.width;
			case ValueType::HeightPercentage:
				return (value * view.height) / view.height;
			default: throw "Invalid value type in RenderSize.";
		}
		throw "Error when normalizing RenderSize.";
	}

	public:
	static RenderSize Pixels(float pix)
	{
		return RenderSize(pix, ValueType::Pixels);
	}

	static RenderSize FromPctOrNum(PctOrNum value)
	{
		return RenderSize(value.value, value.isPct ? ValueType::GenericPercentage : ValueType::Pixels);
	}

	float Get(NormalizationContext context)
	{
		return Normalize(context);
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