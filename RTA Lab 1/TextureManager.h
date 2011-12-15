#pragma once

#include <vector>

using namespace std;

struct TextureEntry
{
	unsigned int id;
	char* name;
};

class TextureManager
{
public:
	TextureManager(void);
	~TextureManager(void);
	void Load(char * filename, char * name, unsigned long width, unsigned long height);
	unsigned int GetTexture(char * name);
	static TextureManager * GetCurrentInstance();
private:
	static TextureManager * pInstance;
	int GetIndexOf(char* name);
	vector<TextureEntry> textures;
};

