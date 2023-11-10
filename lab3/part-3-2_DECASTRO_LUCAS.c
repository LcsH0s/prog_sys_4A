#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

void printTime(const struct stat *fileStat) {
    printf("Date de création : %s", ctime(&fileStat->st_ctime));
}

int main(int argc, char *argv[]) {
    const char *file_path = argv[1];
    struct stat f_stat;

    if (stat(file_path, &f_stat) == -1) {
        perror("stat");
        return 2;
    }

    printf("Num Inode : %lu\n", (unsigned long)f_stat.st_ino);
    printf("Type de fichier : ");

    if (S_ISREG(f_stat.st_mode)) {
        printf("Fichier Normal\n");
    } else if (S_ISDIR(f_stat.st_mode)) {
        printf("Dossier\n");
    } else if (S_ISLNK(f_stat.st_mode)) {
        printf("Lien symbolique\n");
    } else {
        printf("Autre\n");
    }

    printf("Taille : %ld octets\n", (long)f_stat.st_size);
    printf("Nombre : %ld\n", (long)f_stat.st_nlink);
    printTime(&f_stat);

    return 0;
}
