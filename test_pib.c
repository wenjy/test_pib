/* test_pib extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_test_pib.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ void test_pib_test1()
 */
PHP_FUNCTION(test_pib_test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "test_pib");
}
/* }}} */

/* {{{ string test_pib_test2( [ string $var ] )
 */
PHP_FUNCTION(test_pib_test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	retval = strpprintf(0, "Hello %s", var);

	RETURN_STR(retval);
}
/* }}}*/

static double php_fahrenheit_to_celsius(double f)
{
	return ((double)5/9) * (double)(f - 32);
}

// 华氏度 转换为 摄氏度
PHP_FUNCTION(fahrenheit_to_celsius)
{
	double f;

	// 可以使用宏也可以使用这个来解析参数
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &f) == FAILURE)
	{
		return;
	}
	
	RETURN_DOUBLE(php_fahrenheit_to_celsius(f));
}

static double php_celsius_to_fahrenheit(double c)
{
    return (((double)9/5) * c) + 32 ;
}

PHP_FUNCTION(celsius_to_fahrenheit)
{
    double c;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &c) == FAILURE) {
        return;
    }

    RETURN_DOUBLE(php_celsius_to_fahrenheit(c));
}

PHP_FUNCTION(temperature_converter)
{
    double t;
    zend_long mode = TEMP_CONVERTER_TO_CELSIUS; // 给默认值，调用时如果不传则默认该值
    zend_string *result;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d|l", &t, &mode) == FAILURE) {
        return;
    }

    switch (mode)
    {
        case TEMP_CONVERTER_TO_CELSIUS:
            result = strpprintf(0, "%.2f degrees fahrenheit gives %.2f degrees celsius", t, php_fahrenheit_to_celsius(t));
            RETURN_STR(result);
        case TEMP_CONVERTER_TO_FAHRENHEIT:
            result = strpprintf(0, "%.2f degrees celsius gives %.2f degrees fahrenheit", t, php_celsius_to_fahrenheit(t));
            RETURN_STR(result);
        default:
            php_error(E_WARNING, "Invalid mode provided, accepted values are 1 or 2");
    }
}

PHP_FUNCTION(multiple_fahrenheit_to_celsius)
{
    HashTable *temperatures;
    zval *data;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "h", &temperatures) == FAILURE) {
        return;
    }
    if (zend_hash_num_elements(temperatures) == 0) {
        return;
    }

    array_init_size(return_value, zend_hash_num_elements(temperatures));

    ZEND_HASH_FOREACH_VAL(temperatures, data)
        zval dup;
        ZVAL_COPY_VALUE(&dup, data);
        convert_to_double(&dup);
    add_next_index_double(return_value, php_fahrenheit_to_celsius(Z_DVAL(dup)));
    ZEND_HASH_FOREACH_END();
}

PHP_FUNCTION(fahrenheit_to_celsius_ref)
{
    zval *param;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &param) == FAILURE) {
        return;
    }

    ZVAL_DEREF(param);
    convert_to_double(param);

    ZVAL_DOUBLE(param, php_fahrenheit_to_celsius(Z_DVAL_P(param)));
}

/* 
* 真正的 C 全局 定义保护变量的结构体 解析为 :
*
* typedef struct _zend_test_pib_globals {
*    zend_long rnd;
* } zend_test_pib_globals;
*/
ZEND_BEGIN_MODULE_GLOBALS(test_pib)
	zend_long rnd;
	zend_ulong cur_score;
    zval scores;
ZEND_END_MODULE_GLOBALS(test_pib)

// 创建全局变量
/* 解析为 zend_test_pib_globals test_pib_globals; */
ZEND_DECLARE_MODULE_GLOBALS(test_pib)

static void pib_rnd_init(void)
{
	/* 重置当前分数 */
    TEST_PIB_G(cur_score) = 0;
    /* 在 0 到 100 之间随机一个数字 */
    php_random_int(0, 100, &TEST_PIB_G(rnd), 0);
}

PHP_FUNCTION(pib_guess)
{
    zend_long r;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &r) == FAILURE) {
        return;
    }

    if (r == TEST_PIB_G(rnd)) {
		add_next_index_long(&TEST_PIB_G(scores), TEST_PIB_G(cur_score));
        /* 将数字重置以进行猜测 */
        pib_rnd_init();
        RETURN_TRUE;
    }

	TEST_PIB_G(cur_score)++;

    if (r < TEST_PIB_G(rnd)) {
        RETURN_STRING("more");
    }

    RETURN_STRING("less");
}

PHP_FUNCTION(pib_get_scores)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    RETVAL_ZVAL(&TEST_PIB_G(scores), 1, 0);
}

PHP_FUNCTION(pib_reset)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    pib_rnd_init();
}

PHP_GINIT_FUNCTION(test_pib)
{
    /* ZEND_SECURE_ZERO 是 memset(0)。也可以解析为 bzero() */
    ZEND_SECURE_ZERO(test_pib_globals, sizeof(*test_pib_globals));
}

PHP_MINIT_FUNCTION(test_pib)
{
	// CONST_CS 大小写敏感 CONST_PERSISTENT 持久常量
    REGISTER_LONG_CONSTANT("TEMP_CONVERTER_TO_CELSIUS", TEMP_CONVERTER_TO_CELSIUS, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("TEMP_CONVERTER_TO_FAHRENHEIT", TEMP_CONVERTER_TO_FAHRENHEIT, CONST_CS|CONST_PERSISTENT);

    return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(test_pib)
{
#if defined(ZTS) && defined(COMPILE_DL_TEST_PIB)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	array_init(&TEST_PIB_G(scores));
	pib_rnd_init();
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(test_pib)
{
    zval_dtor(&TEST_PIB_G(scores));

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(test_pib)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "test_pib support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_test_pib_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_test_pib_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

// 最后参数为1 必填参数
ZEND_BEGIN_ARG_INFO_EX(arginfo_fahrenheit_to_celsius, 0, 0, 1)
    ZEND_ARG_INFO(0, fahrenheit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_celsius_to_fahrenheit, 0, 0, 1)
    ZEND_ARG_INFO(0, celsius)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_temperature_converter, 0, 0, 1)
    ZEND_ARG_INFO(0, temperature)
    ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_multiple_fahrenheit_to_celsius, 0, 0, 1)
    ZEND_ARG_ARRAY_INFO(0, temperatures, 0)
ZEND_END_ARG_INFO();

// 引用传参
ZEND_BEGIN_ARG_INFO_EX(arginfo_fahrenheit_to_celsius_ref, 0, 0, 1)
    ZEND_ARG_INFO(1, fahrenheit)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_pib_guess, 0, 0, 1)
    ZEND_ARG_INFO(0, num)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_pib_reset, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pib_get_scores, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ test_pib_functions[]
 */
static const zend_function_entry test_pib_functions[] = {
	PHP_FE(test_pib_test1,		arginfo_test_pib_test1)
	PHP_FE(test_pib_test2,		arginfo_test_pib_test2)
	PHP_FE(fahrenheit_to_celsius, arginfo_fahrenheit_to_celsius)
	PHP_FE(celsius_to_fahrenheit, arginfo_celsius_to_fahrenheit)
	PHP_FE(temperature_converter, arginfo_temperature_converter)
	PHP_FE(multiple_fahrenheit_to_celsius, arginfo_multiple_fahrenheit_to_celsius)
	PHP_FE(fahrenheit_to_celsius_ref, arginfo_fahrenheit_to_celsius_ref)
	PHP_FE(pib_guess, arginfo_pib_guess)
	PHP_FE(pib_reset, arginfo_pib_reset)
	PHP_FE(pib_get_scores, arginfo_pib_get_scores)
	PHP_FE_END
};
/* }}} */

/* {{{ test_pib_module_entry
 */
zend_module_entry test_pib_module_entry = {
	STANDARD_MODULE_HEADER,
	"test_pib",					/* Extension name */
	test_pib_functions,			/* zend_function_entry 函数入口 */
	NULL,							/* PHP_MINIT - Module initialization 模块初始化 */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown 模块关闭 */
	PHP_RINIT(test_pib),			/* PHP_RINIT - Request initialization 请求初始化 */
	PHP_RSHUTDOWN(test_pib),		/* PHP_RSHUTDOWN - Request shutdown 请求关闭 */
	PHP_MINFO(test_pib),			/* PHP_MINFO - Module info 模块信息 */
	PHP_TEST_PIB_VERSION,		/* Version 版本号 */
	PHP_MODULE_GLOBALS(test_pib),
    PHP_GINIT(test_pib),
    NULL,
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_TEST_PIB
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(test_pib)
#endif
