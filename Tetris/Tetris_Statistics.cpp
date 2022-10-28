#include "Tetris_Statistics.h"
#include <string>

Tetris_Statistics* Tetris_Statistics::singleton = nullptr;

void Tetris_Statistics::init()
{
	int rc;
	std::string sql_stmt = "CREATE TABLE game(ID INTEGER UNIQUE, Name TEXT, Lines INTEGER, Level INTEGER, Score INTEGER)";
	sqlite3_stmt* stmt;

	rc = sqlite3_prepare_v2(db, sql_stmt.c_str(), static_cast<int>(sql_stmt.length()), &stmt, NULL);
	
	if(rc == SQLITE_OK)
		rc = sqlite3_step(stmt);

	if (rc == SQLITE_DONE)
		rc = sqlite3_finalize(stmt);

	if (rc == SQLITE_OK)
		std::cout << "Table << game >> has been created..." << std::endl;
	else
		std::cout << "Error: query " << sql_stmt << " failed with error code " << rc << std::endl;

	return;
}

void Tetris_Statistics::add_stats(std::string name, int lines, int level, int score)
{
	int rc;
	std::string sql_stmt = "INSERT INTO game VALUES(random(),'" + name + "'," + std::to_string(lines) + "," + std::to_string(level) + "," + std::to_string(score) + ")";
	sqlite3_stmt* stmt;

	rc = sqlite3_prepare_v2(db, sql_stmt.c_str(), static_cast<int>(sql_stmt.length()), &stmt, NULL);

	if (rc == SQLITE_OK)
		rc = sqlite3_step(stmt);

	if (rc == SQLITE_DONE)
		rc = sqlite3_finalize(stmt);

	if (rc == SQLITE_OK)
		std::cout << "new entry into table << game >> has been added..." << std::endl;
	else
		std::cout << "Error: query " << sql_stmt << " failed with error code " << rc << std::endl;

	return;
}


