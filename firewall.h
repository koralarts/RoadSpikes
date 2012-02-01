#ifndef FIREWALL_H
#define FIREWALL_H
#include <QString>

#define DEF_INTERFACE "eth1"

class Firewall
{
public:
	Firewall();
	Firewall(QString interface);

private:
	QString interface;
};

#endif // FIREWALL_H
