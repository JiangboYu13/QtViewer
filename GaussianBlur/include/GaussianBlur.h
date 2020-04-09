#pragma once 
#include "ImgProcessorBase.h"
#include <QString>
#include <QObject>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>



class GaussianBlur:public ImgProcessorBase
{

	Q_OBJECT
	public:
		GaussianBlur(const QString& grpName="", QWidget* parent=nullptr);
		virtual void process(const cv::Mat&, cv::Mat&) override;
};
