#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #include <windows.h>
#endif

#define MAX_PEMAIN 10
#define MAX_NAMA 50
#define FILENAME "game_state.txt"
#define LEADERBOARD_FILE "leaderboard.txt"
#define POINT_BENAR 10
#define POINT_BOM -10

typedef struct {
    int jumlah_pemain;
    int giliran;
    int batas_bawah;
    int batas_atas;
    int angka_bom;
} GameState;

typedef struct {
    char nama[MAX_NAMA];
    int points;
    int urutan;  // Untuk menyimpan urutan asli pemain
} Player;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Fungsi untuk membersihkan buffer input
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fungsi untuk memvalidasi nama
int isValidName(const char *nama) {
    // Cek panjang minimal
    if (strlen(nama) < 3) return 0;

    // Cek karakter yang diperbolehkan (huruf, angka, dan spasi)
    for (int i = 0; nama[i]; i++) {
        if (!isalnum(nama[i]) && nama[i] != ' ' && nama[i] != '_') {
            return 0;
        }
    }
    return 1;
}

// Fungsi untuk mengurutkan pemain berdasarkan poin (descending)
void sortLeaderboard(Player *players, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (players[j].points < players[j + 1].points) {
                Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }
}

void saveLeaderboard(Player *players, int count) {
    FILE *file = fopen(LEADERBOARD_FILE, "wb");
    if (file == NULL) {
        printf("Error: Tidak dapat menyimpan leaderboard\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, file);
    fwrite(players, sizeof(Player), count, file);
    fclose(file);
}

int loadLeaderboard(Player *players) {
    FILE *file = fopen(LEADERBOARD_FILE, "rb");
    if (file == NULL) {
        return 0;
    }
    int count;
    fread(&count, sizeof(int), 1, file);
    fread(players, sizeof(Player), count, file);
    fclose(file);
    return count;
}

void displayLeaderboard(Player *players, int count) {
    printf("\n=================== LEADERBOARD ===================\n");
    printf("+---------+-----------------+--------+-------------+\n");
    printf("| Ranking | Nama Pemain     | Poin   | No. Pemain |\n");
    printf("+---------+-----------------+--------+-------------+\n");

    for (int i = 0; i < count; i++) {
        printf("| %-7d | %-15s | %-6d | %-11d |\n",
               i + 1, players[i].nama, players[i].points, players[i].urutan + 1);
    }

    printf("+---------+-----------------+--------+-------------+\n");
}

void initializeGame(GameState *state, Player *players) {
    printf("\n============= GAME TEBAK ANGKA BOM =============\n\n");
    do {
        printf("Masukkan jumlah pemain (1 - %d): ", MAX_PEMAIN);
        if (scanf("%d", &state->jumlah_pemain) != 1) {
            clearInputBuffer();
            printf("Input tidak valid! Masukkan angka.\n");
            continue;
        }
        clearInputBuffer();

        if (state->jumlah_pemain < 1 || state->jumlah_pemain > MAX_PEMAIN) {
            printf("Jumlah pemain harus antara 1 dan %d!\n", MAX_PEMAIN);
        }
    } while (state->jumlah_pemain < 1 || state->jumlah_pemain > MAX_PEMAIN);

    printf("\n============= REGISTRASI PEMAIN =============\n");
    // Input nama pemain
    for (int i = 0; i < state->jumlah_pemain; i++) {
        do {
            printf("\nPemain %d:\n", i + 1);
            printf("Masukkan nama (min. 3 karakter, hanya huruf, angka, spasi, dan _): ");
            if (fgets(players[i].nama, MAX_NAMA, stdin) != NULL) {
                // Hapus newline di akhir string jika ada
                players[i].nama[strcspn(players[i].nama, "\n")] = 0;

                if (isValidName(players[i].nama)) {
                    players[i].points = 0;
                    players[i].urutan = i;  // Simpan urutan asli pemain
                    break;
                }
            }
            printf("Nama tidak valid! Silakan coba lagi.\n");
        } while (1);
    }

    state->giliran = 0;
    state->batas_bawah = 1;
    state->batas_atas = 100;

    srand(time(NULL));
    state->angka_bom = (rand() % 100) + 1;

    printf("\n=========== PERMAINAN DIMULAI! ===========\n");
    printf("Setiap tebakan benar: +%d poin\n", POINT_BENAR);
    printf("Tebakan angka bom: %d poin\n", POINT_BOM);
    printf("==========================================\n\n");
}

void saveGameState(GameState *state) {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error: Tidak dapat menyimpan state permainan\n");
        return;
    }

    fwrite(state, sizeof(GameState), 1, file);
    fclose(file);
}

int loadGameState(GameState *state) {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        return 0;
    }

    fread(state, sizeof(GameState), 1, file);
    fclose(file);
    return 1;
}

void playGame(GameState *state, Player *players) {
    while (1) {
        clearScreen();
        int currentPlayer = state->giliran % state->jumlah_pemain;
        printf("\n================================================\n");
        printf("Giliran: %s (Pemain %d)\n", players[currentPlayer].nama, currentPlayer + 1);
        printf("Poin saat ini: %d\n", players[currentPlayer].points);
        printf("================================================\n");

        int angka;

        if (state->batas_bawah == state->batas_atas) {
            printf("\nRentang hanya memiliki satu angka (%d).\n", state->batas_bawah);
            printf("%s harus menebak angka ini.\n", players[currentPlayer].nama);
            break;
        }

        do {
            printf("Masukkan angka (%d - %d): ", state->batas_bawah, state->batas_atas);
            if (scanf("%d", &angka) != 1) {
                clearInputBuffer();
                printf("Input tidak valid! Masukkan angka.\n");
                continue;
            }
            clearInputBuffer();

            if (angka < state->batas_bawah || angka > state->batas_atas) {
                printf("Angka harus dalam rentang %d - %d!\n",
                       state->batas_bawah, state->batas_atas);
            }
        } while (angka < state->batas_bawah || angka > state->batas_atas);

        if (angka == state->angka_bom) {
            printf("\n💣 BOOM! %s menebak angka bom! %d point\n",
                   players[currentPlayer].nama, POINT_BOM);
            players[currentPlayer].points += POINT_BOM;
            break;
        }

        players[currentPlayer].points += POINT_BENAR;
        printf("\n✅ Tebakan benar! %s mendapat +%d point!\n",
               players[currentPlayer].nama, POINT_BENAR);

        if (state->batas_bawah == state->angka_bom && state->batas_atas == state->angka_bom) {
            printf("\n🏆 %s MENANG! Bonus +%d point!\n",
                   players[currentPlayer].nama, POINT_BENAR * 2);
            players[currentPlayer].points += POINT_BENAR * 2;
            break;
        }

        if (angka < state->angka_bom) {
            state->batas_bawah = angka + 1;
        } else {
            state->batas_atas = angka - 1;
        }

        printf("\nRentang baru: %d - %d\n", state->batas_bawah, state->batas_atas);
        printf("\nTekan Enter untuk melanjutkan ke pemain berikutnya...");
        getchar();  // Tunggu user menekan Enter

        state->giliran++;
        saveGameState(state);
    }

    printf("\n=============== PERMAINAN SELESAI ===============\n");
    // Sort dan tampilkan leaderboard
    sortLeaderboard(players, state->jumlah_pemain);
    displayLeaderboard(players, state->jumlah_pemain);
    saveLeaderboard(players, state->jumlah_pemain);
}
