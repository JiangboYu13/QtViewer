#include "CheckableLabel.h"
#include <iostream>
void CheckableLabel::setImg(const QPixmap& img)
{
	mPix = new QPixmap(img);
	updateImg(mIsChecked);
}
void CheckableLabel::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton && mPix!=nullptr)
	{
		mIsChecked = !mIsChecked;
		updateImg(mIsChecked);
	}
}
void CheckableLabel::updateImg(bool ischecked)
{
	int opac = ischecked ? 255 : 100;
	QPixmap temp(mPix->size());
	temp.fill(Qt::transparent);
	QPainter p1(&temp);
	p1.setCompositionMode(QPainter::CompositionMode_Source);
	p1.drawPixmap(0, 0, *mPix);
	p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	p1.fillRect(temp.rect(), QColor(0, 0, 0, opac));
	p1.end();
	setPixmap(temp);
}
