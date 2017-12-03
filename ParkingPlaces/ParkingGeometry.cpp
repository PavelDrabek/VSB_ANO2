#include "stdafx.h"
#include "ParkingGeometry.h"


ParkingGeometry::ParkingGeometry()
{
}

ParkingGeometry::ParkingGeometry(string geometryFilePath)
{
	 loadGeometry(geometryFilePath);
}

ParkingGeometry::~ParkingGeometry()
{
}

vector<Place> ParkingGeometry::getPlaces(Mat& parking)
{
	vector<Place> places;

	for (int i = 0; i < placesGeometry.size(); i++)
	{
		Place p(placesGeometry[i]);

		Mat outMat(parkingPlaceResolution, CV_8U, 3);
		
		Mat srcMat(4, 2, CV_32F);
		srcMat.at<float>(0, 0) = p.x1;
		srcMat.at<float>(0, 1) = p.y1;
		srcMat.at<float>(1, 0) = p.x2;
		srcMat.at<float>(1, 1) = p.y2;
		srcMat.at<float>(2, 0) = p.x3;
		srcMat.at<float>(2, 1) = p.y3;
		srcMat.at<float>(3, 0) = p.x4;
		srcMat.at<float>(3, 1) = p.y4;

		Mat destMat(4, 2, CV_32F);
		destMat.at<float>(0, 0) = 0;
		destMat.at<float>(0, 1) = 0;
		destMat.at<float>(1, 0) = outMat.cols;
		destMat.at<float>(1, 1) = 0;
		destMat.at<float>(2, 0) = outMat.cols;
		destMat.at<float>(2, 1) = outMat.rows;
		destMat.at<float>(3, 0) = 0;
		destMat.at<float>(3, 1) = outMat.rows;

		Mat H = findHomography(srcMat, destMat, 0);
		//cout << H << endl;
		warpPerspective(parking, outMat, H, parkingPlaceResolution);

		p.img = outMat;
		places.push_back(p);

		//imshow("outMat", outMat);
		//waitKey(10);
	}

	return places;
}


void ParkingGeometry::loadGeometry(string geometryFilePath)
{
	FILE *file = fopen(geometryFilePath.c_str(), "r");
	if (file == NULL) return;

	int ps_count;
	fscanf(file, "%d\n", &ps_count); // read count of polygons

	placesGeometry.resize(ps_count);

	for (int i = 0; i < ps_count; i++) {
		int p = 0;
		int poly_size;
		fscanf(file, "%d->", &poly_size); // read count of polygon vertexes
		int *celyRadek = new int[poly_size * 2];
		for (int j = 0; j < poly_size; j++) {
			int x, y;
			fscanf(file, "%d,%d;", &x, &y); // read vertex coordinates
			celyRadek[p] = x;//cely radek nactu zde
			celyRadek[p + 1] = y;
			p = p + 2;
		}

		placesGeometry[i].x1 = celyRadek[0];
		placesGeometry[i].y1 = celyRadek[1];
		placesGeometry[i].x2 = celyRadek[2];
		placesGeometry[i].y2 = celyRadek[3];
		placesGeometry[i].x3 = celyRadek[4];
		placesGeometry[i].y3 = celyRadek[5];
		placesGeometry[i].x4 = celyRadek[6];
		placesGeometry[i].y4 = celyRadek[7];
		//printf("}\n");
		free(celyRadek);
		fscanf(file, "\n"); // read end of line
	}
	fclose(file);
}
