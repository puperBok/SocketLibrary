#include "stdafx.h"
#include "SessionManager.h"

CHAR* CSessionManager::CreateSession(CHAR* userID)
{
	std::mt19937 binary_engine((unsigned int)time(NULL));
	std::mt19937 character_engine((unsigned int)time(NULL));
	std::mt19937 number_engine((unsigned int)time(NULL));

	std::uniform_int_distribution<int> binary_distribution(0, 1);
	std::uniform_int_distribution<int> character_distribution(97, 122);
	std::uniform_int_distribution<int> number_distribution(48, 57);

	auto binary_generator = bind(binary_distribution, binary_engine);
	auto character_generator = bind(character_distribution, character_engine);
	auto number_generator = bind(number_distribution, number_engine);

	CHAR session[41];

	ZeroMemory(session, sizeof(session));

	for (int i = 0; i < 40; i++)
	{
		switch (binary_generator())
		{
		case 0:
			session[i] = character_generator();
			break;
		case 1:
			session[i] = number_generator();
			break;
		}
	}
	session[40] = NULL;

	return session;
}

BOOL CSessionManager::UpdateSession(CHAR* userID)
{
	return TRUE;
}

BOOL CSessionManager::DeleteSession(CHAR* userID)
{
	return TRUE;
}