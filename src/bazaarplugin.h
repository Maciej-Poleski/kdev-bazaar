#ifndef BAZAAR_PLUGIN_H
#define BAZAAR_PLUGIN_H

#include <QtCore/QVariantList>

#include <interfaces/iplugin.h>
#include <vcs/interfaces/idistributedversioncontrol.h>
#include <vcs/vcspluginhelper.h>

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

    virtual QString name() const override;

    virtual KDevelop::VcsJob* add(const KUrl::List& localLocations, RecursionMode recursion) override;
    virtual KDevelop::VcsJob* annotate(const KUrl& localLocation, const KDevelop::VcsRevision& rev) override;
    virtual KDevelop::VcsJob* commit(const QString& message, const KUrl::List& localLocations, RecursionMode recursion) override;
    virtual KDevelop::VcsJob* copy(const KUrl& localLocationSrc, const KUrl& localLocationDstn) override;
    virtual KDevelop::VcsImportMetadataWidget* createImportMetadataWidget(QWidget* parent) override;
    virtual KDevelop::VcsJob* createWorkingCopy(const KDevelop::VcsLocation& sourceRepository, const KUrl& destinationDirectory, RecursionMode recursion) override;
    virtual KDevelop::VcsJob* diff(const KUrl& fileOrDirectory, const KDevelop::VcsRevision& srcRevision, const KDevelop::VcsRevision& dstRevision, KDevelop::VcsDiff::Type, RecursionMode recursion) override;
    virtual KDevelop::VcsJob* init(const KUrl& localRepositoryRoot) override;
    virtual bool isVersionControlled(const KUrl& localLocation) override;
    virtual KDevelop::VcsJob* log(const KUrl& localLocation, const KDevelop::VcsRevision& rev, long unsigned int limit) override;
    virtual KDevelop::VcsJob* log(const KUrl& localLocation, const KDevelop::VcsRevision& rev, const KDevelop::VcsRevision& limit) override;
    virtual KDevelop::VcsJob* move(const KUrl& localLocationSrc, const KUrl& localLocationDst) override;
    virtual KDevelop::VcsJob* pull(const KDevelop::VcsLocation& localOrRepoLocationSrc, const KUrl& localRepositoryLocation) override;
    virtual KDevelop::VcsJob* push(const KUrl& localRepositoryLocation, const KDevelop::VcsLocation& localOrRepoLocationDst) override;
    virtual KDevelop::VcsJob* remove(const KUrl::List& localLocations) override;
    virtual KDevelop::VcsJob* repositoryLocation(const KUrl& localLocation) override;
    virtual KDevelop::VcsJob* resolve(const KUrl::List& localLocations, RecursionMode recursion) override;
    virtual KDevelop::VcsJob* revert(const KUrl::List& localLocations, RecursionMode recursion) override;
    virtual KDevelop::VcsJob* status(const KUrl::List& localLocations, RecursionMode recursion) override;
    virtual KDevelop::VcsJob* update(const KUrl::List& localLocations, const KDevelop::VcsRevision& rev, RecursionMode recursion) override;
    virtual KDevelop::VcsLocationWidget* vcsLocation(QWidget* parent) const override;
    virtual KDevelop::ContextMenuExtension contextMenuExtension(KDevelop::Context* context) override;

private slots:
    void parseBzrStatus(KDevelop::DVcsJob* job);
    void parseBzrLog(KDevelop::DVcsJob* job);
    void parseBzrRoot(KDevelop::DVcsJob* job);

private:
    KDevelop::VcsPluginHelper* _vcsPluginHelper;
};

#endif // BAZAAR_PLUGIN_H

