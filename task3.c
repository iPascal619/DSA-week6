#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100

typedef struct {
    int adjacencyMatrix[MAX_USERS][MAX_USERS];
    char names[MAX_USERS][50];
    int userCount;
} Graph;

void initGraph(Graph* g) {
    g->userCount = 0;
    memset(g->adjacencyMatrix, 0, sizeof(g->adjacencyMatrix));
}

int addUser(Graph* g, const char* name) {
    if (g->userCount < MAX_USERS) {
        strcpy(g->names[g->userCount], name);
        return g->userCount++;
    }
    return -1;
}

void addConnection(Graph* g, int user1, int user2) {
    g->adjacencyMatrix[user1][user2] = 1;
    g->adjacencyMatrix[user2][user1] = 1;
}

void DFS(Graph* g, int user, int visited[], int depth, int maxDepth, int recommendations[]) {
    if (depth > maxDepth) return;
    
    visited[user] = 1;
    
    for (int i = 0; i < g->userCount; i++) {
        if (g->adjacencyMatrix[user][i] && !visited[i]) {
            DFS(g, i, visited, depth + 1, maxDepth, recommendations);
        } else if (!g->adjacencyMatrix[user][i] && !visited[i] && depth > 1) {
            recommendations[i] = 1;
        }
    }
}

void recommendFriends(Graph* g, int user) {
    int visited[MAX_USERS] = {0};
    int recommendations[MAX_USERS] = {0};
    
    DFS(g, user, visited, 0, 2, recommendations);
    
    printf("Friend recommendations for %s:\n", g->names[user]);
    for (int i = 0; i < g->userCount; i++) {
        if (recommendations[i]) {
            printf("- %s\n", g->names[i]);
        }
    }
}

int findUser(Graph* g, const char* name) {
    for (int i = 0; i < g->userCount; i++) {
        if (strcmp(g->names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    Graph g;
    initGraph(&g);
    char name1[50], name2[50];
    int choice;

    while (1) {
        printf("\n1. Add user\n2. Add connection\n3. Get friend recommendations\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter user name: ");
                scanf("%s", name1);
                int index = addUser(&g, name1);
                if (index != -1) {
                    printf("User added with index: %d\n", index);
                } else {
                    printf("Maximum user limit reached.\n");
                }
                break;
            case 2:
                printf("Enter first user name: ");
                scanf("%s", name1);
                printf("Enter second user name: ");
                scanf("%s", name2);
                int user1 = findUser(&g, name1);
                int user2 = findUser(&g, name2);
                if (user1 != -1 && user2 != -1) {
                    addConnection(&g, user1, user2);
                    printf("Connection added.\n");
                } else {
                    printf("One or both users not found.\n");
                }
                break;
            case 3:
                printf("Enter user name for recommendations: ");
                scanf("%s", name1);
                int user = findUser(&g, name1);
                if (user != -1) {
                    recommendFriends(&g, user);
                } else {
                    printf("User not found.\n");
                }
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
