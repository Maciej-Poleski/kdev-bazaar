#ifndef BAZAAR_PLUGIN_H
#define BAZAAR_PLUGIN_H

#include <interfaces/iplugin.h>

#include <QtCore/QVariantList>

class bazaarPlugin : public KDevelop::IPlugin
{
    Q_OBJECT
    
public:
    explicit bazaarPlugin(QObject *parent, const QVariantList& args = QVariantList());
    virtual ~bazaarPlugin();
};

#endif // BAZAAR_PLUGIN_H

