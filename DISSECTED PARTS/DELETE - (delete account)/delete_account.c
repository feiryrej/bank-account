#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ACC_DETAILS 20

FILE *file; // file for account details

int indexAccounts = 0; // if this 0, then that means that there are no existing account in the file

// Structure to represent a bank account
struct accountDetails {
    char accountNumber[6];
    char accountPin[5];
    float accountBalance;
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
    printf("\t\t|    Do you want to delete    |\n");
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


// Function for deleting an existing account
void deleteAccount() {
    int deleteAgain;
    
    do {
        if(indexAccounts == 0) {
            printf("\n");
            printf("\n\tDatabase empty. No existing account found.\n\n");
            printf("\n");
            return;
        }
        else {
            int numberInp;
            char deleteAcc[6];
            int accInd;
            printf("\n");
            
		    printf("\t+---------------------------------------------------+\n");
		    printf("\t|                 DELETE NEW ACCOUNT                |\n");
		    printf("\t+---------------------------------------------------+\n");

            printf("\n\tEnter the account number of the account you want to delete: ");
            while(scanf("%d", &numberInp) != 1 || numberInp < 10000 || numberInp > 99999) {
                printf("\tInvalid input. Enter a valid account number.\n");

                while(getchar() != '\n');
                printf("\tEnter the account number of the account you want to delete: ");
            }
            itoa(numberInp, deleteAcc, 10);

            int found = 0;
            for(int i = 0; i < MAX_ACC_DETAILS; i++) {
                if(strcmp(accounts[i].accountNumber, deleteAcc) == 0) {
                    found = 1;
                    accInd = i;
                    pinChecker(i);
                    break;
                }
            }
            
            if(!found) {
                printf("\tAccount Number not found. Try again.\n");
                return deleteAccount();
            }

            printf("\n");
    		
    		int choice;
            printf("\n\tAre you sure you want to delete this account?\n\n\t[1] Yes\n\t[2] No\n\n");
            printf("\tEnter your choice: ");
			while (scanf("%d", &choice) != 1 || choice > 2 || choice < 1) {
	            printf("\tInvalid input. Enter numbers from 1-2 only.\n");
	            while (getchar() != '\n');
	            printf("\tEnter your choice: ");
       	 	}
       	 	
            if(choice == 2) return deleteAccount();
			
			// deletes the account using swapping
            for(int i = 0; i < indexAccounts; i++) {
                if(i >= accInd) {
                    accounts[i] = accounts[i + 1];
                }
            }

            indexAccounts--;
            updateAccountsFile();

            printf("\tAccount deleted successfully.\n\n");

            deleteAgain = repeatTransaction();
	    	if(deleteAgain == 2) return;
        }
    } while(deleteAgain == 1);
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

    deleteAccount();

    printf("\n\tThank you for using the program.\n");

    return 0;
}
