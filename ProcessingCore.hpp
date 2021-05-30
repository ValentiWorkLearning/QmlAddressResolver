#ifndef PROCESSINGCORE_HPP
#define PROCESSINGCORE_HPP

#include <QThread>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <memory>
#include <set>
#include <thread>
#include <variant>
#include <vector>

#include "WordListModel.hpp"

namespace AddressSearchProcessingNs
{
using StringWithDistance = std::pair<std::string, int>;

using ResultVector = std::multiset<StringWithDistance, decltype([](const auto& a, const auto& b) {
                                       const auto& [leftWord, leftval] = a;
                                       const auto& [rightStr, rigthtval] = b;
                                       return leftval < rigthtval;
                                   })>;

using ReturnType = QList<std::shared_ptr<WordItem>>;

template <class... Ts> struct overload : Ts...
{
    using Ts::operator()...;
};
template <class... Ts> overload(Ts...) -> overload<Ts...>;

class ProcessingCore : public QObject
{

    Q_OBJECT
public:
    ProcessingCore(
        const QVector<QString>& _dataToProcess,
        const QString& _textToSearch,
        QObject* _parent = nullptr);

public slots:
    void startSearch();

signals:

    void onResultsReady(ReturnType pList);
    void onComputationCompleted();

private:
    int getDistance(const QString& s1, const QString& s2);

    ReturnType getAllDistances(const QVector<QString>& text, const QString& standard);

private:
    QString m_fileName;
    QString m_textToSearch;
    QVector<QString> m_text;
};
} // namespace AddressSearchProcessingNs
#endif
