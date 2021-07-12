module;
#include <map>
export module TextureManager;
import Texture;

export class TextureManager
{
	public:
	~TextureManager()
	{
		for (const auto& x : textures)
		{
			delete x.second;
		}
	}

	private:
	std::map<unsigned int, Texture*> textures;
	unsigned int lastId;

	public:
	unsigned int Load(const char* path, void(*modifier)(Texture& tex))
	{
		const auto tex = new Texture(path);
		modifier(*tex);
		const auto id = lastId++;
		textures[id] = tex;
		return id;
	}

	Texture* const Get(unsigned int id)
	{
		return textures[id];
	}
};