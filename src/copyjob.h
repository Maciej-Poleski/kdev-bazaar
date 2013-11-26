#ifndef COPYJOB_H
#define COPYJOB_H

#include <KUrl>

#include <vcs/vcsjob.h>

class CopyJob : public KDevelop::VcsJob
{
    Q_OBJECT

public:
    CopyJob(const KUrl& localLocationSrc, const KUrl& localLocationDstn, KDevelop::IPlugin* parent = 0, OutputJobVerbosity verbosity = OutputJob::Verbose);

    virtual KDevelop::IPlugin* vcsPlugin() const override;
    virtual KDevelop::VcsJob::JobStatus status() const override;
    virtual QVariant fetchResults() override;
    virtual void start() override;

protected:
    virtual bool doKill() override;

private:
    KDevelop::IPlugin* _plugin;
    KUrl _source;
    KUrl _destination;
};

#endif // COPYJOB_H
