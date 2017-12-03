#include "stdafx.h"
#include "Evaluator.h"

Evaluator::Evaluator(string detectorFilePath, string groundTruthFilePath)
{
	detectorOutputFile.open(detectorFilePath);
	groundTruthFile.open(groundTruthFilePath);
}

Evaluator::~Evaluator()
{
	detectorOutputFile.close();
	groundTruthFile.close();
}

void Evaluator::evaluate()
{
	int detectorLine, groundTruthLine;
	int falsePositives = 0;
	int falseNegatives = 0;
	int truePositives = 0;
	int trueNegatives = 0;
	while (true)
	{
		if (!(detectorOutputFile >> detectorLine)) break;
		groundTruthFile >> groundTruthLine;

		int detect = detectorLine;
		int ground = groundTruthLine;

		//false positives
		if ((detect == 1) && (ground == 0)) {
			falsePositives++;
		}

		//false negatives
		if ((detect == 0) && (ground == 1)) {
			falseNegatives++;
		}

		//true positives
		if ((detect == 1) && (ground == 1)) {
			truePositives++;
		}

		//true negatives
		if ((detect == 0) && (ground == 0)) {
			trueNegatives++;
		}
	}

	cout << "falsePositives " << falsePositives << endl;
	cout << "falseNegatives " << falseNegatives << endl;
	cout << "truePositives " << truePositives << endl;
	cout << "trueNegatives " << trueNegatives << endl;
	float acc = (float)(truePositives + trueNegatives) / (float)(truePositives + trueNegatives + falsePositives + falseNegatives);
	cout << "Accuracy " << acc << endl;

	waitKey(0);
}