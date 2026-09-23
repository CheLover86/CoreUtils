#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    char *dir_path = ".";  
    
    if (argc > 1) {
        dir_path = argv[1];
    }

    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("ls: Cannot open directory");
        return EXIT_FAILURE;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        
        printf("%s  ", entry->d_name);
    }
    
    printf("\n");

    closedir(dir);
    return EXIT_SUCCESS;
}
