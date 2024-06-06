## Available dirs
/bin
/boot
/dev
/etc
/grub
/home
/lib
/lib64
/lost+found
/media
/mnt
/opt
/proc
/root
/run
/sbin
/sys
/tmp
/usr
/var
./Build
./Documents
./Downloads
./Multimedia
./Projects
./Projects/AoC
./Projects/C
./Projects/C/awm
./Projects/C/Kokos
./Projects/C/OpenGL
./Projects/C/Radix-Tree
./Projects/C/Tamagotchi
./Projects/C++
./Projects/C++/PRiR
./Projects/C++/PRiR-inverse-matrix
./Projects/Java
./Projects/Java/aiw
./Projects/Java/edf-visualizer
./Projects/Java/SONB
./Projects/Java/tests
./Projects/Python
./Projects/Web
./Projects/Web/concertify
./Projects/Web/Litter
./Projects/Web/React-Tick-Tac-Toe
./Projects/Web/TI
./Projects/Web/ZSIZI
./Scripts
./Scripts/aliases
./Scripts/files
./Scripts/music
./Scripts/programs
./Scripts/system
./System

## Commands and results
/D -> /dev
doc -> Documents
jav -> Projects/Java
pp -> Projects/C++
c -> Projects/C
p -> Projects
~ -> $HOME
p t -> Projects/Web/TI
p c t -> Projects/C/Tamagotchi
s -> System
sc s -> Scripts/system
a -> Scripts/aliases
A -> Scripts/AoC
inv -> Projects/PRiR-inverse-matrix
./a -> ERROR
p/g -> ERROR


## Scoring priority
- depth
later components more important:
- start/end of dirname
- start/end of word
- case sensitivity
- length of word that matches
- length of dirname that matches
