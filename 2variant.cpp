#include "RealCordToImg.h"
#include "json.hpp"
#include <fstream>
using namespace cv;
int main() {
	using json = nlohmann::json;
	std::ifstream fcord("config.json");
	json jsn;
	fcord >> jsn;
	std::string fn, cn;
	cv::Point2f tl, br;
	fn = jsn.at("filename").get<std::string>();
	cn = jsn.at("cordinatelistname").get<std::string>();
	tl.x = jsn.at("topleftx").get<float>();
	tl.y = jsn.at("toplefty").get<float>();
	br.x = jsn.at("botrightx").get<float>();
	br.y = jsn.at("botrighty").get<float>();
	fcord.close();
	Mat im = imread(fn);
	if (im.empty()) return -1;
	RealCordToImg rci;
	unsigned long long t1, t2;
	fcord.open(cn);
	if (!fcord.is_open()) return -2;
	float x1, x2, y1, y2;
	fcord >> x1 >> y1 >> t1;
	rci.adddefpos(im, tl, br);
	drawMarker(im, rci.retimgpos(cv::Point2f(x1, y1)), cv::Scalar(255, 0, 255), 2, 20, 2);
	while (!fcord.eof()) {
		fcord >> x2 >> y2 >> t2;
		line(im, rci.retimgpos(cv::Point2f(x1, y1)), rci.retimgpos(cv::Point2f(x2, y2)), cv::Scalar(0, 0, 255), 2);
		drawMarker(im, rci.retimgpos(cv::Point2f(x2, y2)), cv::Scalar(255, 0, 255), 2, 20, 2);
		imshow("map", im);
		if(waitKey(t2 - t1)==27) break;
		t1 = t2;
		x1 = x2;
		y1 = y2;
	}
	waitKey(0);
	return 0;
}