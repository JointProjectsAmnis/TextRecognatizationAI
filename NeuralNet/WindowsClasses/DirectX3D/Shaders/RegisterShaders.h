#pragma once
#include <iostream>
#include <vector>

#include "Shader.h"

class RegisterShaders
{
public:

	Shader* shaderDefault;
	Shader* shaderTriangle;
	Shader* shaderImage;

	std::vector<Shader*> shader = std::vector<Shader*>();

	Graphics* graphics;

	RegisterShaders(Graphics* graphics);
	~RegisterShaders();

	void AddShader(Shader* shader);
	void InitShaders();
};

