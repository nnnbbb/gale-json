#include "gale-json.h"

#include <assert.h> /* assert() */
#include <errno.h>  /* errno, ERANGE */
#include <stdlib.h> /* NULL */
#include <math.h>   /* HUGE_VAL */
#include <stdio.h>

#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch) ((ch) >= '1' && (ch) <= '9')

#define EXPECT(c, ch)                   \
    do {                                \
        assert(*context->json == (ch)); \
        context->json++;                \
    } while (0)

typedef struct {
    const char* json;
} gale_context;

static void gale_parse_whitespace(gale_context* context) {
    const char* p = context->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
        p++;
    }
    context->json = p;
}

static int not_singular(gale_context* context) {
    gale_parse_whitespace(context);
    if (context->json[0] != '\0') {
        return GALE_PARSE_ROOT_NOT_SINGULAR;
    }
    return GALE_PARSE_OK;
}

static int gale_parse_literal(
    gale_context* context,
    gale_value* value,
    const char* literal,
    gale_type type
) {
    size_t i;
    EXPECT(context, literal[0]);
    for (i = 0; literal[i + 1]; i++) {
        if (context->json[i] != literal[i + 1]) {
            return GALE_PARSE_INVALID_VALUE;
        }
    }
    context->json += i;
    value->type = type;
    return GALE_PARSE_OK;
}

static int gale_parse_number(gale_context* context, gale_value* value) {
    const char* p = context->json;
    if (*p == '-') {
        p++;
    }
    if (*p == '0') {
        p++;
    } else {
        if (!ISDIGIT1TO9(*p)) {
            return GALE_PARSE_INVALID_VALUE;
        }
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == '.') {
        p++;
        if (!ISDIGIT(*p)) {
            return GALE_PARSE_INVALID_VALUE;
        }
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') {
            p++;
        }
        if (!ISDIGIT(*p)) {
            return GALE_PARSE_INVALID_VALUE;
        }
        for (p++; ISDIGIT(*p); p++);
    }
    errno = 0;
    value->n = strtod(context->json, NULL);
    if (errno == ERANGE && (value->n == HUGE_VAL || value->n == -HUGE_VAL)) {
        return GALE_PARSE_NUMBER_TOO_BIG;
    }
    value->type = GALE_NUMBER;
    context->json = p;
    return GALE_PARSE_OK;
}


static int gale_parse_value(gale_context* context, gale_value* value) {
    switch (*context->json) {
        case 'n':
            return gale_parse_literal(context, value, "null", GALE_NULL);
        case 'f':
            return gale_parse_literal(context, value, "false", GALE_FALSE);
        case 't':
            return gale_parse_literal(context, value, "true", GALE_TRUE);
        default:
            return gale_parse_number(context, value);
        case '\0':
            return GALE_PARSE_EXPECT_VALUE;
    }
}

int gale_parse(gale_value* value, const char* json) {
    gale_context context;
    int ret;
    assert(value != NULL);
    context.json = json;
    value->type = GALE_NULL;
    gale_parse_whitespace(&context);
    if ((ret = gale_parse_value(&context, value)) == GALE_PARSE_OK) {
        gale_parse_whitespace(&context);
        if (*context.json != '\0') {
            value->type = GALE_NULL;
            ret = GALE_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

gale_type gale_get_type(const gale_value* value) {
    assert(value != NULL);
    return value->type;
}

double gale_get_number(const gale_value* value) {
    assert(value != NULL && value->type == GALE_NUMBER);
    return value->n;
}
