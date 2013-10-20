#include "bazaarplugin.h"

#include <KPluginFactory>
#include <KLocale>
#include <KAboutData>

using namespace KDevelop;

K_PLUGIN_FACTORY(KDevBazaarFactory, registerPlugin<BazaarPlugin>();)
K_EXPORT_PLUGIN(KDevBazaarFactory(
    KAboutData("kdevbazaar", "kdevbazaar", ki18n("Bazaar"),
               "0.1", ki18n("A plugin to support bazaar version control systems"), KAboutData::License_BSD)
))

BazaarPlugin::BazaarPlugin(QObject* parent, const QVariantList& args) : IPlugin(KDevBazaarFactory::componentData(), parent)
{
  Q_UNUSED(args);
}

BazaarPlugin::~BazaarPlugin()
{

}
