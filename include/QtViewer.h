#pragma once 
#include <QMainWindow>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QObject>
#include <QMetaType>
#include <QGroupBox>
#include "GaussianBlur.h"
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
    bool loadFile(const QString &);

private slots:
    void open();
    void onProcess();
private:
    void createActions();
    void setImage(const cv::Mat &matImage, QImage::Format format=QImage::Format_RGB888);
    void setImageProc(const cv::Mat &matImage, QImage::Format format=QImage::Format_RGB888);
    void setupModeGroup(QGroupBox*); 
    ImgProcessorBase* mProcessor;
    QWidget* mainwindow;
    cv::Mat mCvImg;
    QLabel* mImgLabel;
    QLabel* mImgLabelProc;
    QGroupBox* mModeGrp;
};
