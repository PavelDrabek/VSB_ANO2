// GaussianMixture.cpp : Defines the entry point for the console application.
//

#include <ctime>

int main()
{
	const std::string filename = "../images/dt_passat.mpg";
	cv::VideoCapture video;
	video.open(filename);
	if (!video.isOpened()) {
		return -1;
	}

	cv::Mat frame, detection, graph;

	video >> frame;
	detection = cv::Mat(frame.rows, frame.cols, CV_8UC1);
	MixtureOfGaussian mog(frame, 3);

	while (true) {
		video >> frame;
		mog.nextFrame(frame, detection);
		drawFPS(frame);

		mog.visualize(graph, point, frame.at<cv::Vec3b>(point)[0]);
		cv::imshow("graph", graph);
		cv::setMouseCallback("frame", CallBackFunc, NULL);

		cv::imshow("frame", frame);
		cv::imshow("detection", detection);
		if (cv::waitKey(1) == ' ') {
			break;
		}
	}

	video.release();

	return 0;
}

