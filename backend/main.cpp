#include "crow_all.h"
#include "sqlite3.h" // SQLite kutuphanesini ekledik
#include <iostream>

// --- VERITABANI YARDIMCI FONKSIYONLARI ---

// Veritabanini baslat ve tablo olustur
void veritabaniKurulum() {
    sqlite3* db;
    // 1. Veritabanini ac (Yoksa olusturur)
    int rc = sqlite3_open("database.db", &db);
    
    if (rc) {
        std::cerr << "Veritabani acilamadi: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // 2. Tabloyu olustur (Eger yoksa)
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "username TEXT NOT NULL,"
                      "password TEXT NOT NULL);";
    
    char* errMsg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Hatasi: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Veritabani ve Tablo kontrol edildi: OK" << std::endl;
    }

    // 3. Test kullanicisi ekle (Sadece test icin, her seferinde hata vermesin diye INSERT OR IGNORE mantigi gerekir ama basit tutalim)
    // Basitlik adina: Burayi simdilik manuel ekleme gibi dusunelim.
    // Gercek projede once kullanici var mi diye bakilir.
    
    sqlite3_close(db);
}

// Kullaniciyi dogrula
bool kullaniciKontrol(std::string user, std::string pass) {
    sqlite3* db;
    sqlite3_open("database.db", &db);
    
    // SQL Sorgusu: Kullanici adi ve sifresi esleseni bul
    std::string sql = "SELECT id FROM users WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    
    // Sorguyu hazirla
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    // Soru isaretleri yerine gelen verileri koy (Guvenlik icin - SQL Injection onler)
    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);

    bool bulundu = false;
    // Sorguyu calistir ve satir geliyor mu bak
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        bulundu = true;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return bulundu;
}

// Yeni kullanici kaydet (Test icin main icinde cagiracagiz)
void kullaniciEkle(std::string user, std::string pass) {
    sqlite3* db;
    sqlite3_open("database.db", &db);
    
    std::string sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);
    
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    std::cout << "Kullanici eklendi: " << user << std::endl;
}
// -----------------------------------------

int main()
{
    // Sunucu baslarken veritabanini hazirla
    veritabaniKurulum();
    
    // TEST ICIN: Baslangicta 'deneme' adinda bir kullanici olusturalim
    // (Bunu bir kere calistirdiktan sonra yorum satirina alabilirsin)
    kullaniciEkle("deneme", "12345");


    crow::SimpleApp app;

    CROW_ROUTE(app, "/login")
        .methods("POST"_method)
    ([](const crow::request& req){
        
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Hatali JSON");

        std::string k = x["kullanici"].s();
        std::string s = x["sifre"].s();

        std::cout << "Giris Denemesi: " << k << std::endl;

        crow::json::wvalue sonuc;
        
        // VERITABANI KONTROLU
        if (kullaniciKontrol(k, s)) {
            sonuc["cevap"] = "Giris Basarili! Veritabaninda bulundu.";
            return crow::response(200, sonuc);
        } else {
            sonuc["cevap"] = "Kullanici adi veya sifre yanlis!";
            return crow::response(401, sonuc);
        }
    });

    std::cout << "Backend hazir: Port 8085" << std::endl;
    app.port(8085).multithreaded().run();
}