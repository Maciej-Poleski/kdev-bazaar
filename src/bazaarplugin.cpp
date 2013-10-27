#include "bazaarplugin.h"

#include <QtCore/QDir>

#include <KPluginFactory>
#include <KLocale>
#include <KAboutData>
#include <vcs/widgets/standardvcslocationwidget.h>
#include <vcs/dvcs/dvcsjob.h>

#include "importmetadatawidget.h"

using namespace KDevelop;

K_PLUGIN_FACTORY(KDevBazaarFactory, registerPlugin<BazaarPlugin>();)
K_EXPORT_PLUGIN(KDevBazaarFactory(
                    KAboutData("kdevbazaar", "kdevbazaar", ki18n("Bazaar"),
                               "0.1", ki18n("A plugin to support bazaar version control systems"), KAboutData::License_BSD)
                ))

static QDir toQDir(const KUrl& url)
{
    return QDir(url.toLocalFile());
}

static QDir workingCopy(const KUrl& path)
{
    QDir dir = toQDir(path);
    while (!dir.exists(".bzr") && dir.cdUp());

    return dir;
}

BazaarPlugin::BazaarPlugin(QObject* parent, const QVariantList& args) : IPlugin(KDevBazaarFactory::componentData(), parent)
{
    // TODO: check if there is bzr executable

    KDEV_USE_EXTENSION_INTERFACE( KDevelop::IBasicVersionControl )
    KDEV_USE_EXTENSION_INTERFACE( KDevelop::IDistributedVersionControl )

    setObjectName("Bazaar");
}

BazaarPlugin::~BazaarPlugin()
{
}

QString BazaarPlugin::name() const
{
    return QString::fromUtf8("Bazaar");
}

VcsJob* BazaarPlugin::add(const KUrl::List& localLocations, IBasicVersionControl::RecursionMode recursion)
{
    DVcsJob* job = new DVcsJob(workingCopy(localLocations[0]), this);
    job->setType(VcsJob::Add);
    *job << "bzr" << (recursion==Recursive? "add": "add --no-recurse")<< localLocations;
    return job;
}

VcsJob* BazaarPlugin::annotate(const KUrl& localLocation, const VcsRevision& rev)
{

}

VcsJob* BazaarPlugin::commit(const QString& message, const KUrl::List& localLocations, IBasicVersionControl::RecursionMode recursion)
{
    if(recursion==Recursive)
    {
        QDir dir = workingCopy(localLocations[0]);
        DVcsJob* job = new DVcsJob(dir, this);
        job->setType(VcsJob::Commit);

        *job << "bzr" << "commit"<<localLocations << "-m" << message;
        return job;
    }
    else
    {
        // TODO: Consider this situation
    }
}

VcsJob* BazaarPlugin::copy(const KUrl& localLocationSrc, const KUrl& localLocationDstn)
{

}

VcsImportMetadataWidget* BazaarPlugin::createImportMetadataWidget(QWidget* parent)
{
    return new ImportMetadataWidget(parent);
}

VcsJob* BazaarPlugin::createWorkingCopy(const VcsLocation& sourceRepository, const KUrl& destinationDirectory, IBasicVersionControl::RecursionMode recursion)
{

}

VcsJob* BazaarPlugin::diff(const KUrl& fileOrDirectory, const VcsRevision& srcRevision, const VcsRevision& dstRevision, VcsDiff::Type, IBasicVersionControl::RecursionMode recursion)
{

}

VcsJob* BazaarPlugin::init(const KUrl& localRepositoryRoot)
{
    DVcsJob* job = new DVcsJob(toQDir(localRepositoryRoot), this);
    job->setType(VcsJob::Import);
    *job << "bzr" << "init";
    return job;
}

bool BazaarPlugin::isVersionControlled(const KUrl& localLocation)
{

}

VcsJob* BazaarPlugin::log(const KUrl& localLocation, const VcsRevision& rev, long unsigned int limit)
{

}

VcsJob* BazaarPlugin::log(const KUrl& localLocation, const VcsRevision& rev, const VcsRevision& limit)
{

}

VcsJob* BazaarPlugin::move(const KUrl& localLocationSrc, const KUrl& localLocationDst)
{

}

VcsJob* BazaarPlugin::pull(const VcsLocation& localOrRepoLocationSrc, const KUrl& localRepositoryLocation)
{

}

VcsJob* BazaarPlugin::push(const KUrl& localRepositoryLocation, const VcsLocation& localOrRepoLocationDst)
{

}

VcsJob* BazaarPlugin::remove(const KUrl::List& localLocations)
{

}

VcsJob* BazaarPlugin::repositoryLocation(const KUrl& localLocation)
{

}

VcsJob* BazaarPlugin::resolve(const KUrl::List& localLocations, IBasicVersionControl::RecursionMode recursion)
{

}

VcsJob* BazaarPlugin::revert(const KUrl::List& localLocations, IBasicVersionControl::RecursionMode recursion)
{

}

VcsJob* BazaarPlugin::status(const KUrl::List& localLocations, IBasicVersionControl::RecursionMode recursion)
{

}

VcsJob* BazaarPlugin::update(const KUrl::List& localLocations, const VcsRevision& rev, IBasicVersionControl::RecursionMode recursion)
{

}

VcsLocationWidget* BazaarPlugin::vcsLocation(QWidget* parent) const
{
    return new KDevelop::StandardVcsLocationWidget(parent);
}

