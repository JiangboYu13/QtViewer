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
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSizePolicy>
#include <iostream>
#include <QListView>
#include <QTreeView>
#include <QTableView>
QtViewer::QtViewer(QWidget *parent)
	: QMainWindow(parent)
{
	mCurModeIdx = -1;
	setupImageList();
	setupProcLabel();
	mMainLayout = new QGridLayout(this);
	mMainLayout->addWidget(mImgList, 0, 0, 10, 1);
	mMainLayout->addWidget(mImgLabelProc, 0, 1, 10, 1);
	mMainLayout->setColumnStretch(0, 1);
	mMainLayout->setColumnStretch(1, 1);
	setupProcessor();
	setupModeGroup();
	createActions();
	mainwindow = new QWidget(this);
	mainwindow->setLayout(mMainLayout);
	setCentralWidget(mainwindow);
	resize(QGuiApplication::primaryScreen()->availableSize()*3/5);
	statusBar()->showMessage(tr("Initialized"));
}

void QtViewer::setupImageList()
{
	mImgList = new QFrame(this);
	mImgList->setBackgroundRole(QPalette::Base);
	mImgList->setFrameShape(QFrame::Panel);
	mImgList->setFrameShadow(QFrame::Sunken);
	mImgList->setLineWidth(3);
	mImgListLayout = new QGridLayout(this);
	mImgList->setLayout(mImgListLayout);


	mImgLabel = new QLabel(this);
	mImgLabel->setBackgroundRole(QPalette::Base);
	mImgLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	mImgLabel->setScaledContents(true);
	mImgLabel->setFrameShape(QFrame::Panel);
	mImgLabel->setFrameShadow(QFrame::Sunken);
	mImgLabel->setLineWidth(3);
	mImgLabel->hide();
}
std::vector<int> QtViewer::getChecked()
{
	std::vector<int> ret;
	if (mImgListLayout)
	{

		for (int i = 0; i < mImgListLayout->count(); ++i)
		{
			if (dynamic_cast<CheckableLabel*>(mImgListLayout->itemAt(i)->widget())->checked())
				ret.push_back(i);
		}
	}
	return ret;
}
void QtViewer::setupProcLabel()
{
	mImgLabelProc = new QLabel(this);
	mImgLabelProc->setBackgroundRole(QPalette::Base);
	mImgLabelProc->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	mImgLabelProc->setScaledContents(true);
	mImgLabelProc->setFrameShape(QFrame::Panel);
	mImgLabelProc->setFrameShadow(QFrame::Sunken);
	mImgLabelProc->setLineWidth(3);
}
void QtViewer::setupProcessor()
{
	mProContainer.push_back(new GaussianBlur(tr("Gaussian Blur"), this));
	mProContainer.push_back(new Canny(tr("Canny"), this));
	mProContainer.push_back(new ORB(tr("ORB"), this));
	mProContainer.push_back(new SIFT(tr("SIFT"), this));
	for (auto processor : mProContainer)
	{
		connect(processor, &ImgProcessorBase::valueChanged, this, &QtViewer::onProcess);
		connect(processor, &ImgProcessorBase::log, this, &QtViewer::onLog);
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
	if (mCurModeIdx>-1)
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
void QtViewer::onLog(QString msg)
{
	statusBar()->showMessage(msg);
}
void QtViewer::onProcess()
{

	std::vector<int> checkedIdx = getChecked();
	if (checkedIdx.size() == 0) return;
	std::vector<cv::Mat> checkedImg;
	for (int idx : checkedIdx)
		checkedImg.push_back(mCvImgs[idx]);
	if (checkedImg.size() != 0 && mCurModeIdx > -1)
	{
		cv::Mat img;
		mProContainer[mCurModeIdx]->process(checkedImg, img);
		if (!img.empty())
			setImageProc(img);
	}
}

void QtViewer::addImage(const cv::Mat &matImage, QImage::Format format)
{

	QImage newImage((uchar*)matImage.data, matImage.cols, matImage.rows, format);
	QLabel* newLabel = new CheckableLabel(this);
	newLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	newLabel->setScaledContents(true);
	dynamic_cast<CheckableLabel*>(newLabel)->setImg(QPixmap::fromImage(newImage));
	mImgListLayout->addWidget(newLabel, int((mCvImgs.size() - 1) / 2), int((mCvImgs.size() - 1) % 2));
	/*QLabel* newLabel = new QLabel(this);
	newLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	newLabel->setScaledContents(true);
	newLabel->setPixmap(QPixmap::fromImage(newImage));
	mImgList->setCellWidget(int(mCvImgs.size() / 2), int(mCvImgs.size() % 2), newLabel);*/
}

void QtViewer::setImageProc(const cv::Mat &matImage, QImage::Format format)
{
	QImage newImage((uchar*)matImage.data, matImage.cols, matImage.rows, format);
	mImgLabelProc->setPixmap(QPixmap::fromImage(newImage));
}
bool QtViewer::loadFile(const QStringList &fileNames)
{
	for (int i = 0; i < fileNames.size(); i++)
	{
		const QString fileName = fileNames[i];
		cv::Mat img = cv::imread(fileName.toStdString(), cv::IMREAD_COLOR);
		cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
		mCvImgs.push_back(img);
		if (!mCvImgs.back().data) {
			mCvImgs.pop_back();
			/*QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
			tr("Cannot load %1")
			.arg(QDir::toNativeSeparators(fileName)));*/
			continue;
		}
		addImage(mCvImgs.back());
		////! [2]
		//cv::cvtColor(mCvImgs.back(), mCvImg, CV_BGR2RGB);
		//setImage(mCvImg);
		//setWindowFilePath(fileName);
		//onProcess();
		//const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
		//	.arg(QDir::toNativeSeparators(fileName)).arg(mCvImg.cols).arg(mCvImg.rows).arg(mCvImg.channels());
		//statusBar()->showMessage(message);
	}

	return true;
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
	dialog.setFileMode(QFileDialog::ExistingFiles);
	initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);
	

	while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles())) {}
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
