# Water_flow_measuring_unit

# 1
Yoy have to add "Require all granted" replacing "Require local" in apache config file

however some time it can be risk into your system in security side, therefore, you can put folowing data on apache config file

</Directory>
Order Deny,Allow
     Deny from all
     Allow from 127.0.0.1
     Allow from ::1
     Allow from localhost
	 
	 Require local
	 Require ip 192.168.0
</Directory>

# 2
Some time you have to add firewall TCP rule for allow port 80 or *your used port* 

# 3
import "test.sql" script file for your wamp server, some time you have to create "test" database in server

