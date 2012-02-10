#route delete default gw 192.168.1.3 p3p1
#route delete -net 192.168.1.0 netmask 255.255.255.0 dev p3p1

iptables -F -t nat
iptables -F -t mangle

iptables -F
iptables -X
iptables -P INPUT ACCEPT
iptables -P OUTPUT ACCEPT
iptables -P FORWARD ACCEPT

echo "IPTables Reset"
