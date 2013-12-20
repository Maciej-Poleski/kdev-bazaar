#ifndef COPYJOB_H
#define COPYJOB_H

#include <KUrl>

#include <kdevplatform/vcs/vcsjob.h>

namespace KIO
{
class Job;
}

class BazaarPlugin;

class CopyJob : public KDevelop::VcsJob
{
    Q_OBJECT

public:
    CopyJob(const KUrl& localLocationSrc, const KUrl& localLocationDstn, BazaarPlugin* parent = 0, OutputJobVerbosity verbosity = OutputJob::Verbose);

    virtual KDevelop::IPlugin* vcsPlugin() const override;
    virtual KDevelop::VcsJob::JobStatus status() const override;
    virtual QVariant fetchResults() override;
    virtual void start() override;

protected:
    virtual bool doKill() override;

private slots:
    void finish(KJob*);
    void addToVcs(KIO::Job* job, const KUrl& from, const KUrl& to, time_t mtime, bool directory, bool renamed);

private:
    BazaarPlugin* _plugin;
    KUrl _source;
    KUrl _destination;

    JobStatus _status;
    QPointer<KJob> _job;
};

#endif // COPYJOB_H
