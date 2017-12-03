#pragma once
class Detector
{
public:
	Detector(string geometryFilePath);
	~Detector();

	void extractFeatures(Place place, vector<float>& features);

	void detect(string testFilePath, string trainedDetectorFilePath, string resultFilePath, bool draw = false);

	void prepareTrainingData(string filePath, float result, vector<vector<float>>& features, vector<float>& results);

	void trainingDataToMatrices(vector<vector<float>>& features, vector<float>& results, Mat & samples, Mat & responses);

	void xxx(string filePath, float result);

	void train(string positive, string negative, string trainedData);

	void train(Mat & samples, Mat & responses, string saveFilePath);

	void drawDetection(Mat & mat, vector<Place>& places);

private:
	ParkingGeometry geometry;
	cv::Ptr<cv::ml::ANN_MLP> ann;

};

