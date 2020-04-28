#include "GaussianBlur.h"
#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QtGlobal>



GaussianBlur::GaussianBlur(const QString& grpName, QWidget* parent)
        :ImgProcessorBase(grpName, parent)
{

    addConfig(std::make_shared<Config>("Kernel Size", 3, Config::Type::SPIN_INT, 1, 101, 2 ));
    addConfig(std::make_shared<Config>("Sigma X", 0, Config::Type::SPIN_DOUBLE, 0, 200, 0.5));
    addConfig(std::make_shared<Config>("Sigma Y", 0, Config::Type::SPIN_DOUBLE, 0, 200, 0.5));
    setupWidget();
}

void GaussianBlur::process(const  std::vector<cv::Mat>& inImg, cv::Mat& outImg)
{
	if (inImg.size() < 1) return;
    cv::GaussianBlur(inImg[0], outImg,  cv::Size(mConfigs["Kernel Size"]->mValue,mConfigs["Kernel Size"]->mValue),  mConfigs["Sigma X"]->mValue,  mConfigs["Sigma Y"]->mValue);
}

