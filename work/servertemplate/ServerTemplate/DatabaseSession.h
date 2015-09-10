#pragma once
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

class CDatabaseSession
{
public:
	CDatabaseSession();
	~CDatabaseSession();

private:
	MYSQL				mConn;
	MYSQL_RES*			mResult;
	MYSQL_ROW			mRow;
	MYSQL_FIELD*		mField;
	my_ulonglong		iCurrentPos;
	unsigned __int64	mRowNum;
	unsigned int		mFieldNum;
	bool				mHaveResutQuery;
	bool				mIsEof;
	bool				mIsBof;
	bool				mIsConnect;

private:
	void IsConnect(bool value) { mIsConnect = value; }
	void IsBOF(bool value) { mIsBof = value; }
	void IsEOF(bool value) { mIsEof = value; }

public:
	bool Open(const char* host, const char* db_userID, const char* password, const char* databaseName, unsigned int port, const char* unix_socket = NULL, unsigned int client_flag = 0);
	void Close(void);

	bool IsConnect() { return mIsConnect; }

	bool QueryExcute(const char* query);
	bool SelectDatabase(const char* dbName);

	bool IsBOF(void) { return mIsBof; }
	bool IsEOF(void) { return mIsEof; }

	void First(void);
	void Last(void);
	void Next(void);
	void Prev(void);
	void Move(int cnt = 1);

	char* GetData(const unsigned int fieldIndex);
	unsigned int GetFieldSize(void) { return mFieldNum; }
	unsigned __int64 GetRowSize(void) { return mRowNum; }
};
