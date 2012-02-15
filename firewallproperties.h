#ifndef FIREWALLPROPERTIES_H
#define FIREWALLPROPERTIES_H

#include <QDialog>

namespace Ui {
    class FirewallProperties;
}

class FirewallProperties : public QDialog
{
    Q_OBJECT

public:
    explicit FirewallProperties(QWidget *parent = 0);
    ~FirewallProperties();

private:
    Ui::FirewallProperties *ui;
};

#endif // FIREWALLPROPERTIES_H
