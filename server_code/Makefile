FLAG=-L/usr/lib64/mysql -lmysqlclient -ljsoncpp -std=c++11 -lpthread

order_server:order_server.cc
	g++ $^ -o $@ $(FLAG)

.PHONY:clean
clean:
	rm order_server 
