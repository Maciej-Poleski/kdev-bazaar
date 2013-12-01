#include "copyjob.h"

#include <QtCore/QVariant>

#include <interfaces/iplugin.h>
#include <vcs/dvcs/dvcsjob.h>
#include <KIO/CopyJob>

#include "bazaarplugin.h"

CopyJob::CopyJob(const KUrl& localLocationSrc, const KUrl& localLocationDstn, BazaarPlugin* parent, KDevelop::OutputJob::OutputJobVerbosity verbosity)
    : VcsJob(parent, verbosity), _plugin(parent), _source(localLocationSrc),
      _destination(localLocationDstn), _status(KDevelop::VcsJob::JobNotStarted)
{
    setType(JobType::Copy);
    setCapabilities(Killable);
}


KDevelop::IPlugin* CopyJob::vcsPlugin() const
{
    return _plugin;
}

KDevelop::VcsJob::JobStatus CopyJob::status() const
{
    return _status;
}

QVariant CopyJob::fetchResults()
{
    return QVariant();
}

void CopyJob::start()
{
    if (_status != KDevelop::VcsJob::JobNotStarted)
        return;
    KIO::CopyJob* job = KIO::copy(_source, _destination, KIO::HideProgressInfo);
    connect(job, SIGNAL(copyingDone(KIO::Job*, KUrl, KUrl, time_t, bool, bool)), this, SLOT(addToVcs(KIO::Job*, KUrl, KUrl, time_t, bool, bool)));
    _status = KDevelop::VcsJob::JobRunning;
    _job = job;
    job->start();
}

bool CopyJob::doKill()
{
    _status = KDevelop::VcsJob::JobCanceled;
    if (_job)
        return _job->kill(KJob::Quietly);
    else
        return true;
}

void CopyJob::addToVcs(KIO::Job* job, const KUrl& from, const KUrl& to, time_t mtime, bool directory, bool renamed)
{
    (void)job;
    (void)from;
    (void)mtime;
    (void)directory;
    (void)renamed;
    if (_status != KDevelop::VcsJob::JobRunning)
        return;
    KDevelop::VcsJob* job2 = _plugin->add(to, KDevelop::IBasicVersionControl::Recursive);
    connect(job2, SIGNAL(result(KJob*)), this, SLOT(finish(KJob*)));
    _job = job2;
    job2->start();
}

void CopyJob::finish(KJob*)
{
    _status = KDevelop::VcsJob::JobSucceeded;
    emitResult();
    emit resultsReady(this);
    return;
}


#include "copyjob.moc"
