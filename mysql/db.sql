
create database if not exists order_system;
use order_system;

drop table dish_table;

create table if not exists dish_table (
    dish_id int not null primary key auto_increment,
    name varchar(32),
    price int
);

insert into dish_table values(null, "红烧肉", 2200);
insert into dish_table values(null, "回锅肉", 4300);
insert into dish_table values(null, "糖醋里脊", 3100);
insert into dish_table values(null, "红烧鱼块", 5555);
insert into dish_table values(null, "鱼香肉丝", 2222);

drop table order_table;

create table if not exists order_table(
    order_id int not null primary key auto_increment,
    table_id varchar(32),
    time varchar(32),
    dishes varchar(1024),
    status int
);

insert into order_table values(null, "武当", "2020/01/01", "1,2,3,4", 0);
insert into order_table values(null, "少林", "2019/12/31", "1,2", 0);
insert into order_table values(null, "华山", "2020/01/01", "1,3,4", 0);
