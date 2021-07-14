#ifndef _SQLITE_USER_H_
#define _SQLITE_USER_H_

#include <sqlite3.h>

typedef struct
{
	const char *file;
	sqlite3 *db;
} DB_FILE_T;

typedef struct
{
	const char *name;
	const char *member;
} DB_TABLE_T;

extern int openDataBase(DB_FILE_T *dbFile);
extern int sql_exec(DB_FILE_T *dbFile, const char *sql);
extern int sql_create_table(DB_FILE_T *dbFile, DB_TABLE_T table);

#endif