#include "Canny.h"
#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QtGlobal>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QStringList>
Canny::Canny(const QString& grpName, QWidget* parent)
		:ImgProcessorBase(grpName, parent)
{
	addConfig(std::make_shared<Config>("Threshold 1", 10, Config::Type::SPIN_DOUBLE, 1, 200, 1));
	addConfig(std::make_shared<Config>("Threshold 2", 100, Config::Type::SPIN_DOUBLE, 1, 200, 1));
	addConfig(std::make_shared<Config>("Sobel Apature", 3, Config::Type::SPIN_INT, 1, 101, 2 ));
	addConfig(std::make_shared<ComboxConfig>("Combox", QStringList({ "1","2","3", "4", "5"})));
	setupWidget();

}
void Canny::addConfig(std::shared_ptr<Config> config)
{
	mConfigs.push_back(config);
}
void Canny::process(const cv::Mat& inImg, cv::Mat& outImg)
{
    cv::cvtColor(inImg, outImg, CV_RGB2GRAY);
	cv::Canny(outImg, outImg, mConfigs[0]->mValue, mConfigs[1]->mValue, int(mConfigs[2]->mValue));
    cv::cvtColor(outImg, outImg, CV_GRAY2RGB);
}
void Canny::onConfirm()
{
	emit valueChanged();
}
void Canny::setupWidget()
{
	QGridLayout* mainLayout = new QGridLayout(this);
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(1, 3);
	mainLayout->setColumnStretch(2, 1);
	mainLayout->setColumnStretch(3, 3);
	mainLayout->setColumnStretch(4, 1);
	mainLayout->setColumnStretch(5, 3);
	int idx = 0;
	for (auto& config : mConfigs)
	{
		QLabel* configLabel = new QLabel(config->mName, this);
		switch (config->mType)
		{
		case Config::Type::SPIN_INT:
			mConfigWidgets.push_back(new QSpinBox(this));
			dynamic_cast <QSpinBox*>(mConfigWidgets.back())->setRange(int(config->mMin), int(config->mMax));
			dynamic_cast <QSpinBox*>(mConfigWidgets.back())->setSingleStep(int(config->mStep));
			dynamic_cast <QSpinBox*>(mConfigWidgets.back())->setValue(int(config->mValue));
			connect(dynamic_cast <QSpinBox*>(mConfigWidgets.back()), QOverload<int>::of(&QSpinBox::valueChanged), 
				[&](int value) { config->mValue = value; });
			break;
		case Config::Type::SPIN_DOUBLE:
			mConfigWidgets.push_back(new QDoubleSpinBox(this));
			dynamic_cast <QDoubleSpinBox*>(mConfigWidgets.back())->setRange(config->mMin, config->mMax);
			dynamic_cast <QDoubleSpinBox*>(mConfigWidgets.back())->setSingleStep(config->mStep);
			dynamic_cast <QDoubleSpinBox*>(mConfigWidgets.back())->setValue(config->mValue);
			connect(dynamic_cast <QDoubleSpinBox*>(mConfigWidgets.back()), QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
				[&](double value) { config->mValue = value; });
			break;
		case Config::Type::COMBOX:
			mConfigWidgets.push_back(new QComboBox(this));
			dynamic_cast <QComboBox*>(mConfigWidgets.back())->addItems(std::static_pointer_cast<ComboxConfig>(config)->mOptions);
			connect(dynamic_cast <QComboBox*>(mConfigWidgets.back()), QOverload<int>::of(&QComboBox::currentIndexChanged),
				[&](int value) { config->mValue = value; });
			break;
		default:
			break;
		}
		mainLayout->addWidget(configLabel, int(idx/3), idx%3*2, 1, 1, Qt::AlignLeft);
		mainLayout->addWidget(mConfigWidgets.back(), int(idx/3), idx%3*2+1, 1, 1, Qt::AlignLeft);
		idx++;
		
	}
    QPushButton* comfirmBtn = new QPushButton(tr("Confirm"), this);
    mainLayout->addWidget(comfirmBtn, int(idx / 3), idx % 3 * 2, 1, 1, Qt::AlignLeft);
	connect(comfirmBtn, &QPushButton::clicked, this, &Canny::onConfirm);
    setLayout(mainLayout);
}