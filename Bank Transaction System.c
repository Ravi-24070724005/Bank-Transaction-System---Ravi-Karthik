#include <stdio.h>
#include <stdlib.h> 

struct Transaction {
    char type; 
    double amount;
    struct Transaction* next;
};

struct Account {
    double balance;
    struct Transaction* history_top; 
};

void push(struct Transaction** top, char type, double amount) {
    struct Transaction* newTransaction = (struct Transaction*)malloc(sizeof(struct Transaction));
    
    if (newTransaction == NULL) {
        printf("Error: Memory allocation failed. Transaction not recorded.\n");
        return;
    }

    newTransaction->type = type;
    newTransaction->amount = amount;
    newTransaction->next = *top;
    *top = newTransaction;
}

struct Account* createAccount(double initialBalance) {
    struct Account* acc = (struct Account*)malloc(sizeof(struct Account));
    if (acc == NULL) {
        printf("Fatal Error: Could not create account.\n");
        exit(1); 
    }
    acc->balance = initialBalance;
    acc->history_top = NULL; 
    return acc;
}

void deposit(struct Account* acc, double amount) {
    if (amount <= 0) {
        printf("Invalid Amount: Deposit must be greater than zero.\n");
        return;
    }

    acc->balance += amount;
    push(&(acc->history_top), 'D', amount);

    printf("Successfully deposited $%.2f.\n", amount);
    printf("New Balance: $%.2f\n", acc->balance);
}

void withdraw(struct Account* acc, double amount) {
    if (amount <= 0) {
        printf("Invalid Amount: Withdrawal must be greater than zero.\n");
        return;
    }
    if (amount > acc->balance) {
        printf("Insufficient Funds: Cannot withdraw $%.2f. Current balance is $%.2f.\n", 
               amount, acc->balance);
        return;
    }

    acc->balance -= amount;
    push(&(acc->history_top), 'W', amount);

    printf("Successfully withdrew $%.2f.\n", amount);
    printf("New Balance: $%.2f\n", acc->balance);
}

void viewLastTransaction(struct Account* acc) {
    if (acc->history_top == NULL) {
        printf("No transactions in history.\n");
        return;
    }

    struct Transaction* lastTx = acc->history_top;

    printf("Last Transaction: ");
    if (lastTx->type == 'D') {
        printf("Deposit");
    } else {
        printf("Withdrawal");
    }
    printf(" of $%.2f\n", lastTx->amount);
}

void undoLastTransaction(struct Account* acc) {
    if (acc->history_top == NULL) {
        printf("No transactions to undo.\n");
        return;
    }

    struct Transaction* txToUndo = acc->history_top;
    acc->history_top = txToUndo->next;

    if (txToUndo->type == 'D') {
        acc->balance -= txToUndo->amount;
        printf("UNDO: Reversed deposit of $%.2f.\n", txToUndo->amount);
    } else if (txToUndo->type == 'W') {
        acc->balance += txToUndo->amount;
        printf("UNDO: Reversed withdrawal of $%.2f.\n", txToUndo->amount);
    }

    printf("New Balance: $%.2f\n", acc->balance);
    free(txToUndo);
}

void cleanup(struct Account* acc) {
    struct Transaction* current = acc->history_top;
    struct Transaction* temp;

    printf("Cleaning up transaction history...\n");
    
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }

    free(acc);
    printf("Cleanup complete. Goodbye!\n");
}

void printMenu() {
    printf("\n--- Simple Banking System ---\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. View Balance\n");
    printf("4. View Last Transaction\n");
    printf("5. Undo Last Transaction\n");
    printf("6. Exit\n");
    printf("------------------------------\n");
    printf("Enter your choice: ");
}

int main() {
    struct Account* myAccount = createAccount(1000.00);
    int choice;
    double amount;

    printf("Welcome! Your initial balance is $%.2f\n", myAccount->balance);

    while (1) {
        printMenu();
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1: 
                printf("Enter deposit amount: ");
                scanf("%lf", &amount);
                deposit(myAccount, amount);
                break;
            case 2: 
                printf("Enter withdrawal amount: ");
                scanf("%lf", &amount);
                withdraw(myAccount, amount);
                break;
            case 3: 
                printf("Current Balance: $%.2f\n", myAccount->balance);
                break;
            case 4: 
                viewLastTransaction(myAccount);
                break;
            case 5: 
                undoLastTransaction(myAccount);
                break;
            case 6: 
                cleanup(myAccount);
                return 0;
            default:
                printf("Invalid choice. Please select from 1 to 6.\n");
        }
    }

    return 0; 
}