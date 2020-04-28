#pragma once 
#include "ImgProcessorBase.h"
#include <QString>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
class ORB:public ImgProcessorBase
{
	Q_OBJECT
	public:
		ORB(const QString& grpName="", QWidget* parent=nullptr);
		virtual void process(const std::vector<cv::Mat>& inImg, cv::Mat& outImg) override;
};
