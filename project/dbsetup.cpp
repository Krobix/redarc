#include "dbsetup.h"
#include <json/reader.h>
#include <sqlite3.h>
#include <json/json.h>
#include <filesystem>

int DbSetup::createdb(){
    sqlite3_stmt *submission_stmt;
    sqlite3_stmt *reply_stmt;
    sqlite3_stmt *img_stmt;
    const char *u = 0;
    int submission_status = sqlite3_prepare_v3(db, 
        R"(CREATE TABLE submissions(
            name varchar(16) primary key,
            author varchar(24),
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

    int reply_status = sqlite3_prepare_v3(db, 
        R"(CREATE TABLE replies(
            name varchar(32) primary key,
            author varchar(24)
            time bigint,
            author_flair_text varchar(64),
            body text,
            controversiality decimal,
            score int,
            stickied boolean))",
        -1, 0, &reply_stmt, &u);

    int img_status = sqlite3_prepare_v3(db,
            R"(CREATE TABLE media(
            is_local boolean,
            source text))",
        -1, 0, &img_stmt, &u);

    if(submission_status){
        return submission_status;
    }
    if(reply_status){
        return 10000 + reply_status;
    }
    if(img_status){
        return 20000 + img_status;
    }

    submission_status = sqlite3_step(submission_stmt);
    if(submission_status!=101){
        return 30000 + submission_status;
    }
    sqlite3_reset(submission_stmt);
    reply_status = sqlite3_step(reply_stmt);
    if(reply_status!=101){
        return 40000 + reply_status;
    }
    sqlite3_reset(reply_stmt);
    img_status = sqlite3_step(img_stmt) ;
    if(img_status!=101){
        return 50000 + img_status;
    }

    return 0;
}

int DbSetup::opendb(std::string path){
    bool exists;
    int status;

    dir = path;
    path = dir + "/data.db";

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

int DbSetup::inssubmission(std::istream &in, const bool local_image){
    Json::Value subjs;
    Json::CharReaderBuilder rbuilder;
    std::string err;
    std::string querystr;
    bool json_status, ismedia;
    std::string mediasrc = "";

    json_status = Json::parseFromStream(rbuilder, in, &subjs, &err);

    if(!json_status || !subjs.isMember("is_reddit_media_domain")){
        return 1;
    }

    ismedia = subjs["is_reddit_media_domain"].asBool();

    if(ismedia){
        mediasrc = insmedia(subjs["url"].asString(), local_image);
    }

}