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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_c_array.h"

/* If you declare any globals in php_c_array.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(c_array)
*/

/* True global resources - no need for thread safety here */
static int le_c_array_rsrc;

/* {{{ c_array_functions[]
 *
 * Every user visible function must have an entry in c_array_functions[].
 */
zend_function_entry c_array_functions[] = {
	PHP_FE(c_array_create,				NULL)		/* Create a c array */
	PHP_FE(c_array_destroy,				NULL)		/* Destroy a c array */
	PHP_FE(c_array_add_entry,			NULL)		/* Add to a c array structure */
	PHP_FE(c_array_get_structure,		NULL)		/* Fetch structure of array */
	PHP_FE(c_array_finalize,			NULL)		/* Finalize structure of array */
	PHP_FE(c_array_set,					NULL)		/* Set value in array */
	PHP_FE(c_array_set_row,				NULL)		/* Set a row of values in one go */
	PHP_FE(c_array_push,				NULL)		/* Push to end of array */
	PHP_FE(c_array_fetch,				NULL)		/* Get value from array */
	PHP_FE(c_array_fetch_row,			NULL)		/* Get one row of values from array */
	PHP_FE(c_array_set_length,			NULL)		/* Change array length */
	PHP_FE(c_array_fetch_length,		NULL)		/* Fetch array length */
	PHP_FE(c_array_fetch_max_index,		NULL)		/* Fetch array's highest index */
	PHP_FE(c_array_sort,				NULL)		/* Sort a C array */
	PHP_FE(c_array_serialize,			NULL)		/* Serialize a C array */
	PHP_FE(c_array_serialize_row,		NULL)		/* Serialize one row of C array */
	{NULL, NULL, NULL}	/* Must be the last line in c_array_functions[] */
};
/* }}} */

/* {{{ c_array_module_entry
 */
zend_module_entry c_array_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"c_array",
	c_array_functions,
	PHP_MINIT(c_array),
	PHP_MSHUTDOWN(c_array),
	PHP_RINIT(c_array),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(c_array),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(c_array),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_C_ARRAY
ZEND_GET_MODULE(c_array)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("c_array.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_c_array_globals, c_array_globals)
    STD_PHP_INI_ENTRY("c_array.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_c_array_globals, c_array_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_c_array_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_c_array_init_globals(zend_c_array_globals *c_array_globals)
{
	c_array_globals->global_value = 0;
	c_array_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(c_array)
{
	/* If you have INI entries, uncomment these lines 
	ZEND_INIT_MODULE_GLOBALS(c_array, php_c_array_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	*/
	REGISTER_LONG_CONSTANT("C_ARRAY_ASSOC", C_ARRAY_ASSOC, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("C_ARRAY_NUM", C_ARRAY_NUM, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("C_ARRAY_BOTH", C_ARRAY_BOTH, CONST_CS | CONST_PERSISTENT);
	le_c_array_rsrc = zend_register_list_destructors_ex(c_array_dtor,
			NULL, "c array", module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(c_array)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(c_array)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(c_array)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(c_array)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "c_array support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ proto resource c_array_create()
 * Creates a new C array */
PHP_FUNCTION(c_array_create)
{
	c_array *c_arr;

	c_arr = emalloc(sizeof(c_array));
	c_arr->len = 0;
	c_arr->size = 0;
	c_arr->entries_len = 0;
	c_arr->entries = NULL;
	c_arr->data = NULL;
	c_arr->final = 0;
	c_arr->next_index = 0;
	c_arr->max_index = -1;

	ZEND_REGISTER_RESOURCE(return_value, c_arr, le_c_array_rsrc);
}
/* }}} */

/* {{{ proto resource c_array_destroy()
 * Destroys a C array */
PHP_FUNCTION(c_array_destroy)
{
	zval *r;
	c_array *c_arr;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
				"r", &r)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &r, -1, le_c_array_name, le_c_array_rsrc);
	zend_list_delete(Z_LVAL_P(r));
}
/* }}} */

/* {{{ proto void c_array_add_entry(resource $c_array, string $name,
 * string $type)
 * Adds named element to array
 */
PHP_FUNCTION(c_array_add_entry)
{
	zval *zc_array;
	c_array *c_arr;
	char *name;
	int name_len;
	char *type;
	int type_len;
	c_array_entry_type entry_type;
	c_array_entry new_entry;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rss",
				&zc_array, &name, &name_len, &type, &type_len)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	if (c_arr->final) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Array is already finalized, cannot alter structure");
		return;
	}

    if (strcmp(type, "string") == 0) {
		entry_type = C_ARRAY_STRING;
	} else if (strcmp(type, "integer") == 0) {
		entry_type = C_ARRAY_INTEGER;
	} else if (strcmp(type, "long") == 0) {
		entry_type = C_ARRAY_LONG;
	} else if (strcmp(type, "float") == 0) {
		entry_type = C_ARRAY_FLOAT;
	} else if (strcmp(type, "double") == 0) {
		entry_type = C_ARRAY_DOUBLE;
	} else {
		php_error_docref1(NULL TSRMLS_CC, type, E_WARNING,
				"type is not valid");
		return;
	}

	/* Ok, name and type are valid.  Let's go */
	new_entry.name = estrdup(name);
	new_entry.name_len = strlen(name);
	new_entry.type = entry_type;
	switch(entry_type) {
		case C_ARRAY_STRING:
			new_entry.size = sizeof(char *);
			break;
		case C_ARRAY_INTEGER:
			new_entry.size = sizeof(int);
			break;
		case C_ARRAY_LONG:
			new_entry.size = sizeof(long);
			break;
		case C_ARRAY_FLOAT:
			new_entry.size = sizeof(float);
			break;
		case C_ARRAY_DOUBLE:
			new_entry.size = sizeof(double);
			break;
	}
	if (c_arr->entries_len == 0) {
		new_entry.offset = 0;
	} else {
		/* Previous entry offset + previous entry size */
		new_entry.offset = c_arr->entries[c_arr->entries_len-1].offset + c_arr->entries[c_arr->entries_len-1].size;
	}

	if (c_arr->entries_len == 0) {
		c_arr->entries = emalloc(sizeof(c_array_entry));
	} else {
		c_arr->entries = erealloc(c_arr->entries, sizeof(c_array_entry) * (c_arr->entries_len + 1));
	}

	c_arr->entries[c_arr->entries_len] = new_entry;
	c_arr->entries_len++;

	c_arr->size += new_entry.size;
}
/* }}} */

/* {{{ proto void c_array_get_structure(resource $c_array)
 * Gets the structure of a c_array and returns it as a php array
 */
PHP_FUNCTION(c_array_get_structure)
{
	zval *zc_array;
	c_array *c_arr;
	int i;
	zval *zentry;


	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r",
				&zc_array)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	array_init(return_value);
	for (i = 0; i < c_arr->entries_len; i++) {
		ALLOC_INIT_ZVAL(zentry);
		array_init(zentry);

		add_assoc_string(zentry, "name", c_arr->entries[i].name, 1);
		switch(c_arr->entries[i].type) {
		case C_ARRAY_STRING:
			add_assoc_string(zentry, "type", "string", 1);
			break;
		case C_ARRAY_INTEGER:
			add_assoc_string(zentry, "type", "integer", 1);
			break;
		case C_ARRAY_LONG:
			add_assoc_string(zentry, "type", "long", 1);
			break;
		case C_ARRAY_FLOAT:
			add_assoc_string(zentry, "type", "float", 1);
			break;
		case C_ARRAY_DOUBLE:
			add_assoc_string(zentry, "type", "double", 1);
			break;
		}
		add_assoc_long(zentry, "offset", (long)c_arr->entries[i].offset);
		add_assoc_long(zentry, "size", (long)c_arr->entries[i].size);

		add_index_zval(return_value, (long)i, zentry);
	}
}
/* }}} */

/* {{{ proto void c_array_finalize(resource $c_array)
 * Finalize the structure of a C array
 */
PHP_FUNCTION(c_array_finalize)
{
	zval *zc_array;
	c_array *c_arr;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r",
				&zc_array)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	c_arr->final = 1;
}
/* }}} */

/* {{{ proto void c_array_set(resource $c_array, int $index, mixed $entry, mixed $value)
 * Set value in C array
 */
PHP_FUNCTION(c_array_set)
{
	zval *zc_array;
	c_array *c_arr;
	long index;
	zval *zentry;
	zval *zvalue;
	int entry_index;
	char *entry_name;
	int data_offset;
	int i;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlzz",
				&zc_array, &index, &zentry, &zvalue)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	if (!c_arr->final) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Array must be finalized before adding data");
		return;
	}

	switch(Z_TYPE_P(zentry)) {
	case IS_LONG:
		entry_index = (int)Z_LVAL_P(zentry);
		if (entry_index >= c_arr->entries_len) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Entry index out of bounds");
			return;
		}
		break;
	case IS_STRING:
		entry_name = Z_STRVAL_P(zentry);
		entry_index = -1;
		for (i = 0; i < c_arr->entries_len; i++) {
			if (strcmp(entry_name, c_arr->entries[i].name) == 0) {
				entry_index = i;
				break;
			}
		}
		if (entry_index == -1) {
			php_error_docref1(NULL TSRMLS_CC, entry_name, E_WARNING, "Could not find entry name");
			return;
		}
		break;
	default:
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Entry id must be integer or string");
		return;
	}

	/* Expand array if necessary.  Unset values will have random data. */
	c_array_alloc_data(c_arr, index + 1);
	c_arr->max_index = MAX(c_arr->max_index, index);
	data_offset = c_arr->size * index + c_arr->entries[entry_index].offset;

	switch(c_arr->entries[entry_index].type) {
	case C_ARRAY_STRING:
		if (Z_TYPE_P(zvalue) == IS_NULL) {
			*((char **)(c_arr->data + data_offset)) = NULL;
		} else {
			convert_to_string_ex(&zvalue);
			*((char **)(c_arr->data + data_offset)) = estrdup(Z_STRVAL_P(zvalue));
		}
		break;
	case C_ARRAY_INTEGER:
		convert_to_long_ex(&zvalue);
		*((int *)(c_arr->data + data_offset)) = (int)Z_LVAL_P(zvalue);
		break;
	case C_ARRAY_LONG:
		convert_to_long_ex(&zvalue);
		*((long *)(c_arr->data + data_offset)) = Z_LVAL_P(zvalue);
		break;
	case C_ARRAY_FLOAT:
		convert_to_double_ex(&zvalue);
		*((float *)(c_arr->data + data_offset)) = (float)Z_DVAL_P(zvalue);
		break;
	case C_ARRAY_DOUBLE:
		convert_to_double_ex(&zvalue);
		*((double *)(c_arr->data + data_offset)) = Z_DVAL_P(zvalue);
		break;
	}
}
/* }}} */

/* {{{ proto void c_array_set_row(resource $c_array, int $index, array $values)
 * Set row of values in C array.
 * Input keys will be used to match entries in C array.
 */
PHP_FUNCTION(c_array_set_row)
{
	zval *zc_array;
	c_array *c_arr;
	long index;
	zval *zvalue_arr;
	int array_count;
	int data_offset;
	int new_data_offset;
	int i;
	char *new_data;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rla",
				&zc_array, &index, &zvalue_arr)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	if (!c_arr->final) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Array must be finalized before adding data");
		return;
	}

	array_count = (int)zend_hash_num_elements(Z_ARRVAL_P(zvalue_arr));
	if (array_count != c_arr->entries_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Array length does not match row length");
		return;
	}

	new_data = emalloc(c_arr->size);

	for (i = 0; i < c_arr->entries_len; i++) {
		zval **zvalue_p;
		zval *zvalue;

		if (zend_hash_find(Z_ARRVAL_P(zvalue_arr), c_arr->entries[i].name, c_arr->entries[i].name_len + 1, (void**)&zvalue_p) != FAILURE) {
			// Success
		} else if (zend_hash_index_find(Z_ARRVAL_P(zvalue_arr), (long)i, (void**)&zvalue_p) != FAILURE) {
			// Success
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Passed array did not contain all keys");
			efree(new_data);
			return;
		}

		zvalue = *zvalue_p;

		new_data_offset = c_arr->entries[i].offset;

		switch(c_arr->entries[i].type) {
		case C_ARRAY_STRING:
			if (Z_TYPE_P(zvalue) == IS_NULL) {
				*((char **)(new_data + new_data_offset)) = NULL;
			} else {
				convert_to_string_ex(&zvalue);
				*((char **)(new_data + new_data_offset)) = estrdup(Z_STRVAL_P(zvalue));
			}
			break;
		case C_ARRAY_INTEGER:
			convert_to_long_ex(&zvalue);
			*((int *)(new_data + new_data_offset)) = (int)Z_LVAL_P(zvalue);
			break;
		case C_ARRAY_LONG:
			convert_to_long_ex(&zvalue);
			*((long *)(new_data + new_data_offset)) = Z_LVAL_P(zvalue);
			break;
		case C_ARRAY_FLOAT:
			convert_to_double_ex(&zvalue);
			*((float *)(new_data + new_data_offset)) = (float)Z_DVAL_P(zvalue);
			break;
		case C_ARRAY_DOUBLE:
			convert_to_double_ex(&zvalue);
			*((double *)(new_data + new_data_offset)) = Z_DVAL_P(zvalue);
			break;
		}
	}

	/* Expand array if necessary.  Unset values will have random data. */
	c_array_alloc_data(c_arr, index + 1);
	c_arr->max_index = MAX(c_arr->max_index, index);

	data_offset = c_arr->size * index;

	memcpy(c_arr->data + data_offset, new_data, c_arr->size);

	efree(new_data);
}
/* }}} */

/* {{{ proto void c_array_push(resource $c_array, array $values)
 * Add row of values to end of C array.
 * Input keys will be used to match entries in C array.
 */
PHP_FUNCTION(c_array_push)
{
	zval *zc_array;
	c_array *c_arr;
	int index;
	zval *zvalue_arr;
	int array_count;
	int data_offset;
	int new_data_offset;
	int i;
	char *new_data;
	int new_length;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ra",
				&zc_array, &zvalue_arr)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	if (!c_arr->final) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Array must be finalized before adding data");
		return;
	}

	array_count = (int)zend_hash_num_elements(Z_ARRVAL_P(zvalue_arr));
	if (array_count != c_arr->entries_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Array length does not match row length");
		return;
	}

	new_data = emalloc(c_arr->size);

	for (i = 0; i < c_arr->entries_len; i++) {
		zval **zvalue_p;
		zval *zvalue;

		if (zend_hash_find(Z_ARRVAL_P(zvalue_arr), c_arr->entries[i].name, c_arr->entries[i].name_len + 1, (void**)&zvalue_p) != FAILURE) {
			// Success
		} else if (zend_hash_index_find(Z_ARRVAL_P(zvalue_arr), (long)i, (void**)&zvalue_p) != FAILURE) {
			// Success
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Passed array did not contain all keys");
			efree(new_data);
			return;
		}

		zvalue = *zvalue_p;

		new_data_offset = c_arr->entries[i].offset;

		switch(c_arr->entries[i].type) {
		case C_ARRAY_STRING:
			if (Z_TYPE_P(zvalue) == IS_NULL) {
				*((char **)(new_data + new_data_offset)) = NULL;
			} else {
				convert_to_string_ex(&zvalue);
				*((char **)(new_data + new_data_offset)) = estrdup(Z_STRVAL_P(zvalue));
			}
			break;
		case C_ARRAY_INTEGER:
			convert_to_long_ex(&zvalue);
			*((int *)(new_data + new_data_offset)) = (int)Z_LVAL_P(zvalue);
			break;
		case C_ARRAY_LONG:
			convert_to_long_ex(&zvalue);
			*((long *)(new_data + new_data_offset)) = Z_LVAL_P(zvalue);
			break;
		case C_ARRAY_FLOAT:
			convert_to_double_ex(&zvalue);
			*((float *)(new_data + new_data_offset)) = (float)Z_DVAL_P(zvalue);
			break;
		case C_ARRAY_DOUBLE:
			convert_to_double_ex(&zvalue);
			*((double *)(new_data + new_data_offset)) = Z_DVAL_P(zvalue);
			break;
		}
	}

	/* Expand array if necessary.  Unset values will have random data. */
	/* We use exponential increase in array size for pushing, up to a limit */
	index = c_arr->next_index++;
	if (c_arr->data == NULL) {
		new_length = 8;
		c_array_alloc_data(c_arr, new_length);
	} else if (index >= c_arr->len) {
		new_length = (c_arr->len < 16384 ? c_arr->len * 2 : c_arr->len + 16384);
		c_array_alloc_data(c_arr, new_length);
	}
	c_arr->max_index = MAX(c_arr->max_index, index);

	data_offset = c_arr->size * index;

	memcpy(c_arr->data + data_offset, new_data, c_arr->size);

	efree(new_data);
}
/* }}} */

/* {{{ proto void c_array_fetch(resource $c_array, int $index, mixed $entry)
 * Fetch value from C array
 */
PHP_FUNCTION(c_array_fetch)
{
	zval *zc_array;
	c_array *c_arr;
	long index;
	zval *zentry;
	int entry_index;
	char *entry_name;
	int data_offset;
	int i;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlz",
				&zc_array, &index, &zentry)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	if (!c_arr->final) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Array must be finalized before fetching data");
		return;
	}

	/* Fetching beyond end of array is an error */
	if ((int)index >= c_arr->len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Index out of range");
		return;
	}

	switch(Z_TYPE_P(zentry)) {
	case IS_LONG:
		entry_index = (int)Z_LVAL_P(zentry);
		if (entry_index >= c_arr->entries_len) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Entry index out of bounds");
			return;
		}
		break;
	case IS_STRING:
		entry_name = Z_STRVAL_P(zentry);
		entry_index = -1;
		for (i = 0; i < c_arr->entries_len; i++) {
			if (strcmp(entry_name, c_arr->entries[i].name) == 0) {
				entry_index = i;
				break;
			}
		}
		if (entry_index == -1) {
			php_error_docref1(NULL TSRMLS_CC, entry_name, E_WARNING, "Could not find entry name");
			return;
		}
		break;
	default:
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Entry id must be integer or string");
		return;
	}

	data_offset = c_arr->size * index + c_arr->entries[entry_index].offset;

	switch(c_arr->entries[entry_index].type) {
	case C_ARRAY_STRING:
		if (*((char **)(c_arr->data + data_offset)) == NULL) {
			ZVAL_NULL(return_value);
		} else {
			ZVAL_STRING(return_value, *((char **)(c_arr->data + data_offset)), 1);
		}
		break;
	case C_ARRAY_INTEGER:
		ZVAL_LONG(return_value, (long)*((int *)(c_arr->data + data_offset)));
		break;
	case C_ARRAY_LONG:
		ZVAL_LONG(return_value, *((long *)(c_arr->data + data_offset)));
		break;
	case C_ARRAY_FLOAT:
		ZVAL_DOUBLE(return_value, (double)*((float *)(c_arr->data + data_offset)));
		break;
	case C_ARRAY_DOUBLE:
		ZVAL_DOUBLE(return_value, *((double *)(c_arr->data + data_offset)));
		break;
	}
}
/* }}} */

/* {{{ proto void c_array_fetch_row(resource $c_array, int $index, int $result_type)
 * Fetch one row of data from C array
 */
PHP_FUNCTION(c_array_fetch_row)
{
	zval *zc_array;
	c_array *c_arr;
	long index;
	int data_offset;
	int i;
	zval *zdata;
	long result_type;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl|l",
				&zc_array, &index, &result_type)) {
		return;
	}

	if (ZEND_NUM_ARGS() == 2) {
		result_type = C_ARRAY_ASSOC;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	if (!c_arr->final) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Array must be finalized before fetching data");
		return;
	}

	/* Fetching beyond end of array is an error */
	if (index >= c_arr->len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Index out of range");
		return;
	}

	array_init(return_value);
	for (i = 0; i < c_arr->entries_len; i++) {
		data_offset = c_arr->size * index + c_arr->entries[i].offset;

		ALLOC_INIT_ZVAL(zdata);

		switch(c_arr->entries[i].type) {
		case C_ARRAY_STRING:
			if (*((char **)(c_arr->data + data_offset)) == NULL) {
				ZVAL_NULL(zdata);
			} else {
				ZVAL_STRING(zdata, *((char **)(c_arr->data + data_offset)), 1);
			}
			break;
		case C_ARRAY_INTEGER:
			ZVAL_LONG(zdata, (long)*((int *)(c_arr->data + data_offset)));
			break;
		case C_ARRAY_LONG:
			ZVAL_LONG(zdata, *((long *)(c_arr->data + data_offset)));
			break;
		case C_ARRAY_FLOAT:
			ZVAL_DOUBLE(zdata, (double)*((float *)(c_arr->data + data_offset)));
			break;
		case C_ARRAY_DOUBLE:
			ZVAL_DOUBLE(zdata, *((double *)(c_arr->data + data_offset)));
			break;
		}
		if (result_type & C_ARRAY_NUM) {
			add_index_zval(return_value, (long)i, zdata);
		}
		if (result_type & C_ARRAY_ASSOC) {
			if (result_type & C_ARRAY_NUM) {
				ZVAL_ADDREF(zdata);
			}
			add_assoc_zval(return_value, c_arr->entries[i].name, zdata);
		}
	}
}
/* }}} */

/* {{{ proto void c_array_set_length(resource $c_array, int $size)
 * Sets length of array
 */
PHP_FUNCTION(c_array_set_length)
{
	zval *zc_array;
	c_array *c_arr;
	long length;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl",
				&zc_array, &length)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	if (length < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Length must be >= 0");
		return;
	}

	/* Expand array if necessary.  Unset values will have random data. */
	c_array_alloc_data(c_arr, length);
}
/* }}} */

/* {{{ proto void c_array_fetch_length(resource $c_array)
 * Gets length of array
 */
PHP_FUNCTION(c_array_fetch_length)
{
	zval *zc_array;
	c_array *c_arr;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r",
				&zc_array)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	ZVAL_LONG(return_value, (long)c_arr->len);
}
/* }}} */

/* {{{ proto void c_array_fetch_max_index(resource $c_array)
 * Gets max index of array
 */
PHP_FUNCTION(c_array_fetch_max_index)
{
	zval *zc_array;
	c_array *c_arr;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r",
				&zc_array)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	ZVAL_LONG(return_value, (long)c_arr->max_index);
}
/* }}} */

/* Kludgly, but how else to get this data into the callback? */
static zval *sort_callback;
static c_array *sort_c_arr;
static zval *sort_arguments[2];
static int sort_result_type;

/* {{{ proto void c_array_sort(resource $c_array, string $callback, int $result_type, int $nmemb)
 * Sort an array
 * First optional argument is result type, eg C_ARRAY_BOTH
 * Second optional argument is number of elements to sort
 */
PHP_FUNCTION(c_array_sort)
{
	zval *zc_array;
	c_array *c_arr;
	long nmemb = 0;
	zval *zcallback;
	char *callback_name;
	long result_type = C_ARRAY_BOTH;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz|ll",
				&zc_array, &zcallback, &result_type, &nmemb)) {
		return;
	}
	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	if (nmemb < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot sort negative number of elements");
		return;
	}
	if (nmemb == 0) {
		nmemb = c_arr->max_index + 1;
		if (nmemb == 0) {
			// Array is empty
			return;
		}
	}

	/* verify that the callback will work */
	if (!zend_is_callable(zcallback, 0, &callback_name)) {
		php_error_docref1(NULL TSRMLS_CC, callback_name, E_WARNING,
				"3rd argument is not a valid callback");
		efree(callback_name);
		return;
	} else {
		efree(callback_name);
	}

	/* Kludgy, but how else to get this data into the callback? */
	sort_c_arr = c_arr;
	sort_callback = zcallback;
	sort_result_type = (int)result_type;

	/* Will be re-used in each callback */
	/* The fetches are just to set the structure of the array */
	sort_arguments[0] = c_array_fetch_row_from_data(c_arr, (const char *)c_arr->data, (int)result_type);
	sort_arguments[1] = c_array_fetch_row_from_data(c_arr, (const char *)c_arr->data, (int)result_type);

	qsort(c_arr->data, (int)nmemb, c_arr->size, c_array_sort_compar);

	zval_dtor(sort_arguments[0]);
	FREE_ZVAL(sort_arguments[0]);
	zval_dtor(sort_arguments[1]);
	FREE_ZVAL(sort_arguments[1]);
}
/* }}} */

/* {{{ proto int c_array_sort_compar(const void *a, const void *b)
 * Comparison function for array sorting
 */
int c_array_sort_compar(const void *a, const void *b)
{
	zval retval;
	int compar_retval;
	//printf("Compare %p and %p\n", a, b);fflush(stdout);

	c_array_fetch_row_from_data_alloced(sort_c_arr, (const char *)a, sort_result_type, &sort_arguments[0]);
	c_array_fetch_row_from_data_alloced(sort_c_arr, (const char *)b, sort_result_type, &sort_arguments[1]);
	if (call_user_function(EG(function_table), NULL,
				sort_callback, &retval, 2, sort_arguments TSRMLS_CC) == SUCCESS) {
		compar_retval = Z_LVAL(retval);
		zval_dtor(&retval);
	}

	return compar_retval;
}
/* }}} */

/* {{{ proto static zval *c_array_fetch_row_from_data(c_array *c_arr, char *data, int result_type)
 * Fetches row data from a pointer to an array entry
 * Intended for use in qsort() callback
 * Fourth argument is a fully allocated array element to store the result.
 */
static zval *c_array_fetch_row_from_data(c_array *c_arr, const char *data, int result_type) {
	zval *retval, *zdata;
	int data_offset;
	int i;

	ALLOC_INIT_ZVAL(retval);
	array_init(retval);
	for (i = 0; i < c_arr->entries_len; i++) {
		data_offset = c_arr->entries[i].offset;

		ALLOC_INIT_ZVAL(zdata);

		switch(c_arr->entries[i].type) {
		case C_ARRAY_STRING:
			if (Z_TYPE_P(zdata) == IS_NULL) {
				ZVAL_NULL(zdata);
			} else {
				ZVAL_STRING(zdata, *((char **)(data + data_offset)), 1);
			}
			break;
		case C_ARRAY_INTEGER:
			ZVAL_LONG(zdata, (long)*((int *)(data + data_offset)));
			break;
		case C_ARRAY_LONG:
			ZVAL_LONG(zdata, *((long*)(data + data_offset)));
			break;
		case C_ARRAY_FLOAT:
			ZVAL_DOUBLE(zdata, *((float *)(data + data_offset)));
			break;
		case C_ARRAY_DOUBLE:
			ZVAL_DOUBLE(zdata, *((double *)(data + data_offset)));
			break;
		}
		if (result_type & C_ARRAY_NUM) {
			add_index_zval(retval, i, zdata);
		}
		if (result_type & C_ARRAY_ASSOC) {
			if (result_type & C_ARRAY_NUM) {
				ZVAL_ADDREF(zdata);
			}
			add_assoc_zval(retval, c_arr->entries[i].name, zdata);
		}
	}

	return retval;
}
/* }}} */

/* {{{ proto static void c_array_fetch_row_from_data_alloced(c_array *c_arr, char *data, int result_type, zval **retval)
 * Fetches row data from a pointer to an array entry
 * Intended for use in qsort() callback
 * Fourth argument is a fully allocated array element to store the result.
 */
static void c_array_fetch_row_from_data_alloced(c_array *c_arr, const char *data, int result_type, zval **retval) {
	zval **zdata_p;
	int data_offset;
	int i;

	for (i = 0; i < c_arr->entries_len; i++) {
		data_offset = c_arr->entries[i].offset;

		if (result_type & C_ARRAY_NUM) {
			if (zend_hash_index_find(Z_ARRVAL_PP(retval), i, (void**)&zdata_p) != FAILURE) {
				// Success
			} else {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Passed array did not contain all indices");
				return;
			}
		} else {
			if (zend_hash_find(Z_ARRVAL_PP(retval), c_arr->entries[i].name, c_arr->entries[i].name_len + 1, (void**)&zdata_p) != FAILURE) {
				// Success
			} else {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Passed array did not contain all keys");
				return;
			}
		}

		switch(c_arr->entries[i].type) {
		case C_ARRAY_STRING:
			if (Z_TYPE_PP(zdata_p) == IS_NULL) {
				if (*((char **)(data + data_offset)) == NULL) {
					// Null to null.  Nothing to do
					break;
				} else {
					// Null to string
					ZVAL_STRING(*zdata_p, *((char **)(data + data_offset)), 1);
				}
			} else {
				if (Z_STRVAL_PP(zdata_p) != NULL) {
					efree(Z_STRVAL_PP(zdata_p));
				}
				if (*((char **)(data + data_offset)) == NULL) {
					// String to null
					ZVAL_NULL(*zdata_p);
				} else {
					// String to String
					ZVAL_STRING(*zdata_p, *((char **)(data + data_offset)), 1);
				}
			}
			break;
		case C_ARRAY_INTEGER:
			ZVAL_LONG(*zdata_p, (long)*((int *)(data + data_offset)));
			break;
		case C_ARRAY_LONG:
			ZVAL_LONG(*zdata_p, *((long *)(data + data_offset)));
			break;
		case C_ARRAY_FLOAT:
			ZVAL_DOUBLE(*zdata_p, *((float *)(data + data_offset)));
			break;
		case C_ARRAY_DOUBLE:
			ZVAL_DOUBLE(*zdata_p, *((double *)(data + data_offset)));
			break;
		}
	}
}
/* }}} */

/* {{{ c_array_dtor */
static void c_array_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	c_array *c_arr = (c_array *)rsrc->ptr;
	int i, j;
	char **cptr;

	/* TODO: What about string values inside entries and data? */
	for (i = 0; i < c_arr->entries_len; i++) {
		if (c_arr->entries[i].type == C_ARRAY_STRING) {
			/* Slow but necessary.  Maybe a common memory pool would be better */
			for (j = 0; j < c_arr->len; j++) {
				cptr = (char **)(c_arr->data + (j * c_arr->size) + c_arr->entries[i].offset);
				if (*cptr != NULL) {
					efree(*cptr);
				}
			}
		}
		efree(c_arr->entries[i].name); // Cannot be null
	}
	if (c_arr->entries != NULL) {
		efree(c_arr->entries);
	}
	if (c_arr->data != NULL) {
		efree(c_arr->data);
	}
	efree(c_arr);
}
/* }}} */

/* {{{ c_array_alloc_data() */
static void c_array_alloc_data(c_array *c_arr, int new_len)
{
	if (c_arr->len >= new_len) return; // Cannot reduce size

	if (c_arr->data == NULL) {
		c_arr->data = ecalloc(1, c_arr->size * new_len);
		c_arr->len = new_len;
	} else {
		c_arr->data = erealloc(c_arr->data, c_arr->size * new_len);
		memset((void *)(c_arr->data + c_arr->size * c_arr->len), 0, c_arr->size * (new_len - c_arr->len));
		c_arr->len = new_len;
	}
}
/* }}} */

/* {{{ proto void c_array_serialize(resource $c_array)
 * Return serialized array
 */
PHP_FUNCTION(c_array_serialize)
{
	char *serialized;
	zval *zc_array;
	c_array *c_arr;
	int serialized_len;
	int allocated_len;
	int index, entry_num;
	long return_type = C_ARRAY_NUM;
	char *dptr;
	char buf[8192];
	char *array_entry_header;
	int array_entry_header_len;
	char **cptr;
	int cptr_len;


	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|l", &zc_array, &return_type)) {
		return;
	}

	if (return_type == C_ARRAY_BOTH) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot specify C_ARRAY_BOTH for serializing");
		return;
	}

	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	if (!c_arr->final) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Array must be finalized before fetching data");
		return;
	}

	serialized_len = 0; // Length of data
	allocated_len = 8192; // Space allocated
	serialized = emalloc(allocated_len);

	sprintf(buf, "a:%d:{", c_arr->max_index + 1);
	strcpy(serialized, buf);
	serialized_len += strlen(buf);

	sprintf(buf, "a:%d:{", c_arr->entries_len);
	array_entry_header = estrdup(buf);
	array_entry_header_len = strlen(array_entry_header);

	dptr = c_arr->data;
	for (index = 0; index <= c_arr->max_index; index++) {
		// Outer array index.  Allocate enough for three integer values
		C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 256);
		sprintf(buf, "i:%d;", index);
		strcpy(serialized + serialized_len, buf);
		serialized_len += strlen(buf);
		strcpy(serialized + serialized_len, array_entry_header);
		serialized_len += array_entry_header_len;

		for (entry_num = 0; entry_num < c_arr->entries_len; entry_num++) {
			if (return_type == C_ARRAY_NUM) {
				C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 64);
				sprintf(buf, "i:%d;", entry_num);
				strcpy(serialized + serialized_len, buf);
				serialized_len += strlen(buf);
			} else {
				C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, c_arr->entries[entry_num].name_len + 64);
				sprintf(buf, "s:%d:\"", c_arr->entries[entry_num].name_len);
				strcpy(serialized + serialized_len, buf);
				serialized_len += strlen(buf);
	
				strcpy(serialized + serialized_len, c_arr->entries[entry_num].name);
				serialized_len += c_arr->entries[entry_num].name_len;
	
				strcpy(serialized + serialized_len, "\";");
				serialized_len += 2;
			}
	
			switch(c_arr->entries[entry_num].type) {
			case C_ARRAY_STRING:
				cptr = (char **)(dptr);
				if (*cptr == NULL) {
					C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 2 + 1);
					strcpy(serialized + serialized_len, "N;");
					serialized_len += 2;
				} else {
					cptr_len = strlen(*cptr);
					C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, cptr_len + 64);

					sprintf(buf, "s:%d:\"", cptr_len);
					strcpy(serialized + serialized_len, buf);
					serialized_len += strlen(buf);

					strcpy(serialized + serialized_len, *cptr);
					serialized_len += cptr_len;

					strcpy(serialized + serialized_len, "\";");
					serialized_len += 2;
				}
				break;
			case C_ARRAY_INTEGER:
				sprintf(buf, "i:%d;", *((int *)(dptr)));
				C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 64);
				strcpy(serialized + serialized_len, buf);
				serialized_len += strlen(buf);
				break;
			case C_ARRAY_LONG:
				sprintf(buf, "i:%ld;", *((long *)(dptr)));
				C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 64);
				strcpy(serialized + serialized_len, buf);
				serialized_len += strlen(buf);
				break;
			case C_ARRAY_FLOAT:
				php_gcvt((double)*((float *)(dptr)), C_ARRAY_FLOAT_SERIALIZE_PRECISION, '.', 'E', buf);
				C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, C_ARRAY_FLOAT_SERIALIZE_PRECISION + 32);
				strcpy(serialized + serialized_len, "d:");
				serialized_len += 2;
				strcpy(serialized + serialized_len, buf);
				serialized_len += strlen(buf);
				strcpy(serialized + serialized_len, ";");
				serialized_len += 1;
				break;
			case C_ARRAY_DOUBLE:
				php_gcvt(*((double *)(dptr)), C_ARRAY_DOUBLE_SERIALIZE_PRECISION, '.', 'E', buf);
				C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, C_ARRAY_DOUBLE_SERIALIZE_PRECISION + 32);
				strcpy(serialized + serialized_len, "d:");
				serialized_len += 2;
				strcpy(serialized + serialized_len, buf);
				serialized_len += strlen(buf);
				strcpy(serialized + serialized_len, ";");
				serialized_len += 1;
				break;
			}

			dptr += c_arr->entries[entry_num].size;
		}
		C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 1 + 1);
		strcpy(serialized + serialized_len, "}");
		serialized_len +=1;
	}
	C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 1 + 1);
	strcpy(serialized + serialized_len, "}");
	serialized_len +=1;

	efree(array_entry_header);

	serialized = erealloc(serialized, serialized_len + 1);
	serialized[serialized_len] = 0; // Terminating null
	ZVAL_STRING(return_value, serialized, 0);
}
/* }}} */

/* {{{ proto void c_array_serialize_row(resource $c_array, int $index, int $return_type)
 * Return serialized row from array
 */
PHP_FUNCTION(c_array_serialize_row)
{
	char *serialized;
	zval *zc_array;
	c_array *c_arr;
	int serialized_len;
	int allocated_len;
	int entry_num;
	long index;
	long return_type = C_ARRAY_NUM;
	char *dptr;
	char buf[8192];
	char **cptr;
	int cptr_len;


	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl|l", &zc_array, &index, &return_type)) {
		return;
	}

	if (return_type == C_ARRAY_BOTH) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot specify C_ARRAY_BOTH for serializing");
		return;
	}

	ZEND_FETCH_RESOURCE(c_arr, c_array *, &zc_array, -1, le_c_array_name, le_c_array_rsrc);

	if (!c_arr->final) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Array must be finalized before fetching data");
		return;
	}

	serialized_len = 0; // Length of data
	allocated_len = 8192; // Space allocated
	serialized = emalloc(allocated_len);

	sprintf(buf, "a:%d:{", c_arr->entries_len);
	strcpy(serialized, buf);
	serialized_len += strlen(buf);

	dptr = c_arr->data + c_arr->size * index;
	for (entry_num = 0; entry_num < c_arr->entries_len; entry_num++) {
		if (return_type == C_ARRAY_NUM) {
			C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 64);
			sprintf(buf, "i:%d;", entry_num);
			strcpy(serialized + serialized_len, buf);
			serialized_len += strlen(buf);
		} else {
			C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, c_arr->entries[entry_num].name_len + 64);
			sprintf(buf, "s:%d:\"", c_arr->entries[entry_num].name_len);
			strcpy(serialized + serialized_len, buf);
			serialized_len += strlen(buf);

			strcpy(serialized + serialized_len, c_arr->entries[entry_num].name);
			serialized_len += c_arr->entries[entry_num].name_len;

			strcpy(serialized + serialized_len, "\";");
			serialized_len += 2;
		}

		switch(c_arr->entries[entry_num].type) {
		case C_ARRAY_STRING:
			cptr = (char **)(dptr);
			if (*cptr == NULL) {
				C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 2 + 1);
				strcpy(serialized + serialized_len, "N;");
				serialized_len += 2;
			} else {
				cptr_len = strlen(*cptr);
				C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, cptr_len + 64);

				sprintf(buf, "s:%d:\"", cptr_len);
				strcpy(serialized + serialized_len, buf);
				serialized_len += strlen(buf);

				strcpy(serialized + serialized_len, *cptr);
				serialized_len += cptr_len;

				strcpy(serialized + serialized_len, "\";");
				serialized_len += 2;
			}
			break;
		case C_ARRAY_INTEGER:
			sprintf(buf, "i:%d;", *((int *)(dptr)));
			C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 64);
			strcpy(serialized + serialized_len, buf);
			serialized_len += strlen(buf);
			break;
		case C_ARRAY_LONG:
			sprintf(buf, "i:%ld;", *((long *)(dptr)));
			C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 64);
			strcpy(serialized + serialized_len, buf);
			serialized_len += strlen(buf);
			break;
		case C_ARRAY_FLOAT:
			php_gcvt((double)*((float *)(dptr)), C_ARRAY_FLOAT_SERIALIZE_PRECISION, '.', 'E', buf);
			C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, C_ARRAY_FLOAT_SERIALIZE_PRECISION + 32);
			strcpy(serialized + serialized_len, "d:");
			serialized_len += 2;
			strcpy(serialized + serialized_len, buf);
			serialized_len += strlen(buf);
			strcpy(serialized + serialized_len, ";");
			serialized_len += 1;
			break;
		case C_ARRAY_DOUBLE:
			php_gcvt(*((double *)(dptr)), C_ARRAY_DOUBLE_SERIALIZE_PRECISION, '.', 'E', buf);
			C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, C_ARRAY_DOUBLE_SERIALIZE_PRECISION + 32);
			strcpy(serialized + serialized_len, "d:");
			serialized_len += 2;
			strcpy(serialized + serialized_len, buf);
			serialized_len += strlen(buf);
			strcpy(serialized + serialized_len, ";");
			serialized_len += 1;
			break;
		}

		dptr += c_arr->entries[entry_num].size;
	}
	C_ARRAY_ENSURE_ALLOCED(serialized, serialized_len, allocated_len, 1 + 1);
	strcpy(serialized + serialized_len, "}");
	serialized_len +=1;

	serialized = erealloc(serialized, serialized_len + 1);
	serialized[serialized_len] = 0; // Terminating null
	ZVAL_STRING(return_value, serialized, 0);
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
