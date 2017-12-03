// ParkingPlaces.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	Detector detector(DATA("strecha1_map.txt"));

	detector.train(DATA("full.txt"), DATA("free.txt"), DATA("trained.txt"));
	detector.detect(DATA("test.txt"), DATA("trained.txt"), DATA("detectorOutput.txt"));

	Evaluator evaluator(DATA("detectorOutput.txt"), DATA("groundtruth.txt"));
	evaluator.evaluate();

    return 0;
}

