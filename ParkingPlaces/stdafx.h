// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define DATA_FOLDER "../data/parking/"
#define DATA(x) (DATA_FOLDER + (string)x)

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <string>
#include <cstdlib>

#include "opencv2\opencv.hpp"
#include "opencv2\ml.hpp"

using namespace std;
using namespace cv;

#include "Place.h"
#include "Feature.h"
#include "ParkingGeometry.h"
#include "Evaluator.h"
#include "Detector.h"

