#include <iostream>
#include "yyjson.h"

// 递归打印JSON值的函数
void print_json_value(yyjson_val *val, int indent = 0) {
    // 根据缩进级别打印空格
    std::string indent_str(indent * 2, ' ');

    if (yyjson_is_obj(val)) {
        // 处理JSON对象
        std::cout << indent_str << "{\n";
        yyjson_obj_iter iter;
        yyjson_obj_iter_init(val, &iter);
        yyjson_val *key, *value;
        while ((key = yyjson_obj_iter_next(&iter))) {
            value = yyjson_obj_iter_get_val(key);
            std::cout << indent_str << "  " << yyjson_get_str(key) << ": ";
            print_json_value(value, indent + 1);
        }
        std::cout << indent_str << "}\n";
    } else if (yyjson_is_arr(val)) {
        // 处理JSON数组
        std::cout << indent_str << "[\n";
        size_t idx, max;
        yyjson_val *item;
        yyjson_arr_foreach(val, idx, max, item) {
            std::cout << indent_str << "  " << idx << ": ";
            print_json_value(item, indent + 1);
        }
        std::cout << indent_str << "]\n";
    } else if (yyjson_is_str(val)) {
        // 处理字符串
        std::cout << "\"" << yyjson_get_str(val) << "\"\n";
    } else if (yyjson_is_int(val)) {
        // 处理整数
        std::cout << yyjson_get_int(val) << "\n";
    } else if (yyjson_is_real(val)) {
        // 处理浮点数
        std::cout << yyjson_get_real(val) << "\n";
    } else if (yyjson_is_bool(val)) {
        // 处理布尔值
        std::cout << (yyjson_get_bool(val) ? "true" : "false") << "\n";
    } else if (yyjson_is_null(val)) {
        // 处理null值
        std::cout << "null\n";
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    // 测试格式化
    const char* bad_json = R"({"NMT_Join_1":{}})";
    // 使用yyjson格式化JSON
        yyjson_doc *bad_doc = yyjson_read(bad_json, strlen(bad_json), 0);
        if (bad_doc) {
            // 使用YYJSON_WRITE_PRETTY标志进行格式化输出
            char *pretty_json = yyjson_write(bad_doc, YYJSON_WRITE_PRETTY | YYJSON_WRITE_ESCAPE_UNICODE, NULL);
            if (pretty_json) {
                std::cout << "格式化后的JSON:\n" << pretty_json << std::endl;
                free(pretty_json);  // 释放yyjson_write分配的内存
            }
            yyjson_doc_free(bad_doc);  // 释放文档内存
        } else {
            std::cerr << "JSON解析失败" << std::endl;
        }

    // 更复杂的JSON示例，包含嵌套对象和数组
    const char *json = R"({
        "name": "Mash",
        "star": 4,
        "hits": [2, 2, 1, 3],
        "address": {
            "street": "Main St",
            "city": "New York",
            "zip": 10001
        },
        "tags": ["a", "b", "c"],
        "active": true,
        "score": 9.8,
        "metadata": null
    })";

    // 读取JSON并获取根节点
    yyjson_doc *doc = yyjson_read(json, strlen(json), 0);
    yyjson_val *root = yyjson_doc_get_root(doc);

    // 递归打印整个JSON结构
    std::cout << "递归遍历JSON结构:\n";
    print_json_value(root);

    // 释放文档内存
    yyjson_doc_free(doc);

    return 0;
}
