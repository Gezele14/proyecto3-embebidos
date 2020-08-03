#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> 

/**
 * return substring without from delimiter to end
*/
char * getParam(char *string, const char *delimiter){
  char * token;

  token = strtok(string, "\n");  
  token = strtok(string, delimiter); //puerto: 
  token = strtok(NULL, delimiter);

  return token;
}
/**
 * return a searched param of config file
*/
const char * searchParam(char * configPath, char * key){
  FILE *conf;
  char buff[1000];
  char * token;

  conf = fopen(configPath, "r");
  while(fgets(buff, 1000, conf)) {
    token = strtok(buff, "\n");  
    token = strtok(buff, ":");
    if(strcmp(token,key)==0){
      token = strtok(NULL, ":");
      break;
    }else{
      token = "no";
    }
    
  }
  fclose(conf);
  //int len = strlen(token);
  //token[len-1]='\0';
  return token;
}

/**
 * Check directory if it not exists it will be created
*/
void chkDir(const char* path){
    mkdir(path, 0777);
}

/**
 * Concat String S1 and s2
*/
char* concat(const char *s1, const char *s2)
{
    char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int numImages(char * path){
  struct dirent *de;  // Pointer for directory entry 
  int count = 0;
  // opendir() returns a pointer of DIR type.  
  DIR *dr = opendir(path); 

  if (dr == NULL)  // opendir returns NULL if couldn't open directory 
  { 
      return 0; 
  } 

  // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
  // for readdir() 
  while ((de = readdir(dr)) != NULL) 
          count += 1; 

  closedir(dr);

  return count-1;
}