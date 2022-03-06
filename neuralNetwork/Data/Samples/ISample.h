#pragma once

class ISample
{
public:
	
	virtual void realese() = 0;
	virtual double* getInputData(int id) = 0;
	virtual double* getOutputData(int id) = 0;
};