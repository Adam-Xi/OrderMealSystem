#include <signal.h>
#include <jsoncpp/json/json.h>
#include "httplib.h"
#include "db.hpp"
#include "util.hpp"

const char* CONTENT_TYPE = "application/json";

MYSQL* mysql = NULL;

int main()
{
    using namespace httplib;

    mysql = order_system::MySQLInit();
    signal(SIGINT, [](int){ order_system::MySQLRelease(mysql); exit(0);});
    order_system::DishTable dish_table(mysql);
    order_system::OrderTable order_table(mysql);

    Server server;
    server.Post("/dish", [&dish_table](const Request& req, Response& resp){
                printf("新增菜品!\n");
                Json::Value req_json;
                Json::Value resp_json;
                Json::Reader reader;
                Json::FastWriter writer;
                //1、获取到数据并解析成 JSON 格式
                bool ret = reader.parse(req.body, req_json);
                if(!ret)
                {
                    printf("parse body failed!\n");
                    resp_json["ok"] = false;
                    resp_json["reason"] = "parse body failed!";
                    resp.status = 400;
                    resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                    return ;
                }
                //2、校验JSON中的信息
                if(req_json["name"].empty() || req_json["price"].empty())
                {
                    printf("Request json error format\n");
                    resp_json["ok"] = false;
                    resp_json["reason"] = "Request json error format";
                    resp.status = 400;
                    resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                    return ;
                }
                //3、调用数据库操作进行插入数据
                ret = dish_table.Insert(req_json);
                if(!ret)
                {
                    printf("dish_table insert failed!\n");
                    resp_json["ok"] = false;
                    resp_json["reason"] = "dish_table insert failed!";
                    resp.status = 500;
                    resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                    return ;
                 }
                 //4、构造正确的响应
                 resp_json["ok"] = true;
                 resp.status = 200;
                 resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                 return ;
                });
    server.Get("/dish", [&dish_table](const Request& req, Response& resp){
               (void)req;
               Json::Value resp_json;
               Json::FastWriter writer;
               printf("查看所有菜品!\n");
               //1、不需要处理输入数据，无需进行校验，直接调用数据库进行操作
               bool ret = dish_table.SelectAll(&resp_json);
               if(!ret)
               {
                    printf("select all dish failed!\n");
                    resp_json["ok"] = false;
                    resp_json["reason"] = "select all dish failed!";
                    resp.status = 500;
                    resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                    return ;
               }
               //2、构造正确的响应
               resp.status = 200;
               resp.set_content(writer.write(resp_json), CONTENT_TYPE);
               });
    //正则表达式：带有特殊符号的字符串。特殊符号描述了字符串的特征
    server.Get(R"(/dish/(\d+))", [&dish_table](const Request& req, Response& resp){
               Json::Value resp_json;
               Json::FastWriter writer;
               //1、获取dish_id
               int32_t dish_id = std::stoi(req.matches[1]);
               printf("获取到编号为%d的菜品\n", dish_id);
               //2、执行数据库操作
               bool ret = dish_table.SelectOne(dish_id, &resp_json);
               if(!ret)
               {
                    printf("dish table select one failed!\n");
                    resp_json["ok"] = false;
                    resp_json["reason"] = "select one dish failed!";
                    resp.status = 500;
                    resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                    return ;
               }
               //3、构造正确的响应
               resp.status = 200;
               resp.set_content(writer.write(resp_json), CONTENT_TYPE);
               });
    server.Put(R"(/dish/(\d+))", [&dish_table](const Request& req, Response& resp){
                printf("新增菜品!\n");
                Json::Value req_json;
                Json::Value resp_json;
                Json::Reader reader;
                Json::FastWriter writer;
                //1、获取到dish_id
                int32_t dish_id = std::stoi(req.matches[1]);
                printf("修改编号为 %d 的菜品!\n", dish_id);

                //2、获取到数据并解析成 JSON 格式
                bool ret = reader.parse(req.body, req_json);
                if(!ret)
                {
                    printf("parse body failed!\n");
                    resp_json["ok"] = false;
                    resp_json["reason"] = "parse body failed!";
                    resp.status = 400;
                    resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                    return ;
                }
                //3、校验JSON中的信息
                if(req_json["name"].empty() || req_json["price"].empty())
                {
                    printf("Request json error format\n");
                    resp_json["ok"] = false;
                    resp_json["reason"] = "Request json error format";
                    resp.status = 400;
                    resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                    return ;
                }
                //4、调用数据库操作进行插入数据
                req_json["dish_id"] = dish_id;  //把dish_id加入到json中
                ret = dish_table.Update(req_json);
                if(!ret)
                {
                    printf("dish_table update failed!\n");
                    resp_json["ok"] = false;
                    resp_json["reason"] = "dish_table update failed!";
                    resp.status = 500;
                    resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                    return ;
                 }
                 //5、构造正确的响应
                 resp_json["ok"] = true;
                 resp.status = 200;
                 resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                 return ;
               });
    server.Delete(R"(dish/(\d+))", [&dish_table](const Request& req, Response& resp){
                  Json::Value resp_json;
                  Json::FastWriter writer;
                  //1、获取到dish_id
                  int32_t dish_id = std::stoi(req.matches[1]);
                  printf("删除编号为%d的菜品\n", dish_id);
                  //2、执行数据库操作
                  bool ret = dish_table.Delete(dish_id);
                  if(!ret)
                  {
                      printf("dish_table delete failed!\n");
                      resp_json["ok"] = false;
                      resp_json["reason"] = "dish_table delete failed!";
                      resp.status = 500;
                      resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                      return ;
                  }
                  //3、构造正确的响应
                  resp_json["ok"] = true;
                  resp.status = 200;
                  resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                  return ;
                  });
    server.Post("/order", [&order_table](const Request& req, Response& resp){
                Json::Value req_json;
                Json::Value resp_json;
                Json::Reader reader;
                Json::FastWriter writer;
                printf("新增一个订单\n");
                //1、获取到 body 数据并解析
                bool ret = reader.parse(req.body, req_json);
                if(!ret)
                {
                      printf("order_table parse failed!\n");
                      resp_json["ok"] = false;
                      resp_json["reason"] = "order_table parse failed!";
                      resp.status = 400;
                      resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                      return ;
                }
                //2、校验body中的数据是否符合要求 
                if(req_json["table_id"].empty() || req_json["time"].empty() || req_json["dish_ids"].empty())
                {
                      printf("order_table format error failed!\n");
                      resp_json["ok"] = false;
                      resp_json["reason"] = "order_table format error failed!";
                      resp.status = 400;
                      resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                      return ;
                }
                //构造一些其他需要的字段
                req_json["status"] = 1;  //1表示订单进行中，0表示订单关闭
                //需要把dish_ids（json数组）这个字段转换成dishes（字符串）
                const Json::Value& dish_ids = req_json["dish_ids"];
                req_json["dishes"] = writer.write(dish_ids);
                //4、调用数据库操作插入数据
                ret = order_table.Insert(req_json);
                if(!ret)
                {
                      printf("order_table insert failed!\n");
                      resp_json["ok"] = false;
                      resp_json["reason"] = "order_table insert failed!";
                      resp.status = 500;
                      resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                      return ;
                }
                //5、构造正确的响应
                resp_json["ok"] = true;
                resp.status = 200;
                resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                return ;
                });
    server.Get("/order", [&order_table](const Request& req, Response& resp){
               (void)req;
                printf("获取所有订单\n");
                Json::Value resp_json;
                Json::FastWriter writer;
                //1、执行数据库操作，获取响应信息
                bool ret = order_table.SelectAll(&resp_json);
                if(!ret)
                {
                      printf("order_table Select all failed!\n");
                      resp_json["ok"] = false;
                      resp_json["reason"] = "order_table Select all failed!";
                      resp.status = 500;
                      resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                      return ;
                }
                //2、构造正确的响应
                resp_json["ok"] = true;
                resp.status = 200;
                resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                return ;
                });
    server.Put(R"(/order/(\d+))", [&order_table](const Request& req, Response& resp){
                Json::Value req_json;
                Json::Value resp_json;
                Json::Reader reader;
                Json::FastWriter writer;
                //1、获取到order_id
                int32_t order_id = std::stoi(req.matches[1]);
                printf("修改编号为 %d 的订单状态\n", order_id);
                //2、解析请求中的status
                bool ret = reader.parse(req.body, req_json);
                if(!ret)
                {
                      printf("order_table parse failed!\n");
                      resp_json["ok"] = false;
                      resp_json["reason"] = "order_table parse failed!";
                      resp.status = 400;
                      resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                      return ;
                }
                //3、校验参数内容
                if(req_json["status"].empty())
                {
                      printf("order_table format error failed!\n");
                      resp_json["ok"] = false;
                      resp_json["reason"] = "order_table format error failed!";
                      resp.status = 400;
                      resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                      return ;
                }
                //4、执行数据库操作
                req_json["order_id"] = order_id;
                ret = order_table.ChangeState(req_json);
                if(!ret)
                {
                      printf("order_table ChangeStatus failed!\n");
                      resp_json["ok"] = false;
                      resp_json["reason"] = "order_table ChangeStatus failed!";
                      resp.status = 500;
                      resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                      return ;
                }
                //5、构造正确的响应
                resp_json["ok"] = true;
                resp.status = 200;
                resp.set_content(writer.write(resp_json), CONTENT_TYPE);
                return ;
                });
    server.Get("/table", [](const Request& req, Response& resp){
               (void)resp;
               //http:192.168.43.91:9094?table_id=xxx
               //通过代码突出index.html，即对index.html进行一定的处理和修改
               //从 query_string 获取到 table_id 填到页面的 {{table_id}}
               
               //获取 table_id
               const std::string& table_id = req.get_param_value("table_id");
               printf("table_id = %s\n", table_id.c_str());
               std::string html;
               FileUtil::ReadFile("./wwwroot/index.html", &html);
               std::string html_out;
               StringUtil::Replace(html,"{{table_id}}", table_id, &html_out);
               resp.set_content(html_out, "text/html");
               });
    server.set_base_dir("./wwwroot");
    server.listen("0.0.0.0", 9094);

    return 0;
}
