// DO NOT REMOVE THIS COMMENT!! CSE 3430 lab2.c SP 24 CODE 082108
// STUDENT NAME: William Burks
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Data {
    char title[60];
    char author[60];
    int stockNumber;
    float wholesalePrice;
    float retailPrice;
    int wholesaleQuantity;
    int retailQuantity;
};
typedef struct Node {
    struct Data book;
    struct Node *next;
} Node;
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
    Node *newNodePtr;
    newNodePtr = malloc (sizeof(Node));
    if (newNodePtr == NULL) {
        printf("Error: memory could not be allocated for enough nodes. ");
        printf("Terminating program!\n");
        exit (0);
    }
    else {
        scanf("%[^\n]", newNodePtr->book.title);
        if (strcmp(newNodePtr->book.title, "END_DATA") == 0) {
/* free Node if end of book data detected */
            free(newNodePtr);
            return NULL;
        }
        else {
/* consume newline before author string */
            getchar();
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

/**
 * Insert a new node into a linked list in ascending order of stockNumber
 *
 * This function inserts a new node into a linked list based on the stockNumber
 * value of the node's book data. The new node is inserted at the appropriate
 * position to maintain the ascending order of stockNumber values in the list.
 *
 * @param listHeadPtr A pointer to the head pointer of the linked list
 * @param newNodePtr A pointer to the new node to be inserted
 */
void insertNode(Node **listHeadPtr, Node *newNodePtr) {
    Node *traversePtr = *listHeadPtr;
    // Empty list case: insert first Node
    if (traversePtr == NULL) {
        *listHeadPtr = newNodePtr;
        newNodePtr->next = NULL;
    }
        // Non-empty list: insert new 1st Node before current 1st Node
    else if (newNodePtr->book.stockNumber < traversePtr->book.stockNumber) {
        *listHeadPtr = newNodePtr;
        newNodePtr->next = traversePtr;
    }
        // Non-empty list case: insert new Node after current 1st Node
    else {
        Node *priorNodePtr = traversePtr;
        traversePtr = traversePtr->next;
        while (traversePtr != NULL && traversePtr->book.stockNumber < newNodePtr->book.stockNumber) {
            // Move each pointer to next Node
            priorNodePtr = traversePtr;
            traversePtr = traversePtr->next;
        }
        // Change pointers to add newNodePtr Node to list
        priorNodePtr->next = newNodePtr;
        newNodePtr->next = traversePtr;
    }
}

/**
 * Deletes a Node from a linked list based on the stock number.
 *
 * This function deletes a Node from a linked list, identified by the stock number provided.
 * If the list is empty, it displays an error message. If the Node is found and deleted,
 * it displays a success message. If the Node is not found, it displays an error message.
 *
 * @param listHeadPtr - A pointer to the head of the linked list.
 * @param stockNumToDelete - The stock number of the Node to be deleted.
 *
 * @return void
 */
void deleteNode(Node **listHeadPtr, int stockNumToDelete) {
    Node *traversePtr = *listHeadPtr;
    // Empty list case: ERROR
    if (traversePtr == NULL) {
        printf("ERROR: The list is empty; there are no Nodes to delete!\n");
    }
        // Non-empty list: delete 1st Node
    else if (traversePtr->book.stockNumber == stockNumToDelete) {
        *listHeadPtr = traversePtr->next;
        free(traversePtr);
        printf("Book stock number %d deleted!\n", stockNumToDelete);
    }
        // Non-empty list case: delete Node after current 1st Node
    else {
        Node *priorNodePtr = traversePtr;
        traversePtr = traversePtr->next;
        while (traversePtr != NULL && traversePtr->book.stockNumber < stockNumToDelete) {
            // Move each pointer to next Node
            priorNodePtr = traversePtr;
            traversePtr = traversePtr->next;
        }
        // Check for ERROR; change pointers to delete Node
        if (traversePtr == NULL || traversePtr->book.stockNumber != stockNumToDelete) {
            printf("ERROR: Book stock number %d not found in list!\n", stockNumToDelete);
        }
        else {
            priorNodePtr->next = traversePtr->next;
            free(traversePtr);
            printf("Book stock number %d deleted!\n", stockNumToDelete);
        }
    }
}

/**
 * Allows the user to select operations on a linked list of books.
 *
 * This function prompts the user to enter an integer between 1 and 10 to select an operation on the data.
 * The user can perform operations such as printing the book list, calculating total revenue, total wholesale cost,
 * total investment in inventory, total profit, total number of books sold, average profit, adding a book,
 * deleting a book, and freeing all nodes. The function continues prompting the user until the user selects the option to exit.
 *
 * @param listHeadPtr A pointer to the pointer of the head node of the linked list.
 */
void getUserOption(Node **listHeadPtr) {
    int option;
    Node *newNodePtr;
    int stockNumToDelete;
    do {
        printf("\nPlease enter an integer between 1 and 10 to select an operation on the data: ");
        scanf("%i", &option);
        getchar();
        switch (option){
            case 1:
                printList (*listHeadPtr);
                break;
            case 2:
                printf("\nTotal revenue: %.2f\n", calculateTotalRevenue(*listHeadPtr));
                break;
            case 3:
                printf("\nTotal wholesale cost: %.2f\n", calculateTotalWholesaleCost(*listHeadPtr));
                break;
            case 4:
                printf("\nTotal investment in inventory: %.2f\n",
                       calculateInvestmentInInventory(*listHeadPtr));
                break;
            case 5:
                printf("\nTotal profit: %.2f\n", calculateTotalProfit(*listHeadPtr));
                break;
            case 6:
                printf("\nTotal number of books sold = %d\n", calculateTotalBooksSold(*listHeadPtr));
                break;
            case 7:
                printf("\nAverage profit: %.2f\n", calculateAverageProfit(*listHeadPtr));
                break;
            case 8:
                printf("\nPlease enter the data for the book you wish to add:\n\n");
                newNodePtr = createNodeAndGetData();
                insertNode(listHeadPtr, newNodePtr);
                break;
            case 9:
                printf("\nEnter the stock number of the book you wish to delete: ");
                scanf("%d", &stockNumToDelete);
                deleteNode(listHeadPtr, stockNumToDelete);
                break;
            case 10:
                freeAllNodes(listHeadPtr);
                break;
            default:
                printf("Valid option choices are 1 to 10. Please choose again!\n");
                break;
        }
    } while (option != 10);
}

/**
 * Calculates the total revenue by multiplying the retail price of each book
 *        with its retail quantity and summing up the result.
 *
 * @param listHead A pointer to the head of the linked list of books.
 * @return The total revenue calculated.
 */
double calculateTotalRevenue(const Node *listHead) {
    double totalRevenue = 0.0;
    while (listHead != NULL) {
        totalRevenue += listHead->book.retailPrice * listHead->book.retailQuantity;
        listHead = listHead->next;
    }
    return totalRevenue;
}
/**
 * Calculates the investment in inventory based on the given list of books.
 *
 * This function traverses the linked list of books and calculates the investment
 * by subtracting the retail quantity from the wholesale quantity and multiplying
 * it by the wholesale price of each book. The investment is the sum of these
 * calculations. The function assumes that the list of books is properly initialized
 * and terminated, i.e., the last node's next pointer is NULL.
 *
 * @param listHead Pointer to the head node of the linked list of books.
 *
 * @return The total investment in inventory.
 */
double calculateInvestmentInInventory(const Node *listHead) {
    double investment = 0.0;
    while (listHead != NULL) {
        investment += (listHead->book.wholesaleQuantity - listHead->book.retailQuantity) * listHead->book.wholesalePrice;
        listHead = listHead->next;
    }
    return investment;
}
/**
 * @brief Calculates the total wholesale cost of a linked list of books.
 *
 * This function iterates through the given linked list and calculates the total wholesale cost by
 * multiplying the wholesale price of each book by its wholesale quantity and adding it to the total.
 *
 * @param listHead A pointer to the head node of the linked list.
 * @return The total wholesale cost as a double.
 */
double calculateTotalWholesaleCost(const Node *listHead) {
    double totalWholesaleCost = 0.0;
    while (listHead != NULL) {
        totalWholesaleCost += listHead->book.wholesalePrice * listHead->book.wholesaleQuantity;
        listHead = listHead->next;
    }
    return totalWholesaleCost;
}
/**
 * Calculates the total profit for a given list of books.
 *
 * @param listHead A pointer to the head of the linked list of books
 * @return The total profit
 */
double calculateTotalProfit(const Node *listHead) {
    double totalRevenue = calculateTotalRevenue(listHead);
    double totalWholesaleCost = calculateTotalWholesaleCost(listHead);
    double investmentInInventory = calculateInvestmentInInventory(listHead);
    return (totalRevenue - totalWholesaleCost) + investmentInInventory;
}
/**
 * Calculates the total number of books sold.
 *
 * This function iterates through a linked list of Node structures and
 * sums up the retail quantities of all books in the list.
 *
 * @param listHead The head of the linked list.
 * @return The total number of books sold.
 */
int calculateTotalBooksSold(const Node *listHead) {
    int totalBooksSold = 0;
    while (listHead != NULL) {
        totalBooksSold += listHead->book.retailQuantity;
        listHead = listHead->next;
    }
    return totalBooksSold;
}
/**
 * Calculates the average profit per book sold based on the given list of books.
 *
 * @param listHead The head of the linked list of books.
 * @return The average profit per book sold.
 *
 * This function calculates the average profit per book sold by dividing the total profit
 * by the total number of books sold. If the total number of books sold is zero, it returns 0.0
 * to avoid division by zero.
 */
double calculateAverageProfit(const Node *listHead) {
    double totalProfit = calculateTotalProfit(listHead);
    int totalSales = calculateTotalBooksSold(listHead);
    if (totalSales == 0) {
        return 0.0; // Avoid division by zero
    }
    return totalProfit / totalSales;
}
/**
 * Prints the titles of the books in a given linked list.
 *
 * This function receives a pointer to the head node of a linked list
 * and prints the titles of all the books in the list.
 *
 * @param listHead   A pointer to the head node of the linked list.
 */
void printList(const Node *listHead) {
    const Node *traversePtr = listHead;
    printf("\nBook list:\n");
    while (traversePtr != NULL) { /* determine not at end of list */
        printf("%s\n", traversePtr->book.title);
        traversePtr = traversePtr->next;
    }
    printf("\n");
}
/**
 * Frees all the nodes in a linked list.
 *
 * This function frees the memory of all the nodes in a linked list and sets the list head pointer to NULL.
 *
 * @param[in,out] listHeadPtr A pointer to the head pointer of the linked list.
 */
void freeAllNodes(Node **listHeadPtr) {
    Node *traversePtr = *listHeadPtr;
    Node *restOfListPtr = *listHeadPtr;
    while (restOfListPtr != NULL) { /* determine list is not empty */
        restOfListPtr = restOfListPtr->next;
        free(traversePtr);
        traversePtr = restOfListPtr;
    }
    *listHeadPtr = NULL; /* set listHeadPtr back to NULL after space freed */
}
