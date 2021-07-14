#include <stdio.h>
#include <unistd.h>

#include "sqlite_user.h"
#include "log_printf.h"

/**
 * @brief 打开一个db文件，没有则创建
 * @param dbFile db文件结构体
 * @return 0:成功 -1:失败
 */
int openDataBase(DB_FILE_T *dbFile)
{
	int ret;

	ret = sqlite3_open(dbFile->file, &dbFile->db);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(dbFile->db);
		logPrintf("datebase open failed!");
		return -1;
	}

	return 0;
}

/**
 * @brief 执行sql语句
 * @param dbFile db文件结构体
 * @return 0:成功 -1:失败
 */
int sql_exec(DB_FILE_T *dbFile, const char *sql)
{
	int ret;
	char *err = NULL;

	ret = sqlite3_exec(dbFile->db, sql, NULL, NULL, &err);
	if (ret != SQLITE_OK)
	{
		logPrintf("sql exec error:%s", err);
		return -1;
	}

	logPrintf(sql);

	return 0;
}

/**
 * @brief 创建table
 * @param dbFile db文件结构体
 * @param 
 * @return 0:成功 -1:失败
 */
int sql_create_table(DB_FILE_T *dbFile, DB_TABLE_T table)
{
	char sql[1024];

	sprintf(sql, "create table if not exists %s(%s);", 
		table.name, table.member);
	
	sql_exec(dbFile, sql);

	return 0;
}