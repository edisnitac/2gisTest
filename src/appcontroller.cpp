#include "../include/appcontroller.h"
#include <QFileInfo>
#include <QDebug>
#include <QMetaObject>

AppController::AppController(QObject *parent)
	: QObject(parent),
	  m_processor(new FileProcessor()),
	  m_workerThread(new QThread(this)),
	  m_progress(0.0),
	  m_isProcessing(false),
	  m_isPaused(false)
{
	setupThread();
}

AppController::~AppController()
{
	cancelProcessing();

	if (m_workerThread->isRunning()) {
		m_workerThread->quit();
		m_workerThread->wait();
	}
}

void AppController::setupThread()
{
	m_processor->moveToThread(m_workerThread);

	connect(m_processor, &FileProcessor::progressChanged, this, [this](double progress) {
		m_progress = progress;
		emit progressChanged();
	});

	connect(m_processor, &FileProcessor::topWordsUpdated, this, [this](const QVariantList &words) {
		m_topWords = words;
		emit topWordsChanged();
	});

	connect(m_processor, &FileProcessor::finished, this, [this]() {
		m_isProcessing = false;
		m_isPaused = false;
		emit stateChanged(0);
	});

	connect(m_processor, &FileProcessor::errorOccurred, this, &AppController::errorOccurred);

	connect(m_workerThread, &QThread::finished, m_processor, &QObject::deleteLater);
	connect(m_workerThread, &QThread::finished, m_workerThread, &QObject::deleteLater);

	m_workerThread->start(QThread::LowPriority);
}

void AppController::openFile(const QUrl &fileUrl)
{
	qDebug() << "Opening file:" << fileUrl.toLocalFile();
	if (m_isProcessing) {
		emit errorOccurred("Завершите текущую обработку перед открытием нового файла");
		return;
	}

	QString filePath = fileUrl.toLocalFile();
	QFileInfo fileInfo(filePath);

	if (!fileInfo.exists()) {
		emit errorOccurred("Файл не существует: " + filePath);
		return;
	}

	if (!fileInfo.isReadable()) {
		emit errorOccurred("Нет прав на чтение файла: " + filePath);
		return;
	}

	m_currentFile = filePath;
	m_progress = 0.0;
	m_topWords.clear();
	AppController::startProcessing();
	emit progressChanged();
	emit topWordsChanged();
}

void AppController::startProcessing()
{
	if (m_currentFile.isEmpty()) {
		emit errorOccurred("Файл не выбран");
		return;
	}

	if (!m_isProcessing) {
		m_isProcessing = true;
		m_isPaused = false;
		emit stateChanged(1);

		QMetaObject::invokeMethod(m_processor, "processFile",
								Qt::QueuedConnection,
								Q_ARG(QString, m_currentFile));
	}
}

void AppController::pauseProcessing()
{
	if (m_isProcessing && !m_isPaused) {
		m_isPaused = true;
		emit stateChanged(2);
		QMetaObject::invokeMethod(m_processor, "pauseProcessing", Qt::DirectConnection);
	}
}

void AppController::resumeProcessing()
{
	if (m_isProcessing && m_isPaused) {
		m_isPaused = false;
		emit stateChanged(1);
		QMetaObject::invokeMethod(m_processor, "resumeProcessing", Qt::DirectConnection);
	}
}

void AppController::cancelProcessing()
{
	if (m_isProcessing) {
		QMetaObject::invokeMethod(m_processor, "cancelProcessing", Qt::DirectConnection);

		m_isProcessing = false;
		m_isPaused = false;
		m_progress = 0.0;
		m_topWords.clear();
		emit progressChanged();
		emit topWordsChanged();
		emit stateChanged(0);
	}
}
