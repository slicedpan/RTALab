#pragma once
class Material
{
public:
	Material(void);
	~Material(void);
	float Ambient[3];
	float Diffuse[3];
	float Specular[3];
	float Shininess;
	void SetMaterial();
	static Material* DebugMaterial();
};

