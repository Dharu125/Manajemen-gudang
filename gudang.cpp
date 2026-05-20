#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

// ============================================================
//  STRUKTUR DATA
// ============================================================

// Node untuk Linked List Tunggal (data barang)
struct Barang {
    char nama[100];
    int  jumlah;
    char tanggal[20]; // format: DD-MM-YYYY
    Barang* next;     // pointer ke node berikutnya (LL tunggal)
};

// Node untuk Linked List Ganda (data admin)
struct Admin {
    char username[50];
    Admin* next; // pointer ke node berikutnya
    Admin* prev; // pointer ke node sebelumnya (LL ganda)
};

// ============================================================
//  VARIABEL GLOBAL (pointer head)
// ============================================================
Barang* headBarang = NULL; // head linked list tunggal barang
Admin*  headAdmin  = NULL; // head linked list ganda admin

// ============================================================
//  FUNGSI UTILITAS
// ============================================================

// Membandingkan tanggal format DD-MM-YYYY
int bandingkanTanggal(const char* a, const char* b) {
    int da, ma, ya, db, mb, yb;
    sscanf(a, "%d-%d-%d", &da, &ma, &ya);
    sscanf(b, "%d-%d-%d", &db, &mb, &yb);
    if (ya != yb) return ya - yb;
    if (ma != mb) return ma - mb;
    return da - db;
}

// Validasi format tanggal DD-MM-YYYY (sederhana)
bool validasiTanggal(const char* tgl) {
    if (strlen(tgl) != 10) return false;
    if (tgl[2] != '-' || tgl[5] != '-') return false;
    int d, m, y;
    if (sscanf(tgl, "%d-%d-%d", &d, &m, &y) != 3) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;
    return true;
}

void pauseLayar() {
    printf("\nTekan Enter untuk melanjutkan...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

// ============================================================
//  MANAJEMEN ADMIN (Linked List Ganda)
// ============================================================

void tambahAdmin(const char* username) {
    Admin* baru = new Admin;
    strcpy(baru->username, username);
    baru->next = NULL;
    baru->prev = NULL;

    if (headAdmin == NULL) {
        headAdmin = baru;
        return;
    }
    Admin* temp = headAdmin;
    while (temp->next != NULL) temp = temp->next;
    temp->next  = baru;
    baru->prev  = temp;
}

void muatAdmin() {
    FILE* f = fopen("admin.dat", "r");
    if (!f) {
        tambahAdmin("admin");
        return;
    }
    char buf[50];
    while (fscanf(f, "%49s", buf) == 1) {
        tambahAdmin(buf);
    }
    fclose(f);
    if (headAdmin == NULL) tambahAdmin("admin");
}

void simpanAdmin() {
    FILE* f = fopen("admin.dat", "w");
    if (!f) { printf("Gagal menyimpan data admin!\n"); return; }
    Admin* temp = headAdmin;
    while (temp != NULL) {
        fprintf(f, "%s\n", temp->username);
        temp = temp->next;
    }
    fclose(f);
}

bool cekAdmin(const char* username) {
    Admin* temp = headAdmin;
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0) return true;
        temp = temp->next;
    }
    return false;
}

void tampilkanAdmin() {
    printf("\n=== Daftar Admin (Traversal Maju - LL Ganda) ===\n");
    Admin* temp = headAdmin;
    Admin* ekor = NULL;
    int no = 1;
    while (temp != NULL) {
        printf("%d. %s\n", no++, temp->username);
        ekor = temp;
        temp = temp->next;
    }
    printf("\n=== Daftar Admin (Traversal Mundur - LL Ganda) ===\n");
    no = 1;
    while (ekor != NULL) {
        printf("%d. %s\n", no++, ekor->username);
        ekor = ekor->prev;
    }
}

void menuTambahAdmin() {
    char usr[50];
    printf("\n=== Tambah Admin Baru ===\n");
    printf("Masukkan username baru: ");
    scanf("%49s", usr);

    if (cekAdmin(usr)) {
        printf("Username '%s' sudah terdaftar!\n", usr);
    } else {
        tambahAdmin(usr);
        simpanAdmin();
        printf("Admin '%s' berhasil ditambahkan.\n", usr);
    }
}

// ============================================================
//  MANAJEMEN BARANG (Linked List Tunggal)
// ============================================================

void tambahBarang(const char* nama, int jumlah, const char* tanggal) {
    Barang* baru = new Barang;
    strcpy(baru->nama, nama);
    baru->jumlah = jumlah;
    strcpy(baru->tanggal, tanggal);
    baru->next = NULL;

    if (headBarang == NULL) {
        headBarang = baru;
        return;
    }
    Barang* temp = headBarang;
    while (temp->next != NULL) temp = temp->next;
    temp->next = baru;
}

void muatBarang() {
    FILE* f = fopen("barang.dat", "r");
    if (!f) return;
    char nama[100], tgl[20];
    int jml;
    while (fscanf(f, " %99[^|]|%d|%19s", nama, &jml, tgl) == 3) {
        tambahBarang(nama, jml, tgl);
    }
    fclose(f);
}

void simpanBarang() {
    FILE* f = fopen("barang.dat", "w");
    if (!f) { printf("Gagal menyimpan data barang!\n"); return; }
    Barang* temp = headBarang;
    while (temp != NULL) {
        fprintf(f, "%s|%d|%s\n", temp->nama, temp->jumlah, temp->tanggal);
        temp = temp->next;
    }
    fclose(f);
}

void menuInputBarang() {
    char nama[100], tgl[20];
    int jml;

    printf("\n=== Input Barang Masuk ===\n");
    printf("Nama barang  : ");
    scanf(" %99[^\n]", nama);
    printf("Jumlah barang: ");
    scanf("%d", &jml);
    if (jml <= 0) { printf("Jumlah harus lebih dari 0!\n"); return; }
    printf("Tanggal masuk (DD-MM-YYYY): ");
    scanf("%19s", tgl);
    if (!validasiTanggal(tgl)) {
        printf("Format tanggal tidak valid! Gunakan DD-MM-YYYY.\n");
        return;
    }

    tambahBarang(nama, jml, tgl);
    simpanBarang();
    printf("Barang '%s' berhasil ditambahkan ke gudang.\n", nama);
}

// ============================================================
//  OUTPUT / TAMPILKAN BARANG
// ============================================================

void cetakHeader() {
    printf("%-5s %-25s %-10s %-12s\n", "No", "Nama Barang", "Jumlah", "Tanggal Masuk");
    printf("%-5s %-25s %-10s %-12s\n", "----", "-------------------------", "----------", "------------");
}

void tampilkanSemuaBarang(bool ascending) {
    if (headBarang == NULL) {
        printf("Tidak ada barang di gudang.\n");
        return;
    }

    int n = 0;
    Barang* temp = headBarang;
    while (temp) { n++; temp = temp->next; }

    Barang** arr = new Barang*[n];
    temp = headBarang;
    for (int i = 0; i < n; i++) { arr[i] = temp; temp = temp->next; }

    // Bubble Sort berdasarkan nama
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++) {
            int cmp = strcmp(arr[j]->nama, arr[j+1]->nama);
            if (ascending ? cmp > 0 : cmp < 0) {
                Barang* t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t;
            }
        }

    printf("\n=== Daftar Barang (%s) ===\n", ascending ? "Ascending A-Z" : "Descending Z-A");
    cetakHeader();
    for (int i = 0; i < n; i++)
        printf("%-5d %-25s %-10d %-12s\n", i+1, arr[i]->nama, arr[i]->jumlah, arr[i]->tanggal);

    delete[] arr;
}

void menuOutput() {
    int pilihan;
    printf("\n=== Output Barang ===\n");
    printf("1. Tampilkan Ascending (A-Z)\n");
    printf("2. Tampilkan Descending (Z-A)\n");
    printf("Pilihan: ");
    scanf("%d", &pilihan);
    if (pilihan == 1) tampilkanSemuaBarang(true);
    else if (pilihan == 2) tampilkanSemuaBarang(false);
    else printf("Pilihan tidak valid.\n");
}

// ============================================================
//  SORTING & SIMPAN HASIL KE FILE
// ============================================================

void menuSorting() {
    if (headBarang == NULL) { printf("Tidak ada barang.\n"); return; }

    int n = 0;
    Barang* temp = headBarang;
    while (temp) { n++; temp = temp->next; }

    Barang** arr = new Barang*[n];
    temp = headBarang;
    for (int i = 0; i < n; i++) { arr[i] = temp; temp = temp->next; }

    int pilihan;
    printf("\n=== Sorting Barang ===\n");
    printf("1. Sort berdasarkan Nama (A-Z)\n");
    printf("2. Sort berdasarkan Tanggal Masuk (Terlama ke Terbaru)\n");
    printf("Pilihan: ");
    scanf("%d", &pilihan);

    if (pilihan != 1 && pilihan != 2) {
        printf("Pilihan tidak valid.\n");
        delete[] arr;
        return;
    }

    // Bubble Sort
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++) {
            bool tukar = false;
            if (pilihan == 1)
                tukar = strcmp(arr[j]->nama, arr[j+1]->nama) > 0;
            else
                tukar = bandingkanTanggal(arr[j]->tanggal, arr[j+1]->tanggal) > 0;
            if (tukar) { Barang* t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t; }
        }

    const char* judulSort = (pilihan == 1) ? "Berdasarkan Nama" : "Berdasarkan Tanggal";
    printf("\n=== Hasil Sorting %s ===\n", judulSort);
    cetakHeader();
    for (int i = 0; i < n; i++)
        printf("%-5d %-25s %-10d %-12s\n", i+1, arr[i]->nama, arr[i]->jumlah, arr[i]->tanggal);

    // Simpan ke file
    FILE* f = fopen("hasil_sorting.txt", "w");
    if (f) {
        fprintf(f, "=== Hasil Sorting %s ===\n", judulSort);
        fprintf(f, "%-5s %-25s %-10s %-12s\n", "No", "Nama Barang", "Jumlah", "Tanggal Masuk");
        fprintf(f, "%-5s %-25s %-10s %-12s\n", "----", "-------------------------", "----------", "------------");
        for (int i = 0; i < n; i++)
            fprintf(f, "%-5d %-25s %-10d %-12s\n", i+1, arr[i]->nama, arr[i]->jumlah, arr[i]->tanggal);
        fclose(f);
        printf("\nHasil sorting disimpan ke file 'hasil_sorting.txt'.\n");
    } else {
        printf("Gagal menyimpan hasil sorting!\n");
    }

    delete[] arr;
}

// ============================================================
//  SEARCHING
// ============================================================

// Binary Search berdasarkan nama (array harus sudah terurut)
int binarySearchNama(Barang** arr, int n, const char* keyword) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int cmp = strcmp(arr[mid]->nama, keyword);
        if (cmp == 0) return mid;
        if (cmp < 0) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

void menuSearch() {
    if (headBarang == NULL) { printf("Tidak ada barang.\n"); return; }

    int n = 0;
    Barang* temp = headBarang;
    while (temp) { n++; temp = temp->next; }

    Barang** arr = new Barang*[n];
    temp = headBarang;
    for (int i = 0; i < n; i++) { arr[i] = temp; temp = temp->next; }

    int pilihan;
    printf("\n=== Cari Barang ===\n");
    printf("1. Cari berdasarkan Nama (Binary Search)\n");
    printf("2. Cari berdasarkan Tanggal (Linear Search)\n");
    printf("Pilihan: ");
    scanf("%d", &pilihan);

    bool ditemukan = false;

    if (pilihan == 1) {
        char keyword[100];
        printf("Masukkan nama barang: ");
        scanf(" %99[^\n]", keyword);

        // Sort untuk binary search
        for (int i = 0; i < n - 1; i++)
            for (int j = 0; j < n - i - 1; j++)
                if (strcmp(arr[j]->nama, arr[j+1]->nama) > 0) {
                    Barang* t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t;
                }

        int idx = binarySearchNama(arr, n, keyword);
        if (idx != -1) {
            printf("\n=== Hasil Pencarian Nama: '%s' ===\n", keyword);
            cetakHeader();
            int no = 1;
            // Cek semua yang namanya sama (duplikat)
            for (int i = 0; i < n; i++)
                if (strcmp(arr[i]->nama, keyword) == 0) {
                    printf("%-5d %-25s %-10d %-12s\n", no++, arr[i]->nama, arr[i]->jumlah, arr[i]->tanggal);
                    ditemukan = true;
                }
        }

    } else if (pilihan == 2) {
        char tgl[20];
        printf("Masukkan tanggal (DD-MM-YYYY): ");
        scanf("%19s", tgl);

        // Linear Search
        printf("\n=== Hasil Pencarian Tanggal: '%s' ===\n", tgl);
        cetakHeader();
        int no = 1;
        for (int i = 0; i < n; i++) {
            if (strcmp(arr[i]->tanggal, tgl) == 0) {
                printf("%-5d %-25s %-10d %-12s\n", no++, arr[i]->nama, arr[i]->jumlah, arr[i]->tanggal);
                ditemukan = true;
            }
        }
    } else {
        printf("Pilihan tidak valid.\n");
        delete[] arr;
        return;
    }

    if (!ditemukan) printf("Barang tidak ditemukan.\n");
    delete[] arr;
}

// ============================================================
//  HAPUS BARANG
// ============================================================

void menuHapusBarang() {
    if (headBarang == NULL) { printf("Tidak ada barang.\n"); return; }

    char nama[100];
    printf("\n=== Hapus Barang ===\n");
    printf("Masukkan nama barang yang ingin dihapus: ");
    scanf(" %99[^\n]", nama);

    Barang* curr = headBarang;
    Barang* prev = NULL;
    bool ditemukan = false;

    while (curr != NULL) {
        if (strcmp(curr->nama, nama) == 0) {
            ditemukan = true;
            printf("Hapus '%s' (Jml: %d, Tgl: %s)? (y/n): ",
                   curr->nama, curr->jumlah, curr->tanggal);
            char k;
            scanf(" %c", &k);
            if (k == 'y' || k == 'Y') {
                if (prev == NULL) headBarang = curr->next;
                else prev->next = curr->next;
                Barang* hapus = curr;
                curr = curr->next;
                delete hapus;
                printf("Barang berhasil dihapus.\n");
            } else {
                prev = curr;
                curr = curr->next;
            }
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    if (!ditemukan) printf("Barang '%s' tidak ditemukan.\n", nama);
    else simpanBarang();
}

// ============================================================
//  EDIT STOK BARANG
// ============================================================

void menuEditBarang() {
    if (headBarang == NULL) { printf("Tidak ada barang.\n"); return; }

    char nama[100];
    printf("\n=== Edit Stok Barang ===\n");
    printf("Masukkan nama barang yang ingin diedit: ");
    scanf(" %99[^\n]", nama);

    Barang* temp = headBarang;
    bool ditemukan = false;

    while (temp != NULL) {
        if (strcmp(temp->nama, nama) == 0) {
            ditemukan = true;
            printf("Ditemukan: %s | Stok: %d | Tanggal: %s\n",
                   temp->nama, temp->jumlah, temp->tanggal);
            int stokBaru;
            printf("Masukkan stok baru: ");
            scanf("%d", &stokBaru);
            if (stokBaru < 0) {
                printf("Stok tidak boleh negatif!\n");
            } else {
                temp->jumlah = stokBaru;
                printf("Stok berhasil diperbarui menjadi %d.\n", stokBaru);
                simpanBarang();
            }
        }
        temp = temp->next;
    }

    if (!ditemukan) printf("Barang '%s' tidak ditemukan.\n", nama);
}

// ============================================================
//  LOGIN
// ============================================================

bool login() {
    char username[50];
    int percobaan = 0;
    const int MAX_PERCOBAAN = 3;

    printf("========================================\n");
    printf("|      SISTEM MANAJEMEN GUDANG         |\n");
    printf("|   Silakan Login untuk Melanjutkan    |\n");
    printf("========================================\n\n");

    while (percobaan < MAX_PERCOBAAN) {
        printf("Username: ");
        scanf("%49s", username);

        if (cekAdmin(username)) {
            printf("\nLogin berhasil! Selamat datang, %s.\n\n", username);
            return true;
        } else {
            percobaan++;
            printf("Username tidak ditemukan! Percobaan %d/%d.\n\n", percobaan, MAX_PERCOBAAN);
        }
    }

    printf("Terlalu banyak percobaan gagal. Program ditutup.\n");
    return false;
}

// ============================================================
//  MENU UTAMA
// ============================================================

void tampilkanMenu() {
    printf("\n=======================================\n");
    printf("|        MENU UTAMA - GUDANG           |\n");
    printf("========================================\n");
    printf("|  1. Input Barang                     |\n");
    printf("|  2. Output / Tampilkan Barang        |\n");
    printf("|  3. Sorting Barang                   |\n");
    printf("|  4. Search Barang                    |\n");
    printf("|  5. Hapus Barang                     |\n");
    printf("|  6. Edit Stok Barang                 |\n");
    printf("|  7. Tambah Admin                     |\n");
    printf("|  8. Keluar                           |\n");
    printf("========================================\n");
    printf("Pilihan: ");
}

// ============================================================
//  FREE MEMORY
// ============================================================

void bebaskanMemori() {
    Barang* b = headBarang;
    while (b) { Barang* tmp = b->next; delete b; b = tmp; }
    Admin* a = headAdmin;
    while (a) { Admin* tmp = a->next; delete a; a = tmp; }
}

// ============================================================
//  MAIN
// ============================================================

int main() {
    muatAdmin();
    muatBarang();

    if (!login()) {
        bebaskanMemori();
        return 0;
    }

    int pilihan;
    bool jalan = true;

    while (jalan) {
        tampilkanMenu();
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: menuInputBarang();  pauseLayar(); break;
            case 2: menuOutput();       pauseLayar(); break;
            case 3: menuSorting();      pauseLayar(); break;
            case 4: menuSearch();       pauseLayar(); break;
            case 5: menuHapusBarang();  pauseLayar(); break;
            case 6: menuEditBarang();   pauseLayar(); break;
            case 7:
                menuTambahAdmin();
                tampilkanAdmin();
                pauseLayar();
                break;
            case 8:
                printf("\nTerima kasih telah menggunakan Sistem Manajemen Gudang.\n");
                jalan = false;
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                pauseLayar();
        }
    }

    bebaskanMemori();
    return 0;
}
