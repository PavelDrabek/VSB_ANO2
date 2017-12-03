#pragma once
class Evaluator
{
public:
	Evaluator(std::string detectorFilePath, std::string groundTruthFilePath);
	~Evaluator();

	void evaluate();

private:
	std::ifstream detectorOutputFile, groundTruthFile;
};

