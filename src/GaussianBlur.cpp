#include "GaussianBlur.h"
#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QtGlobal>
GaussianBlur::GaussianBlur(const QString& grpName, QWidget* parent)
		:ImgProcessorBase(grpName, parent)
{
	mKsize = cv::Size(5,5);
	mSigmaY = 0;
	mSigmaX = 0;
	setupWidget();

}

void GaussianBlur::process(const cv::Mat& inImg, cv::Mat& outImg)
{
	cv::GaussianBlur(inImg, outImg, mKsize, mSigmaX, mSigmaY);
}
void GaussianBlur::onConfirm()
{
	emit valueChanged();
}
void GaussianBlur::onKSizeChanged(int value)
{
	mKsize = cv::Size(value,value);
}
void GaussianBlur::onSigmaChanged(double value)
{
	mSigmaY = sigmaXValue->value();
	mSigmaX = sigmaYValue->value();
}
void GaussianBlur::setupWidget()
{
	
	QLabel* ksizeLabel = new QLabel(tr("Kernel Size"));
	ksizeValue = new QSpinBox(this);
	ksizeValue->setRange(1, 21);
    ksizeValue->setSingleStep(2);
    ksizeValue->setValue(5);

    QLabel* sigmaXLabel = new QLabel(tr("SigmaX"));
	sigmaXValue = new QDoubleSpinBox(this);
	sigmaXValue->setRange(0, 20);
    sigmaXValue->setSingleStep(0.5);
    sigmaXValue->setValue(mSigmaX);

    QLabel* sigmaYLabel = new QLabel(tr("SigmaY"));
    sigmaYValue = new QDoubleSpinBox(this);
    sigmaYValue->setRange(0, 20);
    sigmaYValue->setSingleStep(0.5);
    sigmaYValue->setValue(mSigmaY);

    QPushButton* comfirmBtn = new QPushButton(tr("Confirm"), this);


    connect(ksizeValue, QOverload<int>::of(&QSpinBox::valueChanged), this, &GaussianBlur::onKSizeChanged);
    connect(sigmaXValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &GaussianBlur::onSigmaChanged);
    connect(sigmaYValue, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &GaussianBlur::onSigmaChanged);
    connect(comfirmBtn, &QPushButton::clicked, this, &GaussianBlur::onConfirm);

    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 3);
    mainLayout->setColumnStretch(2, 1);
    mainLayout->setColumnStretch(3, 3);
    mainLayout->addWidget(ksizeLabel, 0, 0, 1, 1, Qt::AlignLeft);
    mainLayout->addWidget(ksizeValue, 0, 1, 1, 1, Qt::AlignLeft);


    mainLayout->addWidget(sigmaXLabel, 0, 2, 1, 1, Qt::AlignLeft);
    mainLayout->addWidget(sigmaXValue, 0, 3, 1, 1, Qt::AlignLeft);

    mainLayout->addWidget(sigmaYLabel, 1, 0, 1, 1, Qt::AlignLeft);
    mainLayout->addWidget(sigmaYValue, 1, 1, 1, 1, Qt::AlignLeft);


    mainLayout->addWidget(comfirmBtn, 1, 2, 1, 1, Qt::AlignLeft);

    setLayout(mainLayout);
}