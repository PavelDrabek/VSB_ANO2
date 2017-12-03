#pragma once
class Place
{
public:
	Place() : occupied(false) {};
	~Place();

	int x1, y1, x2, y2, x3, y3, x4, y4;
	bool occupied;
	Mat img;

	Place(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, bool occupied = false) :
		x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), x4(x4), y4(y4), occupied(occupied) {}

	Point2i p1() { return Point2i(x1, y1); }
	Point2i p2() { return Point2i(x2, y2); }
	Point2i p3() { return Point2i(x3, y3); }
	Point2i p4() { return Point2i(x4, y4); }
};

