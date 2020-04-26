#pragma once
#include <vector>

#include "Vec2.h"
#include "IBindable.h"

class IndexBuffer : public IBindable
{
	private:
	unsigned int bufID;

	mutable std::vector<unsigned int> indecies;

	public:
	IndexBuffer();
	IndexBuffer(const std::vector<unsigned int>& ind);
	~IndexBuffer();

	inline unsigned int IndexCount() const { return indecies.size(); }

	void Bind() const override;
	void Unbind() const override;

	void GenerateFromVerticies(int numOfVertices) const;
};