#ifndef ADDRULE_H
#define ADDRULE_H

#include <QDialog>

namespace Ui {
	class addRule;
}

class AddRule : public QDialog
{
    Q_OBJECT

public:
	explicit AddRule(QWidget *parent = 0);
	~AddRule();

private slots:
	void on_okCancel_accepted();

private:
	Ui::addRule *ui;
};

#endif // ADDRULE_H
