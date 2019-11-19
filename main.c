#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>

//recursively scans all files and subdirectories. inner is used to determine the spacing of the print for subfiles and subdirectories
int scan_directory(DIR * dir_stream, char dir_name[256],int inner){
  int totalsize = 0;
  char file_name[256];
  char full_name[512];
  struct dirent * file = readdir(dir_stream);
  struct stat file_info;
  //until we reach the end, loop through
  while(file != NULL){
    strcpy(full_name,dir_name);
    strcat(full_name,"/");
    strcat(full_name,file->d_name);
    stat(full_name, &file_info);
    //if its a regular file
    if (S_ISREG(file_info.st_mode)){
      //inner is used here
      for(size_t i = 0; i < inner; i++){
        printf("\t");
      }
      printf("%s: %ld bytes\n",full_name,file_info.st_size);
      totalsize+=file_info.st_size;
    //if its a subdirectory
    } else if (S_ISDIR(file_info.st_mode)){
        for(size_t i = 0; i < inner; i++){
          printf("\t");
        }
        printf("%s\n",full_name);
        //if the subdirectory isn't "." or ".." recursively scan it
        if(strcmp(file->d_name,".") != 0 && strcmp(file->d_name,"..") !=0){
          DIR * sub_dir_stream = opendir(full_name);
          totalsize+=scan_directory(sub_dir_stream,full_name,inner+1);
        }
    }

    file = readdir(dir_stream);
  }
  return totalsize;
}

int main(int argc, char const * argv[]) {
  char dir_name[256];

  //scan input for directory name
  if (argc > 1) {
      strcpy(dir_name,argv[1]);
    } else{
      printf("You did not enter a directory to scan\nWe recommend scanning \'dir-testing\'.Please enter a directory to scan:\n");
      char input[256];
      while(fgets(input, 256, stdin) == NULL){
      }
      input[strlen(input)-1] = 0;
      strcpy(dir_name,input);
    }

  DIR * dir_stream= opendir(dir_name); // open directory
  if (dir_stream == NULL) {
      printf("Errno: %s\n", strerror(errno));
      return 0;
  }
  
  printf("\n");
  printf("Scanning directory: %s ...\n",dir_name);
  printf("\n");
  int totalsize = scan_directory(dir_stream,dir_name,0);

  printf("\nTotal Diectory Size: %d Bytes\n",totalsize);


}
