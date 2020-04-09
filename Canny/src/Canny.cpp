#include "Canny.h"
Canny::Canny(const QString& grpName, QWidget* parent)
		:ImgProcessorBase(grpName, parent)
{
	addConfig(std::make_shared<Config>("Threshold 1", 10, Config::Type::SPIN_DOUBLE, 1, 200, 1));
	addConfig(std::make_shared<Config>("Threshold 2", 100, Config::Type::SPIN_DOUBLE, 1, 200, 1));
	addConfig(std::make_shared<Config>("Sobel Apature", 3, Config::Type::SPIN_INT, 3, 7, 2 ));
	//addConfig(std::make_shared<ComboxConfig>("Combox", QStringList({ "1","2","3", "4", "5"})));
	setupWidget();
}

void Canny::process(const cv::Mat& inImg, cv::Mat& outImg)
{
    cv::cvtColor(inImg, outImg, CV_RGB2GRAY);
	cv::Canny(outImg, outImg, mConfigs[0]->mValue, mConfigs[1]->mValue, int(mConfigs[2]->mValue));
    cv::cvtColor(outImg, outImg, CV_GRAY2RGB);
}