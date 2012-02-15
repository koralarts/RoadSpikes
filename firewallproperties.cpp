#include "firewallproperties.h"
#include "ui_firewallproperties.h"
#include <QDebug>
#include <QtNetwork/QNetworkInterface>
#include <QFile>
#include <QFileDialog>

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

    ui->setupUi(this);

    foreach(QNetworkInterface iface, interface) {
        qDebug() << "Interface: " << iface.name();
        this->ui->internalInterface->addItem(iface.name());
        this->ui->externalInterface->addItem(iface.name());
    }
}

FirewallProperties::~FirewallProperties()
{
    delete ui;
}

void FirewallProperties::on_buttonBox_accepted()
{
    QFile fw(QDir::homePath() + "/firewall.rs");
    QString fwName = QFileDialog::getSaveFileName(0, "Save Firewall Properties", "default_firewall.sh");
    QString allTheThings;
    QFile deffw(fwName);

    if(!fw.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open firewall.rs";
        return;
    }

    allTheThings = fw.readAll();

    allTheThings = allTheThings.replace(VAR_IN_NET, this->ui->internalAddress->text());
    allTheThings = allTheThings.replace(VAR_IN_INT, this->ui->internalInterface->currentText());
    allTheThings = allTheThings.replace(VAR_EX_INT, this->ui->externalInterface->currentText());
    allTheThings = allTheThings.replace(VAR_DE_GAT, this->ui->defaultGateway->text());
    allTheThings = allTheThings.replace(VAR_IN_MAC, this->ui->internalMachine->text());
    allTheThings = allTheThings.replace(VAR_TC_SER, this->ui->tcp->text());
    allTheThings = allTheThings.replace(VAR_UD_SER, this->ui->udp->text());
    allTheThings = allTheThings.replace(VAR_IC_SER, this->ui->icmp->text());

    if(!deffw.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open " << fwName;
        return;
    }

    deffw.write(allTheThings.toAscii());

    fw.close();
    deffw.close();
}
