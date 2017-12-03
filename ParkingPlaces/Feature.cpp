#include "stdafx.h"
#include "Feature.h"

void Feature::HOG(const Mat &place, vector<float> &features) {
	vector<float> descriptors;

	// placesSize(160, 176)
	HOGDescriptor hog;
	hog.winSize = place.size();
	hog.blockSize = Size(16, 16);
	hog.cellSize = Size(16, 16);
	hog.compute(place, descriptors);

	for (auto &d : descriptors) {
		features.emplace_back(d);
	}
}

void Feature::Canny(const Mat& place, vector<float>& features, uchar t1, uchar t2)
{
	Mat srcGray, edges;

	// Convert and blur
	cvtColor(place, srcGray, CV_BGR2GRAY);
	medianBlur(srcGray, srcGray, 3);

	// Canny
	cv::Canny(srcGray, edges, t1, t2);

	// Count edges
	float n = 0;
	for (int y = 0; y < edges.rows; y++) {
		for (int x = 0; x < edges.cols; x++) {
			if (edges.at<uchar>(y, x) > 0) {
				n++;
			}
			features.push_back((float)edges.at<uchar>(y, x));
		}
	}

	features.push_back(n);
}

void Feature::AvgColorHSV(const Mat& place, vector<float>& features) 
{
	Vec3f hsv;

	Mat HSV;
	cvtColor(place, HSV, CV_BGR2HSV);

	Scalar s = mean(HSV);

	/*for (int y = 0; y < place.rows; y++) {
		for (int x = 0; x < place.cols; x++) {

			Vec3f bgr = place.at<Vec3f>(y, x);

		}
	}*/


	features.push_back(s[0]);
	features.push_back(s[1]);
	features.push_back(s[2]);
}

void Feature::getDominantColorIndex(Mat& mat, int& i1, int& i2, int& i3)
{
	/// Establish the number of bins
	int histSize = 32;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; 
	bool accumulate = false;

	Mat hist;
	calcHist(&mat, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

	i1 = i2 = i3 = 0;

	for (int i = 0; i < hist.rows; i++) {
		float v = hist.at<float>(i, 0);
		float v1 = hist.at<float>(i1, 0);
		float v2 = hist.at<float>(i2, 0);
		float v3 = hist.at<float>(i3, 0);

		if (v > v3) {
			if (v > v2) {
				if (v > v1) {
					i3 = i2;
					i2 = i1;
					i1 = i;
				} else {
					i3 = i2;
					i2 = i;
				}
			} else {
				i3 = i;
			}
		}
	}
}

void Feature::HistogramHSV(const Mat& place, vector<float>& features)
{
	Mat HSV;
	cvtColor(place, HSV, CV_BGR2HSV);

	for (int x = 0; x < HSV.cols; x++) {
		for (int y = 0; y < HSV.rows; y++) {
			Vec3b v = HSV.at<Vec3b>(y, x);
			if (v[2] < 100) {
				v[0] = 0;
			}
			if (v[1] < 30) {
				v[0] = 0;
			}
			HSV.at<Vec3b>(y, x) = v;
		}
	}

	vector<Mat> splited;
	split(HSV, splited);

	//imshow("hsv", splited[0]);
	//waitKey(0);

	int i1 = 0, i2 = 0, i3 = 0;
	getDominantColorIndex(splited[0], i1, i2, i3); features.push_back(i1); features.push_back(i2); features.push_back(i3);
	//cout << i1 << " " << i2 << " " << i3 << endl;
	//getDominantColorIndex(splited[1], i1, i2, i3); features.push_back(i1); features.push_back(i2); features.push_back(i3);
	//getDominantColorIndex(splited[2], i1, i2, i3); features.push_back(i1); features.push_back(i2); features.push_back(i3);
}
