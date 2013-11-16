
#ifndef BZRANNOTATETASK_H
#define BZRANNOTATETASK_H

#include <QObject>
#include <QStringList>

#include <unordered_map>

#include <vcs/vcsevent.h>

namespace KDevelop
{
class DVcsJob;
}

class BzrAnnotateTask : public QObject
{
    Q_OBJECT
public:

    explicit BzrAnnotateTask(KDevelop::DVcsJob* job, QObject* parent = 0);

    void start();

signals:
    void resultIsReady();

private slots:
    void parseNextLine();
    void prepareCommitInfo(std::size_t revision);
    void setResultOfJobAndFinish();
    void parseBzrLog(KDevelop::DVcsJob* job);

private:
    KDevelop::DVcsJob* _job;
    QStringList _outputLines;
    std::size_t _currentLine;
    std::unordered_map<std::size_t,KDevelop::VcsEvent> _commits;
    QVariantList _results;
};

#endif // BZRANNOTATETASK_H
