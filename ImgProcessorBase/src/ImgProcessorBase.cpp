#include "ImgProcessorBase.h"
#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QtGlobal>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QStringList>
#include <QKeyEvent>
#include <QCheckBox>
void ImgProcessorBase::addConfig(std::shared_ptr<Config> config)
{
	mConfigs[config->mName] = config;
}


void ImgProcessorBase::onConfirm()
{
	emit valueChanged();
}
void ImgProcessorBase::keyPressEvent( QKeyEvent *k )
{
	if (k->key()==Qt::Key_Enter||k->key()==Qt::Key_Return)
	{
		emit valueChanged();
	}
}
void ImgProcessorBase::setupWidget()
{
	QGridLayout* mainLayout = new QGridLayout(this);
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(1, 3);
	mainLayout->setColumnStretch(2, 1);
	mainLayout->setColumnStretch(3, 3);
	mainLayout->setColumnStretch(4, 1);
	mainLayout->setColumnStretch(5, 3);
	int idx = 0;
	for (auto& mapconfig : mConfigs)
	{
		auto& config = mapconfig.second;
		QLabel* configLabel = new QLabel(config->mName, this);
		switch (config->mType)
		{
		case Config::Type::SPIN_INT:
			mConfigWidgets.push_back(new QSpinBox(this));
			dynamic_cast <QSpinBox*>(mConfigWidgets.back())->setRange(int(config->mMin), int(config->mMax));
			dynamic_cast <QSpinBox*>(mConfigWidgets.back())->setSingleStep(int(config->mStep));
			dynamic_cast <QSpinBox*>(mConfigWidgets.back())->setValue(int(config->mValue));
			connect(dynamic_cast <QSpinBox*>(mConfigWidgets.back()), QOverload<int>::of(&QSpinBox::valueChanged), 
				[&](int value) { config->onValueChanged(value); });
			break;
		case Config::Type::SPIN_DOUBLE:
			mConfigWidgets.push_back(new QDoubleSpinBox(this));
			dynamic_cast <QDoubleSpinBox*>(mConfigWidgets.back())->setRange(config->mMin, config->mMax);
			dynamic_cast <QDoubleSpinBox*>(mConfigWidgets.back())->setSingleStep(config->mStep);
			dynamic_cast <QDoubleSpinBox*>(mConfigWidgets.back())->setValue(config->mValue);
			connect(dynamic_cast <QDoubleSpinBox*>(mConfigWidgets.back()), QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
				[&](double value){config->onValueChanged(value);});
			break;
		case Config::Type::COMBOX:
			mConfigWidgets.push_back(new QComboBox(this));
			dynamic_cast <QComboBox*>(mConfigWidgets.back())->addItems(std::static_pointer_cast<ComboxConfig>(config)->mOptions);
			connect(dynamic_cast <QComboBox*>(mConfigWidgets.back()), QOverload<int>::of(&QComboBox::currentIndexChanged),
				[&](double value){config->onValueChanged(value);});
			break;
		case Config::Type::CHECKBOX:
			mConfigWidgets.push_back(new QCheckBox (this));
			dynamic_cast <QCheckBox*>(mConfigWidgets.back())->setChecked(config->mValue);
			connect(dynamic_cast <QCheckBox*>(mConfigWidgets.back()),&QCheckBox::toggled,
				[&](bool value){config->onValueChanged(value);});
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
	connect(comfirmBtn, &QPushButton::clicked, this, &ImgProcessorBase::onConfirm);
    setLayout(mainLayout);
    setFocusPolicy(Qt::StrongFocus);
}