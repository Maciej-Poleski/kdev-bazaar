#ifndef BAZAAR_PLUGIN_H
#define BAZAAR_PLUGIN_H

#include <interfaces/iplugin.h>
#include <vcs/interfaces/idistributedversioncontrol.h>
#include <vcs/vcspluginhelper.h>

#include <QtCore/QVariantList>

namespace KDevelop
{
class DVcsJob;
}

class BazaarPlugin : public KDevelop::IPlugin, public KDevelop::IDistributedVersionControl
{
    Q_OBJECT
    Q_INTERFACES(KDevelop::IBasicVersionControl KDevelop::IDistributedVersionControl)
public:
    explicit BazaarPlugin(QObject* parent, const QVariantList& args = QVariantList());
    virtual ~BazaarPlugin();

    virtual QString name() const;

    virtual KDevelop::VcsJob* add(const KUrl::List& localLocations, RecursionMode recursion);
    virtual KDevelop::VcsJob* annotate(const KUrl& localLocation, const KDevelop::VcsRevision& rev);
    virtual KDevelop::VcsJob* commit(const QString& message, const KUrl::List& localLocations, RecursionMode recursion);
    virtual KDevelop::VcsJob* copy(const KUrl& localLocationSrc, const KUrl& localLocationDstn);
    virtual KDevelop::VcsImportMetadataWidget* createImportMetadataWidget(QWidget* parent);
    virtual KDevelop::VcsJob* createWorkingCopy(const KDevelop::VcsLocation& sourceRepository, const KUrl& destinationDirectory, RecursionMode recursion);
    virtual KDevelop::VcsJob* diff(const KUrl& fileOrDirectory, const KDevelop::VcsRevision& srcRevision, const KDevelop::VcsRevision& dstRevision, KDevelop::VcsDiff::Type, RecursionMode recursion);
    virtual KDevelop::VcsJob* init(const KUrl& localRepositoryRoot);
    virtual bool isVersionControlled(const KUrl& localLocation);
    virtual KDevelop::VcsJob* log(const KUrl& localLocation, const KDevelop::VcsRevision& rev, long unsigned int limit);
    virtual KDevelop::VcsJob* log(const KUrl& localLocation, const KDevelop::VcsRevision& rev, const KDevelop::VcsRevision& limit);
    virtual KDevelop::VcsJob* move(const KUrl& localLocationSrc, const KUrl& localLocationDst);
    virtual KDevelop::VcsJob* pull(const KDevelop::VcsLocation& localOrRepoLocationSrc, const KUrl& localRepositoryLocation);
    virtual KDevelop::VcsJob* push(const KUrl& localRepositoryLocation, const KDevelop::VcsLocation& localOrRepoLocationDst);
    virtual KDevelop::VcsJob* remove(const KUrl::List& localLocations);
    virtual KDevelop::VcsJob* repositoryLocation(const KUrl& localLocation);
    virtual KDevelop::VcsJob* resolve(const KUrl::List& localLocations, RecursionMode recursion);
    virtual KDevelop::VcsJob* revert(const KUrl::List& localLocations, RecursionMode recursion);
    virtual KDevelop::VcsJob* status(const KUrl::List& localLocations, RecursionMode recursion);
    virtual KDevelop::VcsJob* update(const KUrl::List& localLocations, const KDevelop::VcsRevision& rev, RecursionMode recursion);
    virtual KDevelop::VcsLocationWidget* vcsLocation(QWidget* parent) const;
    virtual KDevelop::ContextMenuExtension contextMenuExtension(KDevelop::Context* context);
    
private slots:
    void parseBzrStatus(KDevelop::DVcsJob*);

private:
    KDevelop::VcsPluginHelper* _vcsPluginHelper;
};

#endif // BAZAAR_PLUGIN_H

