/*
 * Copyright 2008 The qDecoder Project. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE QDECODER PROJECT ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE QDECODER PROJECT BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "qDecoder.h"

int main(void) {
	Q_ENTRY *req = qCgiRequestParse(NULL);

	/* fetch queries */
	time_t expire = (time_t)qEntryGetInt(req, "expire");
	const char *mode = qEntryGetStr(req, "mode");
	const char *name   = qEntryGetStr(req, "name");
	const char *value  = qEntryGetStr(req, "value");

	/* start session. */
	Q_ENTRY *sess = qSessionInit(req, NULL);

	/* Mose case, you don't need to set timeout. this is just example */
	if (expire > 0) qSessionSetTimeout(sess, expire);

	switch (mode[0]) {
		case 's': {
			qEntryPutStr(sess, name, value, true);
			break;
		}
		case 'i': {
			qEntryPutInt(sess, name, atoi(value), true);
			break;
		}
		case 'r': {
			qEntryRemove(sess, name);
			break;
		}
		case 'd': {
			qSessionDestroy(sess);
			qCgiResponseSetContentType(req, "text/html");
			printf("Session destroied.\n");
			return 0;
			break;
		}
	}
	/* screen out */
	qCgiResponseSetContentType(req, "text/plain");
	qEntryPrint(sess, stdout, true);

	/* save session & free allocated memories */
	qSessionSave(sess);
	qEntryFree(sess);
	qEntryFree(req);
	return 0;
}
