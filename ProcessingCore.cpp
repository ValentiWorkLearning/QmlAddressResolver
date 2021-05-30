#include "ProcessingCore.hpp"
#include <QDebug>

namespace AddressSearchProcessingNs
{
ProcessingCore::ProcessingCore(
    const QVector<QString>& _dataToProcess,
    const QString& _textToSearch,
    QObject* _parent)
    : QObject{_parent}, m_textToSearch{_textToSearch}, m_text{_dataToProcess}
{
}

int ProcessingCore::getDistance(const QString& s1, const QString& s2)
{
    const std::size_t len1 = s1.size(), len2 = s2.size();
    std::vector<std::vector<unsigned int>> d(len1 + 1, std::vector<unsigned int>(len2 + 1));

    d[0][0] = 0;
    for (unsigned int i = 1; i <= len1; ++i)
        d[i][0] = i;
    for (unsigned int i = 1; i <= len2; ++i)
        d[0][i] = i;

    for (unsigned int i = 1; i <= len1; ++i)
        for (unsigned int j = 1; j <= len2; ++j)
            d[i][j] = std::min(
                {d[i - 1][j] + 1,
                 d[i][j - 1] + 1,
                 d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1)});

    return d[len1][len2];
}

ReturnType ProcessingCore::getAllDistances(
    const QVector<QString>& text,
    const QString& standard)
{
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    ReturnType v;
    for (auto& word : text)
    {
        const int64_t maxDist = standard.length() / 2;
        int result = getDistance(word, standard);

        if (result == 0)
        {
            v.push_back(std::make_shared<WordItem>(word, result));
            return v;
        }
        if (result <= maxDist)
        {
            v.push_back(std::make_shared<WordItem>(word, result));
        }
    }

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    qDebug() << ms_double.count() << "ms";
    return v;
}

void ProcessingCore::startSearch()
{
    auto value = getAllDistances(m_text, m_textToSearch);
    m_text.clear();

    std::ranges::sort(
                value,
        [](const auto& _left, const auto& _right){
            return _left->getCloseToStandard().toInt() < _right->getCloseToStandard().toInt();
        }
    );
    onResultsReady(value);
    onComputationCompleted();
}

} // namespace AddressSearchProcessingNs
