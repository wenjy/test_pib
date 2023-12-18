/* test_pib extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "main/SAPI.h"
#include "ext/standard/info.h"
#include "php_test_pib.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* 
* 定义保护变量的结构体 解析为 :
*
* typedef struct _zend_test_pib_globals {
*    zend_long rnd;
* } zend_test_pib_globals;
*/
ZEND_BEGIN_MODULE_GLOBALS(test_pib)
	zend_long rnd;
	zend_ulong cur_score;
    zval scores;
	zend_ulong max_rnd; // ini使用
ZEND_END_MODULE_GLOBALS(test_pib)

// 创建全局变量
/* 解析为 zend_test_pib_globals test_pib_globals; */
ZEND_DECLARE_MODULE_GLOBALS(test_pib)

/**
 * 真实全局变量是非线程保护的真实 C 全局变量
 * 规则：在处理请求时，不能安全地写入此类全局变量
 * 通常在 PHP 中，我们需要此类变量并将其用作只读变量
*/

static zend_string *more, *less; // 代替字符返回
static zend_ulong max = 100;

// 验证 0 到 1000 之间的正整数
ZEND_INI_MH(onUpdateMaxRnd)
{
    zend_long tmp;

    zend_long *p;
#ifndef ZTS
    char *base = (char *) mh_arg2;
#else
    char *base;

    base = (char *) ts_resource(*((int *) mh_arg2));
#endif

    p = (zend_long *) (base+(size_t) mh_arg1);

    tmp = zend_atol(ZSTR_VAL(new_value), (int)ZSTR_LEN(new_value));

    if (tmp < 0 || tmp > 1000) {
        return FAILURE;
    }

    *p = tmp;

    return SUCCESS;
}

ZEND_INI_DISP(MaxRnd)
{
    char disp[100] = {0};
    zend_ulong tmp = 0;

    if (type == ZEND_INI_DISPLAY_ORIG && ini_entry->modified && ini_entry->orig_value) {
        tmp = ZEND_STRTOUL(ZSTR_VAL(ini_entry->orig_value), NULL, 10);
    } else if (ini_entry->value) {
        tmp = ZEND_STRTOUL(ZSTR_VAL(ini_entry->value), NULL, 10);
    }

    tmp /= 10;

    memset(disp, '#', tmp);
    memset(disp + tmp, '.', 100 - tmp);

    php_write(disp, 100);
}

// 可以使用默认验证器，也可以自己定义函数
PHP_INI_BEGIN()
	// STD_PHP_INI_ENTRY_EX("test_pib.rnd_max", "100", PHP_INI_ALL, onUpdateMaxRnd, max_rnd, zend_test_pib_globals, test_pib_globals, MaxRnd)
	STD_PHP_INI_ENTRY("test_pib.rnd_max", "100", PHP_INI_ALL, onUpdateMaxRnd, max_rnd, zend_test_pib_globals, test_pib_globals)
PHP_INI_END()

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

static void register_persistent_string(char *str, zend_string **result)
{
	// 初始化持久字符
    *result = zend_string_init(str, strlen(str), 1);
    zend_string_hash_val(*result);

	GC_ADD_FLAGS(*result, IS_STR_INTERNED);
}

static void pib_rnd_init(void)
{
	/* 重置当前分数 */
    TEST_PIB_G(cur_score) = 0;
    /* 在 0 到 100 之间随机一个数字 */
    php_random_int(0, max, &TEST_PIB_G(rnd), 0);
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
        RETURN_STR(more);
    }

    RETURN_STR(less);
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

/* {{{ PHP_GINIT_FUNCTION
 */
PHP_GINIT_FUNCTION(test_pib)
{
    /* ZEND_SECURE_ZERO 是 memset(0)。也可以解析为 bzero() */
    ZEND_SECURE_ZERO(test_pib_globals, sizeof(*test_pib_globals));
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(test_pib)
{
	// CONST_CS 大小写敏感 CONST_PERSISTENT 持久常量
    REGISTER_LONG_CONSTANT("TEMP_CONVERTER_TO_CELSIUS", TEMP_CONVERTER_TO_CELSIUS, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("TEMP_CONVERTER_TO_FAHRENHEIT", TEMP_CONVERTER_TO_FAHRENHEIT, CONST_CS|CONST_PERSISTENT);

    register_persistent_string("more", &more);
	register_persistent_string("less", &less);

	char *pib_max;
    if (pib_max = getenv("PIB_RAND_MAX")) {
        if (!strchr(pib_max, '-')) {
            max = ZEND_STRTOUL(pib_max, NULL, 10);
        }
    }

	// 注册INI结构
	REGISTER_INI_ENTRIES();

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(test_pib)
{
    zend_string_release(more);
    zend_string_release(less);

	// 释放INI结构
	UNREGISTER_INI_ENTRIES();

    return SUCCESS;
}
/* }}} */

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
	time_t t;
    char cur_time[32];

    time(&t);
    php_asctime_r(localtime(&t), cur_time);

	php_info_print_table_start();
		php_info_print_table_header(2, "test_pib support", "enabled");
		php_info_print_table_row(2, "Current time", cur_time);
	php_info_print_table_end();

	php_info_print_box_start(0);
        if (!sapi_module.phpinfo_as_text) {
            php_write(TEST_PIB_HTML, strlen(TEST_PIB_HTML));
        } else {
            php_write(TEST_PIB_TXT, strlen(TEST_PIB_TXT));
        }
    php_info_print_box_end();

	// 将 INI 设置显示为扩展信息的一部分
	DISPLAY_INI_ENTRIES();
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
	PHP_MINIT(test_pib),		/* PHP_MINIT - Module initialization 模块初始化 */
	PHP_MSHUTDOWN(test_pib),	/* PHP_MSHUTDOWN - Module shutdown 模块关闭 */
	PHP_RINIT(test_pib),			/* PHP_RINIT - Request initialization 请求初始化 */
	PHP_RSHUTDOWN(test_pib),		/* PHP_RSHUTDOWN - Request shutdown 请求关闭 */
	PHP_MINFO(test_pib),			/* PHP_MINFO - Module info 模块信息 */
	PHP_TEST_PIB_VERSION,		/* Version 版本号 */
	PHP_MODULE_GLOBALS(test_pib),
    PHP_GINIT(test_pib),
	NULL,	/* GSHUTDOWN() */
	NULL,	/* PRSHUTDOWN() */
    STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_TEST_PIB
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(test_pib)
#endif
