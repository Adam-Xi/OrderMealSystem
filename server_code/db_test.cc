#include "db.hpp"

//DishTable::Insert()
void TestDishTablInsert()
{
    MYSQL* mysql = order_system::MySQLInit();
    order_system::DishTable dish_table(mysql);

    Json::Value dish;
    dish["name"] = "宫保鸡丁";
    dish["price"] = 1000;
    bool ret = dish_table.Insert(dish);
    printf("%d\n", ret);

    order_system::MySQLRelease(mysql);
}

//DishTable::SelectAll()
void TestDishTableSelectAll()
{
    MYSQL* mysql = order_system::MySQLInit();
    order_system::DishTable dish_table(mysql);
    
    Json::Value dishes;
    int ret = dish_table.SelectAll(&dishes);
    printf("ret = %d\n", ret);
    Json::StyledWriter writer;  //StyledWrite带换行，缩进，能够打印的较为好看
    printf("%s\n", writer.write(dishes).c_str());

    order_system::MySQLRelease(mysql);
}

//DishTable::SelectOne()
void TestDishTableSelectOne()
{
    MYSQL* mysql = order_system::MySQLInit();
    order_system::DishTable dish_table(mysql);

    Json::Value dish;
    int ret = dish_table.SelectOne(1, &dish);
    printf("ret = %d\n", ret);
    Json::StyledWriter writer;  //StyledWrite带换行，缩进，能够打印的较为好看
    printf("%s\n", writer.write(dish).c_str());

    order_system::MySQLRelease(mysql);
}

//DishTable::Update()
void TestDishTableUpdate()
{
    MYSQL* mysql = order_system::MySQLInit();
    order_system::DishTable dish_table(mysql);

    Json::Value dish;
    dish["dish_id"] = 1;
    dish["name"] = "红烧鱼块";
    dish["price"] = 3000;
    int ret = dish_table.Update(dish);
    printf("ret = %d\n", ret);

    order_system::MySQLRelease(mysql);
}

//DishTable::Delete()
void TestDishTableDelete()
{
    MYSQL* mysql = order_system::MySQLInit();
    order_system::DishTable dish_table(mysql);

    int ret = dish_table.Delete(7);
    printf("ret = %d\n", ret);

    order_system::MySQLRelease(mysql);
}


//OrderTable::Insert()
void TestOrderTableInsert()
{
    MYSQL* mysql = order_system::MySQLInit();
    order_system::OrderTable order_table(mysql);

    Json::Value order;
    order["table_id"] = "峨眉";
    order["time"] = "2020/01/04 14:45";
    order["dishes"] = "[1, 2, 3]";
    order["status"] = 1;
    int ret = order_table.Insert(order);
    printf("ret = %d\n", ret);

    order_system::MySQLRelease(mysql);
}

//OrderTable::SelectAll()
void TestOrderTableSelectAll()
{
    MYSQL* mysql = order_system::MySQLInit();
    order_system::OrderTable order_table(mysql);

    Json::Value orders;
    int ret = order_table.SelectAll(&orders);
    printf("%d\n", ret);
    Json::StyledWriter writer;
    printf("%s\n", writer.write(orders).c_str());

    order_system::MySQLRelease(mysql);
}

//OrderTable::ChangeStatus()
void TestOrderTableChangeStatus()
{
    MYSQL* mysql = order_system::MySQLInit();
    order_system::OrderTable order_table(mysql);

    Json::Value order;
    order["order_id"] = 1;
    order["status"] = 1;
    int ret = order_table.ChangeState(order);
    printf("ret = %d\n", ret);

    order_system::MySQLRelease(mysql);
}

int main()
{
    //TestDishTableInsert();
    //TestDishTableSelectAll();
    //TestDishTableSelectOne();   
    //TestDishTableUpdate();
    //TestDishTableDelete();

    //TestOrderTableInsert();
    //TestOrderTableSelectAll();
    TestOrderTableChangeStatus();
    return 0;
}
