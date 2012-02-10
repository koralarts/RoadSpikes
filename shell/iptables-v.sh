while :
do
	clear
	iptables -t mangle -L -v
	echo "-----------------"
	echo " NORMAL TABLE "
	echo "----------------"
	iptables -L -v
	sleep 1
done
