# 数据库的操作：


> #include <mysql/mysql.h>
>
>   //1、创建句柄
> MYSQL* mysql = mysql_init(NULL);
>   //2、建立连接
> if(mysql_real_connect(mysql, "127.0.0.1", "root", "9826", "order_system", 3306, NULL, 0) == NULL)
> {
>     printf("mysql connect failed! %s\n", mysql_error(mysql));
>     return NULL;
> }
>   //3、设置编码方式
> mysql_set_character_set(mysql, "utf8");
>   //4、拼接SQL
> char sql[1024 * 4] = {0};
> sprintf(sql, "select dish_id, name, price from dish_table where dish_id = %d", dish_id);
>   //5、执行SQL语句
> int ret = mysql_query(mysql_, sql);
>   //6、构造SQL结果集合
> MYSQL_RES* result = mysql_store_result(mysql_);
>   //7、获取结果集合中的行数
> int rows = mysql_num_rows(result);
>   //8、根据行数遍历每一行，获取每一行中的数据
> MYSQL_ROW row = mysql_fetch_row(result);
>   //9、释放结果集合的空间，避免内存泄漏
> mysql_free_result(result);
>   //10、关闭数据库句柄
> mysql_close(mysql);



# 前后端交互需要的接口：
----(Restful风格的API)
a)使用HTTP方法表示操作的语义
> POST表示插入、GET表示查找、PUT表示修改、DELETE表示删除
b)使用url path表示要操作的资源
> /dish表示操作菜、/order表示操作订单
c)使用body表示操作过程中的一些具体数据，通常使用json格式来组织数据
> json是一种数据组织格式，可以用一个字符串来表示一个结构化的数据

1、新增菜品
    //传统的方式：Get/dish?method=insert&name=京酱肉丝&price=2000

    请求：
    POST /dish
    {
        "name":"红烧肉",
        "price":2000
    }
    Content-Type:application/json
    响应：
    HTTP/1.1 200 OK
    {
        "ok":true,
        "reason":""
    }

2、查看所有菜品
    请求：
    GET /dish
    响应：
    HTTP/1.1 200 OK
    [
        {
            "dish_id":1,
            "name":"红烧肉",
            "price":2200
        },
        {
            "dish_id":2,
            "name":"回锅肉",
            "price":4300
        }
        ...
    ]

3、查看指定菜品
    请求：
    GET /dish/:dish_id
    响应：
    HTTP /1.1 200 OK
    {
        "dish_id":1,
        "name":"红烧肉",
        "price":2200
    }

4、修改菜品
    请求：
    PUT /dish/:dish_id
    {
        "name":"毛家红烧肉",
        "price":8000
    }
    响应：
    {
        "ok":true
    }

5、删除菜品
    //传统的方式：Get/dish?method=delete&dish_id=1
    请求：
    DELETE /dish/:dish_id
    响应：
    HTTP /1.1 200 OK
    {
        "ok":true
    }

6、新增订单
    请求：
    POST /order
    {
        "table_id":"武当",
        "time":"2020/01/04 10:00",
        "dish_ids":[1,2,3]
    }
    响应：
    HTTP /1.1 200 OK
    {
        "ok":true
    }

7、查看订单
    请求：
    GET /order

    响应：
    HTTP /1.1 200 OK
    [
    {
        order_id:1,
        table_id:"武当",
        time:"2020/01/04 12:00",
        status:1,
        dishes:
            [
                1,2,3
            ]
    },
    {
        ...
    },
    ...
    ]

8、修改订单状态
    请求：
    PUT /order/:order_id
    {
        status:1
    }
    响应：
    HTTP /1.1 200 OK
    {
        "ok":true
    }

# jsoncpp
### 一个核心类
Json::Value 表示一个 JSON 对象，用法和 std::map 十分相似
### 两个重要方法
Writer::write：把 Json 对象序列化成字符串(std::string)
Reader::parse：把字符串反序列化为 Json 对象

# 服务器程序核心流程
1、读取请求并解析（反序列化）
2、根据请求计算响应（和业务相关）
3、把响应写回客户端（序列化）

# 构造HTTP请求
1、curl：Linux下的命令
2、Postman：跨平台的程序，可以很方便的构造HTTP请求

# 前端页面
1、用户点餐页面
菜名 | 价格 | 选择 | 总价 | 提交按钮
2、商家订单查看页面
桌号 | 时间 | 查看详细信息 | 修改状态
3、商家菜品管理页面
菜名 | 价格 | 修改 | 删除
3、商家菜品管理页面
