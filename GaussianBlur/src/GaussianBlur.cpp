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
    addConfig(std::make_shared<Config>("Sigma X", 10, Config::Type::SPIN_DOUBLE, 0, 200, 0.5));
    addConfig(std::make_shared<Config>("Sigma Y", 100, Config::Type::SPIN_DOUBLE, 0, 200, 0.5));
    //addConfig(std::make_shared<ComboxConfig>("Combox", QStringList({ "1","2","3", "4", "5"})));
    setupWidget();
}

void GaussianBlur::process(const cv::Mat& inImg, cv::Mat& outImg)
{
    cv::GaussianBlur(inImg, outImg,  cv::Size(mConfigs[0]->mValue,mConfigs[0]->mValue),  mConfigs[1]->mValue,  mConfigs[2]->mValue);
}

