#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ACC_DETAILS 20

FILE *file; // file for account details

int indexAccounts = 0; // if this reaches 20 (== MAX_ACC_DETAILS) then account limit is reached

// Structure to represent a bank account
struct accountDetails {
    char accountNumber[6];  // Holds the account number (up to 5 digits)
    char accountPin[5];     // Stores the account PIN (up to 4 digits)
    float accountBalance;   // Stores the account balance
} accounts[MAX_ACC_DETAILS];

// Function to update the accounts.txt file with the updated balance
void updateAccountsFile() {
    file = fopen("accounts.txt", "w");
    for(int i = 0; i < indexAccounts; i++) {
        fprintf(file, "%s %s %0.2f\n", accounts[i].accountNumber, accounts[i].accountPin, accounts[i].accountBalance);
    }
    fclose(file);
}


// Function to ask the user if they want to go back to the main menu
int repeatTransaction() {
    int answer;
    printf("\n");
    printf("\t\t+-----------------------------+\n");
    printf("\t\t|    Do you want to create    |\n");
    printf("\t\t|       another account?      |\n");
    printf("\t\t|                             |\n");
    printf("\t\t| [1] Yes                     |\n");
    printf("\t\t| [2] No                      |\n");
    printf("\t\t+-----------------------------+\n");
    printf("\n\tChoice: ");
    while (scanf("%d", &answer) != 1 || (answer != 1 && answer != 2)) {
        printf("\tInvalid input. Enter either 1 or 2.\n");
        while (getchar() != '\n');
        printf("\tChoice: ");
    }
    return answer;
}


// Function for the initial balance when making a new account
float createDeposit() {
    float deposit;

    printf("\n\tEnter initial deposit of at least P500 for the account: ");
    // takes a float integer for initial deposit input from the user to be checked 
    while(scanf("%f", &deposit) != 1) {
    	printf("\tInvalid input. Try again.\n");
    	
    	while(getchar() != '\n');
    	printf("\tEnter initial deposit for the account: ");
	}
    // checks if the initial deposit inputted by the user is a valid amount
	if(deposit > 1000000.00 || deposit < 500.00) {
		printf("\tInvalid amount. Initial deposit should at least be P500.\n");
		return createDeposit();
	}
    printf("\tInitial deposit acknowledged.\n");

    return deposit;
}


// Function for creating a new account which will reflect in the .txt file
void createAccount() {
	int createAgain;
	
	do{
	    if(indexAccounts >= MAX_ACC_DETAILS) {
	        printf("\n");
	        printf("\n\tMaximum number of accounts reached. Cannot add more accounts.\n\n");
	        printf("\n");
	        return;
		}
	
	    char accNo[6]; // the character array storage for a valid account number 
	    
	    printf("\n");
		printf("\t+---------------------------------------------------+\n");
		printf("\t|                 CREATE NEW ACCOUNT                |\n");
		printf("\t+---------------------------------------------------+\n");
	
	    printf("\n\tEnter a new account number: ");
	    if(strlen(scanf("%s", accNo) != 5))
	    {
	    	printf("Invalid input. Your pin number should consist of 4 digits only.\n")
	    	return createAccount();
		}
		
		for(int i = 0; i < 5; i++)
		{
			if(accNo[i] < '0' || accNo[i] > '9')
			{
				printf("Invalid input. Enter integer values only.\n");
				return createAccount();
			}
		}
	
	    int found = 0;
	    for(int i = 0; i < MAX_ACC_DETAILS; i++) {
	        if(strcmp(accounts[i].accountNumber, accNo) == 0) {
	            found = 1;
	            break;
	        }
	    }
	
	    if(found) {
	        printf("\tAccount Number already exists. Try again.\n");
	        return createAccount();
	    }
	    printf("\tAccount Number acknowledged.\n");
	
	    char pin[5]; // the character array storage for a valid pin
		
	    printf("\n\tEnter a new PIN for the account: ");
	    if(strlen(scanf("%s", pin) != 4))
	    {
	    	printf("Invalid input. Your pin number should consist of 4 digits only.\n")
	    	return createAccount();
		}
		
		for(int i = 0; i < 4; i++)
		{
			if(accNo[i] < '0' || accNo[i] > '9')
			{
				printf("Invalid input. Enter integer values only.\n");
				return createAccount();
			}
		}
	    printf("\tPIN acknowledged.\n");
	
	    float initialBalance = createDeposit();
	
	    for(int i = 0; i < MAX_ACC_DETAILS; i++) {
	        if(strlen(accounts[i].accountNumber) == 0) {
	            indexAccounts = i;
	            break;
	        }
	    }
	
	    strcpy(accounts[indexAccounts].accountNumber, accNo);
	    strcpy(accounts[indexAccounts].accountPin, pin);
	    accounts[indexAccounts].accountBalance = initialBalance;
	
	    indexAccounts++;
	    updateAccountsFile();
	        
	    printf("\n\tAccount created succesfully.\n\n");
	    
	    createAgain = repeatTransaction();
	    if(createAgain == 2) return;
	} while(createAgain == 1);
}


int main() {
    file = fopen("accounts.txt", "r");

    if (file == NULL) {
        printf("\t\t+-------------------------------------------------+\n");
        printf("\t\t|             Unable to open file.                |\n");
        printf("\t\t+-------------------------------------------------+\n");
        return 1;
    }

    printf("\t\t+-------------------------------------------------+\n");
    printf("\t\t|              File opened successfully.          |\n");
    printf("\t\t+-------------------------------------------------+\n");

    // Initialize the accounts array
    for (int i = 0; i < MAX_ACC_DETAILS; i++) {
        fscanf(file, "%s %s %f\n", accounts[i].accountNumber, accounts[i].accountPin, &accounts[i].accountBalance);
        
        if(strlen(accounts[i].accountNumber) != 0) {
            indexAccounts++;
        }
    }
    fclose(file);

	createAccount();

	printf("\n\tThank you for using the program.\n");

    return 0;
}
