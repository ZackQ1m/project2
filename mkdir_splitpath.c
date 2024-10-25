#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

void buildFullPath(struct NODE* node, char* fullPath) {
    // Base case: stop at the root node
    if (node == root) {
        strcpy(fullPath, "");  // Set empty for root; avoid leading '/'
        return;
    }

    // Recursive call to build the path from the parent first
    if (node->parentPtr != NULL && node->parentPtr != root) {
        buildFullPath(node->parentPtr, fullPath);
        strcat(fullPath, "/");  // Add '/' separator between directories
    }

    // Append the current directory's name
    strcat(fullPath, node->name);
}

void mkdir(char pathName[]) {
    char baseName[64];  // The new directory name
    char dirName[256];  // The path leading to the parent directory

    // Check if the pathName is valid
    if (strcmp(pathName, "/") == 0 || strcmp(pathName, "") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

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

    // Create the new directory node
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->parentPtr = parentDir;
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;

    // Add the new directory to the parent's child list
    if (parentDir->childPtr == NULL) {
        parentDir->childPtr = newDir;  // If no children, add as first child
    } else {
        // Traverse to the last sibling and append the new directory
        temp = parentDir->childPtr;
        while (temp->siblingPtr != NULL) {
            temp = temp->siblingPtr;
        }
        temp->siblingPtr = newDir;  // Add as the last sibling
    }

    // Determine if we should print full path or only baseName
    if (parentDir == cwd) {
        // Print base name only for directories created directly under cwd
        printf("MKDIR SUCCESS: node %s successfully created\n", baseName);
    } else {
        // Print full path for nested directories
        char fullPath[512] = "";
        buildFullPath(newDir, fullPath);
        printf("MKDIR SUCCESS: node %s successfully created\n", fullPath);
    }
}



struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    // Handle the root path case
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    // Find the last '/' in the path to separate dirName and baseName
    char* lastSlash = strrchr(pathName, '/');
    if (lastSlash != NULL) {
        // Copy everything before the last '/' into dirName
        strncpy(dirName, pathName, lastSlash - pathName);
        dirName[lastSlash - pathName] = '\0';  // Null-terminate dirName

        // Copy the last part after '/' into baseName
        strcpy(baseName, lastSlash + 1);
    } else {
        // If no '/', the entire path is the baseName (e.g., "file.txt")
        strcpy(dirName, "");
        strcpy(baseName, pathName);
    }

    // Determine if we're using an absolute or relative path
    struct NODE* currentDir = (dirName[0] == '/') ? root : cwd;
    char* token = strtok(dirName, "/");

    // Traverse directories in dirName
    while (token != NULL) {
        struct NODE* temp = currentDir->childPtr;
        int found = 0;

        // Search for the directory in current directory's children
        while (temp != NULL) {
            if (strcmp(temp->name, token) == 0 && temp->fileType == 'D') {
                currentDir = temp;  // Move to the found directory
                found = 1;
                break;
            }
            temp = temp->siblingPtr;
        }

        // If directory isn't found, print error and return NULL
        if (!found) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }

        token = strtok(NULL, "/");  // Move to the next directory in path
    }

    // Return pointer to the directory where baseName should be created
    return currentDir;
}
