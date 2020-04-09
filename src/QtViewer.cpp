#include "QtViewer.h"
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QStatusBar>
#include <QGridLayout>
#include <QRadioButton>
QtViewer::QtViewer(QWidget *parent)
   : QMainWindow(parent), mImgLabel(new QLabel), mImgLabelProc(new QLabel)
{
    mImgLabel->setBackgroundRole(QPalette::Base);
    mImgLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    mImgLabel->setScaledContents(true);
    mImgLabel->setFrameShape(QFrame::Panel);
	mImgLabel->setFrameShadow(QFrame::Sunken);
	mImgLabel->setLineWidth(3);


    mImgLabelProc->setBackgroundRole(QPalette::Base);
    mImgLabelProc->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    mImgLabelProc->setScaledContents(true);
	mImgLabelProc->setFrameShape(QFrame::Panel);
	mImgLabelProc->setFrameShadow(QFrame::Sunken);
	mImgLabelProc->setLineWidth(3);
    mModeGrp = new QGroupBox(tr("Mode Seletion"),this);
    setupModeGroup(mModeGrp);

    mProcessor = new GaussianBlur(tr("Gaussian Blur"), this);
    connect(mProcessor, &ImgProcessorBase::valueChanged, this, &QtViewer::onProcess);
    createActions();
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->addWidget(mImgLabel, 0, 0, 10, 1);
    mainLayout->addWidget(mImgLabelProc, 0, 1, 10, 1);
    mainLayout->addWidget(mModeGrp, 10, 0, 2, 1);
    mainLayout->addWidget(mProcessor, 10, 1, 2, 1);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 1);
    mainwindow = new QWidget(this);
    mainwindow->setLayout(mainLayout);
    setCentralWidget(mainwindow);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}
void QtViewer::onProcess()
{
	cv::Mat img;
	mProcessor->process(mCvImg,img);
	setImageProc(img);
}
void QtViewer::setupModeGroup(QGroupBox* grp)
{
	QGridLayout* grp_layout = new QGridLayout(this);
	QRadioButton* gaussian_mode = new QRadioButton("Gaussian Mode", this);
	QRadioButton* gray_mode = new QRadioButton("Grayscale Mode", this);
	grp_layout->addWidget(gaussian_mode, 0, 0, 1, 1, Qt::AlignLeft);
	grp_layout->addWidget(gray_mode, 0, 1, 1, 1, Qt::AlignLeft);
	grp->setLayout(grp_layout);
}
bool QtViewer::loadFile(const QString &fileName)
{
    mCvImg = cv::imread(fileName.toStdString(), CV_LOAD_IMAGE_COLOR);
    if (!mCvImg.data) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1")
                                 .arg(QDir::toNativeSeparators(fileName)));
        return false;
    }
//! [2]
    cv::cvtColor(mCvImg, mCvImg, CV_BGR2RGB);
    setImage(mCvImg);
    cv::Mat gray;
    cv::cvtColor(mCvImg, gray, CV_RGB2GRAY);
    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(mCvImg.cols).arg(mCvImg.rows).arg(mCvImg.channels());
    statusBar()->showMessage(message);
    return true;
}

void QtViewer::setImage(const cv::Mat &matImage,QImage::Format format)
{
	QImage newImage((uchar*)matImage.data, matImage.cols, matImage.rows, format);
    mImgLabel->setPixmap(QPixmap::fromImage(newImage));
}

void QtViewer::setImageProc(const cv::Mat &matImage,QImage::Format format)
{
	QImage newImage((uchar*)matImage.data, matImage.cols, matImage.rows, format);
	mImgLabelProc->setPixmap(QPixmap::fromImage(newImage));
}
static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void QtViewer::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}
//! [1]


void QtViewer::createActions()
//! [17] //! [18]
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &QtViewer::open);
    openAct->setShortcut(QKeySequence::Open);

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

}
//! [18]
