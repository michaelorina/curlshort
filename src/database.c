#include "../include/base62.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_FILE "curlshort.db"

void init_db() {
    sqlite3 *db;
    char *err_msg = NULL;

    if (sqlite3_open(DB_FILE, &db) != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS urls ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "original_url TEXT NOT NULL, "
                  "short_url TEXT UNIQUE);";


    if (sqlite3_exec(db, sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Error creating table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    sqlite3_close(db);
}

char *insert_url(const char *original_url) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *short_url = malloc(10);
    if (!short_url) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    if (sqlite3_open(DB_FILE, &db) != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        free(short_url);
        return NULL;
    }

    const char *insert_sql = "INSERT INTO urls (original_url) VALUES (?)";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing insert statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free(short_url);
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, original_url, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error inserting URL: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        free(short_url);
        return NULL;
    }

    int last_id = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);

    if (last_id == 0) {
        fprintf(stderr, "Error retrieving last insert ID\n");
        sqlite3_close(db);
        free(short_url);
        return NULL;
    }

    encode_base62(last_id, short_url);
    printf("Generated short URL: %s\n", short_url);

    const char *update_sql = "UPDATE urls SET short_url = ? WHERE id = ?";
    if (sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing update statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free(short_url);
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, short_url, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, last_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error updating short URL: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        free(short_url);
        return NULL;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return short_url;
}
