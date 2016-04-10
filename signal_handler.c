/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Mateusz Kaczanowski <kaczanowski.mateusz@gmail.com>          |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_signal_handler.h"

#ifdef PHP_WIN32
# include "win32/php_stdint.h"
# include "win32/signal.h"
#else
# include <signal.h>
# include <stdint.h>
#endif

#if PHP_MAJOR_VERSION < 7
#define _DECLARE_ZVAL(name) zval * name
#define hp_ptr_dtor(val) zval_ptr_dtor(&val);
#else
#include <zend_string.h>
#define _DECLARE_ZVAL(name) zval name ## _v; zval * name = &name ## _v
#define hp_ptr_dtor(val) zval_ptr_dtor(val);
#endif

ZEND_DECLARE_MODULE_GLOBALS(signal_handler);
static  PHP_GINIT_FUNCTION(signal_handler);

/* {{{ signal_handler_functions[]
 */
const zend_function_entry signal_handler_functions[] = {
	PHP_FE(attach_signal, NULL)
	PHP_FE(detach_signal, NULL)
	#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION == 3 && PHP_RELEASE_VERSION >= 7) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 4) || (PHP_MAJOR_VERSION > 5)
		PHP_FE_END
	#else
		{NULL,NULL,NULL}
	#endif
};
/* }}} */

/* {{{ signal_handler_module_entry
 */
zend_module_entry signal_handler_module_entry = {
	#if ZEND_MODULE_API_NO >= 20010901
		STANDARD_MODULE_HEADER,
	#endif
	"signal_handler",
	signal_handler_functions,
	PHP_MINIT(signal_handler),
	PHP_MSHUTDOWN(signal_handler),
	PHP_RINIT(signal_handler),
	PHP_RSHUTDOWN(signal_handler),
	PHP_MINFO(signal_handler),
	#if ZEND_MODULE_API_NO >= 20010901
		PHP_SIGNAL_HANDLER_VERSION,
	#endif
	PHP_MODULE_GLOBALS(signal_handler),
	PHP_GINIT(signal_handler),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_SIGNAL_HANDLER
ZEND_GET_MODULE(signal_handler)
#endif

/* {{{ PHP_GINIT_FUNCTION
*/
static PHP_GINIT_FUNCTION(signal_handler)
{
	memset(signal_handler_globals, 0, sizeof(*signal_handler_globals));
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
*/
PHP_RINIT_FUNCTION(signal_handler)
{
	zend_hash_init(&SIGNAL_HANDLER_G(php_signal_table), 16, NULL, ZVAL_PTR_DTOR, 0);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(signal_handler)
{
	zend_hash_destroy(&SIGNAL_HANDLER_G(php_signal_table));
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(signal_handler)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(signal_handler)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(signal_handler)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Signal Handler support", "enabled");
	php_info_print_table_end();
}
/* }}} */


void php_signal_callback_handler(int signo)
{
	TSRMLS_FETCH();

	#if PHP_MAJOR_VERSION >= 7
	zval *handle;
	_DECLARE_ZVAL(retval);
	_DECLARE_ZVAL(param);
	handle = zend_hash_index_find(&SIGNAL_HANDLER_G(php_signal_table), signo);
	if (!handle) {
	#else
	zval *param, **handle, *retval;
	MAKE_STD_ZVAL(retval);
	MAKE_STD_ZVAL(param);
	if(zend_hash_index_find(&SIGNAL_HANDLER_G(php_signal_table), signo, (void **) &handle) == FAILURE){
	#endif
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Callback function not found for signo: %d", signo);
		return;
	}

	ZVAL_NULL(retval);
	ZVAL_LONG(param, signo);

	/* Call the function */
	#if PHP_MAJOR_VERSION >= 7
	call_user_function(EG(function_table), NULL, handle, retval, 1, param TSRMLS_CC);
	#else
	call_user_function(EG(function_table), NULL, *handle, retval, 1, &param TSRMLS_CC);
	#endif

	hp_ptr_dtor(param);
	hp_ptr_dtor(retval);
}

/* {{{ proto attach_signal(int signo, callback)
attach signal handler */
PHP_FUNCTION(attach_signal)
{
	zval *handle, **dest_handle = NULL;

	#if PHP_MAJOR_VERSION >= 7
	zend_string *func_name;
	#else
	char *func_name;
	#endif
	long signo;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &signo, &handle) == FAILURE) {
		return;
	}

	/* Check signal code */
	if (signo < 1 || signo > 32) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid signal");
		RETURN_FALSE;
	}

	/* Check if given parameter is callable */
	#if ZEND_MODULE_API_NO <= 20060613
	if (!zend_is_callable(handle, 0, &func_name)) {
	#else
	if (!zend_is_callable(handle, 0, &func_name TSRMLS_CC)) {
	#endif

		#if PHP_MAJOR_VERSION >= 7
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s is not a callable function name error", ZSTR_VAL(func_name));
		zend_string_release(func_name);
		#else
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s is not a callable function name error", func_name);
		efree(func_name);
		#endif
		RETURN_FALSE;
	}

	#if PHP_MAJOR_VERSION >= 7
	zend_string_release(func_name);
	#else
	efree(func_name);
	#endif

	/* Set the handler for the signal */
	if (signal(signo, php_signal_callback_handler) == SIG_ERR) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while setting a signal handler for signo: %lu", signo);
		RETURN_FALSE;
	}

	/* Add the function name to our signal table */
	#if PHP_MAJOR_VERSION >= 7
	zend_hash_index_update(&SIGNAL_HANDLER_G(php_signal_table), signo, handle);
	zval_add_ref(handle);
	#else
	zend_hash_index_update(&SIGNAL_HANDLER_G(php_signal_table), signo, (void **) &handle, sizeof(zval *), (void **) &dest_handle);
 	if (dest_handle) zval_add_ref(dest_handle);
 	#endif

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto detach_signal(int signo)
detach signal handler */
PHP_FUNCTION(detach_signal)
{
	long signo;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &signo) == FAILURE) {
		return;
	}

	/* Check signal code */
	if (signo < 1 || signo > 32) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid signal");
		RETURN_FALSE;
	}

	/* If signal handler is registred, set SIG_DFL */
	if(zend_hash_index_exists(&SIGNAL_HANDLER_G(php_signal_table), signo) && signal(signo, SIG_DFL) != SIG_ERR){
		zend_hash_index_del(&SIGNAL_HANDLER_G(php_signal_table), signo);
		RETURN_TRUE;
	}else{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot detach signo: %lu", signo);
		RETURN_FALSE;
	}
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
