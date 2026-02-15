#pragma once
#include <string>
#include <vector>

struct ScoreRow {
    int score = 0;
    std::string name;
    std::string created_at;
};

class Database {
public:
    bool init(const std::string& dbPath);
    void shutdown();

    bool insertScore(const std::string& name, int score);
    std::vector<ScoreRow> topScores(int limit);

    bool enabled() const;

private:
#if SNAKEPLUS_WITH_SQLITE
    struct sqlite3* db_ = nullptr;
#endif
    std::string path_;
};
