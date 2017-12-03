#pragma once
class ParkingGeometry
{
public:
	ParkingGeometry();
	ParkingGeometry(string geometryFilePath);
	~ParkingGeometry();
	void loadGeometry(string geometryFilePath);

	vector<Place> getPlaces(Mat& parking);


private:
	vector<Place> placesGeometry;

	Size parkingPlaceResolution = Size(32, 32);
};

