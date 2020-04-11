#pragma once 
#include <QGroupBox>
#include <QString>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QObject>
#include <QStringList>
#include <memory>
#include <map>
class Config{	
public:
	enum class Type { SPIN_INT, SPIN_DOUBLE, COMBOX, CHECKBOX};
	void onValueChanged(double value) { mValue = value; }
	Config(QString name, double value, Type type, double min=0, double max=100, double step=1): 
		mName(name), mMin(min), mMax(max), mStep(step), mValue(value), mType(type){}
	QString mName;
	double mMin;
	double mMax;
	double mStep;
	double mValue;
	Type mType;
};
class ComboxConfig:public Config {
public:
	ComboxConfig(QString name, QStringList options) : mOptions(options), Config(name, 0, Type::COMBOX, 0, options.size() - 1, 1) {}
	QStringList mOptions;
};

class ImgProcessorBase:public QGroupBox
{
	Q_OBJECT
public:
	ImgProcessorBase(const QString& grpName, QWidget* parent):QGroupBox(grpName, parent), mGrpName(grpName){};
	virtual void process(const cv::Mat&, cv::Mat&) = 0;
	QString modeName() { return mGrpName;};
signals:
    void valueChanged();
protected slots:
	void onConfirm();
	virtual void keyPressEvent(QKeyEvent *event) override;
protected:
	QString mGrpName;
	std::vector<QWidget*> mConfigWidgets;
	std::map<QString, std::shared_ptr<Config>> mConfigs;
	void setupWidget();
	void addConfig(std::shared_ptr<Config>);

};

