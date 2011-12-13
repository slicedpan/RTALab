#include "ModelManager.h"

ModelManager* ModelManager::pInstance;

ModelManager::ModelManager(void)
{	
}

ModelManager::~ModelManager(void)
{
}

ModelManager* ModelManager::CurrentInstance()
{
	if (pInstance == 0)
		pInstance = new ModelManager();
	return pInstance;
}

void ModelManager::Load(char* filename, char* name, bool createList)
{
	ModelEntry modelEntry;
	modelEntry.name = name;
	modelEntry.model = glmReadOBJ(filename);
	glmVertexNormals(modelEntry.model, 90, false);
	modelEntry.list = 0;
	if (createList)
		modelEntry.list = glmList(modelEntry.model, GLM_SMOOTH);		
	models.push_back(modelEntry);
}

GLMmodel * ModelManager::GetModel(char* name)
{
	int index = FindModel(name);
	if (index >= 0)
		return models[index].model;
	else
		return 0;
}

GLuint ModelManager::GetList(char* name)
{
	int index = FindModel(name);
	if (index >= 0)
		return models[index].list;
	else 
		return 0;
}

int ModelManager::FindModel(char* name)
{
	for (unsigned int i = 0; i < models.size(); ++i)
	{
		if (!strcmp(name, models[i].name))
			return i;
	}
	return -1;
}