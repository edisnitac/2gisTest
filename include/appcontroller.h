#include <QObject>
#include <QUrl>
#include "FileProcessor.h"

class AppController : public QObject
{
	Q_OBJECT
	Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
	Q_PROPERTY(QVariantList topWords READ topWords NOTIFY topWordsChanged)
	Q_PROPERTY(bool isProcessing READ isProcessing NOTIFY stateChanged)
	Q_PROPERTY(bool isPaused READ isPaused NOTIFY stateChanged)

public:
	explicit AppController(QObject *parent = nullptr);
	~AppController();

	double progress() const { return m_progress; }
	QVariantList topWords() const { return m_topWords; }
	bool isProcessing() const { return m_isProcessing; }
	bool isPaused() const { return m_isPaused; }

public slots:
	void openFile(const QUrl &fileUrl);
	void startProcessing();
	void pauseProcessing();
	void resumeProcessing();
	void cancelProcessing();

signals:
	void progressChanged();
	void topWordsChanged();
	void stateChanged(int state);
	void errorOccurred(const QString &message);

private:
	void setupThread();

	FileProcessor *m_processor;
	QThread *m_workerThread;
	double m_progress = 0;
	QVariantList m_topWords;
	QString m_currentFile;
	bool m_isProcessing = false;
	bool m_isPaused = false;
};
