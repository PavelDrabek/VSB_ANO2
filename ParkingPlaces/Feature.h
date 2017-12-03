#pragma once
class Feature
{
public:

	static void HOG(const Mat & place, vector<float>& features);

	static void Canny(const Mat & place, vector<float>& features, uchar t1, uchar t2);
	static void AvgColorHSV(const Mat & place, vector<float>& features);
	static void getDominantColorIndex(Mat & mat, int & i1, int & i2, int & i3);
	static void HistogramHSV(const Mat & place, vector<float>& features);
};

