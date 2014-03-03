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

#ifndef PHP_SIGNAL_HANDLER_H
#define PHP_SIGNAL_HANDLER_H

extern zend_module_entry signal_handler_module_entry;
#define phpext_signal_handler_ptr &signal_handler_module_entry

#define PHP_SIGNAL_HANDLER_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_SIGNAL_HANDLER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SIGNAL_HANDLER_API __attribute__ ((visibility("default")))
#else
#	define PHP_SIGNAL_HANDLER_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(signal_handler);
PHP_MSHUTDOWN_FUNCTION(signal_handler);
PHP_RINIT_FUNCTION(signal_handler);
PHP_RSHUTDOWN_FUNCTION(signal_handler);
PHP_MINFO_FUNCTION(signal_handler);

PHP_FUNCTION(attach_signal);
PHP_FUNCTION(detach_signal);

ZEND_BEGIN_MODULE_GLOBALS(signal_handler);
	HashTable php_signal_table;
ZEND_END_MODULE_GLOBALS(signal_handler);

ZEND_EXTERN_MODULE_GLOBALS(signal_handler);

#ifdef ZTS
#define SIGNAL_HANDLER_G(v) TSRMG(signal_handler_globals_id, zend_signal_handler_globals *, v)
#else
#define SIGNAL_HANDLER_G(v) (signal_handler_globals.v)
#endif

#endif	/* PHP_SIGNAL_HANDLER_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
