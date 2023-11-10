#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>


#define FILENAME_MAX_LENGTH 256

void generation_password_aleatoire(char *mdp, int l) {
    const char mdp_alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < l; i++) {
        int j = rand() % (sizeof(mdp_alphabet) - 1);
        mdp[i] = mdp_alphabet[j];
    }
    mdp[l] = '\0';
}

int main(int argc, char *argv[]) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    srand(time(NULL)); // time seeding for password

    char date_str[20];
    struct passwd *pwd = getpwuid(getuid());
    char mdp[11];
    FILE *file = fopen(argv[1], "w");

    if (file == NULL) {
        perror("Erreur: Le fichier n'a pas pu etre ouvert.");
        return 1;
    }

    strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", tm_info);
    generation_password_aleatoire(mdp, 10);

    fprintf(file, "<new_user>\n");
    fprintf(file, "  <LDC_date>%s</LDC_date>\n", date_str);
    fprintf(file, "  <LDC_name>%s</LDC_name>\n", pwd->pw_name);
    fprintf(file, "  <LDC_uid>%d</LDC_uid>\n", pwd->pw_uid);
    fprintf(file, "  <LDC_password>%s</LDC_password>\n", mdp);
    fprintf(file, "</new_user>\n");
    fclose(file);

    chmod(argv[1], S_IRUSR);
    return 0;
}
