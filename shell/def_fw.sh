#VARIABLES
DEF_GATEWAY="192.168.1.3"
IN_INTERFACE="p3p1"
OUT_INTERFACE="em1"

#DEFAULT POLICIES
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT DROP

#BLOCK ALL EXTERNAL TRAFFIC TO PORTS 32768 - 32775 AND 137 - 139
iptables -A INPUT -p tcp -m multiport --dports 32768:32775,137:139 -j DROP
iptables -A INPUT -p udp -m multiport --dports 32768:32775,137:139 -j DROP

#BLOCK ALL EXTERNAL TCP TRAFFIC TO PORTS 111 AND 515
iptables -A INPUT -p tcp -m multiport --dports 111:515 -j DROP

#BLOCK ALL TELNET PACKETS
iptables -A INPUT -p tcp --dport 23 -j DROP
iptables -A OUTPUT -p tcp --sport 23 -j DROP

#DROP ALL TCP PACKETS WITH THE SYN AND FIN BIT SET
iptables -A INPUT -p tcp --tcp-flags SYN,FIN SYN,FIN-j DROP
iptables -A OUTPUT -p tcp --tcp-flags SYN,FIN SYN,FIN-j DROP

#ALLOW INBOUND/OUTBOUND tcp, udp, icmp FROM ALL PORTS
iptables -A INPUT -p tcp -j ACCEPT
iptables -A OUTPUT -p tcp -j ACCEPT
iptables -A INPUT -p udp -j ACCEPT
iptables -A OUTPUT -p udp -j ACCEPT
iptables -A INPUT -p icmp -j ACCEPT
iptables -A OUTPUT -p icmp -j ACCEPT


