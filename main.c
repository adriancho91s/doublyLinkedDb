#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define UP 119
#define RIGHT 100
#define LEFT 97
#define DOWN 115
#define ESC 27
#define ENTER 10

struct node {
    struct node *prev;
    char name[30];
    char surname[30];
    char phone[15];
    char whatsApp[15];
    char email[32];
    char status[10];
    struct node *next;
};

typedef struct {
    int id;
    char name[30];
} FileNode;

struct node *findNodeAtPosition(struct node **first, int position);

void print(struct node **first, int order);
void clearScreen();
void delay();
int counterNodes(struct node **first);
void addNodeInOrder(struct node **first, char name[30], char surname[30], char phone[15], char whatsapp[15], char email[30]);
void deleteList(struct node **first);
int getKey();
void navigateDatabase(struct node **first);
int editContact(struct node **first, char name[30], char surname[30]);

int main() {
    struct node *first = NULL;

    char *options[] = {
            "Add a new contact to the database",
            "Print the database (interactive)",
            "Edit a contact in the database",
            "Print the database (choose the order)",
            "Print the database (ASC)",
            "Exit"
    };
    int numOptions = sizeof(options) / sizeof(options[0]);

    //Menu for the user
    int option;
    do {
        clearScreen();
        printf("\tWelcome to the linked list program\n\n");
        for (int i = 0; i < numOptions; i++) {
            printf("%d. %s\n", i + 1, options[i]);
        }
        fflush(stdin);
        printf("\nChoose an option: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                clearScreen();
                char name[30];
                char surname[30];
                char phone[15];
                char whatsapp[15];
                char email[30];
                printf("Enter the name: ");
                scanf("%s", name);
                printf("Enter the surname: ");
                scanf("%s", surname);
                printf("Enter the phone: ");
                scanf("%s", phone);
                printf("Enter the WhatsApp: ");
                scanf("%s", whatsapp);
                printf("Enter the email: ");
                scanf("%s", email);
                addNodeInOrder(&first, name, surname, phone, whatsapp, email);
                break;
            case 2:
                clearScreen();
                navigateDatabase(&first);
                break;
            case 3:
                clearScreen();
                if (first == NULL) {
                    printf("The linked list is empty\n");
                    delay();
                    break;
                }
                printf("You are going to edit a contact\n");
                char nameToFind[30];
                char surnameToFind[30];
                printf("Enter the name: ");
                scanf("%s", name);
                printf("Enter the surname: ");
                scanf("%s", surname);
                int isSuccess = editContact(&first, nameToFind, surnameToFind);
                if (isSuccess == 0) {
                    printf("The contact was not found\n");
                    delay();
                    break;
                }
                printf("The contact was edited successfully\n");
                printf("Press enter to continue\n");
                fflush(stdin);
                fgetc(stdin);
                break;
            case 4:
                clearScreen();
                if (first == NULL) {
                    printf("The linked list is empty\n");
                    delay();
                    break;
                }
                int order = 0;
                printf("Choose the order (0 ASC, 1 DESC): ");
                scanf("%d", &order);
                print(&first, order);
                printf("Press enter to continue\n");
                fgetc(stdin);
                fgetc(stdin);
                break;
            case 5:
                clearScreen();
                if (first == NULL) {
                    printf("The linked list is empty\n");
                    delay();
                    break;
                }
                print(&first, 0);
                printf("Press enter to continue\n");
                fgetc(stdin);
                fgetc(stdin);
                break;
            case 6:
                deleteList(&first);
                break;
            default:
                printf("Invalid option\n");
                break;
        }

    } while (option != numOptions);
    return 0;
}

void addNodeInOrder(struct node **first, char name[30], char surname[30], char phone[15], char whatsapp[15], char email[30]) {
    struct node *newNode = (struct node *) malloc(sizeof(struct node));
    strcpy(newNode->name, name);
    strcpy(newNode->surname, surname);
    strcpy(newNode->phone, phone);
    strcpy(newNode->whatsApp, whatsapp);
    strcpy(newNode->email, email);
    strcpy(newNode->status, "Active");
    newNode->next = NULL;
    newNode->prev = NULL;

    if (*first == NULL || strcmp(surname, (*first)->surname) < 0) {
        newNode->next = *first;
        if (*first != NULL) {
            (*first)->prev = newNode;
        }
        *first = newNode;
    } else {
        struct node *current = *first;
        while (current->next != NULL && strcmp(surname, current->next->surname) > 0) {
            current = current->next;
        }
        newNode->next = current->next;
        if (current->next != NULL) {
            current->next->prev = newNode;
        }
        current->next = newNode;
        newNode->prev = current;
    }
}

/*
 * Delete the linked list
 * @return void
 * */
void deleteList(struct node **first) {
    struct node *current = *first;
    struct node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *first = NULL;
    free(*first);
}

/*
 * Get the key pressed by the user
 * @return int The key pressed by the user
 * */
int getKey() {
    struct termios oldTerm, newTerm;
    int option;

    tcgetattr(STDIN_FILENO, &oldTerm);
    newTerm = oldTerm;
    newTerm.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);

    option = getchar(); //Get Key

    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);

    return option;
}


/*
 * Navigate the database controlling the address of the current node
 * @param first The pointer to the pointer of the first node in the linked list
 * @return void
 * */
void navigateDatabase(struct node **first) {
    if (*first == NULL) {
        printf("The linked list is empty\n");
        delay();
        return;
    }
    struct node *current = *first;
    int position = 1;
    int option = 0;
    printf("Use the arrow keys to navigate the database\n");
    delay();
    fflush(stdin);
    do {
        clearScreen();
        printf("╔════╦═══════════════╦═══════════════╦═══════════════╦═══════════════╦══════════════════════════════╦══════════╗\n");
        printf("║ %-3s║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", "Pos", "Name", "Surname", "Phone", "WhatsApp", "Email", "Status");
        printf("╠════╬═══════════════╬═══════════════╬═══════════════╬═══════════════╬══════════════════════════════╬══════════║\n");
        printf("║ %-3d║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", position, current->name, current->surname, current->phone, current->whatsApp, current->email, current->status);
        printf("╚════╩═══════════════╩═══════════════╩═══════════════╩═══════════════╩══════════════════════════════╩══════════╝\n");


        printf("\x1b[31m Press ESC to exit from navigate database mode \x1b[0m\n");
        printf("\x1b[31m Press ENTER to delete or restore the contact \x1b[0m\n");
        printf("\x1b[33m Press W or A to go to the previous contact \x1b[0m\n");
        printf("\x1b[34m Press S or D to go to the next contact \x1b[0m\n");
        option = getKey();
        switch (option) {
            case UP:
            case LEFT:
                if (current->prev != NULL) {
                    current = current->prev;
                    position--;
                } else {
                    clearScreen();
                    printf("This is the beginning of the database\n");
                    delay();
                }
                break;
            case DOWN:
            case RIGHT:
                if (current->next != NULL) {
                    current = current->next;
                    position++;
                } else {
                    clearScreen();
                    printf("This is the end of the database\n");
                    delay();
                }
                break;
            case ENTER:
                if (strcmp(current->status, "Active") == 0) {
                    printf("Do you want to delete the contact? (Press enter to confirm or any key to cancel)\n");
                    option = getKey();
                    if (option == ENTER) {
                        strcpy(current->status, "Deleted");
                    }
                } else {
                    printf("Do you want to restore the contact? (Press enter to confirm or any key to cancel)\n");
                    option = getKey();
                    if (option == ENTER) {
                        strcpy(current->status, "Active");
                    }
                }
                break;
            case ESC:
                break;
            default:
                clearScreen();
                printf("Invalid key\n");
                delay();
                break;
        }
    } while (option != ESC);
}


/*
 * Print the linked list
 * @param first The pointer to the pointer of the first node in the linked list
 * @param to print the doubly linked list ASC or DESC if 0 ASC, 1 DESC
 * @return void
 * */
void print(struct node **first, int order) {
    struct node *temp = *first;
    int position = 1;
    if (temp == NULL) {
        printf("The linked list is empty\n");
        return;
    }
    if (order == 0) {
        printf("Printing the linked list in ASC order\n");

        printf("╔════╦═══════════════╦═══════════════╦═══════════════╦═══════════════╦══════════════════════════════╦══════════╗\n");
        printf("║ %-3s║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", "Pos", "Name", "Surname", "Phone", "WhatsApp", "Email", "Status");
        printf("╠════╬═══════════════╬═══════════════╬═══════════════╬═══════════════╬══════════════════════════════╬══════════║\n");
        while (temp != NULL) {
            if (temp->next == NULL) {
                printf("║ %-3d║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", position, temp->name, temp->surname, temp->phone, temp->whatsApp, temp->email, temp->status);
                printf("╚════╩═══════════════╩═══════════════╩═══════════════╩═══════════════╩══════════════════════════════╩══════════╝\n");
            } else {
                printf("║ %-3d║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", position, temp->name, temp->surname, temp->phone, temp->whatsApp, temp->email, temp->status);
            }
            temp = temp->next;
            position++;
        }
    } else {
        temp = findNodeAtPosition(first, counterNodes(first));
        printf("Printing the linked list in DESC order\n");
        printf("╔════╦═══════════════╦═══════════════╦═══════════════╦═══════════════╦══════════════════════════════╦══════════╗\n");
        printf("║ %-3s║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", "Pos", "Name", "Surname", "Phone", "WhatsApp", "Email", "Status");
        printf("╠════╬═══════════════╬═══════════════╬═══════════════╬═══════════════╬══════════════════════════════╬══════════║\n");
        while (temp != NULL) {
            if (temp->prev == NULL) {
                printf("║ %-3d║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", position, temp->name, temp->surname, temp->phone, temp->whatsApp, temp->email, temp->status);
                printf("╚════╩═══════════════╩═══════════════╩═══════════════╩═══════════════╩══════════════════════════════╩══════════╝\n");
            } else {
                printf("║ %-3d║%-15s║%-15s║%-15s║%-15s║%-30s║%-10s║\n", position, temp->name, temp->surname, temp->phone, temp->whatsApp, temp->email, temp->status);
            }
            temp = temp->prev;
            position++;
        }
    }

}

/*
 * Edit a contact in the linked list finding it by name and surname
 * @param first The pointer to the pointer of the first node in the linked list
 * @param name The name of the contact to edit
 * @param surname The surname of the contact to edit
 * @return void
 * */
int editContact(struct node **first, char name[30], char surname[30]){
    struct node *temp = *first;
    int option;
    // search the contact using
    if (temp == NULL) {
        printf("The contact was not found\n");
        delay();
        return 0;
    }
    do {
        clearScreen();
        printf("Choose the field to edit\n");
        printf("1. Name\n");
        printf("2. Surname\n");
        printf("3. Phone\n");
        printf("4. WhatsApp\n");
        printf("5. Email\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                printf("Enter the new name: ");
                scanf("%s", temp->name);
                break;
            case 2:
                printf("Enter the new surname: ");
                scanf("%s", temp->surname);
                break;
            case 3:
                printf("Enter the new phone: ");
                scanf("%s", temp->phone);
                break;
            case 4:
                printf("Enter the new WhatsApp: ");
                scanf("%s", temp->whatsApp);
                break;
            case 5:
                printf("Enter the new email: ");
                scanf("%s", temp->email);
                break;
            case 6:
                return 1;
                break;
            default:
                printf("Invalid option\n");
                break;
        }
    } while (option != 6);
    return 1;
}

/*
 * Clear the screen
 * @return void
 * */
void clearScreen() {
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

/*
 * Delay the program
 * @return void
 * */
void delay() {
#ifdef WINDOWS
    system("timeout 1 > nul");
#else
    system("sleep 1");
#endif
}


/*
 * Find a node at a given position in the linked list
 * @param position The position of the node to find
 * @return struct node* The node at the given position
 * */
struct node *findNodeAtPosition(struct node **first, int position) {
    if (position < 0) {
        printf("Invalid position\n");
        return NULL;
    }
    struct node *temp = *first;
    int i = 1;
    while (temp != NULL && i < position) {
        temp = temp->next;
        i++;
    }
    return temp;
}

/*
 * Count the number of nodes in the doubly linked list
 * @param first The pointer to the pointer of the first node in the linked list
 * @return int The number of nodes in the doubly linked list
 * */
int counterNodes(struct node **first) {
    int contador = 0;
    struct node *temp = *first;
    while (temp != NULL) {
        contador++;
        temp = temp->next;
    }
    return contador;
}
