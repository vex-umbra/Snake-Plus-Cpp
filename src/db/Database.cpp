#include "db/Database.h"
#include <iostream>

#if SNAKEPLUS_WITH_SQLITE
    #include <sqlite3.h>
#endif

bool Database::enabled() const {
#if SNAKEPLUS_WITH_SQLITE
    return true;
#else
    return false;
#endif
}

bool Database::init(const std::string& dbPath) {
    path_ = dbPath;
#if SNAKEPLUS_WITH_SQLITE
    if (sqlite3_open(path_.c_str(), &db_) != SQLITE_OK) {
        std::cerr << "SQLite open failed: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    const char* sql =
        "CREATE TABLE IF NOT EXISTS scores("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "score INTEGER NOT NULL,"
        "created_at TEXT DEFAULT (datetime('now'))"
        ");";

    char* err = nullptr;
    if (sqlite3_exec(db_, sql, nullptr, nullptr, &err) != SQLITE_OK) {
        std::cerr << "SQLite schema failed: " << (err ? err : "") << "\n";
        sqlite3_free(err);
        return false;
    }
    return true;
#else
    (void)dbPath;
    return false;
#endif
}

void Database::shutdown() {
#if SNAKEPLUS_WITH_SQLITE
    if (db_) sqlite3_close(db_);
    db_ = nullptr;
#endif
}

bool Database::insertScore(const std::string& name, int score) {
#if SNAKEPLUS_WITH_SQLITE
    if (!db_) return false;
    const char* sql = "INSERT INTO scores(name, score) VALUES(?, ?);";
    sqlite3_stmt* st = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &st, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_text(st, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(st, 2, score);

    bool ok = (sqlite3_step(st) == SQLITE_DONE);
    sqlite3_finalize(st);
    return ok;
#else
    (void)name; (void)score;
    return false;
#endif
}

std::vector<ScoreRow> Database::topScores(int limit) {
    std::vector<ScoreRow> rows;
#if SNAKEPLUS_WITH_SQLITE
    if (!db_) return rows;
    const char* sql = "SELECT name, score, created_at FROM scores ORDER BY score DESC LIMIT ?;";
    sqlite3_stmt* st = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &st, nullptr) != SQLITE_OK) return rows;
    sqlite3_bind_int(st, 1, limit);

    while (sqlite3_step(st) == SQLITE_ROW) {
        ScoreRow r;
        r.name = (const char*)sqlite3_column_text(st, 0);
        r.score = sqlite3_column_int(st, 1);
        r.created_at = (const char*)sqlite3_column_text(st, 2);
        rows.push_back(r);
    }
    sqlite3_finalize(st);
#else
    (void)limit;
#endif
    return rows;
}
