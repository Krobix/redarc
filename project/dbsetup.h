#pragma once
#include <drogon/drogon.h>
#include <sqlite3.h>
#include <iostream>

namespace DbSetup {
    sqlite3 *db;

    int createdb();

    int opendb(const std::string &path);

    int insdata(std::istream &in, const bool local_images);
}