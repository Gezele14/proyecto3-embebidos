#ifndef _UTIL_H
#define _UTIL_H

char * getParam(char *string, const char *delimiter);
const char * searchParam(char * configPath, char * key);
void chkDir(const char* path);
char* concat(const char *s1, const char *s2);
int numImages(char * path);

#endif