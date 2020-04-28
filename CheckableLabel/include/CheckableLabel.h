#pragma once 
#include <QGroupBox>
#include <QString>
#include <QObject>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
class CheckableLabel:public QLabel
{
	Q_OBJECT
public:
	CheckableLabel(QWidget * parent = nullptr) : QLabel(parent) {}
	void setImg(const QPixmap&);
	bool checked() { return mIsChecked; }
private:
	void updateImg(bool ischecked);
	bool mIsChecked = false;
	QPixmap* mPix=nullptr;
protected:
	void mousePressEvent(QMouseEvent* event);
	virtual ~CheckableLabel() { if (mPix) delete mPix; }
};




