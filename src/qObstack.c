/**************************************************************************
 * qDecoder - Web Application Interface for C/C++   http://www.qDecoder.org
 *
 * Copyright (C) 2007 Seung-young Kim.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

/**
 * @file qObstack.c Another simple implementation of GNU obstack
 *
 * @note
 * This API can be disabled(excluded in the library) with compile option
 * -DWITHOUT_OBSTACK.
 *
 * @code
 *   [Code sample]
 *   Q_OBSTACK *obstack;
 *   char *final;
 *   int *tmp = "CDE";
 *
 *   obstack = qObstackInit();		// get new obstack
 *
 *   qObstackGrowStr(obstack, "AB");	// no need to supply size
 *   qObstackGrowStrf(obstack, "%s", tmp);	// for formatted string
 *   qObstackGrow(obstack, "FGH", 3);	// same effects as above but this can
 *					// be used for object or binary
 *
 *   final = (char *)qObstackFinish(obstack);
 *   printf("%s\n", final);
 *
 *   qObstackFree(obstack);
 *
 *   [Result]
 *   ABCDEFGH
 * @endcode
 *
 *ABCDEFGH
 */

#ifndef WITHOUT_OBSTACK

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "qDecoder.h"

/**
 * Under-development
 *
 * @since not released yet
 */
Q_OBSTACK *qObstackInit(void) {
	Q_OBSTACK *obstack;

	obstack = (Q_OBSTACK *)malloc(sizeof(Q_OBSTACK));
	if(obstack == NULL) return NULL;

	memset((void *)obstack, 0, sizeof(Q_OBSTACK));
	return obstack;
}

/**
 * Under-development
 *
 * @since not released yet
 */
bool qObstackGrow(Q_OBSTACK *obstack, void *data, int size) {
	if(obstack == NULL || data == NULL || size <= 0) return false;

	char *name;
	void *value;

	name = (char *)malloc(10+1);
	if(name == NULL) return false;
	sprintf(name, "%d", size);

	value = (void *)malloc(size);
	if(value == NULL) { free(name); return false; }
	memcpy(value, data, size);

	obstack->last = qEntryAdd(obstack->last, name, (char *)value, 2);
	if(obstack->first == NULL) obstack->first = obstack->last;

	obstack->size += size;
	obstack->num++;

	return true;
}

/**
 * Under-development
 *
 * @since not released yet
 */
bool qObstackGrowStr(Q_OBSTACK *obstack, char *str) {
	return qObstackGrow(obstack, (void *)str, strlen(str));
}

/**
 * Under-development
 *
 * @since not released yet
 */
bool qObstackGrowStrf(Q_OBSTACK *obstack, char *format, ...) {
	if(obstack == NULL) return false;

	char str[1024];
	va_list arglist;

	va_start(arglist, format);
	vsnprintf(str, sizeof(str)-1, format, arglist);
	str[sizeof(str)-1] = '\0';
	va_end(arglist);

	return qObstackGrow(obstack, (void *)str, strlen(str));
}

/**
 * Under-development
 *
 * @since not released yet
 */
void *qObstackFinish(Q_OBSTACK *obstack) {
	if(obstack == NULL) return NULL;

	Q_ENTRY *entries;
	void *dp;

	if(obstack->final != NULL) free(obstack->final);

	obstack->final = dp = (void *)malloc(obstack->size + 1);
	if(obstack->final == NULL) return NULL;

	for (entries = obstack->first; entries; entries = entries->next) {
		int size = atoi(entries->name);
		memcpy(dp, (void *)entries->value, size);
		dp += size;
	}
	*((char *)dp) = '\0';

	return obstack->final;
}

/**
 * Under-development
 *
 * @since not released yet
 */
void *qObstackGetFinal(Q_OBSTACK *obstack) {
	if(obstack == NULL) return NULL;
	return obstack->final;
}

/**
 * Under-development
 *
 * @since not released yet
 */
int qObstackGetSize(Q_OBSTACK *obstack) {
	if(obstack == NULL) return 0;
	return obstack->size;
}

/**
 * Under-development
 *
 * @since not released yet
 */
int qObstackGetNum(Q_OBSTACK *obstack) {
	if(obstack == NULL) return 0;
	return obstack->num;
}

/**
 * Under-development
 *
 * @since not released yet
 */
bool qObstackFree(Q_OBSTACK *obstack) {
	if(obstack == NULL) return false;
	qEntryFree(obstack->first);
	free(obstack);
	return true;
}

#endif /* WITHOUT_OBSTACK */
