#include "../include/wordcounter.h"
#include <QDebug>
#include <QRegularExpression>

WordCounter::WordCounter()
{
	m_wordFrequencies.reserve(10000); // Оптимизация для большого кол-ва слов
}

void WordCounter::processTextChunk(const QString &text)
{
	static const QRegularExpression wordRegex(R"(\w+)", QRegularExpression::UseUnicodePropertiesOption);
	if (text.isEmpty()) {
		qWarning() << "Received empty text chunk";
		return;
	}
	auto matchIterator = wordRegex.globalMatch(text);
	while (matchIterator.hasNext()) {
		auto word = matchIterator.next().captured().toLower();
		word = emit sanitizeWord(word);
		if (!word.isEmpty()) {
			m_wordFrequencies[word]++;
		}
	}
}

QVariantList WordCounter::getTopWords(int count) const
{
	QVector<QPair<QString, int>> sortedWords;
	sortedWords.reserve(m_wordFrequencies.size());

	for (auto it = m_wordFrequencies.begin(); it != m_wordFrequencies.end(); ++it) {
		sortedWords.append(qMakePair(it.key(), it.value()));
	}

	std::sort(sortedWords.begin(), sortedWords.end(),
		[](const QPair<QString, int> &a, const QPair<QString, int> &b) {
			return a.second > b.second;
		});

	QVariantList result;
	int resultCount = qMin(count, sortedWords.size());
	for (int i = 0; i < resultCount; ++i) {
		QVariantMap entry;
		entry["word"] = sortedWords[i].first;
		entry["count"] = sortedWords[i].second;
		result.append(entry);
	}

	return result;
}

void WordCounter::reset()
{
	m_wordFrequencies.clear();
}

QString WordCounter::sanitizeWord(const QString &word) const
{
	QString cleaned;
	cleaned.reserve(word.size());
	for (const QChar &c : word) {
		if (c.isLetter()) {
			cleaned.append(c);
		}
	}
	return cleaned;
}
