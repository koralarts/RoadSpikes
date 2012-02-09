##############################################
############### Testing Script ###############
##############################################

echo "------------------------------------------------------------"
echo "------ BLOCK TCP Packets 32768 - 32775 AND 137 - 139 -------"
echo "------------------------------------------------------------"

for (( i = 137; i <= 139; i++ ))
do
	echo "---------"
	echo "Port: $i"
	echo "---------"
	hping3 -p $i -c 10 192.168.0.3
done

for (( i = 32768; i <= 32775; i++ ))
do
	echo "---------"
	echo "Port: $i"
	echo "---------"
	hping3 -p $i -c 10 192.168.0.3
done

echo "------------------------------------------------------------"
echo "------ BLOCK UDP Packets 32768 - 32775 AND 137 - 139 -------"
echo "------------------------------------------------------------"

for (( i = 137; i <= 139; i++ ))
do
	echo "---------"
	echo "Port: $i"
	echo "---------"
	hping3 -2 -p $i -c 10 192.168.0.3
done

for (( i = 32768; i <= 32775; i++ ))
do
	echo "---------"
	echo "Port: $i"
	echo "---------"
	hping3 -2 -p $i -c 10 192.168.0.3
done

echo "--------------------------------------------"
echo "------ BLOCK TCP Packets 111 and 515 -------"
echo "--------------------------------------------"

echo "---------"
echo "Port 111"
echo "---------"
hping3 -p 111 -c 10 192.168.0.3
echo "---------"
echo "Port 515"
echo "---------"
hping3 -p 515 -c 10 192.168.0.3