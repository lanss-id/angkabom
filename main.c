#include <stdio.h>
#include <stdlib.h>

#define MAX_PEMAIN 10
#define MAX_NAMA 50

// Forward declarations
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
    int urutan;
} Player;

void initializeGame(GameState *state, Player *players);
void saveGameState(GameState *state);
int loadGameState(GameState *state);
void playGame(GameState *state, Player *players);
void saveLeaderboard(Player *players, int count);
int loadLeaderboard(Player *players);

int main() {
    GameState gameState;
    Player players[MAX_PEMAIN];

    printf("\n==============================================\n");
    printf("         SELAMAT DATANG DI GAME BOMB         \n");
    printf("==============================================\n");

    // Mencoba load game state yang tersimpan
    if (!loadGameState(&gameState)) {
        // Jika tidak ada state tersimpan, mulai game baru
        initializeGame(&gameState, players);
        saveGameState(&gameState);
    } else {
        printf("\nMelanjutkan permainan tersimpan...\n");
        // Load leaderboard jika ada
        if (!loadLeaderboard(players)) {
            printf("Memulai dengan leaderboard baru...\n");
        }
    }

    playGame(&gameState, players);

    printf("\n================ GAME OVER ================\n");
    printf("   Pemain yang menang, ambil hadiah di Edu   \n");
    printf("============================================\n\n");

    // Menghapus file state setelah game selesai
    remove("game_state.txt");

    return 0;
}
