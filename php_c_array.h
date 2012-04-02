/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2006 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1 2006/01/01 12:50:00 sniper Exp $ */

#ifndef PHP_C_ARRAY_H
#define PHP_C_ARRAY_H

#define C_ARRAY_ASSOC      1<<0
#define C_ARRAY_NUM        1<<1
#define C_ARRAY_BOTH       (C_ARRAY_ASSOC|C_ARRAY_NUM)

#define C_ARRAY_ENSURE_ALLOCED(str, cur_len, alloced, need) do {	\
	while (cur_len + need >= alloced) {								\
		alloced += 8192;											\
		str = erealloc(str, alloced);								\
	}																\
} while (0)

#define C_ARRAY_FLOAT_SERIALIZE_PRECISION	7
#define C_ARRAY_DOUBLE_SERIALIZE_PRECISION	16

typedef enum c_array_entry_types_t {
	C_ARRAY_STRING,
	C_ARRAY_INTEGER,
	C_ARRAY_LONG,
	C_ARRAY_FLOAT,
	C_ARRAY_DOUBLE
} c_array_entry_type;

typedef struct {
	char *name;  /* Name of element */
	int name_len; /* Length of name */
	c_array_entry_type type;    /* Type of element */
	size_t size; /* Size of element */
	int offset;  /* Offset of element from start of struct */
} c_array_entry;

typedef struct {
	int len;    /* Length of array */
	int size;   /* Size of array entry */
	int entries_len; /* Length of array description */
	c_array_entry *entries; /* Array description */
	char *data; /* Actual array data is stored here */
	int final; /* Set when structure can no longer be altered */
	int next_index; /* Next index for pushing data */
	int max_index; /* Highest index present in array */
} c_array;

extern zend_module_entry c_array_module_entry;
#define phpext_c_array_ptr &c_array_module_entry

#ifdef PHP_WIN32
#define PHP_C_ARRAY_API __declspec(dllexport)
#else
#define PHP_C_ARRAY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define le_c_array_name "C array"

PHP_MINIT_FUNCTION(c_array);
PHP_MSHUTDOWN_FUNCTION(c_array);
PHP_RINIT_FUNCTION(c_array);
PHP_RSHUTDOWN_FUNCTION(c_array);
PHP_MINFO_FUNCTION(c_array);

PHP_FUNCTION(confirm_c_array_compiled);	/* For testing, remove later. */
PHP_FUNCTION(c_array_create);
PHP_FUNCTION(c_array_destroy);
PHP_FUNCTION(c_array_add_entry);
PHP_FUNCTION(c_array_get_structure);
PHP_FUNCTION(c_array_finalize);
PHP_FUNCTION(c_array_push);
PHP_FUNCTION(c_array_set);
PHP_FUNCTION(c_array_set_row);
PHP_FUNCTION(c_array_fetch);
PHP_FUNCTION(c_array_fetch_row);
PHP_FUNCTION(c_array_set_length);
PHP_FUNCTION(c_array_fetch_length);
PHP_FUNCTION(c_array_fetch_max_index);
PHP_FUNCTION(c_array_sort);
PHP_FUNCTION(c_array_serialize);
PHP_FUNCTION(c_array_serialize_row);
int c_array_sort_compar(const void *, const void *);
static zval *c_array_fetch_row_from_data(c_array *, const char *, int);
static void c_array_fetch_row_from_data_alloced(c_array *, const char *, int, zval **);
static void c_array_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC);
static void c_array_alloc_data(c_array *, int);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(c_array)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(c_array)
*/

/* In every utility function you add that needs to use variables 
   in php_c_array_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as C_ARRAY_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define C_ARRAY_G(v) TSRMG(c_array_globals_id, zend_c_array_globals *, v)
#else
#define C_ARRAY_G(v) (c_array_globals.v)
#endif

#endif	/* PHP_C_ARRAY_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
