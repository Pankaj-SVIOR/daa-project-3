// =====================================================
// database.h  —  SQLite Helper + DB Setup
// =====================================================

#ifndef DATABASE_H
#define DATABASE_H

#include "../sqlite/sqlite3.h"
#include <string>
#include <iostream>

const std::string DB_PATH = "database/free_slot_system.db";

sqlite3* openDB() {
    sqlite3* db = NULL;
    if (sqlite3_open(DB_PATH.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open DB: " << sqlite3_errmsg(db) << std::endl;
        return NULL;
    }
    return db;
}

void closeDB(sqlite3* db) {
    if (db) sqlite3_close(db);
}

// =====================================================
// setupDB() — Creates all tables + seeds default data
// =====================================================
void setupDB() {
    sqlite3* db = openDB();
    if (!db) return;

    const char* queries[] = {
        // Teachers table — now has password + department
        "CREATE TABLE IF NOT EXISTS teachers ("
        "  teacher_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name       TEXT UNIQUE,"
        "  department TEXT,"
        "  email      TEXT,"
        "  password   TEXT DEFAULT 'pass123'"
        ");",

        // Rooms table
        "CREATE TABLE IF NOT EXISTS rooms ("
        "  room_id   INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  room_name TEXT UNIQUE,"
        "  room_type TEXT,"
        "  capacity  INTEGER"
        ");",

        // Timetable — fixed college schedule
        "CREATE TABLE IF NOT EXISTS timetable ("
        "  id         INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  room_id    INTEGER,"
        "  day        TEXT,"
        "  slot       INTEGER,"
        "  subject    TEXT,"
        "  teacher_id INTEGER,"
        "  section    TEXT"
        ");",

        // Bookings — teacher room reservations
        "CREATE TABLE IF NOT EXISTS bookings ("
        "  booking_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  teacher_id INTEGER,"
        "  room_id    INTEGER,"
        "  date       TEXT,"
        "  slot       INTEGER,"
        "  status     TEXT"
        ");"
    };

    char* err = NULL;
    for (int i = 0; i < 4; i++) {
        sqlite3_exec(db, queries[i], NULL, NULL, &err);
        if (err) { std::cerr << "DB Error: " << err << std::endl; sqlite3_free(err); err = NULL; }
    }

    // Add password column if it doesn't exist yet (for existing databases)
    sqlite3_exec(db,
        "ALTER TABLE teachers ADD COLUMN password TEXT DEFAULT 'pass123';",
        NULL, NULL, NULL);  // ignore error if column already exists
    sqlite3_exec(db,
        "ALTER TABLE teachers ADD COLUMN email TEXT DEFAULT '';",
        NULL, NULL, NULL);

    closeDB(db);
    std::cout << "Database ready." << std::endl;
}

#endif // DATABASE_H
