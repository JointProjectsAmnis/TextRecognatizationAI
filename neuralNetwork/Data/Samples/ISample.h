#pragma once

#include "../../Decl.h"
class ISample
{
public:
	
	DECL virtual void realese() = 0;
	DECL virtual double* getInputData(int id) = 0;
	DECL virtual double* getOutputData(int id) = 0;
};