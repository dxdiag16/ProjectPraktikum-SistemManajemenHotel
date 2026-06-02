#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string.h>
#include <sstream>
using namespace std;

int validasiInput(string prompt) {
    string input;
    int value;

    while(true) {
        cout << prompt;
        getline(cin, input);

        bool valid = true;

        for(char c : input) {
            if(!isdigit(c)) {
                valid = false;
                break;
            }
        }

        if(valid && !input.empty()) {
            value = stoi(input);
            if(value < 0) {
                cout << "Input tidak boleh negatif!" << endl;
                continue;
            }
            return value;
        } else {
            cout << "Input harus berupa angka!" << endl;
        }
    }
}

struct TipeKamar {
    char namaTipe[20];
    int hargaPerMalam;
    int kapasitas;
    char fasilitas[100];
};

struct Kamar {
    int noKamar;
    TipeKamar tipeKamar;
    char status[15];
    char namaTamu[50];
};

struct Tamu {
    char namaTamu[50];
    char noKTP[20];
    int noKamar;
    TipeKamar tipeKamar;
    char tanggalCheckIn[15];
    char tanggalCheckOutRencana[15]; 
    char tanggalCheckOutAktual[15];
    int lamaMenginap;
    char noTelepon[15];
    char status[15];
};

struct Tanggal {
    int hari;
    int bulan;
    int tahun;
};

Tanggal StringKeTanggal(string tanggal) {
    Tanggal t;

    t.hari = stoi(tanggal.substr(0,2));
    t.bulan = stoi(tanggal.substr(3,2));
    t.tahun = stoi(tanggal.substr(6,4));

    return t;
}

Tanggal HitungCheckOut(Tanggal masuk, int lamaMenginap) {
    // tambahkan angka 0 di awal, supaya indeks 1 = Januari, indeks 12 = Desember
    int hariDalamBulan[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    masuk.hari += lamaMenginap;

    while(true) {

        int batasHari = hariDalamBulan[masuk.bulan];
        if((masuk.bulan == 2)) {
            if((masuk.tahun % 4 == 0 && masuk.tahun % 100 != 0) || (masuk.tahun % 400 == 0)) {
                batasHari = 29;
            }
        }

        if(masuk.hari <= batasHari) {
            break;
        }
        
        masuk.hari -= batasHari;
        masuk.bulan++;

        if(masuk.bulan > 12) {
            masuk.bulan = 1;
            masuk.tahun++;
        }
    }

    return masuk;
}

// cek apakah suatu tahun merupakan tahun kabisat atau bukan
bool TahunKabisat(int tahun) {
    // rumus tahun kabisat itu habis dibagi 4 DAN tidak habis dibagi 100, 
    // atau harus habis dibagi 400 (khusus tahun abad kelipatan 100)
    return (tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0);
}

// mengonversi tanggal ke dalam bentuk satu angka bulat besar (Absolute Date) untuk memudahkan perhitungan selisih hari
int TotalHari(Tanggal t) {
    int total = 0;
    // angka 0 di awal adalah "dummy" supaya: Bulan 1 = Januari, Bulan 2 = Februari, dst
    int hariDalamBulan[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // itung semua hari dari tahun-tahun sebelumnya (dari tahun ke-1)
    for(int y = 1; y < t.tahun; y++) {
        if (TahunKabisat(y) == true) {
            total += 366; // kalau tahun tersebut kabisat, tambah 366 hari ke dalam "tabungan" total
        } else {
            total += 365; // kalau tahun biasa (normal), cuma ditambah 365 hari
        }
    }
    // kalo tahun dari tanggal yang dicek adalah kabisat, Februari diubah jadi 29 hari.
    if (TahunKabisat(t.tahun)) {
        hariDalamBulan[2] = 29;
    }
    // itung semua hari dari bulan-bulan sebelumnya di tahun berjalan
    for(int b = 1; b < t.bulan; b++) {
        total += hariDalamBulan[b];
    }
    // tambahkan sisa hari pada bulan yang sekarang
    total += t.hari;
    return total;
}

int HitungLamaMenginap(Tanggal masuk, Tanggal keluar) {
    return TotalHari(keluar) - TotalHari(masuk);
}

string TanggalKeString(Tanggal t) {

    char hasil[15];

    sprintf(
        hasil,
        "%02d-%02d-%04d",
        t.hari,
        t.bulan,
        t.tahun
    );

    return hasil;
}

void simpanKeFile(Kamar DaftarKamar[], Tamu DaftarTamu[], int isiDataTamu) {
    ofstream file("hotel.txt");
    if (!file.is_open()) {
        cout << "[ERROR] Gagal membuka file untuk menyimpan data!\n";
        return;
    }

    // 1. Tulis data KAMAR (20 baris fixed)
    for (int i = 0; i < 20; i++) {
        file << "KAMAR " << DaftarKamar[i].noKamar << "|"
             << DaftarKamar[i].tipeKamar.namaTipe << "|"
             << DaftarKamar[i].tipeKamar.hargaPerMalam << "|"
             << DaftarKamar[i].tipeKamar.kapasitas << "|"
             << DaftarKamar[i].tipeKamar.fasilitas << "|"
             << DaftarKamar[i].status << "|"
             << DaftarKamar[i].namaTamu << "\n";
    }

    // 2. Tulis data TAMU (dinamis)
    for (int i = 0; i < isiDataTamu; i++) {
        file << "TAMU " << DaftarTamu[i].namaTamu << "|"
             << DaftarTamu[i].noKTP << "|"
             << DaftarTamu[i].noKamar << "|"
             << DaftarTamu[i].tipeKamar.namaTipe << "|"
             << DaftarTamu[i].tipeKamar.hargaPerMalam << "|"
             << DaftarTamu[i].tanggalCheckIn << "|"
             << DaftarTamu[i].tanggalCheckOutRencana << "|"
             << DaftarTamu[i].tanggalCheckOutAktual << "|"
             << DaftarTamu[i].lamaMenginap << "|"
             << DaftarTamu[i].noTelepon << "|"
             << DaftarTamu[i].status << "\n";
    }

    file.close();
}

// Fungsi untuk membaca data dari hotel.txt saat program dijalankan
bool loadDariFile(Kamar DaftarKamar[], Tamu DaftarTamu[], int &isiDataTamu) {
    ifstream file("hotel.txt");
    if (!file.is_open()) {
        return false; // File belum ada (jalankan inisialisasi default)
    }

    string baris;
    int indexKamar = 0;
    isiDataTamu = 0;

    while (getline(file, baris)) {
        if (baris.empty()) continue;

        stringstream ss(baris);
        string prefix;
        ss >> prefix; // Membaca KAMAR atau TAMU

        // Hilangkan spasi setelah prefix untuk membaca data sisa
        string sisa;
        getline(ss, sisa);
        if (!sisa.empty() && sisa[0] == ' ') sisa.erase(0, 1);

        stringstream lineData(sisa);
        string field;

        if (prefix == "KAMAR" && indexKamar < 20) {
            // Parse data kamar
            getline(lineData, field, '|'); DaftarKamar[indexKamar].noKamar = stoi(field);
            getline(lineData, field, '|'); strcpy(DaftarKamar[indexKamar].tipeKamar.namaTipe, field.c_str());
            getline(lineData, field, '|'); DaftarKamar[indexKamar].tipeKamar.hargaPerMalam = stoi(field);
            getline(lineData, field, '|'); DaftarKamar[indexKamar].tipeKamar.kapasitas = stoi(field);
            getline(lineData, field, '|'); strcpy(DaftarKamar[indexKamar].tipeKamar.fasilitas, field.c_str());
            getline(lineData, field, '|'); strcpy(DaftarKamar[indexKamar].status, field.c_str());
            getline(lineData, field, '|'); strcpy(DaftarKamar[indexKamar].namaTamu, field.c_str());
            indexKamar++;
        } 
        else if (prefix == "TAMU") {
            // Parse data tamu
            getline(lineData, field, '|'); strcpy(DaftarTamu[isiDataTamu].namaTamu, field.c_str());
            getline(lineData, field, '|'); strcpy(DaftarTamu[isiDataTamu].noKTP, field.c_str());
            getline(lineData, field, '|'); DaftarTamu[isiDataTamu].noKamar = stoi(field);
            getline(lineData, field, '|'); strcpy(DaftarTamu[isiDataTamu].tipeKamar.namaTipe, field.c_str());
            getline(lineData, field, '|'); DaftarTamu[isiDataTamu].tipeKamar.hargaPerMalam = stoi(field);
            getline(lineData, field, '|'); strcpy(DaftarTamu[isiDataTamu].tanggalCheckIn, field.c_str());
            getline(lineData, field, '|'); strcpy(DaftarTamu[isiDataTamu].tanggalCheckOutRencana, field.c_str());
            getline(lineData, field, '|'); strcpy(DaftarTamu[isiDataTamu].tanggalCheckOutAktual, field.c_str());
            getline(lineData, field, '|'); DaftarTamu[isiDataTamu].lamaMenginap = stoi(field);
            getline(lineData, field, '|'); strcpy(DaftarTamu[isiDataTamu].noTelepon, field.c_str());
            getline(lineData, field, '|'); strcpy(DaftarTamu[isiDataTamu].status, field.c_str());
            isiDataTamu++;
        }
    }

    file.close();
    return true;
}

void initKamar(Kamar DaftarKamar[], TipeKamar Tipe[]) {
    int index = 0;

    for(int no = 101; no <= 106; no++) {
        DaftarKamar[index].noKamar = no;
        DaftarKamar[index].tipeKamar = Tipe[0];

        strcpy(DaftarKamar[index].status, "TERSEDIA");
        strcpy(DaftarKamar[index].namaTamu, "-");

        index++;
    }

    for(int no = 201; no <= 206; no++) {
        DaftarKamar[index].noKamar = no;
        DaftarKamar[index].tipeKamar = Tipe[1];

        strcpy(DaftarKamar[index].status, "TERSEDIA");
        strcpy(DaftarKamar[index].namaTamu, "-");

        index++;
    }

    for(int no = 301; no <= 305; no++) {
        DaftarKamar[index].noKamar = no;
        DaftarKamar[index].tipeKamar = Tipe[2];

        strcpy(DaftarKamar[index].status, "TERSEDIA");
        strcpy(DaftarKamar[index].namaTamu, "-");

        index++;
    }

    for(int no = 401; no <= 403; no++) {
        DaftarKamar[index].noKamar = no;
        DaftarKamar[index].tipeKamar = Tipe[3];

        strcpy(DaftarKamar[index].status, "TERSEDIA");
        strcpy(DaftarKamar[index].namaTamu, "-");

        index++;
    }
}

int HitungKamarTersedia(Kamar DaftarKamar[]) {
    int tersedia = 0;

    for(int i = 0; i < 20; i++)
    {
        if(strcmp(DaftarKamar[i].status, "TERSEDIA") == 0) {
            tersedia++;
        }
    }

    return tersedia;
}

int HitungKamarTerisi(Kamar DaftarKamar[]) {
    int terisi = 0;

    for(int i = 0; i < 20; i++) {
        if(strcmp(DaftarKamar[i].status, "TERISI") == 0) {
            terisi++;
        }
    }

    return terisi;
}

void TampilanRingkasanTipe(Kamar DaftarKamar[], TipeKamar Tipe[]) {
    int total[4] = {0};
    int tersedia[4] = {0};

    int totalKamar = 20;
    int kamarTersedia = 0;
    int kamarTerisi = 0;

    for(int i = 0; i < 20; i++) {

        if(strcmp(DaftarKamar[i].status, "TERSEDIA") == 0) {
            kamarTersedia++;
        } else {
            kamarTerisi++;
        }

        for(int j = 0; j < 4; j++) {

            if(strcmp(DaftarKamar[i].tipeKamar.namaTipe, Tipe[j].namaTipe) == 0) {
                total[j]++;

                if(strcmp(DaftarKamar[i].status, "TERSEDIA") == 0) {
                    tersedia[j]++;
                }
            }
        }
    }

    cout << "  Total Kamar : "
         << totalKamar
         << " | Tersedia : "
         << kamarTersedia
         << " | Terisi : "
         << kamarTerisi
         << endl;

    cout << "\n  Ringkasan per Tipe:\n";

    cout << "  +--------------+--------+-----------+------+---------+\n";
    cout << "  | Tipe         | Total  | Tersedia  | Kap  | Harga   |\n";
    cout << "  +--------------+--------+-----------+------+---------+\n";

    for(int i = 0; i < 4; i++) {
    cout << "  | " << left << setw(12) << Tipe[i].namaTipe
         << " | " << right << setw(5) << total[i]
         << "  | " << setw(8) << tersedia[i]
         << "  | " << setw(3) << Tipe[i].kapasitas
         << "  | " << setw(8) << Tipe[i].hargaPerMalam
         << "|\n";
    }

    cout << "  +--------------+--------+-----------+------+---------+\n";
}

void tampilkanKamarTersedia(Kamar DaftarKamar[], TipeKamar Tipe[]) {

    cout << "\n  Daftar Kamar Tersedia: \n";
    cout << "  No  | Kamar | Lantai | Tipe      | Kap   | Harga/Malam\n";
    cout << "  ----+-------+--------+-----------+-------+-------------\n";

    int nomor = 1;

    for(int i = 0; i < 20; i++) {

        if(strcmp(DaftarKamar[i].status, "TERSEDIA") == 0) {
            int lantai = DaftarKamar[i].noKamar / 100;

            cout << right << setw(3) << "" <<  left << setw(2)  << nomor++ << " | "
                 << left << setw(5) << DaftarKamar[i].noKamar << " | "
                 << right << setw(3) << "" << left << setw(3) << lantai << " | "
                 << left << setw(9) << DaftarKamar[i].tipeKamar.namaTipe << " | "
                 << right << setw(2) << "" << left << setw(3) << DaftarKamar[i].tipeKamar.kapasitas << " | "
                 << right << setw(3) << "" <<  left << setw(7) << DaftarKamar[i].tipeKamar.hargaPerMalam << " | "
                 << endl;
        }
    }

    cout << "  ----+-------+--------+-----------+-------+-------------\n";
}

int CariKamar(Kamar DaftarKamar[], int noKamar) {
    for(int i = 0; i < 20; i++) {
        if(DaftarKamar[i].noKamar == noKamar) {
            return i;
        }
    }
    return -1;
}

void AddGuest(Kamar DaftarKamar[], TipeKamar Tipe[], Tamu DaftarTamu[], int size, int &isiDataTamu) {
    int jumlahTamu;

    system("cls");
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(19) << "" << "TAMBAH DATA TAMU"  << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    jumlahTamu = validasiInput("Jumlah tamu check in (0 untuk kembali): ");

    if(HitungKamarTersedia(DaftarKamar) == 0) {
        cout << "Maaf, seluruh kamar sedang penuh!" << endl;

        system("pause");
        system("cls");
        return;
    }

    if(jumlahTamu > HitungKamarTersedia(DaftarKamar)) {
        cout << "Kamar tersedia hanya " << HitungKamarTersedia(DaftarKamar) << " kamar!" << endl;
        system("pause");
        system("cls");
        return;
    }

    if(jumlahTamu == 0) {
        system("cls");
        return;
    }

    if(jumlahTamu < 0) {
        cout << "Angka tidak valid! coba lagi." << endl;
        return;
    }

    if(isiDataTamu + jumlahTamu > size) {
        cout << "Data tamu penuh!" << endl;
        return;
    }

    cout << setw(55) << "" << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;

   TampilanRingkasanTipe(DaftarKamar, Tipe);
   tampilkanKamarTersedia(DaftarKamar, Tipe);

   for(int i = 0; i < jumlahTamu; i++) {
        cout << "\n";
        int index;
        while(true) {
            cout << "--- Data Tamu ke-" << i+1 << " ---" << endl;
            cout << "No. Kamar        : "; cin >> DaftarTamu[isiDataTamu].noKamar;
        
            index = CariKamar(DaftarKamar,DaftarTamu[isiDataTamu].noKamar);

            if(index == -1) {
                cout << "Kamar tidak ditemukan!" << endl;
                continue;
            }

            if(strcmp(DaftarKamar[index].status, "TERISI") == 0) {
                cout << "Kamar sudah terisi!" << endl;
                continue;
            }   
            break;
        }
        cin.ignore();

        cout << "Nama Tamu        : ";
        cin.getline(DaftarTamu[isiDataTamu].namaTamu, 50);
        cout << "No.KTP           : ";
        cin.getline(DaftarTamu[isiDataTamu].noKTP, 20);
        cout << "Tanggal Check In : ";
        cin.getline(DaftarTamu[isiDataTamu].tanggalCheckIn, 15);
        cout << "Lama Menginap    : "; cin >> DaftarTamu[isiDataTamu].lamaMenginap;
        cin.ignore();

        Tanggal checkIn = StringKeTanggal(DaftarTamu[isiDataTamu].tanggalCheckIn);
        Tanggal checkOut = HitungCheckOut(checkIn, DaftarTamu[isiDataTamu].lamaMenginap);
        string hasil = TanggalKeString(checkOut);
        strcpy(DaftarTamu[isiDataTamu].tanggalCheckOutRencana, hasil.c_str());

        cout << "No Telepon       : ";
        cin.getline(DaftarTamu[isiDataTamu].noTelepon, 15);
        cout << ">> Kamar " << DaftarKamar[index].noKamar << " dipilih. Tgl Check Out: " <<  DaftarTamu[isiDataTamu].tanggalCheckOutRencana << endl;

        DaftarTamu[isiDataTamu].tipeKamar = DaftarKamar[index].tipeKamar;
        strcpy(DaftarKamar[index].status, "TERISI");
        strcpy(DaftarKamar[index].namaTamu, DaftarTamu[isiDataTamu].namaTamu);
        strcpy(DaftarTamu[isiDataTamu].status, "CHECK IN");

        isiDataTamu++;
    }

    simpanKeFile(DaftarKamar, DaftarTamu, isiDataTamu);

    cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;
    cout << jumlahTamu << " data tamu berhasil ditambahkan!" << endl;
    cout << "Kamar tersedia sekarang: " << HitungKamarTersedia(DaftarKamar) << endl;
    cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;

    system("pause");
    system("cls");
}

void ShowGuest(Tamu DaftarTamu[], int &isiDataTamu) {
    system("cls");
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(16) << "" << "DATA TAMU - SEMUA ARSIP"  << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;

    if(isiDataTamu == 0) {
        cout << "Belum ada data tamu!" << endl;
        system("pause");
        system("cls");
        return;
    }

    for(int i = 0; i < isiDataTamu; i++) {
        cout << "Data Tamu Ke-" << i + 1 << endl;
        cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;
        cout << "  Nama Tamu         : " << DaftarTamu[i].namaTamu << endl;
        cout << "  No. KTP           : " << DaftarTamu[i].noKTP << endl;
        cout << "  No. Kamar         : " << DaftarTamu[i].noKamar << endl;
        cout << "  Harga/Malam       : " << DaftarTamu[i].tipeKamar.hargaPerMalam << endl;
        cout << "  Tanggal Check In  : " << DaftarTamu[i].tanggalCheckIn << endl;
        cout << "  Tanggal Check Out : " << DaftarTamu[i].tanggalCheckOutRencana << endl;
        cout << "  Lama Menginap     : " << DaftarTamu[i].lamaMenginap << endl;
        cout << "  No. Telepon       : " << DaftarTamu[i].noTelepon << endl;
        cout << "  Status            : " << DaftarTamu[i].status << endl;
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    }
    system("pause");
    system("cls");
}

void CheckOut(Tamu DaftarTamu[], Kamar DaftarKamar[] ,int &isiDataTamu) {
    system("cls");
    int noKamarCheckOut;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(16) << "" << "PROSES CHECK OUT TAMU"  << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    
    if(isiDataTamu == 0) {
        cout << "Belum ada data tamu!" << endl;
        system("pause");
        system("cls");
        return;
    }

    cout << "Masukkan No. Kamar yang Check Out : "; cin >> noKamarCheckOut;
    bool ditemukan = false;

    for(int i = 0; i < isiDataTamu; i++) {
        if(noKamarCheckOut == DaftarTamu[i].noKamar) {
            if(strcmp(DaftarTamu[i].status, "CHECK OUT") == 0) {
                cout << "Tamu sudah check out!" << endl;
                return;
            }

            ditemukan = true;
            cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;
            cout << "DATA TAMU DITEMUKAN: " << endl;
            cout << "  Nama Tamu         : " << DaftarTamu[i].namaTamu << endl;
            cout << "  No. Kamar         : " << DaftarTamu[i].noKamar << endl;
            cout << "  Tanggal Check In  : " << DaftarTamu[i].tanggalCheckIn << endl;
            cout << "  Check Out Pesan   : " << DaftarTamu[i].tanggalCheckOutRencana << endl;
            cin.ignore();

            cout << "\nMasukkan Tanggal Check Out Aktual (DD-MM-YYYY): ";
            cin.getline(DaftarTamu[i].tanggalCheckOutAktual, 15);

            Tanggal tMasuk   = StringKeTanggal(DaftarTamu[i].tanggalCheckIn);
            Tanggal tRencana = StringKeTanggal(DaftarTamu[i].tanggalCheckOutRencana);
            Tanggal tAktual  = StringKeTanggal(DaftarTamu[i].tanggalCheckOutAktual);
            
            int lamaMenginapRencana = DaftarTamu[i].lamaMenginap; // Rencana awal
            int lamaMenginapAsli    = HitungLamaMenginap(tMasuk, tAktual);  // Fakta dilapangan
            int hariTelat           = HitungLamaMenginap(tRencana, tAktual); // Selisih keterlambatan

            if (lamaMenginapAsli <= 0) lamaMenginapAsli = 1;
            if (lamaMenginapRencana <= 0) lamaMenginapRencana = 1;

            int hargaKamar = DaftarTamu[i].tipeKamar.hargaPerMalam;
            int biayaNormal = lamaMenginapRencana * hargaKamar;
            int denda = 0;

            if (hariTelat > 0) {
                denda = hariTelat * (hargaKamar * 1.5); 
            } else {
                hariTelat = 0; 
            }

            int totalBayar = biayaNormal + denda;

            cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;
            cout << "                 NOTA PEMBAYARAN HOTEL                 \n";
            cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;
            cout << "  Harga Kamar / Malam  : Rp " << hargaKamar << endl;
            cout << "  Rencana Menginap     : " << lamaMenginapRencana << " malam\n";
            cout << "  Total Menginap Asli  : " << lamaMenginapAsli << " malam\n";
            cout << "  Keterlambatan        : " << hariTelat << " hari\n";
            cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;
            cout << "  Biaya Menginap Normal: Rp " << biayaNormal << endl;
            cout << "  Biaya Denda Telat    : Rp " << denda << endl;
            cout << "  ---------------------------------------------------\n";
            cout << "  TOTAL YANG HARUS BAYAR: Rp " << totalBayar << endl;
            cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;

            // 8. KONFIRMASI PEMBAYARAN
            char konfirmasi;
            cout << "Konfirmasi Check Out & Pembayaran? (y/n): "; 
            cin >> konfirmasi;
            cin.ignore();

            if(konfirmasi == 'y' || konfirmasi == 'Y') {
                strcpy(DaftarTamu[i].status, "CHECK OUT");
                int indexKamar = CariKamar(DaftarKamar, DaftarTamu[i].noKamar);
                strcpy(DaftarKamar[indexKamar].status, "TERSEDIA");
                strcpy(DaftarKamar[indexKamar].namaTamu, "-");

                simpanKeFile(DaftarKamar, DaftarTamu, isiDataTamu);

                cout << "Check Out berhasil! Terima kasih telah menginap di Hotel Permai Nusantara." << endl;
            } else {
                cout << "Check Out dibatalkan. Tamu masih terdaftar sebagai CHECK IN." << endl;
            }
            break;
        }
    }

    if(!ditemukan) {
        cout << "\n[ERROR] Tidak ditemukan tamu aktif (CHECK IN) di kamar nomor " << noKamarCheckOut << "!\n";
    }
    system("pause");
    system("cls");
}

void Sorting(Tamu DaftarTamu[], int &isiDataTamu) {
    for(int i = 1; i < isiDataTamu; i++) {
        Tamu key = DaftarTamu[i];

        int j = i - 1;

        while(j >= 0 && strcmp(DaftarTamu[j].namaTamu, key.namaTamu) > 0) {
            DaftarTamu[j + 1] = DaftarTamu[j];
            j--;
        }

        DaftarTamu[j + 1] = key;
    }
}


void SortGuest(Tamu DaftarTamu[], int &isiDataTamu, Kamar DaftarKamar[]){
    system("cls");
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(18) << "" << "URUTKAN DATA TAMU"  << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    
    if(isiDataTamu == 0) {
        cout << "Belum ada data tamu!" << endl;
        system("pause");
        system("cls");
        return;
    }

    cout << "[Proses sorting berdasarkan Nama Tamu (A-Z)...]";
    cin.get();
    cout << "\n";

    Sorting(DaftarTamu, isiDataTamu);

    simpanKeFile(DaftarKamar, DaftarTamu, isiDataTamu);

    for(int i = 0; i < isiDataTamu; i++) {
        cout << "Data Tamu Ke-" << i + 1 << endl;
        cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;
        cout << "  Nama Tamu  : " << DaftarTamu[i].namaTamu << right << setw(8) << "" << " | No. Kamar  : " << DaftarTamu[i].noKamar << endl;
        cout << "  Check In   : " << DaftarTamu[i].tanggalCheckIn << right << setw(1) << "" << " | Lama       : " << DaftarTamu[i].lamaMenginap << " hari" << endl;
        cout << "  Status     : " << DaftarTamu[i].status << endl;
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    }

    system("pause");
    system("cls");
}

int BinarySearchNama(Tamu DaftarTamu[], int &isiDataTamu, string cariNama) {
    int left = 0;
    int right = isiDataTamu - 1;

    while(left <= right) {
        int mid = (left + right) / 2;

        int hasil = strcasecmp((DaftarTamu[mid].namaTamu),(cariNama.c_str()));

        if(hasil == 0)
            return mid;

        else if(hasil < 0)
            left = mid + 1;

        else
            right = mid - 1;
    }
    return -1;
}

void SearchGuest(Tamu DaftarTamu[], int &isiDataTamu) {
    system("cls");
    string cariNama;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(20) << "" << "CARI DATA TAMU"  << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    
    if(isiDataTamu == 0) {
        cout << "Belum ada data tamu!" << endl;
        system("pause");
        system("cls");
        return;
    }
    cout << "Masukkan Nama Tamu : ";
    getline(cin, cariNama);

    cout << "\n";
    cout << "[Memuat Data ke memori...]";
    cin.get();
    cout << "[Mengurutkan data (A-Z) untuk binary search...]"; 
    cin.get();
    cout << "[Binary search sedang berjalan...]"; 
    cin.get();
    cout << "\n";

    cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;
    cout << "HASIL PENCARIAN: " << endl;
    cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;
    Sorting(DaftarTamu, isiDataTamu);
    int indeks = BinarySearchNama(DaftarTamu, isiDataTamu, cariNama);

    if(indeks != -1) {
        for(int i = 0; i < isiDataTamu; i++) {
            cout << "Data Tamu Ke-" << i + 1 << endl;
            cout << "  Nama Tamu         : " << DaftarTamu[i].namaTamu << endl;
            cout << "  No. KTP           : " << DaftarTamu[i].noKTP << endl;
            cout << "  No. Kamar         : " << DaftarTamu[i].noKamar << endl;
            cout << "  Harga/Malam       : " << DaftarTamu[i].tipeKamar.hargaPerMalam << endl;
            cout << "  Tanggal Check In  : " << DaftarTamu[i].tanggalCheckIn << endl;
            cout << "  Tanggal Check Out : " << DaftarTamu[i].tanggalCheckOutRencana << endl;
            cout << "  Lama Menginap     : " << DaftarTamu[i].lamaMenginap << endl;
            cout << "  No. Telepon       : " << DaftarTamu[i].noTelepon << endl;
            cout << "  Status            : " << DaftarTamu[i].status << endl;
            cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
        } 
    } else {
        cout << "Maaf, nama tamu " << cariNama << " tidak ditemukan." << endl;
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    }  
    system("pause");
    system("cls");
}

void RoomStatus(Tamu DaftarTamu[], Kamar DaftarKamar[]) {
    system("cls");
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(17) << "" << "STATUS KAMAR HOTEL"  << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(2) << "" << "Total Kamar   : " << 20 << endl;
    cout << setw(2) << "" << "Kamar Tersedia: " << HitungKamarTersedia(DaftarKamar) << endl;
    cout << setw(2) << "" << "Kamar Terisi  : " << HitungKamarTerisi(DaftarKamar) << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;

    cout << "  No  | Kamar | Lantai | Tipe      | Harga/Malam  | Kap | status   | Tamu\n";
    cout << "------+-------+--------+-----------+--------------+-----+----------+--------------\n";

    int nomor = 1;

    for(int i = 0; i < 20; i++) {
        int lantai = DaftarKamar[i].noKamar / 100;

        cout << right << setw(3) << "" <<  left << setw(2)  << nomor++ << " | "
            << left << setw(5) << DaftarKamar[i].noKamar << " | "
            << right << setw(3) << "" << left << setw(3) << lantai << " | "
            << left << setw(9) << DaftarKamar[i].tipeKamar.namaTipe << " | "
            << right << setw(3) << "" <<  left << setw(9) << DaftarKamar[i].tipeKamar.hargaPerMalam << " | "
            << right << setw(1) << "" << left << setw(2) << DaftarKamar[i].tipeKamar.kapasitas << " | "
            << left << setw(8) << DaftarKamar[i].status << " | "
            << DaftarKamar[i].namaTamu
            << endl;
    }
    cout << "=================================================================================\n";

    system("pause");
    system("cls");
}

void TampilanAwal() {
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(16) << "" << "HOTEL PERMAI NUSANTARA"  << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(12) << "" << "Jl.Sudirman No. 88, Surakarta" << endl;
    cout << setw(12) << "" << "Telp: (0271) 123-456" << endl;
    cout << setw(12) << "" << "Email: info@permainusantara.com" << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(7) << "" << "Selamat Datang di Sistem Manajemen Hotel" << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;

    system("pause");
    system("cls");
}

void MenuExit() {
    system("cls");
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(16) << "" << "HOTEL PERMAI NUSANTARA"  << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << setw(55) << "" << endl;
    cout << setw(3) << "" << "Terima Kasih telah menggunakan"  << endl;
    cout << setw(3) << "" << "Sistem Manajemen Hotel Permai Nusantara."  << endl;
    cout << setw(55) << "" << endl;
    cout << setw(3) << "" << "Sampai jumpa!"  << endl;
    cout << setw(55) << "" << endl;
    cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
    cout << "Program Selesai." << endl;
}

int main() {
    TipeKamar Tipe[4] = {
        {"Standard", 300000, 2, "AC, TV, WiFi"},
        {"Deluxe", 500000, 2, "AC, TV, WiFi, Breakfast"},
        {"Suite", 800000, 3, "AC, TV, WiFi, Breakfast, Bathtub"},
        {"Executive", 1200000, 4, "AC, TV, WiFi, Breakfast, Bathtub, Living Room"}
    };
    Kamar DaftarKamar[20];
    Tamu DaftarTamu[100];
    int isiDataTamu = 0;

    initKamar(DaftarKamar, Tipe);

    // MODIFIKASI FILE HANDLING DI SINI
    if (!loadDariFile(DaftarKamar, DaftarTamu, isiDataTamu)) {
        // Jika file tidak ada, inisialisasi default baru dijalankan
        initKamar(DaftarKamar, Tipe);
        simpanKeFile(DaftarKamar, DaftarTamu, isiDataTamu); // Buat file pertama kali
    }

    TampilanAwal();
    
    int pilihan;

    do {
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
        cout << setw(16) << "" << "HOTEL PERMAI NUSANTARA"  << endl;
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl; 
        cout << setw(2) << "" << "Total Kamar   : " << 20 << endl;
        cout << setw(2) << "" << "Kamar Tersedia: " << HitungKamarTersedia(DaftarKamar) << endl;
        cout << setw(2) << "" << "Kamar Terisi  : " << HitungKamarTerisi(DaftarKamar) << endl;   
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl; 
        cout << setw(2) << "" << "1. Add Guest    (Check In)" << endl;
        cout << setw(2) << "" << "2. Show Guests  (Lihat Semua Tamu)" << endl;
        cout << setw(2) << "" << "3. Check Out    (Proses Check Out)" << endl;
        cout << setw(2) << "" << "4. Search Guest (Cari Tamu)" << endl;
        cout << setw(2) << "" << "5. Sort Guest   (Urutkan Data Tamu)" << endl;
        cout << setw(2) << "" << "6. Room Status  (Status Kamar)" << endl;
        cout << setw(2) << "" << "7. Exit" << endl;
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl; 
        pilihan = validasiInput("PIlih Menu (1-7): ");

        switch(pilihan) {
            case 1:
                AddGuest(DaftarKamar, Tipe, DaftarTamu, 100, isiDataTamu);
                break;
            case 2:
                ShowGuest(DaftarTamu, isiDataTamu);
                break;
            case 3:
                CheckOut(DaftarTamu, DaftarKamar, isiDataTamu);
                break;
            case 4:
                SearchGuest(DaftarTamu, isiDataTamu);
                break;
            case 5:
                SortGuest(DaftarTamu, isiDataTamu, DaftarKamar);
                break;
            case 6:
                RoomStatus(DaftarTamu, DaftarKamar);
                break;
            case 7:
                MenuExit();
                break;
            default:
                cout << "[ERROR] Pilihan tidak valid!\n";
        }
    }while(pilihan != 7);
}