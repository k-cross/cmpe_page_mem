all: network_test netlink_prog netlink_ext_test mem_client

network_test: 
	g++ -g testc.cpp -lpthread -lboost_system -o testc

netlink_prog:
	gcc -g netlink_api.c -o net_api

netlink_ext_test:
	g++ -g external_progX.cpp -o netlink_test

mem_client:
	g++ -g RM_Client.cpp -lpthread -lboost_system -o client
