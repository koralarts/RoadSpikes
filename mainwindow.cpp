#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QListWidget>
#include <QStringList>
#include <QProcess>
#include <QMessageBox>
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
 * Helpers
 *
 **/
void MainWindow::setupTable()
{
	QStringList headers;
	headers.append("Chain");
	headers.append("Protocol");
	headers.append("Source");
	headers.append("Destination");
	headers.append("SPort");
	headers.append("DPort");
	headers.append("Target");

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

	//Wait for data before reading
	iptable->waitForReadyRead();
	result = iptable->readAllStandardOutput();

	//Check if iptable finished
	if(!iptable->waitForFinished()) {
		QMessageBox(QMessageBox::Critical, "Error", "iptables stalled",
					QMessageBox::Ok).show();
	}

	parseResult(result);
}

void MainWindow::parseResult(QByteArray res)
{
	QString result(res);
	QString chain;
	QStringList lines = result.split("\n", QString::SkipEmptyParts);
	int counter = 0;

	for(int i = 0; i < lines.size(); i++) {
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
	}
}

void MainWindow::buildRule(QString chain, QString line)
{
	QStringList rule;
	int rowCount;

	rule = line.split(" ", QString::SkipEmptyParts);
	rowCount = this->ui->rulesView->rowCount();

	this->ui->rulesView->insertRow(rowCount);
	this->ui->rulesView->setItem(rowCount, 0,
								 new QTableWidgetItem(chain));
	this->ui->rulesView->setItem(rowCount, this->ui->rulesView->columnCount() -1,
								 new QTableWidgetItem(rule.at(0)));
	this->ui->rulesView->setItem(rowCount, 1,
								 new QTableWidgetItem(rule.at(1)));
}

void MainWindow::on_addRuleButton_clicked()
{
	AddRule *rules = new AddRule;

	rules->show();

	if(rules->result()) {
		getIptable();
	}
}
