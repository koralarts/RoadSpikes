#ALLOW IP FORWARDING
echo 1 > /proc/sys/net/ipv4/ip_forward

#ROUTE TABLE
#route add default gw 192.168.1.3 p3p1
#route add -net 192.168.1.0 netmask 255.255.255.0 dev p3p1


# IPTABLES
iptables -F
iptables -X

iptables --table nat --append POSTROUTING --out-interface em1 -j MASQUERADE
iptables --append FORWARD --in-interface p3p1 -j ACCEPT
