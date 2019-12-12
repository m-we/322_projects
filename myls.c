#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Returns 1 if directory, 0 if file.
int is_dir(char* path) {
         struct stat path_s;
         stat(path, &path_s);
         return S_ISDIR(path_s.st_mode);
}

// Print the timestamp in YYYY/MM/DD HH:MM:SS.
void printtime(int tstamp) {
         struct tm *time = gmtime(&tstamp);
         printf("%d/%02d/%02d %02d:%02d:%02d ", 1900+time->tm_year, 1+time->tm_mon, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
}

// Print permissions rwxrwxrwx.
void permissions(struct stat fstat) {
         printf(fstat.st_mode&S_IRUSR?"r":"-");
         printf(fstat.st_mode&S_IWUSR?"w":"-");
         printf(fstat.st_mode&S_IXUSR?"x":"-");
         printf(fstat.st_mode&S_IRGRP?"r":"-");
         printf(fstat.st_mode&S_IWGRP?"w":"-");
         printf(fstat.st_mode&S_IXGRP?"x":"-");
         printf(fstat.st_mode&S_IROTH?"r":"-");
         printf(fstat.st_mode&S_IWOTH?"w":"-");
         printf(fstat.st_mode&S_IXOTH?"x":"-");
}

// When -l is an option, print the things the assignment said to.
void printlong(char* path, char* name) {
         struct stat fstat;
         stat(path, &fstat);

         struct passwd *pwd = getpwuid(fstat.st_uid);
         struct group *grp = getgrgid(fstat.st_gid);

         if (is_dir(path) == 1) {
                 printf("d ");
         } else {
                 printf("f ");
         }
         permissions(fstat);
         printf(" %s %s ", pwd->pw_name, grp->gr_name);
         printf("%d ", fstat.st_ino);
         printf("%d ", fstat.st_size);
         printtime(fstat.st_mtime);
         printf("%s\n", name);
}

// Call list on a directory.
void list(char* path, int a, int r, int l) {
         DIR *d;
         struct dirent *dir;
         d = opendir(path);
         if (d) {
                 while ((dir = readdir(d)) != NULL) {
                         char p2[80];
                         strcpy(p2, path);
                         strcat(p2, "/");
                         strcat(p2, dir->d_name);
                         if (a == 1 && dir->d_name[0] == '.') {
                                 if (l == 0) {
                                         printf("%s\n", dir->d_name);
                                 } else {
                                         printlong(path, dir->d_name);
                                 }
                         } else if (dir->d_name[0] != '.') {
                                 if (l == 0) {
                                         printf("%s\n", dir->d_name);
                                 } else {
                                         printlong(p2, dir->d_name);
                                 }
                                 if (is_dir(p2) == 1 && r == 1) {
                                         printf("./%s\n", p2);
                                         list(p2, a, r, l);
                                 }
                         }
                 }
                 closedir(d);
         }
}

int main(int argc, char *argv[1]) {
         // dargv tracks where the directory argument is. It is 1 if no options
         // are given and 2 if options are.
         int i, opt, a=0, l=0, r=0, dargv = 0;

         if (argc == 1) {
                 printf("directory required\n");
                 return -1;
         } else if (argc == 2) {
                 dargv = 1;
         } else if (argc == 3) {
                 dargv = 2;
                 while ((opt = getopt(argc, argv, "alR")) != -1) {
                         switch(opt) {
                         case 'a': a = 1; break;
                         case 'l': l = 1; break;
                         case 'R': r = 1; break;
                         case '?': printf("invalid"); return -1; break;
                         default: printf("done\n"); break;
                         }
                 }
         }

         // Error if the directory to be listed is actually a file.
         if (is_dir(argv[dargv]) == 0) {
                 printf("path must be a directory\n");
                 return -1;
         }
         // Call list w/ the respective options.
         list(argv[dargv], a, r, l);

         return 0;
}