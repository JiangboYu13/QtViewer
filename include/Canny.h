#pragma once 
#include "ImgProcessorBase.h"
#include <QString>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
class Canny:public ImgProcessorBase
{
	Q_OBJECT
	public:
		Canny(const QString& grpName="", QWidget* parent=nullptr);
		virtual void process(const cv::Mat&, cv::Mat&) override;	
};
