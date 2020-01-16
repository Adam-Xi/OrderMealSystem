#include <cstdio>
#include <cstdlib>
#include <mysql/mysql.h>

int main()
{
    //1、创建mysql句柄
    MYSQL* mysql = mysql_init(NULL);
    //2、建立连接
    if(mysql_real_connect(mysql, "127.0.0.1", "root", "9826", "order_system", 3306, NULL, 0) == NULL)
    {
        printf("mysql connect failed! %s\n", mysql_error(mysql));
        return 1;
    }
    //3、设置编码方式
    mysql_set_character_set(mysql, "utf8");
    //4、拼装SQL语句
    char sql[1024] = {0};
    sprintf(sql, "select * from dish_table");
    //5、执行SQL语句
    int ret = mysql_query(mysql, sql);
    if(ret)
    {
        printf("mysql_query do failed! %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return 1;
    }
    //6、获取并遍历结果集合
    //a)获取结果集合
    MYSQL_RES* result = mysql_store_result(mysql);
    //b)获取结果中有几行几列
    int rows = mysql_num_rows(result);
    int cols = mysql_num_fields(result);
    //c)、根据行列遍历结果
    for(int row = 0; row < rows; row++)
    {
        //获取一行数据
        MYSQL_ROW mysql_row = mysql_fetch_row(result);
        //使用[]取下标操作能获取每一列
        for(int col = 0; col < cols; col++)
        {
            printf("%s\t", mysql_row[col]);
        }
        printf("\n");
    }
    //7、结果集合需要手动释放
    mysql_free_result(result);
    //8、断开连接
    mysql_close(mysql);
    return 0;
}
