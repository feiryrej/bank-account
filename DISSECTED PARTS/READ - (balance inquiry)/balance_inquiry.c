#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ACC_DETAILS 20

FILE *file; // file for account details

int indexAccounts = 0; // if this 0, then that means that there are no existing account in the file

// Structure to represent a bank account
struct accountDetails {
    char accountNumber[6];  // Holds the account number (up to 5 digits)
    char accountPin[5];     // Stores the account PIN (up to 4 digits)
    float accountBalance;   // Stores the account balance
} accounts[MAX_ACC_DETAILS];

// Function to check the pin entered by the user
void pinChecker(int index) {
	int numberInput; // takes an integer input from the user (to be used for checking because we can't check strings
    char pin[5]; // the character array storage for a valid account number

    printf("\tEnter PIN: ");
    // takes an integer account number input from the user to be checked
    while(scanf("%d", &numberInput) != 1) {
    	printf("\tInvalid input. Try again.\n");

    	while(getchar() != '\n');
    	printf("\tEnter PIN: ");
	}
	// checks if the account number inputted by the user is a 4-digit number
	if(numberInput > 9999 || numberInput < 1000) {
		printf("\tInvalid input. Your PIN should consist of 4 digits only.\n");
		return pinChecker(index);
	}
	// converts the int input to string and stores
	itoa(numberInput, pin, 10); // syntax: itoa(int value, char str, int base)

    if (strcmp(pin, accounts[index].accountPin) != 0) {  //converting the input PIN to a string, the function compares it with the PIN stored in the accounts
        printf("\tThe pin you have entered is incorrect. Try again.\n");
        return pinChecker(index);
    }
}


// Function to check the account number entered by the user
int accountNoChecker() {
    char accNo[6]; // the character array storage for a valid account number
    int found = 0;
    int index;

    printf("\n\tEnter account number: ");
    // takes a string account number input from the user to be checked
    scanf("%s", accNo);

    // checks if the account number inputted by the user is a 5-digit number
    if (strlen(accNo) != 5) {
        printf("\tInvalid input. Your account number should consist of 5 digits only.\n");
        return accountNoChecker();
    }
	
	//the function uses a for loop to iterate through the accounts array
    for (int i = 0; i < MAX_ACC_DETAILS; i++) { //For each account in the array, it compares the account number with the entered account number using the strcmp function
        if (strcmp(accounts[i].accountNumber, accNo) == 0) {
            found = 1;  
            index = i;
            pinChecker(i); //the function calls the pinChecker function with the found index (i) to check the associated PIN.
        }
    }

    if (!found) { //fter the loop, if the found flag is still 0, it means the entered account number does not match any account in the accounts array.
        printf("\tAccount Number not found. Try again.\n\n");
        return accountNoChecker();
    }

    return index; // function returns the index of the matching account to the caller.
}


// Function to display balance
void displayBalance(int index) {
	int choice;

    printf("\n");
    printf("\t\t+-------------------------------------------------+\n");
    printf("\t\t|                BALANCE INQUIRY                  |\n");
    printf("\t\t+-------------------------------------------------+\n");
    printf("\n\tAccount Number: %s\n", accounts[index].accountNumber);
    printf("\n\tAccount Pin: %s\n", accounts[index].accountPin);
    printf("\tAccount Balance: %.2f\n", accounts[index].accountBalance);

    printf("\n");
    printf("\t\t+-----------------------------+\n");
    printf("\t\t|   Do you want to view the   |\n");
    printf("\t\t| balance of another account? |\n");
    printf("\t\t|                             |\n");
    printf("\t\t| [1] Yes                     |\n");
    printf("\t\t| [2] No                      |\n");
    printf("\t\t+-----------------------------+\n");
    printf("\n\tChoice: ");
    while(scanf("%d", &choice) != 1 || choice < 1 || choice > 2) {
    	printf("\tInvalid input. Enter 1/2 keys only.\n");

    	while(getchar() != '\n');
    	printf("\tChoice: ");
	}

	if(choice == 1) {
		index = accountNoChecker();
		displayBalance(index);
	}
	else return;
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

    int index;

    index = accountNoChecker();
    displayBalance(index);

	printf("\n\tThank you for using the program.\n");

    return 0;
}
