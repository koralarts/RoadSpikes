#VARIABLES
DEF_GATEWAY="192.168.1.3"
IN_INTERFACE="p3p1"
OUT_INTERFACE="em1"

#DEFAULT POLICIES
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT DROP

#ALLOW INBOUND/OUTBOUND tcp, udp, icmp FROM ALL PORTS
iptables -A INPUT -p tcp -j ACCEPT
iptables -A OUTPUT -p tcp -j ACCEPT
iptables -A INPUT -p udp -j ACCEPT
iptables -A OUTPUT -p udp -j ACCEPT
iptables -A INPUT -p icmp -j ACCEPT
iptables -A OUTPUT -p icmp -j ACCEPT


