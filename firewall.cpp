#include "firewall.h"

Firewall::Firewall()
{
    this->interface = DEF_INTERFACE;
}

Firewall::Firewall(QString interface)
{
    this->interface = interface;
}
