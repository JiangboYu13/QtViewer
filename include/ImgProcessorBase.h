#pragma once 
#include <QGroupBox>
#include <QString>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
class ImgProcessorBase:public QGroupBox
{
	Q_OBJECT
public:
	ImgProcessorBase(const QString& grpName, QWidget* parent):QGroupBox(grpName, parent){};
	virtual void process(const cv::Mat&, cv::Mat&) = 0;
signals:
    void valueChanged();

};
