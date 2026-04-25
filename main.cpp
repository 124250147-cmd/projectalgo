#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

// ==================== KONSTANTA ====================
const int MAX_MENU = 100;
const int MAX_PELANGGAN = 100;
const int MAX_TRANSAKSI = 100;
const int MAX_ANTRIAN = 100;
const int MAX_PEMBAYARAN = 100;
const int MAX_METODE = 10;

// ==================== MATA UANG FORMATTER ====================
string formatRupiah(int amount) {
    string str = to_string(amount);
    string result;
    int count = 0;
    
    for (int i = str.length() - 1; i >= 0; i--) {
        result = str[i] + result;
        count++;
        if (count % 3 == 0 && i != 0) {
            result = "." + result;
        }
    }
    return "Rp " + result;
}

// ==================== 1. STRUCT MENU ====================
// tbl_menu
struct Menu {
    char kode_menu[6];      // PK, format M001-M099
    char nama_menu[51];
    int harga;
    char kategori[31];
    bool tersedia;
    
    Menu() {
        kode_menu[0] = '\0';
        nama_menu[0] = '\0';
        harga = 0;
        strcpy(kategori, "Umum");
        tersedia = true;
    }
    
    Menu(const char* km, const char* nm, int h, const char* kat) {
        strncpy(kode_menu, km, 5);
        kode_menu[5] = '\0';
        strncpy(nama_menu, nm, 50);
        nama_menu[50] = '\0';
        harga = h;
        strncpy(kategori, kat, 30);
        kategori[30] = '\0';
        tersedia = true;
    }
    
    void display() const {
        cout << "│ " << left << setw(8) << kode_menu
             << "│ " << setw(20) << nama_menu
             << "│ " << setw(12) << formatRupiah(harga)
             << "│ " << setw(15) << kategori
             << "│ " << setw(8) << (tersedia ? "Tersedia" : "Habis") << " │\n";
    }
};

// ==================== 2. STRUCT METODE PEMBAYARAN ====================
// tbl_metode_pembayaran
struct MetodePembayaran {
    int id_metode;          // PK: 1=Tunai,2=Debit,3=Kredit,4=QRIS,5=Transfer
    char nama_metode[31];
    char jenis[20];         // TUNAI, KARTU_DEBIT, KARTU_KREDIT, QRIS, TRANSFER
    float biaya_admin;      // persentase (0.02 = 2%)
    bool aktif;
    char deskripsi[101];
    
    MetodePembayaran() {
        id_metode = 0;
        nama_metode[0] = '\0';
        jenis[0] = '\0';
        biaya_admin = 0.0f;
        aktif = true;
        deskripsi[0] = '\0';
    }
    
    void display() const {
        cout << "│ " << setw(4) << id_metode
             << "│ " << setw(14) << nama_metode
             << "│ " << setw(14) << jenis
             << "│ " << setw(10) << (biaya_admin * 100) << "%"
             << "│ " << setw(8) << (aktif ? "Aktif" : "Nonaktif") << " │\n";
    }
};

// ==================== 3. STRUCT PELANGGAN ====================
// tbl_pelanggan
struct Pelanggan {
    int id_pelanggan;       // PK, AUTO_INCREMENT
    char nama_pelanggan[51];
    int no_meja;
    time_t waktu_kunjungan;
    
    Pelanggan() {
        id_pelanggan = 0;
        nama_pelanggan[0] = '\0';
        no_meja = 0;
        waktu_kunjungan = time(nullptr);
    }
    
    Pelanggan(int id, const char* nama, int meja) {
        id_pelanggan = id;
        strncpy(nama_pelanggan, nama, 50);
        nama_pelanggan[50] = '\0';
        no_meja = meja;
        waktu_kunjungan = time(nullptr);
    }
};

// ==================== 4. STRUCT PESANAN ITEM ====================
// tbl_pesanan_item
struct PesananItem {
    int id_item;            // PK parsial
    int id_transaksi;       // FK ke transaksi
    char kode_menu[6];      // FK ke menu
    char nama_menu[51];     // snapshot
    int harga_satuan;       // snapshot
    int jumlah;
    int subtotal;           // derived: harga_satuan * jumlah
    
    PesananItem() {
        id_item = 0;
        id_transaksi = 0;
        kode_menu[0] = '\0';
        nama_menu[0] = '\0';
        harga_satuan = 0;
        jumlah = 0;
        subtotal = 0;
    }
    
    PesananItem(int id_item, int id_trx, const char* kode, const char* nama, 
                int harga, int jml) {
        this->id_item = id_item;
        this->id_transaksi = id_trx;
        strncpy(this->kode_menu, kode, 5);
        this->kode_menu[5] = '\0';
        strncpy(this->nama_menu, nama, 50);
        this->nama_menu[50] = '\0';
        this->harga_satuan = harga;
        this->jumlah = jml;
        this->subtotal = harga_satuan * jumlah;
    }
    
    void display() const {
        cout << "│ " << setw(5) << id_item
             << "│ " << setw(20) << nama_menu
             << "│ " << setw(5) << jumlah
             << "│ " << setw(12) << formatRupiah(harga_satuan)
             << "│ " << setw(12) << formatRupiah(subtotal) << " │\n";
    }
};

// ==================== 5. STRUCT PEMBAYARAN ====================
// tbl_pembayaran
struct Pembayaran {
    int id_pembayaran;      // PK, AUTO_INCREMENT
    int id_transaksi;       // FK ke transaksi (UNIQUE → 1:1)
    int id_metode;          // FK ke metode_pembayaran
    int jumlah_bayar;
    int total_tagihan;
    int biaya_admin_rp;
    int kembalian;          // derived: jumlah_bayar - total_tagihan
    time_t waktu_bayar;
    bool lunas;
    char catatan[201];
    
    Pembayaran() {
        id_pembayaran = 0;
        id_transaksi = 0;
        id_metode = 0;
        jumlah_bayar = 0;
        total_tagihan = 0;
        biaya_admin_rp = 0;
        kembalian = 0;
        waktu_bayar = time(nullptr);
        lunas = false;
        catatan[0] = '\0';
    }
    
    void hitungKembalian() {
        kembalian = jumlah_bayar - total_tagihan;
        lunas = (kembalian >= 0);
    }
    
    void display() const {
        cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
        cout << "║                        STRUK PEMBAYARAN                        ║\n";
        cout << "╠══════════════════════════════════════════════════════════════╣\n";
        cout << "║ ID Pembayaran  : " << setw(36) << id_pembayaran << " ║\n";
        cout << "║ ID Transaksi   : " << setw(36) << id_transaksi << " ║\n";
        cout << "║ Metode Bayar   : " << setw(36) << (id_metode == 1 ? "Tunai" : 
                                        (id_metode == 2 ? "Debit" :
                                         (id_metode == 3 ? "Kredit" :
                                          (id_metode == 4 ? "QRIS" : "Transfer")))) << " ║\n";
        cout << "║──────────────────────────────────────────────────────────────║\n";
        cout << "║ Total Tagihan  : " << setw(36) << formatRupiah(total_tagihan) << " ║\n";
        if (biaya_admin_rp > 0) {
            cout << "║ Biaya Admin    : " << setw(36) << formatRupiah(biaya_admin_rp) << " ║\n";
        }
        cout << "║ Jumlah Bayar   : " << setw(36) << formatRupiah(jumlah_bayar) << " ║\n";
        cout << "║──────────────────────────────────────────────────────────────║\n";
        cout << "║ Kembalian      : " << setw(36) << formatRupiah(kembalian) << " ║\n";
        cout << "║ Status         : " << setw(36) << (lunas ? "LUNAS" : "BELUM LUNAS") << " ║\n";
        cout << "╚══════════════════════════════════════════════════════════════╝\n";
    }
};

// ==================== 6. STRUCT TRANSAKSI ====================
// tbl_transaksi
struct Transaksi {
    int id_transaksi;       // PK, AUTO_INCREMENT
    int id_pelanggan;       // FK ke pelanggan
    time_t tanggal_waktu;
    int total_bayar;
    char status[10];        // AKTIF, SELESAI, BATAL
    char file_path[101];
    vector<PesananItem> items;  // list item dalam transaksi
    Pembayaran pembayaran;      // pembayaran (relasi 1:1)
    
    Transaksi() {
        id_transaksi = 0;
        id_pelanggan = 0;
        tanggal_waktu = time(nullptr);
        total_bayar = 0;
        strcpy(status, "AKTIF");
        strcpy(file_path, "data/transaksi.txt");
    }
    
    void hitungTotal() {
        total_bayar = 0;
        for (const auto& item : items) {
            total_bayar += item.subtotal;
        }
    }
    
    void displayHeader() const {
        cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
        cout << "║                       STRUK TRANSAKSI                         ║\n";
        cout << "╠══════════════════════════════════════════════════════════════╣\n";
        cout << "║ ID Transaksi   : " << setw(36) << id_transaksi << " ║\n";
        cout << "║ Tanggal        : " << setw(36) << ctime(&tanggal_waktu) << " ║\n";
        cout << "║ Status         : " << setw(36) << status << " ║\n";
    }
    
    void displayItems() const {
        cout << "╠══════════════════════════════════════════════════════════════╣\n";
        cout << "║                         Daftar Pesanan                        ║\n";
        cout << "╠══════════════════════════════════════════════════════════════╣\n";
        for (const auto& item : items) {
            cout << "║ " << left << setw(20) << item.nama_menu
                 << " x" << setw(3) << item.jumlah
                 << "   " << setw(12) << formatRupiah(item.subtotal)
                 << " ║\n";
        }
        cout << "╠══════════════════════════════════════════════════════════════╣\n";
        cout << "║ TOTAL          : " << setw(36) << formatRupiah(total_bayar) << " ║\n";
        cout << "╚══════════════════════════════════════════════════════════════╝\n";
    }
};

// ==================== 7. STRUCT ANTRIAN DAPUR ====================
// tbl_antrian_dapur
struct AntrianDapur {
    int id_antrian;         // PK, AUTO_INCREMENT
    int id_transaksi;       // FK ke transaksi
    int posisi_antri;
    char nama_menu[51];
    int jumlah;
    time_t waktu_masuk;
    time_t waktu_proses;
    char status_antri[10];  // MENUNGGU, DIPROSES, SELESAI
    
    AntrianDapur() {
        id_antrian = 0;
        id_transaksi = 0;
        posisi_antri = 0;
        nama_menu[0] = '\0';
        jumlah = 0;
        waktu_masuk = time(nullptr);
        waktu_proses = 0;
        strcpy(status_antri, "MENUNGGU");
    }
    
    void display() const {
        cout << "│ " << setw(5) << posisi_antri
             << "│ " << setw(20) << nama_menu
             << "│ " << setw(5) << jumlah
             << "│ " << setw(12) << status_antri << " │\n";
    }
};

// ==================== CLASS DATABASE ====================
class DatabaseRestoran {
private:
    vector<Menu> menu;
    vector<MetodePembayaran> metodePembayaran;
    vector<Pelanggan> pelanggan;
    vector<Transaksi> transaksi;
    vector<Pembayaran> pembayaran;
    vector<AntrianDapur> antrianDapur;
    
    int nextIdPelanggan = 1;
    int nextIdTransaksi = 1;
    int nextIdPembayaran = 1;
    int nextIdAntrian = 1;
    
public:
    DatabaseRestoran() {
        initializeData();
    }
    
    void initializeData() {
        // Inisialisasi Menu (10 menu sesuai ERD)
        menu.push_back(Menu("M001", "Ayam Geprek", 15000, "Makanan Berat"));
        menu.push_back(Menu("M002", "Gado-Gado", 13000, "Makanan Ringan"));
        menu.push_back(Menu("M003", "Jus Alpukat", 15000, "Minuman"));
        menu.push_back(Menu("M004", "Es Teh Manis", 5000, "Minuman"));
        menu.push_back(Menu("M005", "Es Jeruk", 6000, "Minuman"));
        menu.push_back(Menu("M006", "Mie Ayam", 12000, "Makanan Berat"));
        menu.push_back(Menu("M007", "Nasi Goreng", 18000, "Makanan Berat"));
        menu.push_back(Menu("M008", "Rendang", 25000, "Makanan Berat"));
        menu.push_back(Menu("M009", "Soto Ayam", 14000, "Makanan Berat"));
        menu.push_back(Menu("M010", "Teh Hangat", 4000, "Minuman"));
        
        // Inisialisasi Metode Pembayaran (5 metode)
        MetodePembayaran m1, m2, m3, m4, m5;
        
        m1.id_metode = 1;
        strcpy(m1.nama_metode, "Tunai / Cash");
        strcpy(m1.jenis, "TUNAI");
        m1.biaya_admin = 0.0f;
        m1.aktif = true;
        strcpy(m1.deskripsi, "Bayar langsung, hitung kembalian");
        metodePembayaran.push_back(m1);
        
        m2.id_metode = 2;
        strcpy(m2.nama_metode, "Kartu Debit");
        strcpy(m2.jenis, "KARTU_DEBIT");
        m2.biaya_admin = 0.0f;
        m2.aktif = true;
        strcpy(m2.deskripsi, "Gesek / tap kartu debit");
        metodePembayaran.push_back(m2);
        
        m3.id_metode = 3;
        strcpy(m3.nama_metode, "Kartu Kredit");
        strcpy(m3.jenis, "KARTU_KREDIT");
        m3.biaya_admin = 0.02f;
        m3.aktif = true;
        strcpy(m3.deskripsi, "2% biaya admin");
        metodePembayaran.push_back(m3);
        
        m4.id_metode = 4;
        strcpy(m4.nama_metode, "QRIS");
        strcpy(m4.jenis, "QRIS");
        m4.biaya_admin = 0.0f;
        m4.aktif = true;
        strcpy(m4.deskripsi, "Scan QR code");
        metodePembayaran.push_back(m4);
        
        m5.id_metode = 5;
        strcpy(m5.nama_metode, "Transfer Bank");
        strcpy(m5.jenis, "TRANSFER");
        m5.biaya_admin = 0.0f;
        m5.aktif = true;
        strcpy(m5.deskripsi, "Transfer rekening");
        metodePembayaran.push_back(m5);
    }
    
    // ==================== MENU MANAGEMENT ====================
    void displayAllMenu() {
        cout << "\n┌──────────┬──────────────────────┬──────────────┬─────────────────┬──────────┐\n";
        cout << "│ Kode Menu│ Nama Menu            │ Harga        │ Kategori        │ Status   │\n";
        cout << "├──────────┼──────────────────────┼──────────────┼─────────────────┼──────────┤\n";
        for (const auto& m : menu) {
            m.display();
        }
        cout << "└──────────┴──────────────────────┴──────────────┴─────────────────┴──────────┘\n";
    }
    
    Menu* findMenuByKode(const char* kode) {
        for (auto& m : menu) {
            if (strcmp(m.kode_menu, kode) == 0 && m.tersedia) {
                return &m;
            }
        }
        return nullptr;
    }
    
    // ==================== PELANGGAN MANAGEMENT ====================
    int tambahPelanggan(const char* nama, int noMeja) {
        Pelanggan p(nextIdPelanggan++, nama, noMeja);
        pelanggan.push_back(p);
        return p.id_pelanggan;
    }
    
    Pelanggan* getPelangganById(int id) {
        for (auto& p : pelanggan) {
            if (p.id_pelanggan == id) return &p;
        }
        return nullptr;
    }
    
    // ==================== TRANSAKSI & PESANAN ====================
    int buatTransaksi(int idPelanggan) {
        Transaksi t;
        t.id_transaksi = nextIdTransaksi++;
        t.id_pelanggan = idPelanggan;
        transaksi.push_back(t);
        return t.id_transaksi;
    }
    
    Transaksi* getTransaksiById(int id) {
        for (auto& t : transaksi) {
            if (t.id_transaksi == id) return &t;
        }
        return nullptr;
    }
    
    void tambahItemKeTransaksi(int idTransaksi, const char* kodeMenu, int jumlah) {
        Transaksi* t = getTransaksiById(idTransaksi);
        if (!t || strcmp(t->status, "AKTIF") != 0) {
            cout << "Transaksi tidak ditemukan atau sudah selesai!\n";
            return;
        }
        
        Menu* m = findMenuByKode(kodeMenu);
        if (!m) {
            cout << "Menu tidak ditemukan!\n";
            return;
        }
        
        int itemId = t->items.size() + 1;
        PesananItem item(itemId, idTransaksi, m->kode_menu, m->nama_menu, 
                         m->harga, jumlah);
        t->items.push_back(item);
        t->hitungTotal();
        
        // Tambah ke antrian dapur
        tambahKeAntrianDapur(idTransaksi, m->nama_menu, jumlah);
        
        cout << "✓ " << m->nama_menu << " x" << jumlah << " ditambahkan ke pesanan!\n";
    }
    
    void displayTransaksiItems(int idTransaksi) {
        Transaksi* t = getTransaksiById(idTransaksi);
        if (!t) {
            cout << "Transaksi tidak ditemukan!\n";
            return;
        }
        
        if (t->items.empty()) {
            cout << "Belum ada item dalam transaksi ini.\n";
            return;
        }
        
        cout << "\n┌───────┬──────────────────────┬───────┬──────────────┬──────────────┐\n";
        cout << "│ ID    │ Nama Menu            │ Jumlah│ Harga Satuan │ Subtotal     │\n";
        cout << "├───────┼──────────────────────┼───────┼──────────────┼──────────────┤\n";
        for (const auto& item : t->items) {
            item.display();
        }
        cout << "└───────┴──────────────────────┴───────┴──────────────┴──────────────┘\n";
        cout << "\nTotal: " << formatRupiah(t->total_bayar) << "\n";
    }
    
    // ==================== METODE PEMBAYARAN ====================
    void displayAllMetodePembayaran() {
        cout << "\n┌──────┬────────────────┬──────────────┬────────────┬──────────┐\n";
        cout << "│ ID   │ Nama Metode    │ Jenis        │ Biaya Admin│ Status   │\n";
        cout << "├──────┼────────────────┼──────────────┼────────────┼──────────┤\n";
        for (const auto& m : metodePembayaran) {
            if (m.aktif) {
                m.display();
            }
        }
        cout << "└──────┴────────────────┴──────────────┴────────────┴──────────┘\n";
    }
    
    MetodePembayaran* getMetodeById(int id) {
        for (auto& m : metodePembayaran) {
            if (m.id_metode == id && m.aktif) return &m;
        }
        return nullptr;
    }
    
    // ==================== PEMBAYARAN ====================
    bool prosesPembayaran(int idTransaksi, int idMetode, int jumlahBayar) {
        Transaksi* t = getTransaksiById(idTransaksi);
        if (!t) {
            cout << "Transaksi tidak ditemukan!\n";
            return false;
        }
        
        if (strcmp(t->status, "AKTIF") != 0) {
            cout << "Transaksi sudah selesai atau dibatalkan!\n";
            return false;
        }
        
        MetodePembayaran* metode = getMetodeById(idMetode);
        if (!metode) {
            cout << "Metode pembayaran tidak valid!\n";
            return false;
        }
        
        Pembayaran p;
        p.id_pembayaran = nextIdPembayaran++;
        p.id_transaksi = idTransaksi;
        p.id_metode = idMetode;
        p.jumlah_bayar = jumlahBayar;
        p.total_tagihan = t->total_bayar;
        p.biaya_admin_rp = (int)(t->total_bayar * metode->biaya_admin);
        p.total_tagihan += p.biaya_admin_rp;
        p.hitungKembalian();
        p.waktu_bayar = time(nullptr);
        strcpy(p.catatan, metode->deskripsi);
        
        if (p.kembalian >= 0) {
            p.lunas = true;
            strcpy(t->status, "SELESAI");
            
            pembayaran.push_back(p);
            t->pembayaran = p;
            
            cout << "\n✅ Pembayaran BERHASIL!\n";
            p.display();
            return true;
        } else {
            cout << "\n❌ Pembayaran GAGAL! Jumlah bayar kurang " 
                 << formatRupiah(-p.kembalian) << "\n";
            return false;
        }
    }
    
    // ==================== ANTRIAN DAPUR ====================
    void tambahKeAntrianDapur(int idTransaksi, const char* namaMenu, int jumlah) {
        AntrianDapur a;
        a.id_antrian = nextIdAntrian++;
        a.id_transaksi = idTransaksi;
        a.posisi_antri = antrianDapur.size() + 1;
        strcpy(a.nama_menu, namaMenu);
        a.jumlah = jumlah;
        strcpy(a.status_antri, "MENUNGGU");
        antrianDapur.push_back(a);
    }
    
    void displayAntrianDapur() {
        if (antrianDapur.empty()) {
            cout << "\n═══════════════════════════════════════════════════════════════\n";
            cout << "                     ANTRIAN DAPUR KOSONG!\n";
            cout << "═══════════════════════════════════════════════════════════════\n";
            return;
        }
        
        cout << "\n┌───────┬──────────────────────┬───────┬──────────────┐\n";
        cout << "│ Posisi│ Nama Menu            │ Jumlah│ Status       │\n";
        cout << "├───────┼──────────────────────┼───────┼──────────────┤\n";
        
        // Tampilkan antrian yang belum selesai
        int displayCount = 0;
        for (const auto& a : antrianDapur) {
            if (strcmp(a.status_antri, "SELESAI") != 0) {
                a.display();
                displayCount++;
            }
        }
        
        if (displayCount == 0) {
            cout << "│                           SEMUA SELESAI!                           │\n";
        }
        cout << "└───────┴──────────────────────┴───────┴──────────────┘\n";
    }
    
    void prosesAntrianSelanjutnya() {
        for (auto& a : antrianDapur) {
            if (strcmp(a.status_antri, "MENUNGGU") == 0) {
                strcpy(a.status_antri, "DIPROSES");
                cout << "\n🔪 Memproses: " << a.nama_menu << " x" << a.jumlah << "\n";
                return;
            }
        }
        cout << "\nTidak ada antrian yang menunggu!\n";
    }
    
    void selesaikanAntrian() {
        for (auto& a : antrianDapur) {
            if (strcmp(a.status_antri, "DIPROSES") == 0) {
                strcpy(a.status_antri, "SELESAI");
                cout << "\n✅ Selesai: " << a.nama_menu << " x" << a.jumlah << "\n";
                return;
            }
        }
        cout << "\nTidak ada antrian yang sedang diproses!\n";
    }
    
    // ==================== LAPORAN ====================
    void laporanPenjualan() {
        cout << "\n═══════════════════════════════════════════════════════════════\n";
        cout << "                      LAPORAN PENJUALAN                         \n";
        cout << "═══════════════════════════════════════════════════════════════\n";
        
        if (pembayaran.empty()) {
            cout << "Belum ada transaksi pembayaran.\n";
            return;
        }
        
        int totalPendapatan = 0;
        int totalAdmin = 0;
        
        cout << "\n┌────────────┬──────────────────┬──────────────────┬──────────────┐\n";
        cout << "│ ID Transaksi│ Metode           │ Total Tagihan    │ Biaya Admin  │\n";
        cout << "├────────────┼──────────────────┼──────────────────┼──────────────┤\n";
        
        for (const auto& p : pembayaran) {
            if (p.lunas) {
                cout << "│ " << setw(10) << p.id_transaksi
                     << "│ " << setw(16) << (p.id_metode == 1 ? "Tunai" :
                                           (p.id_metode == 2 ? "Debit" :
                                            (p.id_metode == 3 ? "Kredit" :
                                             (p.id_metode == 4 ? "QRIS" : "Transfer"))))
                     << "│ " << setw(16) << formatRupiah(p.total_tagihan - p.biaya_admin_rp)
                     << "│ " << setw(12) << formatRupiah(p.biaya_admin_rp) << " │\n";
                totalPendapatan += (p.total_tagihan - p.biaya_admin_rp);
                totalAdmin += p.biaya_admin_rp;
            }
        }
        
        cout << "├────────────┼──────────────────┼──────────────────┼──────────────┤\n";
        cout << "│ TOTAL      │                  │ " << setw(16) << formatRupiah(totalPendapatan)
             << "│ " << setw(12) << formatRupiah(totalAdmin) << " │\n";
        cout << "└────────────┴──────────────────┴──────────────────┴──────────────┘\n";
        
        cout << "\nTotal Pendapatan Bersih: " << formatRupiah(totalPendapatan) << "\n";
        cout << "Total Biaya Admin: " << formatRupiah(totalAdmin) << "\n";
    }
    
    void laporanPerMetode() {
        cout << "\n═══════════════════════════════════════════════════════════════\n";
        cout "                   LAPORAN PER METODE PEMBAYARAN                  \n";
        cout << "═══════════════════════════════════════════════════════════════\n";
        
        int statMetode[6] = {0};       // jumlah transaksi per metode
        int pendapatanMetode[6] = {0}; // pendapatan per metode
        
        for (const auto& p : pembayaran) {
            if (p.lunas && p.id_metode >= 1 && p.id_metode <= 5) {
                statMetode[p.id_metode]++;
                pendapatanMetode[p.id_metode] += (p.total_tagihan - p.biaya_admin_rp);
            }
        }
        
        cout << "\n┌──────────────────┬──────────────────┬──────────────────┐\n";
        cout << "│ Metode           │ Jumlah Transaksi │ Total Pendapatan │\n";
        cout << "├──────────────────┼──────────────────┼──────────────────┤\n";
        
        for (int i = 1; i <= 5; i++) {
            const char* nama = (i == 1 ? "Tunai" : 
                               (i == 2 ? "Debit" :
                                (i == 3 ? "Kredit" :
                                 (i == 4 ? "QRIS" : "Transfer"))));
            if (statMetode[i] > 0) {
                cout << "│ " << setw(16) << nama
                     << "│ " << setw(16) << statMetode[i]
                     << "│ " << setw(16) << formatRupiah(pendapatanMetode[i]) << " │\n";
            }
        }
        
        cout << "└──────────────────┴──────────────────┴──────────────────┘\n";
    }
};

// ==================== MAIN PROGRAM ====================
int main() {
    DatabaseRestoran db;
    int pilihan;
    int currentPelanggan = -1;
    int currentTransaksi = -1;
    
    cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║                    🌟 SELAMAT DATANG DI 🌟                     ║\n";
    cout << "║                       R E S T O  D B                          ║\n";
    cout << "║                   Sistem Manajemen Restoran                   ║\n";
    cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    do {
        cout << "\n═══════════════════════════════════════════════════════════════\n";
        cout << "                         MENU UTAMA                             \n";
        cout << "═══════════════════════════════════════════════════════════════\n";
        cout << " 1. Menu Restoran\n";
        cout << " 2. Pelanggan Baru\n";
        cout << " 3. Buat Transaksi Baru\n";
        cout << " 4. Tambah Pesanan\n";
        cout << " 5. Lihat Pesanan Saya\n";
        cout << " 6. Metode Pembayaran\n";
        cout << " 7. Proses Pembayaran\n";
        cout << " 8. Antrian Dapur\n";
        cout << " 9. Proses Antrian\n";
        cout << "10. Laporan Penjualan\n";
        cout << "11. Laporan Per Metode\n";
        cout << " 0. Keluar\n";
        cout << "═══════════════════════════════════════════════════════════════\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        
        switch (pilihan) {
            case 1: {
                db.displayAllMenu();
                break;
            }
            case 2: {
                char nama[51];
                int meja;
                cout << "\n--- PELANGGAN BARU ---\n";
                cout << "Nama Pelanggan: ";
                cin.ignore();
                cin.getline(nama, 50);
                cout << "Nomor Meja (1-50): ";
                cin >> meja;
                currentPelanggan = db.tambahPelanggan(nama, meja);
                cout << "\n✅ Pelanggan berhasil didaftarkan! ID: " << currentPelanggan << "\n";
                break;
            }
            case 3: {
                if (currentPelanggan == -1) {
                    cout << "\n⚠️ Silakan daftarkan pelanggan terlebih dahulu (menu 2)!\n";
                    break;
                }
                currentTransaksi = db.buatTransaksi(currentPelanggan);
                cout << "\n✅ Transaksi baru berhasil dibuat! ID Transaksi: " << currentTransaksi