#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

void mkdir(char pathName[]) {
    char baseName[64];
    char dirName[256];

    // Check if path is empty or just root
    if (strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    struct NODE* parentDir = splitPath(pathName, baseName, dirName);
    if (parentDir == NULL) {
        return;  // Error already printed by splitPath
    }

    // Check for existing directory
    struct NODE* temp = parentDir->childPtr;
    while (temp != NULL) {
        if (strcmp(temp->name, baseName) == 0 && temp->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        temp = temp->siblingPtr;
    }

    // Create new directory node
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    if (!newDir) {
        printf("ERROR: memory allocation failed\n");
        return;
    }
    strncpy(newDir->name, baseName, 63);
    newDir->name[63] = '\0';
    newDir->fileType = 'D';
    newDir->parentPtr = parentDir;
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;

    // Add to parent's children
    if (parentDir->childPtr == NULL) {
        parentDir->childPtr = newDir;
    } else {
        temp = parentDir->childPtr;
        while (temp->siblingPtr != NULL) {
            temp = temp->siblingPtr;
        }
        temp->siblingPtr = newDir;
    }

    // Use the original pathName for success message
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}

struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    // Handle root directory
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    // Make a copy of pathName to avoid modifying the original
    char pathCopy[256];
    strncpy(pathCopy, pathName, 255);
    pathCopy[255] = '\0';

    // Find the last '/' to separate dirName and baseName
    char* lastSlash = strrchr(pathCopy, '/');
    if (lastSlash != NULL) {
        *lastSlash = '\0';  // Split the string
        strcpy(baseName, lastSlash + 1);
        
        if (lastSlash == pathCopy) {
            // Path starts with '/', but has no other directories
            strcpy(dirName, "/");
        } else if (pathCopy[0] == '/') {
            strcpy(dirName, pathCopy);
        } else {
            strcpy(dirName, pathCopy);
        }
    } else {
        // No '/' in path - it's just a name in current directory
        strcpy(dirName, "");
        strcpy(baseName, pathCopy);
    }

    // Start from root for absolute paths, cwd for relative paths
    struct NODE* currentDir = (pathName[0] == '/') ? root : cwd;
    
    // If only working with root directory
    if (strcmp(dirName, "/") == 0) {
        return root;
    }

    // If dirName is empty, return current directory
    if (strlen(dirName) == 0) {
        return currentDir;
    }

    // Make a copy for tokenization
    char dirCopy[256];
    strcpy(dirCopy, dirName);
    
    // Skip leading '/' for tokenization
    char* start = dirCopy;
    if (dirCopy[0] == '/') {
        start++;
    }

    // Tokenize and traverse
    char* token = strtok(start, "/");
    while (token != NULL) {
        struct NODE* temp = currentDir->childPtr;
        int found = 0;

        while (temp != NULL) {
            if (strcmp(temp->name, token) == 0 && temp->fileType == 'D') {
                currentDir = temp;
                found = 1;
                break;
            }
            temp = temp->siblingPtr;
        }

        if (!found) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }

        token = strtok(NULL, "/");
    }

    return currentDir;
}