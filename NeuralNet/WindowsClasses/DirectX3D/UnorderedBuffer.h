#pragma once

#include "Graphics.h"

class UnorderedBuffer
{
public:

	ID3D11Buffer* buffer;

	Graphics* graphics;

	UnorderedBuffer(Graphics* graphics, UINT sizeData, UINT stride, void* data);
};

