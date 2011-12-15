#include "TextureManager.h"
#include "Utils.h"

TextureManager* TextureManager::pInstance;

TextureManager::TextureManager(void)
{
}


TextureManager::~TextureManager(void)
{
}

void TextureManager::Load(char* filename, char* name, unsigned long width, unsigned long height)
{
	std::vector<unsigned char> data;
	std::vector<unsigned char> texture;
	TextureEntry texEntry;
	texEntry.name = name;
	loadFile(data, std::string(filename));
	decodePNG(texture, width, height, &data[0], data.size(), true);
	glGenTextures(1, &texEntry.id);
	glBindTexture(GL_TEXTURE_2D, texEntry.id);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &texture[0]); 
	glBindTexture(GL_TEXTURE_2D, 0);
	textures.push_back(texEntry);
}

unsigned int TextureManager::GetTexture(char* name)
{
	int index = GetIndexOf(name);
	if (index >= 0)
		return textures[index].id;
}

int TextureManager::GetIndexOf(char* name)
{
	for (int i = 0; i < textures.size(); ++i)
	{
		if (!strcmp(name, textures[i].name))
			return i;
	}
	return -1;
}

TextureManager* TextureManager::GetCurrentInstance()
{
	if (pInstance == 0)
		pInstance = new TextureManager();
	return pInstance;
}

