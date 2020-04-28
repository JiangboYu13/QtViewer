#include "SIFT.h"
#include<chrono>

SIFT::SIFT(const QString& grpName, QWidget* parent)
		:ImgProcessorBase(grpName, parent)
{
	// addConfig(std::make_shared<Config>("Threshold 1", 10, Config::Type::SPIN_DOUBLE, 1, 200, 1));
	
	addConfig(std::make_shared<Config>("Feature Number", 100, Config::Type::SPIN_INT, 50, 500, 10));
	setupWidget();
}

void SIFT::process(const std::vector<cv::Mat>& inImg, cv::Mat& outImg)
{
	typedef std::chrono::duration<int, std::milli> milliseconds_type;
	if (inImg.size() < 2) return;
	cv::Mat img1, img2;
	cv::cvtColor(inImg[0], img1, cv::COLOR_RGB2GRAY);
	cv::cvtColor(inImg[1], img2, cv::COLOR_RGB2GRAY);
	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
	cv::Mat descriptors_1, descriptors_2;
	cv::Ptr<cv::SIFT> sift = cv::SIFT::create( mConfigs["Feature Number"]->mValue);
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	sift->detectAndCompute(img1, cv::Mat(), keypoints_1, descriptors_1);
    sift->detectAndCompute(img2, cv::Mat(), keypoints_2, descriptors_2);
	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	milliseconds_type detect_t = std::chrono::duration_cast<milliseconds_type>(t2 - t1);


	cv::drawKeypoints(img1, keypoints_1, img1, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
	cv::drawKeypoints(img2, keypoints_2, img2, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    std::vector<cv::DMatch> matches;
    cv::Ptr<cv::DescriptorMatcher>matcher = cv::DescriptorMatcher::create("BruteForce");
	t1 = std::chrono::steady_clock::now();
    matcher->match(descriptors_1, descriptors_2, matches);
	t2 = std::chrono::steady_clock::now();
	milliseconds_type match_t = std::chrono::duration_cast<milliseconds_type>(t2 - t1);
	const QString message = tr("Detection costs %1 ms. Matching costs %2 ms").arg(detect_t.count()).arg(match_t.count());
	emit log(message);
    cv::drawMatches(inImg[0], keypoints_1, inImg[1], keypoints_2, matches, outImg);
}																																																	 