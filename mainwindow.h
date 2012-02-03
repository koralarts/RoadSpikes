#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
	void on_actionExit_triggered();

	void on_addRuleButton_clicked();

    void on_deleteRuleButton_clicked();

    void on_flushChainButton_clicked();

    void on_deleteChainButton_clicked();

private:
	Ui::MainWindow *ui; /** Form containing the main window GUI */

	void setupTable();
	void getIptable();
	void parseResult(QByteArray result);
	void buildRule(QString chain, QString line);
    void resetTable();
    void deleteRule(int row);
    void deleteFlushChain(QString chain, QString flushDelSwitch);
};

#endif // MAINWINDOW_H
