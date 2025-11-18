#include "crow_all.h"
#include "sqlite3.h"
#include <iostream>
#include "sha256.h"

// --- AYARLAR ---
const char* DB_NAME = "db_v2.db"; 

// --- YARDIMCI FONKSIYONLAR ---

void veritabaniKurulum() {
    sqlite3* db;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        std::cerr << "Veritabani acilamadi: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "username TEXT NOT NULL,"
                      "password TEXT NOT NULL);"; // Hashli şifre buraya gelecek
    char* errMsg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Hatasi: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    sqlite3_close(db);
}

bool kullaniciVarMi(std::string user) {
    sqlite3* db;
    sqlite3_open(DB_NAME, &db);
    std::string sql = "SELECT id FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }
    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
    
    bool var = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) var = true;
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return var;
}

// KAYIT FONKSIYONU
bool kullaniciKayit(std::string user, std::string pass, std::string& outMesaj) {
    if (kullaniciVarMi(user)) {
        outMesaj = "Kullanici zaten var.";
        return false;
    }

    sqlite3* db;
    sqlite3_open(DB_NAME, &db);
    std::string sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        outMesaj = "SQL Hatasi";
        sqlite3_close(db);
        return false;
    }

    // --- DEBUG LOG ---
    std::string guvenliSifre = sha256(pass);
    std::cout << ">>> KAYIT OLUYOR..." << std::endl;
    std::cout << "Kullanici: [" << user << "]" << std::endl;
    std::cout << "Sifre (Ham): [" << pass << "]" << std::endl;
    std::cout << "Sifre (Hash): [" << guvenliSifre << "]" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    // -----------------------------

    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, guvenliSifre.c_str(), -1, SQLITE_STATIC); // Hashli kaydet

    bool basari = false;
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        basari = true;
        outMesaj = "Kayit Basarili.";
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return basari;
}

// GİRİŞ KONTROL FONKSIYONU
bool kullaniciKontrol(std::string user, std::string pass) {
    sqlite3* db;
    sqlite3_open(DB_NAME, &db);
    std::string sql = "SELECT id FROM users WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    // --- DEBUG LOG ---
    std::string guvenliSifre = sha256(pass);
    std::cout << ">>> GIRIS DENENIYOR..." << std::endl;
    std::cout << "Aranan Kullanici: [" << user << "]" << std::endl;
    std::cout << "Aranan Hash: [" << guvenliSifre << "]" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    // -----------------------------

    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, guvenliSifre.c_str(), -1, SQLITE_STATIC); // Hashli ara

    bool bulundu = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        bulundu = true;
        std::cout << ">>> SONUC: BULUNDU! ✅" << std::endl;
    } else {
        std::cout << ">>> SONUC: BULUNAMADI! ❌" << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return bulundu;
}

int main()
{
    veritabaniKurulum();
    crow::SimpleApp app;

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Hatali JSON");
        std::string k = x["kullanici"].s();
        std::string s = x["sifre"].s();

        crow::json::wvalue sonuc;
        if (kullaniciKontrol(k, s)) {
            sonuc["cevap"] = "Giris Basarili!";
            return crow::response(200, sonuc);
        } else {
            sonuc["cevap"] = "Hatali bilgiler!";
            return crow::response(401, sonuc);
        }
    });

    CROW_ROUTE(app, "/register").methods("POST"_method)
    ([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Hatali JSON");
        std::string k = x["kullanici"].s();
        std::string s = x["sifre"].s();
        std::string msg;

        if (kullaniciKayit(k, s, msg)) {
            crow::json::wvalue r; r["cevap"] = msg;
            return crow::response(201, r);
        } else {
            crow::json::wvalue r; r["cevap"] = msg;
            return crow::response(400, r);
        }
    });

    std::cout << "Backend v2 (Hash Dedektifi) devrede: Port 8085" << std::endl;
    app.port(8085).multithreaded().run();
}
