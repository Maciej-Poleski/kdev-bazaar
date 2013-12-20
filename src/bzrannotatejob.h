#ifndef BZRANNOTATETASK_H
#define BZRANNOTATETASK_H

#include <unordered_map>

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QDir>

#include <KUrl>

#include <kdevplatform/vcs/vcsevent.h>
#include <kdevplatform/vcs/vcsjob.h>

class QDir;
namespace KDevelop
{
class DVcsJob;
}

class BzrAnnotateJob : public KDevelop::VcsJob
{
    Q_OBJECT
public:

    explicit BzrAnnotateJob(const QDir& workingDir, const QString& revisionSpec, const KUrl& localLocation, KDevelop::IPlugin* parent = 0, OutputJobVerbosity verbosity = OutputJob::Verbose);

    virtual QVariant fetchResults() override;
    virtual void start() override;
    virtual JobStatus status() const override;
    virtual KDevelop::IPlugin* vcsPlugin() const override;

protected:
    virtual bool doKill() override;

private slots:
    void parseBzrAnnotateOutput(KDevelop::DVcsJob* job);
    void parseNextLine();
    void prepareCommitInfo(std::size_t revision);
    void parseBzrLog(KDevelop::DVcsJob* job);

private:
    QDir _workingDir;
    QString _revisionSpec;
    KUrl _localLocation;
    KDevelop::IPlugin* _vcsPlugin;

    JobStatus _status;
    QPointer<KJob> _job;

    QStringList _outputLines;
    std::size_t _currentLine;
    std::unordered_map<std::size_t, KDevelop::VcsEvent> _commits;
    QVariantList _results;
};

#endif // BZRANNOTATETASK_H
