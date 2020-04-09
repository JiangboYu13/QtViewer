#pragma once 
#include "ImgProcessorBase.h"
#include <QString>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QAbstractSpinBox>
#include <vector>
#include <memory>
class Canny:public ImgProcessorBase
{
	Q_OBJECT
	public:
		Canny(const QString& grpName="", QWidget* parent=nullptr);
		virtual void process(const cv::Mat&, cv::Mat&) override;
	public slots:
		void onConfirm();
	private:
		std::vector<QWidget*> mConfigWidgets;
		std::vector<std::shared_ptr<Config>> mConfigs;
		void setupWidget();
		void addConfig(std::shared_ptr<Config>);
};
