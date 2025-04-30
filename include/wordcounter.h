#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QVector>
#include <QVariant>

class WordCounter : public QObject
{
	Q_OBJECT
public:
	WordCounter();

	void processTextChunk(const QString &text);

	QVariantList getTopWords(int count) const;

	void reset();

private:
	QString sanitizeWord(const QString &word) const;

	QHash<QString, int> m_wordFrequencies;
};

#endif // WORDCOUNTER_H
