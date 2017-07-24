sudo apt-get install -y hostapd udhcpd vim
sudo vim /etc/udhcpd.conf # config for dhcp, ip range, interface with dhcp etc
sudo vim /etc/default/udhcpd # enable dhcp, to read the config that we have made
sudo vim /etc/init.d/hostapd # [file:initd_hostapd] start hostapd at boot start
sudo vim /etc/default/hostapd # [file:hostapd] define where are the hstapd config file
sudo ifconfig wlan0 192.168.0.1 # define wlan0 with a static IP (tmp)
sudo vim /etc/network/interfaces # change the interfaces file with the wlan0 static IP and eth0 dhcp
sudo vim /etc/hostapd/hostapd.conf # WiFi AP configurations
sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward" # packet forwarding, routing
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE # ativate NAT for eth0
sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT # stackoverflow
sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT # stackoverflow
sudo service hostapd start # start WiFi
sudo service udhcpd start # start DHCP
sudo update-rc.d hostapd enable # enable boot hostapd
sudo update-rc.d hostapd defaults # enable boot hostapd
sudo update-rc.d udhcpd enable # enable boot udhcpd
sudo hostapd -dd /etc/hostapd/hostapd.conf # debug if we need to see if everything is ok
sudo ifdown wlan0 && sudo ifup wlan0
sudo poweroff # and start again to see if everything is ok
