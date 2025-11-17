# C++ & React Full Stack Projesi (Kullanıcı Giriş Sistemi)

Bu proje, yüksek performanslı **C++** dilini (Crow framework) backend, modern **React** kütüphanesini (Vite) ise frontend olarak kullanan tam teşekküllü (Full Stack) bir web uygulamasıdır. Temel amacı, C++'ın web dünyasında nasıl bir API sunucusu olarak kullanılabileceğini ve veritabanı (SQLite) ile nasıl entegre olabileceğini göstermektir.

Proje, temel bir **kullanıcı giriş sistemini** barındırır.

##  Teknoloji Yığını (Tech Stack)

Projede kullanılan ana teknolojiler:

* **Backend:**
    * **C++ (g++ 13.2.0):** Ana sunucu dili.
    * **Crow (v1.2.0):** C++ için yazılmış, hafif ve hızlı bir web mikro-framework'ü.
    * **Asio:** Crow'un ağ işlemleri için kullandığı asenkron kütüphane.
* **Frontend:**
    * **React (Vite):** Modern, hızlı ve reaktif kullanıcı arayüzü kütüphanesi.
    * **Vite Proxy:** Geliştirme aşamasında CORS sorunlarını aşmak için kullanılan tünelleme aracı.
* **Veritabanı:**
    * **SQLite3 (C Amalgamation):** Kurulum gerektirmeyen, dosya tabanlı SQL veritabanı motoru.
* **Format:**
    * **JSON:** Backend ve Frontend arasındaki iletişim dili.

---

##  Proje Mimarisi ve Çalışma Mantığı

Bu projede iki adet yerel sunucu bulunur:

1.  **React Sunucusu (Port 5173):** Kullanıcının gördüğü arayüzü sunar.
2.  **C++ Sunucusu (Port 8085):** Veritabanı işlemlerini yapan, JSON tabanlı API'yi sunar.

Tarayıcı güvenlik kuralları (CORS) gereği, `5173` portundaki React'ın `8085` portundaki C++'a doğrudan istek atması engellenir. Bu sorunu aşmak için **Vite Proxy** kullandık:

* React, isteği `localhost:5173/api/login` adresine atar.
* Vite, bu isteği yakalar ve "arkadan" C++ sunucusuna (`localhost:8085/login`) iletir.
* Böylece tarayıcı, farklı bir sunucuya istek attığını fark etmez ve CORS hatası alınmaz.

---

##  Kurulum ve Derleme

Projeyi çalıştırmak için gerekli bağımlılıkları kurmanız ve backend kodunu derlemeniz gerekmektedir.

### Gerekli Araçlar

* **Node.js (v18+):** React'ı çalıştırmak için.
* **MinGW-w64 (g++ ve gcc):** C++ ve C kodlarını derlemek için. (Sürüm 13+ önerilir).

### 1. Frontend Kurulumu

Frontend klasörüne gidin ve gerekli Node modüllerini yükleyin:

```bash
cd frontend
npm install
```

### 2. Backend Derlemesi (En Önemli Adım)

Backend klasörüne gidin. Bu proje, C (`sqlite3.c`) ve C++ (`main.cpp`) kodlarını birlikte derlemeyi gerektirir.

**Adım 1: SQLite'ı C ile Derle**
Önce SQLite'ın C kodunu bir nesne dosyasına (`.o`) dönüştürürüz.

```bash
cd backend
gcc -c sqlite3.c -o sqlite3.o
```

**Adım 2: C++ Kodunu ve SQLite Nesnesini Birlikte Bağla (Link)**
Şimdi ana C++ kodumuzu, Crow kütüphanesini ve az önce oluşturduğumuz SQLite nesnesini birleştirerek çalıştırılabilir `server.exe` dosyamızı oluştururuz.

```bash
g++ main.cpp sqlite3.o -o server -lws2_32 -lmswsock -I .
```

* `main.cpp` ve `sqlite3.o`: Kendi kodlarımız.
* `-lws2_32 -lmswsock`: Windows ağ kütüphaneleri (Crow ve Asio için şart).
* `-I .`: `asio.hpp` ve `crow_all.h` gibi başlık dosyalarını bu klasörde ara demektir.

---

##  Projeyi Çalıştırma

İki ayrı terminal açmanız gerekmektedir.

**Terminal 1 (Backend Sunucusu):**
`backend` klasöründe olduğunuzdan emin olun ve derlediğiniz sunucuyu çalıştırın.

```bash
cd backend
./server.exe
```
*Beklenen Çıktı: `Backend hazir: Port 8085`*

**Terminal 2 (Frontend Sunucusu):**
`frontend` klasöründe olduğunuzdan emin olun ve React sunucusunu başlatın.

```bash
cd frontend
npm run dev
```
*Tarayıcınız otomatik olarak `http://localhost:5173` adresinde açılacaktır.*

### Test Kullanıcısı

Sistemi test etmek için veritabanına otomatik eklenen test kullanıcısını kullanabilirsiniz:

* **Kullanıcı Adı:** `deneme`
* **Şifre:** `12345`

---

##  Gelecek Hedefleri (Roadmap)

Bu proje bir temel ve ilerde yapmayı düşündüklerim :

* [ ] **SHA-256 Şifreleme:** Şifreleri veritabanında açık metin yerine "hash" olarak saklamak.
* [ ] **Kayıt Ol Sayfası:** React üzerinden yeni kullanıcı oluşturma.
* [ ] **JWT (JSON Web Token):** "Hoşgeldin" sayfasını korumalı hale getirmek ve oturum yönetimi eklemek.
* [ ] **Dockerize:** Projeyi Docker ile konteyner haline getirmek.
