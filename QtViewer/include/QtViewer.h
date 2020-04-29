#pragma once 
#include <QMainWindow>
#include <QRadioButton>
#include <QImage>
#include <QGridLayout>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QTableWidget>
#include <QObject>
#include <QMetaType>
#include <QGroupBox>
#include <vector>
#include <string>
#include <mutex>
#include <QFrame>
#include "GaussianBlur.h"
#include "Canny.h"
#include "CheckableLabel.h"
#include "ORB.h"
#ifdef USE_CONTRIB_FEATURE
	#include "SIFT.h"
#endif
QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE
class QtViewer : public QMainWindow
{
    Q_OBJECT
public: 
    QtViewer(QWidget *parent = nullptr);
    bool loadFile(const QStringList &);

private slots:
    void open();
    void onProcess();
	void onModeChanged();
	void onLog(QString);
private:
	std::vector<int> getChecked();
	void setupImageList();
	void setupProcLabel();
    void createActions();
	void addImage(const cv::Mat &matImage, QImage::Format format = QImage::Format_RGB888);
    void setImageProc(const cv::Mat &matImage, QImage::Format format=QImage::Format_RGB888);
    void setupModeGroup(); 
	void setupProcessor();
    std::vector<ImgProcessorBase*> mProContainer;
	std::vector<QRadioButton*> mModeContainer;
	QGridLayout* mMainLayout;
	int mCurModeIdx;
    QWidget* mainwindow;
    std::vector<cv::Mat> mCvImgs;
	QFrame* mImgList;
    QGridLayout* mImgListLayout;
	QLabel* mImgLabel;
    QLabel* mImgLabelProc;
    QGroupBox* mModeGrp;
	std::mutex mLock;
};
