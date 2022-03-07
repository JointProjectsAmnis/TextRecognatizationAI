#pragma once

class IImageControl
{
public:
	virtual void SetImage(unsigned char* image) = 0;
};