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
 * @file qArg.c Pattern Matching & String Search API
 *
 * A group of functions related to search words classifies query character strings
 * on the basis of spaces and double quotation marks(") and then
 * stores them in the lists. And it provides a group of functions connected with
 * the matching tests and display of target character strings.
 *
 * �˻��� ���� �Լ����� ���� ���ڿ��� �����̽��� ū����ǥ(") �������� ������
 * ����Ʈ�� ����ָ�, ��� ���ڿ����� ��Ī �׽�Ʈ�� ��¿� ���õ� �Լ����� �����Ѵ�.
 *
 * @code
 *   ---- Example --------------------------------------------
 *   Query Input: I am a "pretty girl"
 *   ---------------------------------------------------------
 *         |  |
 *         V  V
 *   ---- qArgMake() -----------------------------------------
 *   qlist[0] = I
 *   qlist[1] = am
 *   qlist[2] = a
 *   qlist[3] = pretty girl
 *   qlist[4] = NULL
 *   Return: 4 (4 Tokens)
 *   ---------------------------------------------------------
 *         |  |
 *         V  V
 *   ---- qArgPrint() ----------------------------------------
 *   'I' (1 bytes)
 *   'am' (2 bytes)
 *   'a' (1 bytes)
 *   'pretty girl' (11 bytes)
 *   ---------------------------------------------------------
 *         |  |
 *         V  V
 *   ---- qArgMatch() ----------------------------------------
 *   Target String: Hi, I'm a pretty boy. Are you pretty girl?
 *                   =  =   =             =       ===========
 *                   0  0   2             2             3
 *   Return: 3 (3 matches: qlist[0], qlist[2], qlist[3])
 *   ---------------------------------------------------------
 *         |  |
 *         V  V
 *   ---- qArgEmprint() --------------------------------------
 *   Target String..: Hi, I'm a pretty boy. Are you pretty girl?
 *   Result.........: Hi, I'm a pretty boy. Are you pretty girl?
 *                     =  =   =             =       ===========
 *                     1  2   3             4             5
 *   Return: 5 (5 matches)
 *   ---------------------------------------------------------
 * @endcode
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "qDecoder.h"

/**
 * Split string by space and double quotation(") then
 * stores each token into token array.
 *
 * @param str	key words source string
 * @param qlist splitted tokens will be stored here
 *
 * @return	number of parsed tokens.
 *
 * @note
 * @code
 *   char *query="I am a \"pretty girl\".";
 *   char *qlist[MAX_TOKENS];
 *   int queries;
 *   queries = qArgMake(query, qlist);
 * @endcode
 */
int qArgMake(char *str, char **qlist) {
	char *query, *sp, *qp;
	int argc;

	query = sp = qRemoveSpace(strdup(str));

	for (argc = 0; *sp != '\0';) {
		switch (*sp) {
			case ' ': { /* skip space */
				sp++;
				break;
			}
			case '"': { /* Double quotation arounded string */
				qlist[argc] = qp = (char *)malloc(strlen(query) +  1);
				for (sp++; *sp != '\0'; sp++, qp++) {
					if (*sp == '"') {
						sp++;
						break;
					}
					*qp = *sp;
				}
				*qp = '\0';
				if (strlen(qlist[argc]) > 0) argc++;
				break;
			}
			default: {
				qlist[argc] = qp = (char *)malloc(strlen(query) +  1);
				for (; *sp != '\0'; sp++, qp++) {
					if (*sp == ' ' || *sp == '"') break;
					*qp = *sp;
				}
				*qp = '\0';
				argc++;
				break;
			}
		}
	}
	qlist[argc] = NULL;
	free(query);
	return argc;
}

/**
 * Performs token-matching(case insensitive) tests.
 *
 * Counts only once the same tokens even if they are matched repeatedly.
 * Refer to the return value of qArgemprint() for the total matching counts
 * in a character string including repeated matchings. Being divided into
 * a hundred parts by the total number of the tokens acquired at qArgMake(),
 * these values can be used for calculating the search accuracy.
 *
 * ���ڿ��� ���� ��ҹ��ڸ� �������� �ʰ� ��ū ��Ī �׽�Ʈ�� �Ѵ�.
 *
 * ���� ��ū�� ���ؼ��� �ߺ��Ǿ� ��Ī�Ǿ 1ȸ�� ����Ѵ�. �ߺ� ��Ī��
 * �����Ͽ� ���ڿ��� �� ��Ī ī��Ʈ�� ���ؼ��� qArgEmprint()�� ���Ͱ��� �����϶�.
 * �� ������ qArgMake()���� ���� ��ū�� �� ������ ������Ͽ� �˻� ��Ȯ����
 * ����ϴµ� ���� �� �ִ�.
 *
 * @param str	target string to test
 * @param qlist query tokens which is generated by qArgMake()
 *
 * @return	the number of token(qlist) found in str.
 *
 * @code
 *   char *qlist[MAX_TOKENS];
 *   int queries, matches;
 *   queries = qArgMake("I am a \"pretty girl\".", qlist);
 *   matches = qArgMatch("Hi, I'm a pretty boy. Are you pretty girl?", qlist);
 * @endcode
 */
int qArgMatch(char *str, char **qlist) {
	char **qp;
	int i;

	if (!*qlist) return 0;
	for (qp = qlist, i = 0; *qp != NULL; qp++) if (qStristr(str, *qp)) i++;

	return i;
}

/**
 * Bold-prints the parts matched((case insensitive) with tokens in a target string.
 *
 * ���ڿ����� ��ū�� ��Ī�Ǵ� �κ��� ����(bold)ó���Ͽ� ����Ѵ�. ��ҹ��ڸ� �������� �ʴ´�.
 *
 * @param mode	same as qPrintf()'s mode value. normally 1 can be used
 * @param str	target string to print
 * @param qlist query tokens which is generated by qArgMake()
 *
 * @return	the number of tokens found in a character string.
 *		unlike qArgMatch(), this returns all the matching counts
 *		including repeated matchings.
 *
 * @code
 *   qArgEmprint(1, "Hi, I'm a pretty boy. Are you pretty girl?", qlist);
 * @endcode
 *
 * @see qPrintf()
 */
int qArgEmprint(int mode, char *str, char **qlist) {
	char *sp, *freestr, *buf, *bp, *op;
	int  i, j, flag, matches;

	if (!*qlist) {
		qPuts(mode, str);
		return 0;
	}

	/* Set character pointer */
	op = str;
	sp = freestr = strdup(str);
	qStrupr(sp);

	if ((bp = buf = (char *)malloc(strlen(str) + 1)) == NULL) qError("Memory allocation fail.");

	for (matches = 0; *sp != '\0';) {
		for (i = 0, flag = 0; qlist[i] != NULL; i++) {
			if (!qStrincmp(sp, qlist[i], strlen(qlist[i]))) {
				*bp = '\0'; /* Mark string end */
				qPuts(mode, buf); /* flash buffer */
				bp = buf; /* reset buffer pointer */
				printf("<b>");
				for (j = 1; j <= (int)strlen(qlist[i]); j++) {
					printf("%c", *op++);
					sp++;
				}
				printf("</b>");
				flag = 1;
				matches++;
				break;
			}
		}
		if (flag == 0) {
			*bp++ = *op++;
			sp++;
		}
	}
	*bp = '\0'; /* Mark string end */
	qPuts(mode, buf); /* Flash buffer */

	free(buf);
	free(freestr);

	return matches;
}

/**
 * Print out all parsed tokens for debugging.
 *
 * @param qlist query tokens which is generated by qArgMake()
 *
 * @return	the number of tokens in qlist.
 *
 * @code
 *   qArgPrint(qlist);
 * @endcode
 */
int qArgPrint(char **qlist) {
	char **qp;
	int amount;

	qContentType("text/html");

	for (amount = 0, qp = qlist; *qp; amount++, qp++) {
		printf("'%s' (%d bytes)<br>\n" , *qp, strlen(*qp));
	}

	return amount;
}

/**
 * Deallocates qlist generated by qArgMake().
 *
 * @param qlist query tokens which is generated by qArgMake()
 *
 * @code
 *   qArgFree(qlist);
 * @endcode
 */
void qArgFree(char **qlist) {
	char **qp;
	for (qp = qlist; *qp; qp++) free(*qp);
	*qlist = NULL;
}
