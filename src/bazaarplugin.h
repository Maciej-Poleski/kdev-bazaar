#ifndef BAZAAR_PLUGIN_H
#define BAZAAR_PLUGIN_H

#include <interfaces/iplugin.h>

#include <QtCore/QVariantList>

class BazaarPlugin : public KDevelop::IPlugin
{
    Q_OBJECT
    
public:
    explicit BazaarPlugin(QObject *parent, const QVariantList& args = QVariantList());
    virtual ~BazaarPlugin();
};

#endif // BAZAAR_PLUGIN_H

