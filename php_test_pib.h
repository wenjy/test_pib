/* test_pib extension for PHP */

#ifndef PHP_TEST_PIB_H
# define PHP_TEST_PIB_H

extern zend_module_entry test_pib_module_entry;
# define phpext_test_pib_ptr &test_pib_module_entry

# define PHP_TEST_PIB_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_TEST_PIB)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#define TEMP_CONVERTER_TO_FAHRENHEIT 2
#define TEMP_CONVERTER_TO_CELSIUS 1

#define TEST_PIB_TXT  "PHP TEST PIB Authors"
#define TEST_PIB_HTML "<h3>" TEST_PIB_TXT "</h3>"

#ifdef ZTS
#define TEST_PIB_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(test_pib, v)
#else
#define TEST_PIB_G(v) (test_pib_globals.v)
#endif

PHP_FUNCTION(fahrenheit_to_celsius);
PHP_FUNCTION(celsius_to_fahrenheit);
PHP_FUNCTION(temperature_converter);
PHP_FUNCTION(multiple_fahrenheit_to_celsius);
PHP_FUNCTION(fahrenheit_to_celsius_ref);
PHP_FUNCTION(pib_guess);
PHP_FUNCTION(pib_reset);
PHP_FUNCTION(pib_get_scores);

#endif	/* PHP_TEST_PIB_H */