#pragma once

class ISample
{
public:
	
	virtual void realese() = 0;
	virtual float* getInputData(int id) = 0;
	virtual float* getOutputData(int id) = 0;
};