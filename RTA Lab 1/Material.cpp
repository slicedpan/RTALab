#include "Material.h"
#include "glut.h"

Material::Material(void)
{
	Ambient[0] = 0.01f;
	Ambient[1] = 0.01f;
	Ambient[2] = 0.01f;

	Diffuse[0] = 0.5f;
	Diffuse[1] = 0.5f;
	Diffuse[2] = 0.5f;

	Specular[0] = 1.0f;
	Specular[1] = 1.0f;
	Specular[2] = 1.0f;

	Shininess = 100.0f;
}


Material::~Material(void)
{
}

void Material::SetMaterial()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &Shininess);
}

Material* Material::DebugMaterial()
{
	Material* debugMaterial;
	debugMaterial = new Material();
	debugMaterial->Diffuse[0] = 1.0f;
	debugMaterial->Diffuse[1] = 0.0f;
	debugMaterial->Diffuse[2] = 0.0f;
	return debugMaterial;
}
