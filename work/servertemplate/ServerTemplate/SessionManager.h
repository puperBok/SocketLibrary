#pragma once

class CSessionManager
{
public:
	static CHAR* CreateSession(CHAR* userID);
	static BOOL	 UpdateSession(CHAR* userID);
	static BOOL  DeleteSession(CHAR* userID);
};

