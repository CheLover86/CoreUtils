#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

void list_directory(const char *dir_path, bool flag_a, bool flag_l, bool flag_R, bool print_header);

void print_permissions(mode_t mode) {
	if (S_ISDIR(mode)) printf("d");
	else if (S_ISLNK(mode)) printf("l");
	else printf("-");

	printf((mode & S_IRUSR) ? "r" : "-");
	printf((mode & S_IWUSR) ? "w" : "-");
	printf((mode & S_IXUSR) ? "x" : "-");


	printf((mode & S_IRGRP) ? "r" : "-");
	printf((mode & S_IWGRP) ? "w" : "-");
	printf((mode & S_IXGRP) ? "x" : "-");
	
	printf((mode & S_IROTH) ? "r" : "-");
	printf((mode & S_IWOTH) ? "w" : "-");
	printf((mode & S_IXOTH) ? "x" : "-");

	printf(" ");
}

// Helper function 4 long format
void print_long_format(const char *dir_path, const char *filename) {
	struct stat file_stat;
	char full_path[1024];

	snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, filename);

	if (lstat(full_path, &file_stat) < 0) {
		perror("ls: stat error");
		return;
	}

	print_permissions(file_stat.st_mode);

	printf("%3ld ", (long)file_stat.st_nlink);

	struct passwd *pw = getpwuid(file_stat.st_uid);
	if (pw) printf("%s ", pw->pw_name);
	else printf("%d ", file_stat.st_uid);
	
	struct group *gr = getgrgid(file_stat.st_gid);
	if (gr) printf("%s ", gr->gr_name);
	else printf("%d ", file_stat.st_gid);

	printf("%8ld ", (long)file_stat.st_size);

	char time_buf[80];
	struct tm *tm_info = localtime(&file_stat.st_mtime);
	strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);
	printf("%s ", time_buf);

	printf("%s\n", filename);
}

void list_directory(const char *dir_path, bool flag_a, bool flag_l, bool flag_R, bool print_header) {
	DIR *dir = opendir(dir_path);
	if (dir == NULL) {
		fprintf(stderr, "ls: %s: ", dir_path);
		perror("");
		return;
	}

	if (print_header) {
		printf("%s:\n", dir_path);
	}

	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
		if (!flag_a && entry->d_name[0] == '.') {
			continue;
		}

		if (flag_l) {
			print_long_format(dir_path, entry->d_name);
		} else {
			printf("%s ", entry->d_name);
		}
	}

	if (!flag_l) {
		printf("\n");
	}

	if (flag_R) {
		rewinddir(dir);

		while ((entry = readdir(dir)) != NULL) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
				continue;
			}

			if (!flag_a && entry->d_name[0] == '.') {
				continue;
			}

			char sub_path[2048];
			snprintf(sub_path, sizeof(sub_path), "%s/%s", dir_path, entry->d_name);

			struct stat sub_stat;
			if (lstat(sub_path, &sub_stat) == 0) {
				if (S_ISDIR(sub_stat.st_mode)) {
					printf("\n");
					list_directory(sub_path, flag_a, flag_l, flag_R, true);
				}
			}
		}
	}
	closedir(dir);
}

int main(int argc, char *argv[]) {
   int opt;
   bool flag_a = false;
   bool flag_l = false;
   bool flag_R = false;

	while((opt = getopt(argc, argv, "alR")) != -1) {
		switch (opt) {
			case 'a':
				flag_a = true;
				break;
			case 'l':
				flag_l = true;
				break;
			case 'R':
				flag_R = true;
				break;
			default:
				fprintf(stderr, "Usage: %s [-alR] [directory]\n", argv[0]);
				return EXIT_FAILURE;
		}
	}

	char *dir_path = ".";
	if (optind < argc) {
		dir_path = argv[optind];
	}

	list_directory(dir_path, flag_a, flag_l, flag_R, flag_R);
	
	return EXIT_SUCCESS;
}
