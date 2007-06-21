/************************************************************************
qDecoder - Web Application Interface for C/C++    http://www.qDecoder.org

Copyright (C) 2001 The qDecoder Project.
Copyright (C) 1999,2000 Hongik Internet, Inc.
Copyright (C) 1998 Nobreak Technologies, Inc.
Copyright (C) 1996,1997 Seung-young Kim.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


Copyright Disclaimer:
  Hongik Internet, Inc., hereby disclaims all copyright interest.
  President, Christopher Roh, 6 April 2000

  Nobreak Technologies, Inc., hereby disclaims all copyright interest.
  President, Yoon Cho, 6 April 2000

  Seung-young Kim, hereby disclaims all copyright interest.
  Author, Seung-young Kim, 6 April 2000

Author:
  Seung-young Kim <wolkykim(at)ziom.co.kr>
************************************************************************/
/* $Revision$ */

#ifndef _QDECODER_H
#define _QDECODER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <libgen.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
/* WIN32 ONLY */
/* to use setmode() function for converting WIN32's stream mode to _O_BINARY */
#include <io.h>
#else
/* UNIX ONLY */
#include <dirent.h>	/* to use opendir() */
#include <unistd.h>	/* to use unlink() */
#include <sys/time.h>	/* to use struct timeval */
#include <sys/file.h>	/* to use flock */
#endif

#ifdef _mysql_h
#define _Q_SUPPORT_DATABASE	"MYSQL"
#define _Q_WITH_MYSQL		(1)
#endif

/*
 * Linked List Structure
 */
typedef struct Q_ENTRY {
	char *name;
	char *value;
	struct Q_ENTRY *next;
} Q_ENTRY;

/*
 * Structure for Independent Database Interface
 * You must include database header file before including qDecoder.h
 * ex) #include "mysql.h"
 *     #include "qDecoder.h"
 */
#ifdef _Q_SUPPORT_DATABASE
typedef struct {
	int connected;		// i1 if opened, 0 if closed;

	struct {
		char	dbtype[16+1];
		char	addr[31+1];
		int	port;
		char	username[31+1];
		char	password[31+1];
		char	database[31+1];
		int	autocommit;
	}
	info;

	// for mysql database
#ifdef _Q_WITH_MYSQL
	MYSQL		mysql;
#endif
} Q_DB;

/*
 * Structure for Database Result Set
 */
typedef struct {
#ifdef _Q_WITH_MYSQL
	MYSQL_RES	*rs;
	MYSQL_FIELD	*fields;
	MYSQL_ROW	row;
	int		rows;
	int		cols;
	int		cursor;
#endif
} Q_DBRESULT;
#endif

/*
 * Structure for Log
 */
typedef struct {
	char	szLogBase[1024];
	char	szFilenameFormat[256];

	char	szFilename[256];	/* generated filename */
	char	szLogPath[1024];	/* final full path*/
	FILE	*fp;

	int	nConsole;
	int	nRotateInterval;
	int	nNextRotate;
	int	nFlushFlag;
} Q_LOG;

/* qDecoder C++ support */
#ifdef __cplusplus
extern "C" {
#endif

/*
 * qDecoder.c
 */
int	qDecoder(void);
void	qDecoderSetUploadBase(char *dir, int olderthan);
char	*qValue(char *format, ...);
int	qiValue(char *format, ...);
char	*qValueDefault(char *defstr, char *format, ...);
char	*qValueNotEmpty(char *errmsg, char *format, ...);
char	*qValueReplace(char *mode, char *name, char *tokstr, char *word);
Q_ENTRY	*qGetFirstEntry(void);
char	*qValueFirst(char *format, ...);
char	*qValueNext(void);
char	*qValueAdd(char *name, char *format, ...);
void	qValueRemove(char *format, ...);
char	qValueType(char *format, ...);
int	qPrint(void);
void	qFree(void);

void	qCookieSet(char *name, char *value, int exp_days, char *path, char *domain, char *secure);
void	qCookieRemove(char *name, char *path, char *domain, char *secure);
char	*qCookieValue(char *format, ...);

void	qFreeAll(void);
void	qReset(void);

/*
 * qSession.c
 */
int	qSession(char *repository);
char	*qSessionAdd(char *name, char *format, ...);
int	qSessionAddInteger(char *name, int valueint);
int	qSessionUpdateInteger(char *name, int plusint);
void	qSessionRemove(char *format, ...);
char	*qSessionValue(char *format, ...);
int	qSessionValueInteger(char *format, ...);
int	qSessionPrint(void);
void	qSessionSave(void);
void	qSessionFree(void);
void	qSessionDestroy(void);
time_t	qSessionSetTimeout(time_t seconds);
char	*qSessionGetID(void);
time_t	qSessionGetCreated(void);

/*
 * qfDecoder.c
 */
Q_ENTRY	*qfDecoder(char *filename);
char	*qfValue(Q_ENTRY *first, char *format, ...);
int	qfiValue(Q_ENTRY *first, char *format, ...);
#define	qfValueFirst	qsValueFirst
#define	qfValueNext	qsValueNext
#define	qfPrint		qsPrint
#define	qfFree		qsFree

/*
 * qsDecoder.c
 */
Q_ENTRY	*qsDecoder(char *str);
char	*qsValue(Q_ENTRY *first, char *format, ...);
int	qsiValue(Q_ENTRY *first, char *format, ...);
char	*qsValueFirst(Q_ENTRY *first, char *format, ...);
char	*qsValueNext(void);
int	qsPrint(Q_ENTRY *first);
void	qsFree(Q_ENTRY *first);

/*
 * qHttpHeader.c
 */
void	qContentType(char *mimetype);
int	qGetContentFlag(void);
void	qResetContentFlag(void);
void	qRedirect(char *url);
void	qJavaScript(char *format, ...);

/*
 * qError.c
 */
void	qError(char *format, ...);
void	qErrorLog(char *file);
void	qErrorContact(char *msg);

/*
 * qEnv.c
 */
char	*qGetenvDefault(char *nullstr, char *envname);

/*
 * qEncode.c
 */
char	*qURLencode(char *str);
char	*qURLdecode(char *str);
char	*qMD5Str(char *string);
char	*qMD5File(char *filename);
unsigned int qFnv32Hash(char *str, unsigned int max);

/*
 * qString.c
 */
int	qPrintf(int mode, char *format, ...);
void	qPuts(int mode, char *buf);
char	*qRemoveSpace(char *str);
char	*qRemoveTailSpace(char *str);
char	*qStrReplace(char *mode, char *srcstr, char *tokstr, char *word);
int	qStr09AZaz(char *str);
char	*qStrupr(char *str);
char	*qStrlwr(char *str);
char	*qStristr(char *big, char *small);
int	qStricmp(char *s1, char *s2);
int	qStrincmp(char *s1, char *s2, size_t len);
char	*qStrtok(char *str, char *token, char *retstop);
char	*qitocomma(int value);
char    *qStrcat(char *str, char *format, ...);
char	*qStrdupBetween(char *str, char *start, char *end);
char	*qUniqId(void);

/*
 * qFile.c
 */
FILE	*qfopen(char *path, char *mode);
int	qfclose(FILE *stream);
int	qCheckFile(char *format, ...);
int	qCatFile(char *format, ...);
char	*qReadFile(char *filename, int *size);
int	qSaveStr(char *sp, int spsize, char *filename, char *mode);
long	qFileSize(char *filename);
char	*qfGetLine(FILE *fp);
char	*qfGets(FILE *fp);
char	*qCmd(char *cmd);

/*
 * qValid.c
 */
int	qCheckEmail(char *email);
int	qCheckURL(char *url);

/*
 * qArg.c
 */
int	qArgMake(char *str, char **qlist);
int	qArgMatch(char *str, char **qlist);
int	qArgPrint(char **qlist);
int	qArgEmprint(int mode, char *str, char **qlist);
void	qArgFree(char **qlist);

/*
 * qAwk.c
 */
FILE	*qAwkOpen(char *filename);
int	qAwkNext(FILE *fp, char array[][1024], char delim);
int	qAwkClose(FILE *fp);
int	qAwkStr(char array[][1024], char *str, char delim);

/*
 * qSed.c
 */
Q_ENTRY	*qSedArgAdd(Q_ENTRY *first, char *name, char *format, ...);
Q_ENTRY *qSedArgAddDirect(Q_ENTRY *first, char *name, char *value);
int	qSedArgPrint(Q_ENTRY *first);
void	qSedArgFree(Q_ENTRY *first);
int	qSedStr(Q_ENTRY *first, char *srcstr, FILE *fpout);
int	qSedFile(Q_ENTRY *first, char *filename, FILE *fpout);

/*
 * qCount.c
 */
int	qCountRead(char *filename);
int	qCountSave(char *filename, int number);
int	qCountUpdate(char *filename, int number);

/*
 * qDownload.c
 */
int	qDownload(char *filename);
int	qDownloadMime(char *filename, char *mime);

/*
 * qTime.c
 */
struct tm *qGetTime(void);
time_t	qGetGMTime(char *gmt, time_t plus_sec);
char	*qGetTimeStr(void);

/*
 * qLog.c
 */
Q_LOG	*qLogOpen(char *pszLogBase, char *pszFilenameFormat, int nRotateInterval, int nFlushFlag);
int	qLogClose(Q_LOG *log);
int	qLogSetConsole(Q_LOG *log, int nFlag);
int	qLogFlush(Q_LOG *log);
int	qLog(Q_LOG *log, char *pszFormat, ...);

/*
 * qSocket.c
 */
int	qSocketOpen(char *hostname, int port);
int	qSocketClose(int sockfd);
int	qSocketWaitReadable(int sockfd, int timeoutms);
int	qSocketRead(char *binary, int size, int sockfd, int timeoutms);
int	qSocketGets(char *str, int size, int sockfd, int timeoutms);
int	qSocketWrite(char *binary, int size, int sockfd);
int	qSocketPuts(char *str, int sockfd);
int	qSocketPrintf(int sockfd, char *format, ...);
int	qSocketSendFile(char *filepath, int offset, int sockfd);
int	qSocketSaveIntoFile(int sockfd, int size, int timeoutms, char *filepath, char *mode);
int	qSocketSetNonblock(int sockfd);
FILE	*qSocketConv2file(int sockfd);

/*
 * qDatabase.c
 */
#ifdef _Q_SUPPORT_DATABASE

Q_DB	*qDbInit(char *dbtype, char *addr, int port, char *username, char *password, char *database, int autocommit);
int	qDbFree(Q_DB *db);
int	qDbOpen(Q_DB *db);
int	qDbClose(Q_DB *db);
char	*qDbGetErrMsg(Q_DB *db);
int	qDbPing(Q_DB *db);
int     qDbGetLastConnStatus(Q_DB *db);

int	qDbExecuteUpdate(Q_DB *db, char *pszQuery);
Q_DBRESULT *qDbExecuteQuery(Q_DB *db, char *pszQuery);

int     qDbGetRows(Q_DBRESULT *result);
int     qDbGetCols(Q_DBRESULT *result);
int	qDbResultNext(Q_DBRESULT *result);
int	qDbResultFree(Q_DBRESULT *result);

char	*qDbGetValue(Q_DBRESULT *result, char *field);
int	qDbGetInt(Q_DBRESULT *result, char *field);
char	*qDbGetValueAt(Q_DBRESULT *result, int idx);
int	qDbGetIntAt(Q_DBRESULT *result, int idx);

int	qDbBeginTran(Q_DB *db);
int	qDbEndTran(Q_DB *db, int nCommit);
int	qDbCommit(Q_DB *db);
int	qDbRollback(Q_DB *db);

#endif

/*
 * qShm.c
 */
int	qShmInit(char *pszKeyPath, size_t nSize, int nIfExistsDestroy);
void	*qShmGet(int nShmId);
int	qShmFree(int nShmId);
int     qShmDestroy(char *pszKeyPath);

#ifdef __cplusplus
}
#endif

#endif
