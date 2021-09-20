/***********************************************************************
name: Ronny Z Valtonen
	readable -- recursively count readable files.
description:
	See CS 360 Files and Directories lectures for details.
***********************************************************************/

/* Includes and definitions */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#ifndef MAX_BUF
#define MAX_BUF 1024
#endif

/**********************************************************************
Take as input some path. Recursively count the files that are readable.
If a file is not readable, do not count it. Return the negative value
of any error codes you may get. However, consider what errors you want
to respond to.
**********************************************************************/
int count = 0;


// Check if the current file is a file or directory.
int isDirectory (char *pathname) {
  struct stat area, *s = &area;
    return (stat (pathname, s) == 0) && S_ISDIR (s->st_mode);
}




int readable(char* inputPath) {
  // Set a buffer for the directory path.
  char cwd[MAX_BUF];
  // Set the errno to 0 for use to check for valid directories.
  errno = 0;

  // Create a struct for the directory to read recursively through.
  struct dirent *dp;

  // If a path was entered, we will use this path for the function.
  if (inputPath != NULL) {
    DIR * dir = opendir(inputPath);
    // If the directory exists
    if (dir) {
      // Recursively read through the directory.
      while ((dp = readdir(dir)) != NULL) {
          if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
              // Copy the inputPath
              strcpy(cwd, inputPath);
              // Cat a '/' for the next path name
              strcat(cwd, "/");
              // Cat the next path name
              strcat(cwd, dp->d_name);
              // DEGUB #1 print the path name.
              // printf("%s\n", dp->d_name);

              // If the file is a regular file, add to the counter.
              // printf("%s\n", cwd);
              if (isDirectory(cwd)) {
                // printf("Is a directrory\n");
              }

              else {
                if (access(cwd, R_OK) == 0) {
                  // printf("Is a file\n");
                  count++;
                  // printf("%d\n", count);
                }
              }
              // printf("%s\n", cwd);
              // Recurse back through the function with the new path.
              readable(cwd);
              // printf("%d\n", count);
          }
      }
    }
    // If the directory does not exist.
    else if (ENOENT == errno) {
      // printf("Directory does not exist\n");

      return errno;
    }

    // The directory IS the file but not readable..
    else {
      // printf("Directory is a unreadable file...\n");
      return 0;
    }
    // Close the directory.
    closedir(dir);

  }





  // If a path was not entered, we will use the current working
  // directory for the function.
  else {

    // Print the current working directory if nothing was passed.
    // Use the buffer to get the cwd.
    getcwd(cwd, MAX_BUF);
    DIR * dir = opendir(cwd);
    // The directory exists.
    if (dir) {
      // Print the path.
      // printf("%s\n", cwd);
      // Recursively read through the directory.
      while ((dp = readdir(dir)) != NULL) {
          if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
              // Make a new path from our base path.
              strcat(cwd, "/");
              strcat(cwd, dp->d_name);
              // Recurse back through the function.
              if (isDirectory(cwd)) {
                printf("Is a directrory\n");
              }

              else {
                if (access(cwd, R_OK) == 0) {
                  printf("Is a file\n");
                  count++;
                  printf("%d\n", count);
                }
              }
              printf("%s\n", cwd);
              // Recurse back through the function with the new path.
              readable(cwd);
          }
      }
    }
    // Directory does not exist.
    else if (ENOENT == errno) {
      printf("Directory does not exist\n");
      return errno;
    }
    // Close the directory.
    closedir(dir);
  }

  // Exit the function.
  return count;
}



int main(int argc, char **argv) {
    // Pass the path to 'int readable'
    printf("%d\n", readable(argv[1]));
  return 0;
}
