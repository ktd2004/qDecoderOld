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
 * @file qFile.c File Handling API
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#ifdef _WIN32	/* to use setmode() function for converting WIN32's stream mode to _O_BINARY */
#include <io.h>
#endif
#include "qDecoder.h"
#include "qInternal.h"

/**********************************************
** Usage : qFileOpen(path, mode);
** Return: Same as fclose().
** Do    : Open file with file lock.
**********************************************/
FILE *qFileOpen(char *path, char *mode) {
	FILE *stream;

	if ((stream = fopen(path, mode)) == NULL) return NULL;
	_flockopen(stream);
	return stream;
}

/**********************************************
** Usage : qFileClose(stream);
** Return: Same as fclose().
** Do    : Close the file stream which is opened by qfopen().
**********************************************/
int qFileClose(FILE *stream) {
	_flockclose(stream);
	return fclose(stream);
}


/*********************************************
** Usage : qFileReadString(file pointer);
** Return: Success string pointer, End of file NULL.
** Do    : Read text stream.
**********************************************/
char *qFileReadString(FILE *fp) {
	int memsize;
	int c, c_count;
	char *string = NULL;

	for (memsize = 1024, c_count = 0; (c = fgetc(fp)) != EOF;) {
		if (c_count == 0) {
			string = (char *)malloc(sizeof(char) * memsize);
			if (string == NULL) {
				DEBUG("Memory allocation failed.");
				return NULL;
			}
		} else if (c_count == memsize - 1) {
			char *stringtmp;

			memsize *= 2;

			/* Here, we do not use realloc(). Because sometimes it is unstable. */
			stringtmp = (char *)malloc(sizeof(char) * (memsize + 1));
			if (stringtmp == NULL) {
				DEBUG("Memory allocation failed.");
				free(string);
				return NULL;
			}
			memcpy(stringtmp, string, c_count);
			free(string);
			string = stringtmp;
		}
		string[c_count++] = (char)c;
	}

	if (c_count == 0 && c == EOF) return NULL;
	string[c_count] = '\0';

	return string;
}

/*********************************************
** Usage : qfGetLine(file pointer);
** Return: Success string pointer, End of file NULL.
** Do    : Read one line from file pointer without length
**         limitation. String will be saved into dynamically
**         allocated memory. The newline, if any, is retained.
**********************************************/
char *qfGetLine(FILE *fp) {
	int memsize;
	int c, c_count;
	char *string = NULL;

	for (memsize = 1024, c_count = 0; (c = fgetc(fp)) != EOF;) {
		if (c_count == 0) {
			string = (char *)malloc(sizeof(char) * memsize);
			if (string == NULL) {
				DEBUG("Memory allocation failed.");
				return NULL;
			}
		} else if (c_count == memsize - 1) {
			char *stringtmp;

			memsize *= 2;

			/* Here, we do not use realloc(). Because sometimes it is unstable. */
			stringtmp = (char *)malloc(sizeof(char) * (memsize + 1));
			if (stringtmp == NULL) {
				DEBUG("Memory allocation failed.");
				free(string);
				return NULL;
			}
			memcpy(stringtmp, string, c_count);
			free(string);
			string = stringtmp;
		}
		string[c_count++] = (char)c;
		if ((char)c == '\n') break;
	}

	if (c_count == 0 && c == EOF) return NULL;
	string[c_count] = '\0';

	return string;
}

/**********************************************
**Usage : qCheckFile(filename);
** Return: If file exist returns true. Or returns false.
** Do    : Check filethat file is existGet environment of CGI.
**********************************************/
bool qCheckFile(char *format, ...) {
	struct stat finfo;
	char filename[1024];
	va_list arglist;

	va_start(arglist, format);
	vsnprintf(filename, sizeof(filename)-1, format, arglist);
	filename[sizeof(filename)-1] = '\0';
	va_end(arglist);

	if (stat(filename, &finfo) < 0) return false;

	return true;
}

/**********************************************
** Usage : qCatFile(filename);
** Return: Success number of characters, Fail -1.
** Do    : Stream out file.
**********************************************/
int qCatFile(char *format, ...) {
	FILE *fp;
	char filename[1024];
	va_list arglist;
	int c, counter;

	va_start(arglist, format);
	vsnprintf(filename, sizeof(filename)-1, format, arglist);
	filename[sizeof(filename)-1] = '\0';
	va_end(arglist);

#ifdef _WIN32
	setmode(fileno(stdin), _O_BINARY);
	setmode(fileno(stdout), _O_BINARY);
#endif

	if ((fp = fopen(filename, "rb")) == NULL) return -1;
	for (counter = 0; (c = fgetc(fp)) != EOF; counter++) {
		putc(c, stdout);
	}

	fclose(fp);
	return counter;
}

/**********************************************
** Usage : qReadFile(filename, integer pointer to store file size);
** Return: Success stream pointer, Fail NULL.
** Do    : Read file to malloced memory.
**********************************************/
char *qReadFile(char *filename, int *size) {
	FILE *fp;
	struct stat fstat;
	char *sp, *tmp;
	int c, i;

	if (size != NULL) *size = 0;
	if (stat(filename, &fstat) < 0) return NULL;
	if ((fp = fopen(filename, "rb")) == NULL) return NULL;

	sp = (char *)malloc(fstat.st_size + 1);
	for (tmp = sp, i = 0; (c = fgetc(fp)) != EOF; tmp++, i++) *tmp = (char)c;
	*tmp = '\0';

	if (fstat.st_size != i) {
		DEBUG("Size(File:%d, Readed:%d) mismatch.", (int)fstat.st_size, i);
		free(sp);
		return NULL;
	}
	fclose(fp);
	if (size != NULL) *size = i;
	return sp;
}

/**********************************************
** Usage : qSaveStr(string pointer, string size, filename, mode)
** Return: Success number bytes stored, File open fail -1.
** Do    : Store string to file.
**********************************************/
int qSaveStr(char *sp, int spsize, char *filename, char *mode) {
	FILE *fp;
	int i;
	if ((fp = fopen(filename, mode)) == NULL) return -1;
	for (i = 0; i < spsize; i++) fputc(*sp++, fp);
	fclose(fp);

	return i;
}

/**********************************************
** Usage : qFileSize(filename);
** Return: Size of file in byte, File not found -1.
**********************************************/
long qFileSize(char *filename) {
	struct stat finfo;

	if (stat(filename, &finfo) < 0) return -1;

	return finfo.st_size;
}

/**********************************************
** Usage : qCmd(external command);
** Return: Size of file in byte, File not found -1.
**********************************************/
char *qCmd(char *cmd) {
	FILE *fp;
	char *str;

	fp = popen(cmd, "r");
	if (fp == NULL) return NULL;
	str = qFileReadString(fp);
	pclose(fp);

	return str;
}
