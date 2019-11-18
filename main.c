#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>

void print_files(char names[256][100], int sizes[100], int num_files, int dir_indexes[100], int num_dir){
  printf("Directories:\n");
  for (size_t i = 0; i < num_dir; i++) {
    printf("\t\t%s  size: %d Bytes\n",names[dir_indexes[i]],sizes[i]);
    strcpy(names[dir_indexes[i]],"");
  }
  printf("Regular files:\n");
  for(size_t i = 0; i < num_files; i++){
    if(strcmp(names[i],"") == 0){
      printf("\t\t%s  size: %d Bytes\n",names[i], sizes[i]);
    }
  }
}

int main(){
  char names[256][100];
  int sizes[100], dir_indexes[100];
  int totalsize, num_files;
  int dir_index = 0;
  DIR * directory = opendir("./");
  DIR * file = readdir(directory);
  struct stat file_info;

  for(size_t i = 0; file != NULL; i++){
    names[i] = file->d_name[256];
    stat("./"+names[i],&file_info);
    sizes[i] = file_info.st_size;
    if (S_ISDIR(file_info.st_mode)){
      dir_indexes[dir_index] = i;
      dir_index++;
    } else{
      totalsize+=sizes[i];
    }
    num_files++;
  }
  printf("Statistics for directory: .\n");
  printf("Total Diectory Size: %d Bytes\n",totalsize);
  print_files(names,sizes,num_files+1,dir_indexes,dir_index+1);
}
