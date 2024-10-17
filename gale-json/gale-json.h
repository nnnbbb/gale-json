#ifndef GALE_JSON_H__
#define GALE_JSON_H__

typedef enum {
    GALE_NULL,
    GALE_FALSE,
    GALE_TRUE,
    GALE_NUMBER,
    GALE_STRING,
    GALE_ARRAY,
    GALE_OBJECT,
} gale_type;

typedef struct {
    double n;
    gale_type type;
} gale_value;

enum {
    GALE_PARSE_OK = 0,
    GALE_PARSE_EXPECT_VALUE,
    GALE_PARSE_INVALID_VALUE,
    GALE_PARSE_ROOT_NOT_SINGULAR,
    GALE_PARSE_NUMBER_TOO_BIG,
};

int gale_parse(gale_value* v, const char* json);

gale_type gale_get_type(const gale_value* v);

double gale_get_number(const gale_value* v);

#endif /* GALE_JSON_H__ */