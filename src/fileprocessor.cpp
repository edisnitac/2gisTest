#include "../include/fileprocessor.h"
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>

FileProcessor::FileProcessor(QObject *parent)
	: QObject{parent}
{

}

void FileProcessor::processFile(const QString &filePath)
{
	m_isCanceled = false;
	m_isPaused = false;
	m_wordCounter.reset();

	CHUNK_SIZE = 1024 * 32;

	qDebug() << "Processing file:" << filePath;
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		emit errorOccurred(tr("Не удалось открыть файл: ") + file.errorString());
		return;
	}

	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	qint64 fileSize = file.size();
	qint64 processedSize = 0;

	QElapsedTimer updateTimer;
	QElapsedTimer fasterTimer;
	updateTimer.start();
	fasterTimer.start();

	while (!stream.atEnd() && !m_isCanceled) {
		while (m_isPaused && !m_isCanceled) {
			QThread::msleep(100);
		}

		readAndProcessChunk(stream, fileSize);
		processedSize = file.pos();

		if (fasterTimer.elapsed() >= FASTER_INTERVAL_MS) {
			CHUNK_SIZE *= 2;
			qDebug() << "CHUNK" << CHUNK_SIZE/1000;
			fasterTimer.restart();
		}

		if (updateTimer.elapsed() >= UPDATE_INTERVAL_MS) {
			double progress = (double)processedSize / fileSize;
			emit progressChanged(progress);
			emit topWordsUpdated(m_wordCounter.getTopWords(15));
			updateTimer.restart();
		}
	}

	file.close();

	if (!m_isCanceled) {
		emit progressChanged(100.0);
		emit topWordsUpdated(m_wordCounter.getTopWords(15));
		emit finished();
	}
}

void FileProcessor::readAndProcessChunk(QTextStream &stream, qint64 fileSize)
{
	QString chunk;
	chunk.reserve(CHUNK_SIZE);

	qint64 remaining = fileSize - stream.pos();
	qint64 readSize = qMin(CHUNK_SIZE, remaining);

	chunk = stream.read(readSize);
	m_wordCounter.processTextChunk(chunk);
}

void FileProcessor::pauseProcessing()
{
	qDebug() << "PAUSE POTROGAL";
	m_isPaused = true;
}

void FileProcessor::resumeProcessing()
{
	qDebug() << "PLAY POTROGAL";
	m_isPaused = false;
}

void FileProcessor::cancelProcessing()
{
	qDebug() << "CANCEL POTROGAL";
	m_isCanceled = true;
	m_wordCounter.reset();
}
