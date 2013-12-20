#ifndef DIFFJOB_H
#define DIFFJOB_H

#include <QtCore/QVariant>

#include <kdevplatform/vcs/vcsjob.h>

namespace KDevelop
{
class DVcsJob;
}

class QDir;
class BazaarPlugin;

class DiffJob : public KDevelop::VcsJob
{
    Q_OBJECT

public:
    DiffJob(const QDir& workingDir, const QString& revisionSpecRange, const KUrl& fileOrDirectory, BazaarPlugin* parent = 0, OutputJobVerbosity verbosity = OutputJob::Silent);

    virtual KDevelop::IPlugin* vcsPlugin() const override;
    virtual KDevelop::VcsJob::JobStatus status() const override;
    virtual QVariant fetchResults() override;
    virtual void start() override;

protected:
    virtual bool doKill() override;

private slots:
    void prepareResult(KJob*);

private:
    BazaarPlugin* _plugin;
    QVariant _result;

    JobStatus _status;
    QPointer<KDevelop::DVcsJob> _job;
};

#endif // DIFFJOB_H
