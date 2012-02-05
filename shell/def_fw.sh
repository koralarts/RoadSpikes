#VARIABLES
DEF_GATEWAY="192.168.1.3"
INTERNAL_NETWORK="192.168.1.0/255.255.255.0"
IN_INTERFACE="p3p1"
OUT_INTERFACE="em1"
TELNET_PORT="23"
SSH_PORT="22"
FTP_PORT="21"
FTP_DATA="20"

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
iptables -A INPUT -p tcp --dport $TELNET_PORT -j DROP
iptables -A OUTPUT -p tcp --sport $TELNET_PORT -j DROP

#DROP ALL TCP PACKETS WITH THE SYN AND FIN BIT SET
iptables -A INPUT -p tcp --tcp-flags SYN,FIN SYN,FIN -j DROP
iptables -A OUTPUT -p tcp --tcp-flags SYN,FIN SYN,FIN -j DROP

#DROP ALL PACKETS WITH A SOURCE ADDRESS FROM OUTSIDE THE INTERNAL NETWORK
iptables -A INPUT -p all ! -s $INTERNAL_NETWORK -j DROP
iptables -A OUTPUT -p all ! -s $INTERNAL_NETWORK -j DROP

#ACCEPT FRAGMENTS


#SET SSH AND FTP CONTROL CONNECTIONS TO MINIMUM DELAY
iptables -t mangle -A PREROUTING -p tcp --dport $SSH_PORT -j TOS --set-tos Minimize-Delay
iptables -t mangle -A PREROUTING -p tcp --dport $FTP_PORT -j TOS --set-tos Minimize-Delay

#SET FTP DATA TO MAXIMUM THROUGHPUT
iptables -t mangle -A PREROUTING -p tcp --dport $FTP_DATA -j TOS --set-tos Maximize-Throughput

#ALLOW INBOUND/OUTBOUND tcp, udp, icmp FROM ALL PORTS
#ACCEPT ALL TCP PACKETS THAT BELONG TO AN EXISTING CONNECTION
#ONLY ALLOW NEW AND ESTABLISHED TRAFFIC
#REJECT PACKETS GOING THE WRONG WAY
iptables -A INPUT -p tcp -m state --state ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT -p udp -m state --state ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p udp -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT -p icmp -m state --state ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p icmp -m state --state NEW,ESTABLISHED -j ACCEPT
