#pragma once 
#include "ImgProcessorBase.h"
#include <QString>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
class SIFT:public ImgProcessorBase
{
	Q_OBJECT
public:
	SIFT(const QString& grpName="", QWidget* parent=nullptr);
	virtual void process(const std::vector<cv::Mat>& inImg, cv::Mat& outImg) override;
};
