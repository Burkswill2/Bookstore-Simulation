// DO NOT REMOVE THIS COMMENT!! CSE 3430 lab2.c SP 24 CODE 082108
// STUDENT NAME: (REPLACE THIS WITH YOUR NAME)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for storing book data
struct Data {
    char title[60];
    char author[60];
    int stockNumber;
    float wholesalePrice;
    float retailPrice;
    int wholesaleQuantity;
    int retailQuantity;
};

// Node structure for the linked list
typedef struct Node {
    struct Data book;
    struct Node *next;
} Node;

// Function prototypes
void getDataAndBuildList(Node **listHeadPtr);
Node *createNodeAndGetData(void);
void insertNode(Node **listHeadPtr, Node *newNodePtr);
void deleteNode(Node **listHeadPtr, int stockNumToDelete);
void getUserOption(Node **listHead);
double calculateTotalRevenue(const Node *listHead);
double calculateInvestmentInInventory(const Node *listHead);
double calculateTotalWholesaleCost(const Node *listHead);
double calculateTotalProfit(const Node *listHead);
int calculateTotalBooksSold(const Node *listHead);
double calculateAverageProfit(const Node *listHead);
void printList(const Node *listHead);
void freeAllNodes(Node **listHeadPtr);

int main(void) {
    Node *listHead = NULL;
    getDataAndBuildList(&listHead);
    getUserOption(&listHead);
    return 0;
}

void getDataAndBuildList(Node **listHeadPtr) {
    Node *newNodePtr;
    printf("Please enter data about the books.\n\n");
    while ((newNodePtr = createNodeAndGetData()) != NULL) {
        insertNode(listHeadPtr, newNodePtr);
    }
}

Node *createNodeAndGetData(void) {
    Node *newNodePtr = malloc(sizeof(Node));
    if (newNodePtr == NULL) {
        printf("Error: memory could not be allocated for enough nodes. ");
        printf("Terminating program!\n");
        exit(0);
    } else {
        scanf("%[^\n]", newNodePtr->book.title);
        if (strcmp(newNodePtr->book.title, "END_DATA") == 0) {
            free(newNodePtr); // Free node if end of book data detected
            return NULL;
        } else {
            getchar(); // Consume newline before author string
            scanf("%[^\n]s", newNodePtr->book.author);
            scanf("%d", &newNodePtr->book.stockNumber);
            scanf("%f", &newNodePtr->book.wholesalePrice);
            scanf("%f", &newNodePtr->book.retailPrice);
            scanf("%d", &newNodePtr->book.wholesaleQuantity);
            scanf("%d", &newNodePtr->book.retailQuantity);
            getchar(); // Consume newline before next title string
        }
        return newNodePtr;
    }
}

void insertNode(Node **listHeadPtr, Node *newNodePtr) {
    // If list is empty or if the new node should be placed before the head
    if (*listHeadPtr == NULL || (*listHeadPtr)->book.stockNumber > newNodePtr->book.stockNumber) {
        newNodePtr->next = *listHeadPtr; // Point the new node to the current head
        *listHeadPtr = newNodePtr; // Update the head to be the new node
    } else {
        // Search for the correct position to insert the new node
        Node *current = *listHeadPtr;
        while (current->next != NULL && current->next->book.stockNumber < newNodePtr->book.stockNumber) {
            current = current->next; // Move to the next node
        }
        // Insert the new node
        newNodePtr->next = current->next;
        current->next = newNodePtr;
    }
}

void deleteNode(Node **listHeadPtr, int stockNumToDelete) {
    Node *temp = *listHeadPtr, *prev = NULL;

    // If head node itself holds the stock number to be deleted
    if (temp != NULL && temp->book.stockNumber == stockNumToDelete) {
        *listHeadPtr = temp->next; // Changed head
        free(temp); // free old head
        printf("Book stock number %d deleted from the inventory.\n", stockNumToDelete);
        return;
    }

    // Search for the stock number to be deleted, keep track of the previous node
    while (temp != NULL && temp->book.stockNumber != stockNumToDelete) {
        prev = temp;
        temp = temp->next;
    }

    // If stock number was not present in linked list
    if (temp == NULL) {
        printf("Book stock number %d not found in the inventory.\n", stockNumToDelete);
        return;
    }

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp); // Free memory
    printf("Book stock number %d deleted from the inventory.\n", stockNumToDelete);
}



void getUserOption(Node **listHeadPtr) {
    int option;
    Node *newNodePtr;
    int stockNumToDelete;
    do {
        printf("\nPlease enter an integer between 1 and 10 to select an operation on the data:\n");
        scanf("%d", &option);
        getchar(); // Consume newline
        switch (option) {
            case 1:
                printList(*listHeadPtr);
                break;
            case 2:
                printf("\nTotal revenue: %.2f\n", calculateTotalRevenue(*listHeadPtr));
                break;
            case 3:
                printf("\nTotal wholesale cost: %.2f\n", calculateTotalWholesaleCost(*listHeadPtr));
                break;
            case 4:
                printf("\nInvestment in inventory: %.2f\n", calculateInvestmentInInventory(*listHeadPtr));
                break;
            case 5:
                printf("\nTotal profit: %.2f\n", calculateTotalProfit(*listHeadPtr));
                break;
            case 6:
                printf("\nTotal number of books sold: %d\n", calculateTotalBooksSold(*listHeadPtr));
                break;
            case 7:
                printf("\nAverage profit per sale: %.2f\n", calculateAverageProfit(*listHeadPtr));
                break;
            case 8:
                {
                    Node *newNode = createNodeAndGetData();
                    if (newNode) {
                        insertNode(listHeadPtr, newNode);
                        printf("New book added.\n");
                    }
                }
                break;
            case 9:
                printf("Enter stock number of the book to delete: ");
                scanf("%d", &stockNumToDelete);
                deleteNode(listHeadPtr, stockNumToDelete);
                break;
            case 10:
                printf("Exiting program...\n");
                freeAllNodes(listHeadPtr);
                break;
            default:
                printf("Invalid option. Please enter a number between 1 and 10.\n");
        }
    } while (option != 10);
}

// Placeholder for functions calculateTotalRevenue, calculateInvestmentInInventory, etc.
double calculateTotalRevenue(const Node *listHead) {
    double totalRevenue = 0.0;
    while (listHead != NULL) {
        totalRevenue += listHead->book.retailPrice * listHead->book.retailQuantity;
        listHead = listHead->next;
    }
    return totalRevenue;
}

double calculateInvestmentInInventory(const Node *listHead) {
    double investment = 0.0;
    while (listHead != NULL) {
        investment += (listHead->book.wholesaleQuantity - listHead->book.retailQuantity) * listHead->book.wholesalePrice;
        listHead = listHead->next;
    }
    return investment;
}

double calculateTotalWholesaleCost(const Node *listHead) {
    double totalWholesaleCost = 0.0;
    while (listHead != NULL) {
        totalWholesaleCost += listHead->book.wholesalePrice * listHead->book.wholesaleQuantity;
        listHead = listHead->next;
    }
    return totalWholesaleCost;
}

double calculateTotalProfit(const Node *listHead) {
    double totalRevenue = calculateTotalRevenue(listHead);
    double totalWholesaleCost = calculateTotalWholesaleCost(listHead);
    double investmentInInventory = calculateInvestmentInInventory(listHead);
    return (totalRevenue - totalWholesaleCost) + investmentInInventory;
}

int calculateTotalBooksSold(const Node *listHead) {
    int totalBooksSold = 0;
    while (listHead != NULL) {
        totalBooksSold += listHead->book.retailQuantity;
        listHead = listHead->next;
    }
    return totalBooksSold;
}

double calculateAverageProfit(const Node *listHead) {
    double totalProfit = calculateTotalProfit(listHead);
    int totalSales = calculateTotalBooksSold(listHead);
    if (totalSales == 0) {
        return 0.0; // Avoid division by zero
    }
    return totalProfit / totalSales;
}


void printList(const Node *listHead) {
    const Node *traversePtr = listHead;
    printf("\nBook list:\n");
    while (traversePtr != NULL) {
        printf("%s\n", traversePtr->book.title);
        traversePtr = traversePtr->next;
    }
    printf("\n");
}

void freeAllNodes(Node **listHeadPtr) {
    Node *traversePtr = *listHeadPtr;
    Node *restOfListPtr = *listHeadPtr;
    while (restOfListPtr != NULL) {
        restOfListPtr = restOfListPtr->next;
        free(traversePtr);
        traversePtr = restOfListPtr;
    }
    *listHeadPtr = NULL; // Set list head pointer back to NULL after freeing space
}
