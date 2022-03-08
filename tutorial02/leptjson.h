#ifndef LEPTJSON_H__
#define LEPTJSON_H__

typedef enum { LEPT_NULL,
               LEPT_FALSE,
               LEPT_TRUE,
               LEPT_NUMBER,
               LEPT_STRING,
               LEPT_ARRAY,
               LEPT_OBJECT } lept_type;

typedef struct {
    double n;
    lept_type type;
} lept_value;

enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR,
    LEPT_PARSE_NUMBER_TOO_BIG
};

int lept_parse(lept_value* v, const char* json);

lept_type lept_get_type(const lept_value* v);

double lept_get_number(const lept_value* v);
#include <ctime>
#include <iostream>

template <class T>
void _print(T arg) {
    std::cout << arg << " ";
}

template <class... Args>
void log(Args... args) {
    time_t t;
    struct tm* tmp;
    char MY_TIME[50];
    time(&t);
    // https://www.geeksforgeeks.org/strftime-function-in-c/
    tmp = localtime(&t);
    strftime(MY_TIME, sizeof(MY_TIME), "[%Y-%m-%d %H:%M:%S]", tmp);
    std::cout << MY_TIME << " ";

    int arr[] = {(_print(args), 0)...};
    std::cout << std::endl;
    (void)arr;
}

#endif /* LEPTJSON_H__ */
