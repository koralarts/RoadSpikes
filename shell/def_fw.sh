#VARIABLES
DEF_GATEWAY="192.168.1.3"
IN_INTERFACE="p3p1"
OUT_INTERFACE="em1"

#DEFAULT POLICIES
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT DROP

#BLOCK ALL EXTERNAL TRAFFIC TO PORTS 32768 - 32775 AND 137 - 139
iptables -A INPUT -m multiport --sports 32768:32775,137:139 -j DROP

#BLOCK ALL EXTERNAL TCP TRAFFIC TO PORTS 111 AND 515
iptables -A INPUT -p tcp --dport 111 -j DROP
iptables -A INPUT -p tcp --dport 515 -j DROP

#ALLOW INBOUND/OUTBOUND tcp, udp, icmp FROM ALL PORTS
iptables -A INPUT -p tcp -j ACCEPT
iptables -A OUTPUT -p tcp -j ACCEPT
iptables -A INPUT -p udp -j ACCEPT
iptables -A OUTPUT -p udp -j ACCEPT
iptables -A INPUT -p icmp -j ACCEPT
iptables -A OUTPUT -p icmp -j ACCEPT


