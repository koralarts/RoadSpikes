#include "firewallproperties.h"
#include "ui_firewallproperties.h"
#include <QDebug>
#include <QtNetwork/QNetworkInterface>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#define VAR_IN_NET      "%INTERNAL_NETWORK_ADDRESS%"
#define VAR_IN_INT      "%INTERNAL_INTERFACE%"
#define VAR_EX_INT      "%EXTERNAL_INTERFACE%"
#define VAR_DE_GAT      "%DEFAULT_GATEWAY%"
#define VAR_IN_MAC      "%INTERNAL_MACHINE%"
#define VAR_TC_SER      "%TCP_SERVICES%"
#define VAR_UD_SER      "%UDP_SERVICES%"
#define VAR_IC_SER      "%ICMP_SERVICES%"

FirewallProperties::FirewallProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirewallProperties)
{
    QList<QNetworkInterface> interface = QNetworkInterface::allInterfaces();
    QFile conf;
    QXmlStreamReader *xmlReader;

    ui->setupUi(this);

    foreach(QNetworkInterface iface, interface) {
        qDebug() << "Interface: " << iface.name();
        this->ui->internalInterface->addItem(iface.name());
        this->ui->externalInterface->addItem(iface.name());
    }

    if(conf.open("~/firewall.xml", QFile::ReadOnly)) {
        xmlReader = new QXmlStreamReader(conf.readAll());


    }
}

FirewallProperties::~FirewallProperties()
{
    delete ui;
}

void FirewallProperties::on_buttonBox_accepted()
{

}
