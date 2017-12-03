#include "stdafx.h"
#include "Detector.h"


Detector::Detector(string geometryFilePath)
{
	geometry.loadGeometry(geometryFilePath);
}

Detector::~Detector()
{
}

void Detector::extractFeatures(Place place, vector<float>& features)
{
	Feature::Canny(place.img, features, 80, 60);
	Feature::HOG(place.img, features);
	//Feature::HistogramHSV(place.img, features);
}

void Detector::detect(string testFilePath, string trainedDetectorFilePath, string resultFilePath, bool draw) 
{
	ann = ml::ANN_MLP::load(trainedDetectorFilePath);

	ifstream is;
	is.open(testFilePath);

	std::ofstream os;
	os.open(resultFilePath);

	vector<float> place_features;
	vector<float> place_result;

	while (!is.eof()) {
		string line;
		is >> line;
		line = DATA(line);

		cout << "Loading: " << line << endl;
		Mat scene = imread(line, CV_LOAD_IMAGE_COLOR);

		// Load places geometry
		vector<Place> places = geometry.getPlaces(scene);
		for (auto &place : places) {
			extractFeatures(place, place_features);

			ann->predict(place_features, place_result);
			place.occupied = place_result[1] > place_result[0]; // if confidence for occupied is greater
			place_features.clear();

			os << place.occupied << std::endl;
		}

		if (draw) {
			drawDetection(scene, places);
		}
	}

	is.close();

}

void Detector::prepareTrainingData(string filePath, float result, vector<vector<float>>& features, vector<float>& results) 
{
	ifstream is;
	is.open(filePath);

	vector<float> place_features;

	while (!is.eof()) {
		string line;
		is >> line;
		line = DATA(line);

		cout << "Loading: " << line << endl;
		Mat scene = imread(line, CV_LOAD_IMAGE_COLOR);

		// Load places geometry
		vector<Place> places = geometry.getPlaces(scene);
		for (auto &place : places) {
			extractFeatures(place, place_features);
			features.push_back(place_features);
			results.push_back(result);
			place_features.clear();
		}
	}

	is.close();
}

void Detector::trainingDataToMatrices(vector<vector<float>>& features, vector<float>& results, Mat& samples, Mat& responses)
{
	int samplesCount = features.size();
	int numFeatures = features[0].size();

	samples = Mat(samplesCount, numFeatures, CV_32F);
	responses = Mat(samplesCount, 2, CV_32F);

	for (int i = 0; i < samplesCount; i++) {
		for (int j = 0; j < numFeatures; j++) {
			samples.at<float>(i, j) = features[i][j];
		}
		// sets 1 to index 0 (if result[i] == 0) or 1 (if result[i] == 1)
		responses.at<float>(i, 0) = 0;
		responses.at<float>(i, 1) = 0;
		responses.at<float>(i, (int)results[i]) = 1;
	}
}

void Detector::train(string positive, string negative, string saveFilePath)
{
	vector<vector<float>> features;
	vector<float> results;

	prepareTrainingData(positive, 1.0f, features, results);
	prepareTrainingData(negative, 0.0f, features, results);

	Mat samples, responses;
	trainingDataToMatrices(features, results, samples, responses);

	train(samples, responses, saveFilePath);
}

void Detector::train(Mat& samples, Mat& responses, string saveFilePath) {
	Mat layers = Mat(3, 1, CV_32S);
	layers.at<int>(0, 0) = samples.cols; // Input (feature count)
	layers.at<int>(1, 0) = samples.cols; // Middle layer
	layers.at<int>(2, 0) = responses.cols; // Output (0, 1)

	ann = ml::ANN_MLP::create();
	ann->setLayerSizes(layers);
	ann->setActivationFunction(ml::ANN_MLP::SIGMOID_SYM);

	cout << "Training ... ";
	if (ann->train(samples, ml::ROW_SAMPLE, responses)) {
		ann->save(saveFilePath);
	} else {
		cout << "Training Failed" << endl;
	}
}

void Detector::drawDetection(Mat& mat, vector<Place>& places) 
{
	int occupied = 0;
	for (auto &place : places) 
	{
		Scalar color = (place.occupied) ? Scalar(0, 0, 255) : Scalar(0, 255, 0);
		if (place.occupied) {
			occupied++;
		}

		line(mat, place.p1(), place.p2(), color);
		line(mat, place.p2(), place.p3(), color);
		line(mat, place.p3(), place.p4(), color);
		line(mat, place.p4(), place.p1(), color);

		Point2i s = (place.p1() + place.p3()) / 2;
		circle(mat, s, 12, color, -1);
	}

	stringstream ss;
	ss << "Occupied: " << occupied << " / " << places.size();
	putText(mat, ss.str(), Point(10, 20), CV_FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 0), 1, CV_AA);

	imshow("detection", mat);
	waitKey(0);
}