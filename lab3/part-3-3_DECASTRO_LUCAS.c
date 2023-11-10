#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    const char *filename = argv[1];
    struct stat fs;
    stat(filename, &fs);

    printf("PERMISSIONS:\n\n");
    printf("Owner read: %s\n", (fs.st_mode & S_IRUSR) ? "Yes" : "No");
    printf("Owner write: %s\n", (fs.st_mode & S_IWUSR) ? "Yes" : "No");
    printf("Owner execute: %s\n\n", (fs.st_mode & S_IXUSR) ? "Yes" : "No");
    printf("Group read: %s\n", (fs.st_mode & S_IRGRP) ? "Yes" : "No");
    printf("Group write: %s\n", (fs.st_mode & S_IWGRP) ? "Yes" : "No");
    printf("Group execute: %s\n\n", (fs.st_mode & S_IXGRP) ? "Yes" : "No");
    printf("Others read: %s\n", (fs.st_mode & S_IROTH) ? "Yes" : "No");
    printf("Others write: %s\n", (fs.st_mode & S_IWOTH) ? "Yes" : "No");
    printf("Others execute: %s\n", (fs.st_mode & S_IXOTH) ? "Yes" : "No");

    return 0;
}
