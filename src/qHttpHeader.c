/***************************************************************************
 * qDecoder - Web Application Interface for C/C++    http://www.qDecoder.org
 *
 * Copyright (C) 2007 Seung-young Kim.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 **************************************************************************/

#include "qDecoder.h"
#include "qInternal.h"


/**********************************************
** Static Values Definition used only internal
**********************************************/

static int _content_type_flag = 0;


/**********************************************
** Usage : qContentType(mime type);
** Do    : Print content type once.
**********************************************/
void qContentType(char *mimetype) {
	if (_content_type_flag) return;

	printf("Content-Type: %s%c%c", mimetype, 10, 10);
	_content_type_flag = 1;
}

/**********************************************
** Usage : qGetContentFlag();
** Return: If qContentType() is executed before, returns 1. Or returns 0.
** Do    : Check execution of qContentType().
**********************************************/
int qGetContentFlag(void) {
	return _content_type_flag;
}

/**********************************************
** Usage : qResetContentFlag();
** Do    : Sets the internal flag of qContentType() to the initial status.
**********************************************/
void qResetContentFlag(void) {
	_content_type_flag = 0;
}

/**********************************************
** Usage : qRedirect(url);
** Do    : Redirect page using Location response-header.
**********************************************/
void qRedirect(char *url) {
	if (qGetContentFlag() == 1) qError("qRedirect(): qRedirect() must be called before qContentType() and any stream out.");
	printf("Location: %s\n\n", url);
}

/**********************************************
** Usage : qJavaScript(...);
** Do    : Print out some JavaScript code.
**********************************************/
void qJavaScript(char *format, ...) {
	char jscode[1024];
	int status;
	va_list arglist;

	va_start(arglist, format);
	status = vsprintf(jscode, format, arglist);
	if (strlen(jscode) + 1 > sizeof(jscode) || status == EOF) qError("qJavaScript(): Message is too long or invalid.");
	va_end(arglist);

	qContentType("text/html");
	printf("<html>\n");
	printf("<head>\n");
	printf("<script language='JavaScript'>\n");
	printf("%s\n", jscode);
	printf("</script>\n");
	printf("</head>\n");
	printf("</html>\n");
}
