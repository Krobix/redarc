#pragma once
#include <drogon/drogon.h>
#include <sqlite3.h>
#include <iostream>

namespace DbSetup {
    sqlite3 *db;
    std::string dir;

    int createdb();

    int opendb(std::string path);

    int inssubmission(std::istream &in, const bool local_image);

    std::string insmedia(const std::string& link, bool local);

    int insreply(std::istream &in);
}