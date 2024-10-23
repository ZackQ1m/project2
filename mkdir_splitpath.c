#include <stdio.h>
#include <string.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW

char baseName[64]; // The new directory name
    char dirName[256]; // The path leading to the parent directory

    // Call splitPath to separate the path into dirName and baseName
    struct NODE* parentDir = splitPath(pathName, baseName, dirName);

    if (parentDir == NULL) {
        // splitPath has already printed an error, return early
        return;
    }

    // Check if a directory with the same name already exists in the parent directory
    struct NODE* temp = parentDir->childPtr;
    while (temp != NULL) {
        if (strcmp(temp->name, baseName) == 0 && temp->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
        temp = temp->siblingPtr;
    }


	if (pathName == NULL || strcmp(pathName, "") == 0 || strcmp(pathName, "/") == 0) {
            printf("MKDIR ERROR: no path provided\n");
            return;
}

    // Create the new directory node
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->parentPtr = parentDir;
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;

    // Add the new directory to the parent's child list
    if (parentDir->childPtr == NULL) {
        parentDir->childPtr = newDir; // If no children, add as first child
    } else {
        // Find the last sibling and append the new directory
        temp = parentDir->childPtr;
        while (temp->siblingPtr != NULL) {
            temp = temp->siblingPtr;
        }
        temp->siblingPtr = newDir; // Add as the last sibling
    }
    if (strcmp(dirName, "/") == 0 || strcmp(dirName, "") == 0) {
    printf("MKDIR SUCCESS: node %s successfully created\n", baseName);
} else {
    printf("MKDIR SUCCESS: node %s/%s successfully created\n", dirName, baseName);
}
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

 if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;  // Return the root node since it's the root directory
    }

    // Find the last '/' in the path to split into dirName and baseName
    char* lastSlash = strrchr(pathName, '/');
    if (lastSlash != NULL) {
        // Copy the directory part to dirName
        strncpy(dirName, pathName, lastSlash - pathName);
        dirName[lastSlash - pathName] = '\0';  // Null-terminate dirName
        
        // Copy the base name (the last part of the path) to baseName
        strcpy(baseName, lastSlash + 1);
    } else {
        // If there's no '/', the path is just the baseName (e.g., "file.txt")
        strcpy(dirName, "");
        strcpy(baseName, pathName);
    }

    // Start from the current working directory or root and traverse the path
    struct NODE* currentDir = (dirName[0] == '/') ? root : cwd;  // Absolute or relative path
    char* token = strtok(dirName, "/");

    while (token != NULL) {
        struct NODE* temp = currentDir->childPtr;
        int found = 0;

        // Look for the directory in the current directory's children
        while (temp != NULL) {
            if (strcmp(temp->name, token) == 0 && temp->fileType == 'D') {
                currentDir = temp;  // Move to the found directory
                found = 1;
                break;
            }
            temp = temp->siblingPtr;
        }

        // If the directory doesn't exist, print an error and return NULL
        if (!found) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }

        token = strtok(NULL, "/");  // Get the next directory in the path
    }

    // Return the directory where the baseName should be located
    return currentDir;
}
