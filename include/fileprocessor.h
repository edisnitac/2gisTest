#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QFile>
#include <QTextStream>
#include "WordCounter.h"

class FileProcessor : public QObject
{
	Q_OBJECT
public:
	explicit FileProcessor(QObject *parent = nullptr);

public slots:
	void processFile(const QString &filePath);

	void pauseProcessing();
	void resumeProcessing();
	void cancelProcessing();

signals:
	void progressChanged(double progress);
	void topWordsUpdated(const QVariantList &words);
	void finished();
	void errorOccurred(const QString &error);

private:
	void readAndProcessChunk(QTextStream &stream, qint64 fileSize);

	std::atomic<bool> m_isPaused{false};
	std::atomic<bool> m_isCanceled{false};

	WordCounter m_wordCounter;

	qint64 CHUNK_SIZE = 1024 * 16;
	static constexpr int UPDATE_INTERVAL_MS = 100;
	static constexpr int FASTER_INTERVAL_MS = 5000;
};

#endif // FILEPROCESSOR_H
