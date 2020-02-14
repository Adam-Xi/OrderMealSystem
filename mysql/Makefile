.PHONY:all
all:insert select

insert:insert.cc
	g++ $^ -o $@ -L/usr/lib64/mysql/ -lmysqlclient

select:select.cc
	g++ $^ -o $@ -L/usr/lib64/mysql/ -lmysqlclient

.PHONY:clean
clean:
	rm insert select
