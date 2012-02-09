#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QListWidget>
#include <QStringList>
#include <QProcess>
#include <QMessageBox>
#include <QProgressDialog>
#include "addrule.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupTable();
    getIptable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * FILE - MenuItem
 **/
void MainWindow::on_actionExit_triggered()
{
    this->close();
}

/**
 * HELPERS
 *
 **/
void MainWindow::setupTable()
{
    QStringList headers;
    headers.append("Chain");
    headers.append("Option");
    headers.append("Protocol");
    headers.append("Source");
    headers.append("Destination");
    headers.append("SPort");
    headers.append("DPort");
    headers.append("Target");
    headers.append("Miscellaneous");

    for(int i = 0; i < headers.size(); i++) {
        this->ui->rulesView->insertColumn(i);
        this->ui->rulesView->setHorizontalHeaderItem(i,
                                                     new QTableWidgetItem(headers.at(i)));
    }
}

void MainWindow::getIptable()
{
    QProcess *iptable = new QProcess();
    QByteArray result;
    //iptable->start("ls");
    iptable->start("iptables", QStringList() << "-L");

    //Check if iptable has started
    if(!iptable->waitForStarted()) {
        QMessageBox(QMessageBox::Critical, "Error", "Cannot Run iptables",
                    QMessageBox::Ok).show();
    }

    //Check if iptable finished
    if(!iptable->waitForFinished()) {
        QMessageBox(QMessageBox::Critical, "Error", "iptables stalled",
                    QMessageBox::Ok).show();
    }
    //Wait for data before reading
    iptable->waitForReadyRead();
    result = iptable->readAllStandardOutput();

    parseResult(result);
}

void MainWindow::parseResult(QByteArray res)
{
    QString result(res);
    QString chain;
    QStringList lines = result.split("\n", QString::SkipEmptyParts);
    QProgressDialog *progress = new QProgressDialog("Populating Table...", "Cancel", 0, lines.size() - 1, this);
    int counter = 0;

    progress->setWindowModality(Qt::WindowModal);
    progress->setAutoClose(true);

    for(int i = 0; i < lines.size(); i++) {
        progress->setValue(i);
        //Check if name of chain
        if(lines.at(i).contains("Chain")) {
            chain = lines.at(i).split(" ").at(1);
            counter = 0;
        }
        counter++;
        //Check if rule
        if(counter > 2) {
            buildRule(chain, lines.at(i));
        }
        if(progress->wasCanceled()) {
            progress->setValue(lines.size() - 1);
            QMessageBox(QMessageBox::Critical, "Exiting", "Population Cancelled",
                        QMessageBox::Ok).exec();
            this->close();
        }
    }

}

void MainWindow::buildRule(QString chain, QString line)
{
    QStringList rule;
    QString miscellaneous;
    int rowCount;
    int portIndex;

    rule = line.split(" ", QString::SkipEmptyParts);
    rowCount = this->ui->rulesView->rowCount();

    this->ui->rulesView->insertRow(rowCount);

    this->ui->rulesView->setItem(rowCount, 0,
                                 new QTableWidgetItem(chain));
    this->ui->rulesView->setItem(rowCount, 1,
                                 new QTableWidgetItem(rule.at(2)));
    this->ui->rulesView->setItem(rowCount, this->ui->rulesView->columnCount() - 2,
                                 new QTableWidgetItem(rule.at(0)));
    this->ui->rulesView->setItem(rowCount, 2,
                                 new QTableWidgetItem(rule.at(1)));
    this->ui->rulesView->setItem(rowCount, 3,
                                 new QTableWidgetItem(rule.at(3)));
    this->ui->rulesView->setItem(rowCount, 4,
                                 new QTableWidgetItem(rule.at(4)));

    if((portIndex = rule.indexOf(QRegExp("^spt[s]?:.*"))) != -1) {
        if(rule.at(portIndex).contains("spts")) {
            this->ui->rulesView->setItem(rowCount, 5,
                                         new QTableWidgetItem(rule.at(portIndex).split("pts:").at(1)));
        } else {
            this->ui->rulesView->setItem(rowCount, 5,
                                         new QTableWidgetItem(rule.at(portIndex).split(":").at(1)));
        }
    }
    if((portIndex = rule.indexOf(QRegExp("^dpt[s]?:.*"))) != -1) {
        if(rule.at(portIndex).contains("dpts")) {
            this->ui->rulesView->setItem(rowCount, 6,
                                         new QTableWidgetItem(rule.at(portIndex).split("pts:").at(1)));
        } else {
            this->ui->rulesView->setItem(rowCount, 6,
                                         new QTableWidgetItem(rule.at(portIndex).split(":").at(1)));
        }
    }

}

void MainWindow::resetTable()
{
    int curRowCount = this->ui->rulesView->rowCount();
    this->ui->rulesView->clear();
    for(int i = curRowCount - 1; i > -1; i--) {
        this->ui->rulesView->removeRow(i);
    }
}

void MainWindow::deleteRule(int row)
{
    QProcess *iptable = new QProcess();
    QString curChain = this->ui->rulesView->item(row, 0)->text();
    int counter = 1;

    while(row > 0 && this->ui->rulesView->item(row, 0)->text() == curChain) {
        counter++;
        row--;
    }

    iptable->startDetached("iptables", QStringList() << "-D" << curChain << QString::number(counter));
}

void MainWindow::deleteFlushChain(QString chain, QString flushDelSwitch)
{
    QProcess *iptable = new QProcess();

    iptable->startDetached("iptables", QStringList() << flushDelSwitch << chain );
}

/**
 * BUTTON SLOTS
 */
void MainWindow::on_addRuleButton_clicked()
{
    AddRule *rules = new AddRule;

    if(rules->exec()) {
        resetTable();
        getIptable();
    }
}

void MainWindow::on_deleteRuleButton_clicked()
{
    QModelIndexList selected = this->ui->rulesView->selectionModel()->selectedRows();

    if(selected.size() > 0) {
        deleteRule(selected.at(0).row());
        this->ui->rulesView->removeRow(selected.at(0).row());
    }
}

void MainWindow::on_flushChainButton_clicked()
{
    QModelIndexList selected = this->ui->rulesView->selectionModel()->selectedRows();
    int row;
    QString chain;

    if(selected.size() > 0) {
        row = selected.at(0).row();
        chain = this->ui->rulesView->item(row, 0)->text();
        deleteFlushChain(chain, "--flush");
        resetTable();
        getIptable();
    }
}

void MainWindow::on_deleteChainButton_clicked()
{
    QModelIndexList selected = this->ui->rulesView->selectionModel()->selectedRows();
    int row;
    QString chain;

    if(selected.size() > 0) {
        row = selected.at(0).row();
        chain = this->ui->rulesView->item(row, 0)->text();
        if(QRegExp("INPUT|OUTPUT|FORWARD").exactMatch(chain)) {
            QMessageBox(QMessageBox::Critical, "Error", "You cannot delete that chain.",
                        QMessageBox::Ok).exec();
        } else {
            deleteFlushChain(chain, "--delete-chain");
            resetTable();
            getIptable();
        }
    }
}
