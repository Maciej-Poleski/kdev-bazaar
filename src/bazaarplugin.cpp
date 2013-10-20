#include "bazaarplugin.h"

#include <KPluginFactory>
#include <KLocale>
#include <KAboutData>

using namespace KDevelop;

K_PLUGIN_FACTORY(KDevbazaarFactory, registerPlugin<bazaarPlugin>();)
K_EXPORT_PLUGIN(KDevbazaarFactory(
    KAboutData("kdevbazaar", "kdevbazaar", ki18n("bazaar"),
               "0.1", ki18n("An example plugin for KDevelop"), KAboutData::License_GPL)
))

bazaarPlugin::bazaarPlugin(QObject* parent, const QVariantList& args) : IPlugin(KDevbazaarFactory::componentData(), parent)
{
  Q_UNUSED(args);
}

bazaarPlugin::~bazaarPlugin()
{

}
