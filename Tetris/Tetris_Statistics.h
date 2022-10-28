#ifndef _TETRIS_STATISTICS_H
#define _TETRIS_STATISTICS_H

#include <sqlite3.h>
#include <iostream>

class Tetris_Statistics
{
private:
    static Tetris_Statistics *singleton;
    sqlite3 *db;

    void init();
    
    Tetris_Statistics()
    {
        // TODO: how to react on a failed open / vreation? 

        int rc;

        rc = sqlite3_open_v2("scores.db", &db, SQLITE_OPEN_READWRITE, NULL); // try open existing db

        if (rc == SQLITE_CANTOPEN)
        {   // if open not successful, create it 
            rc = sqlite3_open_v2("scores.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
            init();
        }

        if (rc == SQLITE_OK)
        {
            std::cout << "Connection to database established... " << std::endl;
        }
        else
        {
            std::cout << "Error: connection to database has not been opened, error code: " << rc << std::endl;
            sqlite3_close_v2(db);
        }
    }

public:
   
    static Tetris_Statistics* getInstance()
    {
        if (!singleton)
            singleton = new Tetris_Statistics();
        
        return singleton;
    }

    ~Tetris_Statistics()
    {
        int rc = sqlite3_close_v2(db);
        
        if (rc == SQLITE_OK)
            std::cout << "\nConnection to database closed... " << std::endl;
        else
            std::cout << "\nError: failed to close connection to database, error code: " << rc << std::endl;
    }

    void add_stats(std::string name, int lines, int level, int score);
};

#endif // !_TETRIS_STATISTICS_H