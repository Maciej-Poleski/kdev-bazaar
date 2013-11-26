#include "copyjob.h"

#include <QtCore/QVariant>

#include <interfaces/iplugin.h>

CopyJob::CopyJob(const KUrl& localLocationSrc, const KUrl& localLocationDstn,
                 KDevelop::IPlugin* parent,
                 KDevelop::OutputJob::OutputJobVerbosity verbosity)
    : VcsJob(parent, verbosity), _plugin(parent), _source(localLocationSrc),
      _destination(localLocationDstn)
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
}

QVariant CopyJob::fetchResults()
{
    return QVariant();
}

void CopyJob::start()
{
}

bool CopyJob::doKill()
{
}

#include "copyjob.moc"
