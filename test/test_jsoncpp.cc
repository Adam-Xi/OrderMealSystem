#include <cstdio>
#include <cstdlib>
#include <jsoncpp/json/json.h>

int main()
{
    Json::Value value;
    value["dish_id"] = 1;
    value["name"] = "红烧肉";
    value["price"] = 2000;

    Json::FastWriter writer;
    const std::string& str = writer.write(value);
    printf("%s\n", str.c_str());

    //原始字符串，形如R"(...)"，在括号内的引号写了也不需要转义
    std::string str2 = R"({"name":"红烧肉", "price":100})";
    Json::Reader reader;
    Json::Value output;
    reader.parse(str2, output);
    printf("%s, %d\n", output["name"].asCString(), output["price"].asInt());

    return 0;
}
