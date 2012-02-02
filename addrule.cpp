#include "addrule.h"
#include "ui_addrule.h"
#include <QProcess>
#include <QDebug>

AddRule::AddRule(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addRule)
{
    ui->setupUi(this);
}

AddRule::~AddRule()
{
    delete ui;
}

void AddRule::on_okCancel_accepted()
{
    QStringList switches;
    QStringList customs;
    QProcess iptable;

    if(this->ui->chainText->text().size() != 0) {
        switches.append("-A");
        switches.append(this->ui->chainText->text());
    }
    if(this->ui->interfaceText->text().size() != 0) {
        switches.append("-i");
        switches.append(this->ui->interfaceText->text());
    }
    if(this->ui->protText->text().size() != 0) {
        switches.append("-p");
        switches.append(this->ui->protText->text());
    }
    if(this->ui->sourceIPText->text().size() != 0) {
        switches.append("-s");
        switches.append(this->ui->sourceIPText->text());
    }
    if(this->ui->destIpText->text().size() != 0) {
        switches.append("-d");
        switches.append(this->ui->destIpText->text());
    }
    if(this->ui->sportText->text().size() != 0) {
        switches.append("--sport");
        switches.append(this->ui->sportText->text());
    }
    if(this->ui->dportText->text().size() != 0) {
        switches.append("--dport");
        switches.append(this->ui->dportText->text());
    }

    customs = this->ui->customText->toPlainText().split("\n", QString::SkipEmptyParts);

    for(int i = 0; i < customs.size(); i++) {
        switches.append(customs.at(i).split(" ", QString::SkipEmptyParts));
    }

    if(this->ui->targetText->text().size() != 0) {
        switches.append("-j");
        switches.append(this->ui->targetText->text());
    }

    iptable.startDetached("iptables", switches);
}
