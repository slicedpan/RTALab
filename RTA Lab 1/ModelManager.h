#pragma once
#include <vector>
#include "glm\glm.h"

using namespace std;

//class GLMmodel

struct ModelEntry
{
	GLMmodel * model;
	GLuint list;
	char* name;
};

class ModelManager
{
public:
	ModelManager(void);
	~ModelManager(void);
	void Load(char* filename, char* name, bool createList);	
	GLuint GetList(char* name);
	GLMmodel* GetModel(char* name);
	static ModelManager* CurrentInstance();
private:
	static ModelManager* pInstance;
	vector<ModelEntry> models;
	int FindModel(char* name);
	void GenerateList(char* name);
};


