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
   : QMainWindow(parent), mImgLabel(new QLabel), mImgLabelProc(new QLabel), mMainLayout(new QGridLayout(this))
{
	mCurModeIdx = -1;
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
	mMainLayout->addWidget(mImgLabel, 0, 0, 10, 1);
	mMainLayout->addWidget(mImgLabelProc, 0, 1, 10, 1);
	setupProcessor();
	setupModeGroup();
	createActions();
	mMainLayout->setColumnStretch(0, 1);
	mMainLayout->setColumnStretch(1, 1);
    mainwindow = new QWidget(this);
    mainwindow->setLayout(mMainLayout);
    setCentralWidget(mainwindow);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
	statusBar()->showMessage(tr("Initialized"));
}


void QtViewer::setupProcessor()
{
	mProContainer.push_back(new GaussianBlur(tr("Gaussian Blur"), this));
	mProContainer.push_back(new Canny(tr("Canny"), this));
	for (auto processor : mProContainer)
	{
		connect(processor, &ImgProcessorBase::valueChanged, this, &QtViewer::onProcess);
		mMainLayout->addWidget(processor, 10, 1, 2, 1);
		processor->hide();
	}
	if (mProContainer.size() > 0) { 
		mCurModeIdx = 0;
		mProContainer[mCurModeIdx]->show();
	}
		
}

void QtViewer::setupModeGroup()
{
	mModeGrp = new QGroupBox(tr("Mode Seletion"), this);
	QGridLayout* grp_layout = new QGridLayout(this);
	for (int idx = 0; idx<mProContainer.size(); idx++)
	{
		mModeContainer.push_back(new QRadioButton(mProContainer[idx]->modeName(), this));
		grp_layout->addWidget(mModeContainer[idx], int(idx / 3), idx % 3, 1, 1, Qt::AlignLeft);
		connect(mModeContainer[idx], &QRadioButton::toggled, this, &QtViewer::onModeChanged);
	}
	if(mCurModeIdx>-1)
		mModeContainer[mCurModeIdx]->setChecked(true);
	mModeGrp->setLayout(grp_layout);
	mMainLayout->addWidget(mModeGrp, 10, 0, 2, 1);
}


void QtViewer::onModeChanged()
{
	for (int idx = 0; idx < mModeContainer.size(); idx++)
	{
		if (mModeContainer[idx]->isChecked() && mCurModeIdx != idx)
		{
				mProContainer[mCurModeIdx]->hide();
				mProContainer[idx]->show();
				statusBar()->showMessage(tr("Mode Changed from %1 to %2")
											.arg(mProContainer[mCurModeIdx]->modeName())
											.arg(mProContainer[idx]->modeName()));
				mCurModeIdx = idx;
				onProcess();
				break;

		}
	}
}
void QtViewer::onProcess()
{
	if (mCvImg.data && mCurModeIdx > -1)
	{
		cv::Mat img;
		mProContainer[mCurModeIdx]->process(mCvImg, img);
		setImageProc(img);
	}
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
    setWindowFilePath(fileName);
	onProcess();
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
