#include "Tetris_Statistics.h"
#include <string>

void Tetris_Statistics::init()
{
    std::string sql_stmt = "CREATE TABLE game(ID INTEGER UNIQUE, Name TEXT, Lines INTEGER, Level INTEGER, Score INTEGER)";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql_stmt.c_str(), static_cast<int>(sql_stmt.length()), &stmt, NULL);
    
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

void Tetris_Statistics::add_stats(Tetris_Stats_entry entry)
{
    std::string sql_stmt = "INSERT INTO game VALUES(random(),'" + entry.name + "'," + std::to_string(entry.lines) + "," + std::to_string(entry.level) + "," + std::to_string(entry.score) + ")";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql_stmt.c_str(), static_cast<int>(sql_stmt.length()), &stmt, NULL);

    if (rc == SQLITE_OK)
        rc = sqlite3_step(stmt);

    if (rc == SQLITE_DONE)
        rc = sqlite3_finalize(stmt);

    if (rc == SQLITE_OK)
        //std::cout << "new entry into table << game >> has been added..." << std::endl;
        ;
    else
        std::cout << "Error: query " << sql_stmt << " failed with error code " << rc << std::endl;

    return;
}

std::vector<Tetris_Stats_entry> Tetris_Statistics::get_high_scores()
{
    int n = 10;
    std::string sql_stmt = "SELECT Name, Lines, Level, Score FROM game ORDER BY Score DESC";
    sqlite3_stmt* stmt;
    std::vector<Tetris_Stats_entry> stats = std::vector<Tetris_Stats_entry>(n);

    int rc = sqlite3_prepare_v2(db, sql_stmt.c_str(), static_cast<int>(sql_stmt.length()), &stmt, NULL);

    if(rc == SQLITE_OK)
    {
        int i = 0;
        rc = sqlite3_step(stmt);
        while (i < n && rc == SQLITE_ROW)
        {   // TODO: column indeces are magic numbers, refactoring needed
            
            stats[i].name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            stats[i].lines = sqlite3_column_int(stmt, 1);
            stats[i].level = sqlite3_column_int(stmt, 2);
            stats[i].score = sqlite3_column_int(stmt, 3);

            rc = sqlite3_step(stmt);
            i++;
        }
    }
    return stats;
}


