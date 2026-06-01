#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
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
    char status[15];
};

void AddGuest() {

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
    TampilanAwal();

    int pilihan;

    do {
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl;
        cout << setw(16) << "" << "HOTEL PERMAI NUSANTARA"  << endl;
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl; 
        cout << setw(2) << "" << "Total Kamar   : " << endl;
        cout << setw(2) << "" << "Kamar Tersedia: " << endl;
        cout << setw(2) << "" << "Kamar Terisi  : " << endl;   
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl; 
        cout << setw(2) << "" << "1. Add Guest    (Check In)" << endl;
        cout << setw(2) << "" << "2. Show Guests  (Lihat Semua Tamu)" << endl;
        cout << setw(2) << "" << "3. Check Out    (Proses Check Out)" << endl;
        cout << setw(2) << "" << "4. Search Guest (Cari Tamu)" << endl;
        cout << setw(2) << "" << "5. Sort Guest   (Urutkan Data Tamu)" << endl;
        cout << setw(2) << "" << "6. Room Status  (Status Kamar)" << endl;
        cout << setw(2) << "" << "7. EXit" << endl;
        cout << setfill('=') << setw(55) << "" << setfill(' ') << endl; 
        pilihan = validasiInput("PIlih Menu (1-7):");

        switch(pilihan) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                MenuExit();
                break;
            default:
                break;
        }
    }while(pilihan != 7);
}