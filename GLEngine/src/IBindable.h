#pragma once
#include <unordered_map>

enum class BindableType
{
	Shader,
	Texture,
	VertexBuffer,
	IndexBuffer
};

class IBindable
{
	private:
	inline static std::unordered_map<BindableType, int*> boundItems;
	unsigned int& boundID;
	BindableType type;

	protected:
	IBindable(unsigned int& ID, BindableType type) : boundID(ID), type(type) {}

	inline bool IsBound() const
	{
		return boundItems.find(type) != boundItems.end() ? (*boundItems[type]) == boundID : false;
	}

	public:
	IBindable() = delete;
	IBindable(const IBindable& other) = delete;
	IBindable(IBindable&& other) = delete;

	virtual inline void Bind() const { boundItems[type] = (int*)&boundID; }
	virtual inline void Unbind() const { boundItems[type] = (int*)-1; }
};