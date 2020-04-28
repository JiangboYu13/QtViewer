#include "ORB.h"
#include<chrono>
#include<opencv2/opencv.hpp>

ORB::ORB(const QString& grpName, QWidget* parent)
		:ImgProcessorBase(grpName, parent)
{
	// addConfig(std::make_shared<Config>("Threshold 1", 10, Config::Type::SPIN_DOUBLE, 1, 200, 1));
	
	addConfig(std::make_shared<Config>("Feature Number", 100, Config::Type::SPIN_INT, 50, 500, 10));
	addConfig(std::make_shared<Config>("ScaleFactor", 1.2f, Config::Type::SPIN_DOUBLE, 1, 2, 0.1));
	addConfig(std::make_shared<Config>("NLevels", 8, Config::Type::SPIN_INT, 5, 15, 1));
	addConfig(std::make_shared<Config>("EdgeThreshold", 31, Config::Type::SPIN_INT, 25, 40, 1));
	addConfig(std::make_shared<Config>("WTA_K", 2, Config::Type::SPIN_INT, 2, 2, 1));
	addConfig(std::make_shared<Config>("PatchSize", 31, Config::Type::SPIN_INT, 11, 51, 1));
	addConfig(std::make_shared<Config>("FastThreshold", 20, Config::Type::SPIN_INT, 10, 30, 1));
	addConfig(std::make_shared<ComboxConfig>("ScoreType", QStringList({ "HARRIS","FAST"})));
	//addConfig(std::make_shared<ComboxConfig>("Detector", QStringList({ "ORB","SIFT","SURF"})));
	//addConfig(std::make_shared<ComboxConfig>("Matcher", QStringList({ "FLANNBASED","BRUTEFORCE","BRUTEFORCE_L1","BRUTEFORCE_HAMMING" })));
	// addConfig(std::make_shared<Config>("Check Box",1, Config::Type::CHECKBOX,0,1));
	setupWidget();
}

void ORB::process(const std::vector<cv::Mat>& inImg, cv::Mat& outImg)
{
	typedef std::chrono::duration<int, std::milli> milliseconds_type;
	if (inImg.size() < 2) return;
	cv::Mat img1 = inImg[0].clone(), img2=inImg[1].clone();
	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
	cv::Mat descriptors_1, descriptors_2;
	cv::Ptr<cv::ORB> orb = cv::ORB::create( mConfigs["Feature Number"]->mValue,
											mConfigs["ScaleFactor"]->mValue,
											mConfigs["NLevels"]->mValue,
											mConfigs["EdgeThreshold"]->mValue,
											0,
											mConfigs["WTA_K"]->mValue,
											mConfigs["ScoreType"]->mValue==0?cv::ORB::HARRIS_SCORE:cv::ORB::FAST_SCORE,
											mConfigs["PatchSize"]->mValue,
											mConfigs["FastThreshold"]->mValue);
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	orb->detectAndCompute(img1, cv::Mat(), keypoints_1, descriptors_1);
    orb->detectAndCompute(img2, cv::Mat(), keypoints_2, descriptors_2);
	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	milliseconds_type detect_t = std::chrono::duration_cast<milliseconds_type>(t2 - t1);


	cv::drawKeypoints(img1, keypoints_1, img1, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
	cv::drawKeypoints(img2, keypoints_2, img2, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    std::vector<cv::DMatch> matches;
    cv::BFMatcher matcher (cv::NORM_HAMMING);
	t1 = std::chrono::steady_clock::now();
    matcher.match(descriptors_1, descriptors_2, matches);
	t2 = std::chrono::steady_clock::now();
	milliseconds_type match_t = std::chrono::duration_cast<milliseconds_type>(t2 - t1);
	const QString message = tr("Detection costs %1 ms. Matching costs %2 ms").arg(detect_t.count()).arg(match_t.count());
	emit log(message);
    cv::drawMatches(img1, keypoints_1, img2, keypoints_2, matches, outImg);
}																																																	 