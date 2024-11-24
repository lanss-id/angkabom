Halo! Saya akan menjelaskan cara menjalankan game Tebak Angka Bomb ini dengan bahasa yang santai dan mudah dimengerti 😊

1. Pertama, yuk clone repository-nya dulu dari GitHub
```bash
git clone https://github.com/username/game-tebak-angka.git
cd game-tebak-angka
```

2. Pastiin kamu udah punya compiler C di komputermu ya! Kalau belum, gampang kok:
   - Buat Windows: Install MinGW (https://mingw-w64.org)
   - Buat Linux: Biasanya udah ada, tapi kalau belum:
     ```bash
     sudo apt-get install build-essential  # Untuk Ubuntu/Debian
     ```
   - Buat Mac: Install Xcode Command Line Tools:
     ```bash
     xcode-select --install
     ```

3. Sekarang compile program-nya. Santai aja, tinggal ketik:
```bash
gcc main.c game.c -o game
```

4. Nah, sekarang tinggal jalanin game-nya:
   - Di Windows: ketik `game` 
   - Di Linux/Mac: ketik `./game`

5. Game udah jalan! Kamu bakal diminta:
   - Masukin jumlah pemain (1-10 orang)
   - Daftar nama tiap pemain (minimal 3 karakter ya)
   - Terus main deh! Tiap pemain nebak angka antara 1-100
   - Hati-hati sama angka BOM! 💣

Tips main:
- Tiap tebakan bener dapet +10 poin
- Kena BOM? Minus 10 poin deh 😅
- Kalau udah selesai, skornya bakal disimpen di leaderboard
- Bisa pause kapan aja, nanti bisa dilanjutin lagi kok

Yang seru, layarnya bakal otomatis bersih tiap ganti pemain, jadi nggak berantakan. Tinggal pencet Enter aja kalau udah siap ganti pemain selanjutnya!

Mau mulai main? Yang menang nanti ambil hadiahnya di Edu ya! 🎮✨

Gimana? Ada yang masih bingung atau mau ditanyain? 😊
