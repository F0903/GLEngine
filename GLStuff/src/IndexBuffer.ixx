module;
#include <Gl/glew.h>
export module IndexBuffer;
import SizedPtr;

export class IndexBuffer
{
	public:
	IndexBuffer()
	{
		glGenBuffers(1, &id);
	}

	private:
	unsigned int id;
	SizedPtr<unsigned int> indices{};

	public:
	void Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void SetData(SizedPtr<unsigned int> indices)
	{
		this->indices = indices;
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.ptrSize, indices.ptr, GL_STATIC_DRAW);
	}

	constexpr size_t GetDataSize() const
	{
		return indices.ptrSize;
	}
};