#include "stdafx.h"
#include "DatabaseSession.h"

CDatabaseSession::CDatabaseSession()
{
	mRowNum = 0;
	mFieldNum = 0;
	iCurrentPos = 0;
	mIsBof = false;
	mIsEof = false;
	mIsConnect = false;
	mHaveResutQuery = false;
	mysql_init(&mConn);
}

CDatabaseSession::~CDatabaseSession()
{
}

bool CDatabaseSession::Open(const char* host, const char* userID, const char* password, const char* databaseName, unsigned int port, const char* unix_socket, unsigned int client_flag)
{
	if (&mConn == NULL)
	{
		return false;
	}

	if (mysql_real_connect(&mConn, host, userID, password, databaseName, port, unix_socket, client_flag) == NULL)
	{
		IsConnect(false);
		return false;
	}

	IsConnect(true);
	return true;
}

void CDatabaseSession::Close(void)
{
	if (&mConn != NULL)
	{
		mysql_close(&mConn);

		mRowNum = 0;
		mFieldNum = 0;
		iCurrentPos = 0;
		mIsBof = false;
		mIsEof = false;
		mIsConnect = false;
		mHaveResutQuery = false;
	}
}

bool CDatabaseSession::SelectDatabase(const char* databaseName)
{
	return !mysql_select_db(&mConn, databaseName);
}

bool CDatabaseSession::QueryExcute(const char* query)
{
	if (&mConn == NULL || !IsConnect())
	{
		return false;
	}

	if (mysql_query(&mConn, query))
	{
		return false;
	}
	else
	{
		mResult = mysql_store_result(&mConn);

		if (mResult)
		{
			mRowNum = mysql_num_rows(mResult);
			mFieldNum = mysql_num_fields(mResult);
			mRow = mysql_fetch_row(mResult);
			mField = mysql_fetch_fields(mResult);

			IsBOF(true);
			IsEOF(false);

			mHaveResutQuery = true;

			return true;
		}
		else
		{
			if (mysql_field_count(&mConn) == 0)
			{
				mRowNum = mysql_affected_rows(&mConn);
				mHaveResutQuery = false;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

char* CDatabaseSession::GetData(const unsigned int fieldIndex)
{
	if (!mHaveResutQuery)
	{
		return NULL;
	}
	if (mRowNum == 0)
	{
		return NULL;
	}
	if (fieldIndex > mFieldNum)
	{
		return NULL;
	}

	return mRow[fieldIndex];
}

void CDatabaseSession::First(void)
{
	IsBOF(true);
	IsEOF(false);

	iCurrentPos = 0;
	mysql_data_seek(mResult, iCurrentPos);
	mRow = mysql_fetch_row(mResult);
}

void CDatabaseSession::Last(void)
{
	IsBOF(false);
	IsEOF(true);

	iCurrentPos = mRowNum - 1;
	mysql_data_seek(mResult, iCurrentPos);
	mRow = mysql_fetch_row(mResult);
}

void CDatabaseSession::Next(void)
{
	if (!IsEOF())
	{
		Move(1);
		if (IsBOF())
		{
			IsBOF(false);
		}
	}
	else
	{
		return;
	}
}

void CDatabaseSession::Prev(void)
{
	if (!IsEOF())
	{
		Move(-1);
		if (IsBOF())
		{
			IsBOF(false);
		}
	}
	else
	{
		return;
	}
}

void CDatabaseSession::Move(int cnt)
{
	my_ulonglong pos = iCurrentPos + cnt;

	if (pos == -1)
	{
		IsBOF(true);
		IsEOF(false);
	}
	else if (pos == mRowNum)
	{
		IsBOF(false);
		IsEOF(true);
	}
	else if ((pos < 0) || (pos > mRowNum))
	{
	}

	iCurrentPos = pos;
	mysql_data_seek(mResult, iCurrentPos);
	mRow = mysql_fetch_row(mResult);
}