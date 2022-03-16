#pragma once
#include "../../Decl.h"
#include "ISample.h"
class Sample : public ISample
{
public:
	// Унаследовано через ISample
	virtual void realese() override;

	virtual double* getInputData(int id) override;

	virtual double* getOutputData(int id) override;
};

