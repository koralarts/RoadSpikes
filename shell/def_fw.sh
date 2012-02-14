#################################################################
################### USER CONFIGURABLE SECTION ###################
#################################################################

#USER-DEFINED VARIABLES
DEF_GATEWAY="192.168.1.3"
INTERNAL_NETWORK="192.168.1.0/255.255.255.0"
IN_INTERFACE="p3p1"
EX_INTERFACE="em1"
ALLOWED_SERVICES="www,ftp,ftp-data,ssh,1024:65535"

#################################################################
##################### IMPLEMENTATION SECTION ####################
#################################################################

#VARIABLES
TELNET_PORT="23"
SSH_PORT="22"
FTP_PORT="21"
FTP_DATA="20"

#DEFAULT POLICIES
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT DROP

#ALLOW FORWARDING IN IN_INTERFACE
iptables --table nat --append POSTROUTING --out-interface em1 -j MASQUERADE
iptables -t nat -A PREROUTING -p tcp -i em1 -m multiport --dports $ALLOWED_SERVICES -j DNAT --to 192.168.1.2

#FORWARDING
iptables --append FORWARD --in-interface $IN_INTERFACE -j ACCEPT

#BLOCK ALL EXTERNAL TRAFFIC TO PORTS 32768 - 32775 AND 137 - 139
iptables -A FORWARD -i $EX_INTERFACE -p tcp -m multiport --dports 32768:32775,137:139 -j DROP
iptables -A FORWARD -i $EX_INTERFACE -p udp -m multiport --dports 32768:32775,137:139 -j DROP

#BLOCK ALL EXTERNAL TCP TRAFFIC TO PORTS 111 AND 515
iptables -A FORWARD -i $EX_INTERFACE -p tcp -m multiport --dports 111,515 -j DROP

#BLOCK ALL TELNET PACKETS
iptables -A FORWARD -p tcp --sport $TELNET_PORT -j DROP

#DROP ALL TCP PACKETS WITH THE SYN AND FIN BIT SET
iptables -A FORWARD -p tcp --tcp-flags ALL FIN,SYN -j DROP

#ACCEPT FRAGMENTS
iptables -A FORWARD -f -j ACCEPT

#SET SSH AND FTP CONTROL CONNECTIONS TO MINIMUM DELAY
iptables -t mangle -A PREROUTING -p tcp --dport $SSH_PORT -j TOS --set-tos Minimize-Delay
iptables -t mangle -A PREROUTING -p tcp --dport $FTP_PORT -j TOS --set-tos Minimize-Delay

#SET FTP DATA TO MAXIMUM THROUGHPUT
iptables -t mangle -A PREROUTING -p tcp --dport $FTP_DATA -j TOS --set-tos Maximize-Throughput

#ALLOW INBOUND/OUTBOUND tcp, udp, icmp FROM ALL PORTS
#ACCEPT ALL TCP PACKETS THAT BELONG TO AN EXISTING CONNECTION
#ONLY ALLOW NEW AND ESTABLISHED TRAFFIC
#REJECT PACKETS GOING THE WRONG WAY
iptables -A FORWARD -p tcp -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p udp -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p icmp -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT -p tcp -m state --state NEW -j DROP

#PORT FORWARDING
iptables -A FORWARD -i $EX_INTERFACE -p tcp --m multiport --dports $ALLOWED_SERVICES -j ACCEPT

#DROP ALL PACKETS WITH A SOURCE ADDRESS FROM OUTSIDE THE INTERNAL NETWORK
iptables -A INPUT -i $EX_INTERFACE -p all -s $INTERNAL_NETWORK -j DROP
