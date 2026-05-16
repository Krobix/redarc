#include "dbsetup.h"
#include <sqlite3.h>
#include <json/json.h>
#include <filesystem>

int DbSetup::createdb(){
    sqlite3_stmt *submission_stmt;
    const char *u = 0;
    int submission_status = sqlite3_prepare_v3(db, 
        R"(CREATE TABLE submissions(
            name varchar(16) primary key,
            author varchar(64),
            time bigint,
            flair varchar(64),
            is_self boolean,
            is_video boolean,
            is_image boolean,
            is_crosspost boolean,
            media int unsigned,
            subreddit varchar(32),
            title varchar(512),
            selftext text,
            cross_name varchar(16),
            score int unsigned,
            ratio decimal))",
        -1, 0, &submission_stmt, &u);
}

int DbSetup::opendb(const std::string &path){
    bool exists;
    int status;

    exists = std::filesystem::exists(path);

    status = sqlite3_open(path.c_str(), &db);
    if(status){
        return status;
    }

    if(exists){
        status = createdb();
    }
    if(status){
        return status;
    }
    return 0;
}