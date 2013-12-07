#include "diffjob.h"

#include <QtCore/QDir>

#include <vcs/dvcs/dvcsjob.h>
#include <vcs/vcsdiff.h>

#include "bazaarplugin.h"

DiffJob::DiffJob(const QDir& workingDir, const QString& revisionSpecRange,
                 const KUrl& fileOrDirectory, BazaarPlugin* parent,
                 KDevelop::OutputJob::OutputJobVerbosity verbosity)
    : VcsJob(parent, verbosity), _plugin(parent),
      _status(KDevelop::VcsJob::JobNotStarted)
{
    setType(KDevelop::VcsJob::Diff);
    setCapabilities(Killable);
    _job = new KDevelop::DVcsJob(workingDir, parent, verbosity);
    _job->setType(VcsJob::Diff);
    *_job << "bzr" << "diff" << "-p1" << revisionSpecRange << fileOrDirectory;
}

KDevelop::IPlugin* DiffJob::vcsPlugin() const
{
    return _plugin;
}

KDevelop::VcsJob::JobStatus DiffJob::status() const
{
    return _status;
}

QVariant DiffJob::fetchResults()
{
    return _result;
}

void DiffJob::start()
{
    if (_status != KDevelop::VcsJob::JobNotStarted)
        return;
    if (_job) {
        connect(_job.data(), SIGNAL(finished(KJob*)), this, SLOT(prepareResult(KJob*)));
        _status = KDevelop::VcsJob::JobRunning;
        _job->start();
    }
}

bool DiffJob::doKill()
{
    _status = KDevelop::VcsJob::JobCanceled;
    if (_job)
        return _job->kill(KJob::Quietly);
    else
        return true;
}

void DiffJob::prepareResult(KJob*)
{
    if (_job->process()->exitStatus() <= 2) {
        KDevelop::VcsDiff diff;
        diff.setDiff(_job->output());
        diff.setBaseDiff(KUrl(_job->directory().absolutePath()));
        _result.setValue(diff);
        _status = KDevelop::VcsJob::JobSucceeded;
    } else {
        setError(_job->process()->exitStatus());
        _status = KDevelop::VcsJob::JobFailed;
    }
    emitResult();
    emit resultsReady(this);
}


#include "diffjob.moc"
