#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ACC_DETAILS 20

FILE *file; // file for account details
FILE *file1; // file for transaction history

int indexAccounts = 0;

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


void updateHistoryFile(int index, const char* transactionType, float amount, const char* date) {
    file1 = fopen("history.txt", "a");

    if (strcmp(transactionType, "Deposited") == 0) {
        fprintf(file1, "\n| %-13s | %-8s | %s | %-13.2f |               | %-13.2f |",
                accounts[index].accountNumber, accounts[index].accountPin, date, amount, accounts[index].accountBalance);
            fprintf(file1, "\n+---------------+----------+------------+---------------+---------------+---------------+");
    } else if (strcmp(transactionType, "Withdrawn") == 0) {
        fprintf(file1, "\n| %-13s | %-8s | %s |               | %-13.2f | %-13.2f |",
                accounts[index].accountNumber, accounts[index].accountPin, date, amount, accounts[index].accountBalance);
            fprintf(file1, "\n+---------------+----------+------------+---------------+---------------+---------------+");
    }

    fclose(file1);
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


// Function to check the account number entered by the user
int accountNoChecker() {
    char accNo[6]; // the character array storage for a valid account number
    int found = 0;
    int index;

    printf("\tEnter account number: ");
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


// Function to ask the user if they want to go back to the main menu
int repeatTransaction() {
    int answer;
    printf("\n");
    printf("\t\t+-----------------------------+\n");
    printf("\t\t|  Do you want to repeat the  |\n");
    printf("\t\t|        transaction?         |\n");
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


// Function to display balance
void displayBalance(int index) {
	int choice;
	
	do {
	    printf("\n");
	    printf("\t\t+-------------------------------------------------+\n");
	    printf("\t\t|                BALANCE INQUIRY                  |\n");
	    printf("\t\t+-------------------------------------------------+\n");
	    printf("\n\tAccount Number: %s\n", accounts[index].accountNumber);
	    printf("\tAccount Balance: %.2f\n", accounts[index].accountBalance);
	    printf("\n");
	    
	    choice = repeatTransaction();
	    if(choice == 2) return;
	} while(choice == 1);  
}

// Function for Withdraw Money
void withdrawMoney(int index) {
	int choice;
	
	do {
	    printf("\n");
	    printf("\n\tInitial Balance: %.2f\n\n", accounts[index].accountBalance);
	    printf("\t\t+-------------------------------------------------+\n");
	    printf("\t\t|                  WITHDRAW MONEY                 |\n");
	    printf("\t\t+-------------------------------------------------+\n");
	    
	    float amount;
	    while (1) {
	        printf("\n\t Enter the amount you want to withdraw: ");
	        
	        while (scanf("%f", &amount) != 1) {
	            printf("\tInvalid input. Enter float or integer values only.\n");
	            while (getchar() != '\n');
	            printf("\t Enter the amount you want to withdraw: ");
	        }
	        
	        if (amount < 500) {
            printf("\n\t Minimum withdrawal amount is 500. Please enter a valid amount.\n\n");
        	} else if (amount > accounts[index].accountBalance) {
            printf("\n\t Invalid amount. The amount you want to withdraw exceeds your current balance.\n\n");
        	} else {
	            // If the withdrawal amount is valid, deduct the amount from the account balance
	            accounts[index].accountBalance -= amount;
	            printf("\n\t Amount successfully withdrawn from your account.\n\n");
	            
	            updateAccountsFile(); // calls the function for updating the balance on the account details file
	            
	        // Add transaction details to history.txt file with the current date
	        time_t now; // Declares a variable now of type time_t
	        //holds the components of the local date and time, such as year, month, day
	        struct tm* local; //Declares a pointer local to a structure of type struct tm.
	        char date[11];
	
			//Gets the current time in seconds and stores it in the now variable.
	        time(&now);
	        //Converts the time in seconds to a local time 
	        local = localtime(&now);
	        strftime(date, sizeof(date), "%m/%d/%Y", local);
	
	        updateHistoryFile(index, "Withdrawn", amount, date); // Pass the correct transaction type "Withdrawn"
	        /*index - retrieve the account details from the accounts array.
	         *"Withdrawn" - string literal representing the transaction type
	         * amount - amount of money that was withdrawn
	         * date - formatted current date obtained earlier in the code */
	        break; // Exit the loop as a valid amount was entered
	        }
	    }
	    
	    choice = repeatTransaction();
	    if(choice == 2) return;
	} while(choice == 1);
}


// Function for Deposit Money
void depositMoney(int index) {
	int choice;
	
	do {
	    printf("\n");
	    printf("\n\tInitial Balance: %.2f\n\n", accounts[index].accountBalance);
	    printf("\t\t+-------------------------------------------------+\n");
	    printf("\t\t|                  DEPOSIT MONEY                  |\n");
	    printf("\t\t+-------------------------------------------------+\n");
	    printf("\n\t Enter the amount you want to deposit: ");
	    float amount;
	    while (scanf("%f", &amount) != 1 || amount < 500 || amount > 1000000) {
	        printf("\tInvalid input. Enter an amount between 500 and 1 million.\n");
	        while (getchar() != '\n');
	        printf("\tEnter the amount you want to deposit: ");
	    }
	
	    /// Add the entered amount to the account balance
	    accounts[index].accountBalance += amount;
	    printf("\n\t Amount successfully deposited to your account.\n\n");
	
	    updateAccountsFile(); // calls the function for updating the balance on the account details file
	
	 	// Add transaction details to history.txt file with the current date
	        time_t now; // Declares a variable now of type time_t
	        //holds the components of the local date and time, such as year, month, day
	        struct tm* local; //Declares a pointer local to a structure of type struct tm.
	        char date[11];
	
			//Gets the current time in seconds and stores it in the now variable.
	        time(&now);
	        //Converts the time in seconds to a local time 
	        local = localtime(&now);
	        strftime(date, sizeof(date), "%m/%d/%Y", local);
	
	        updateHistoryFile(index, "Deposited", amount, date); // Pass the correct transaction type "Deposited"
	        /*index - retrieve the account details from the accounts array.
	         *"Deposited" - string literal representing the transaction type
	         * amount - amount of money that was withdrawn
	         * date - formatted current date obtained earlier in the code */
	         
	        choice = repeatTransaction();
	    	if(choice == 2) return;
	} while(choice == 1);
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

    int index = accountNoChecker(); // Ask for account number and get the corresponding index

    int choice;

    do {
        printf("\n");
        printf("\t+---------------------------------------------------+\n");
        printf("\t| Welcome to PUP-On-Line Banking Systems            |\n");
        printf("\t+---------------------------------------------------+\n");
        printf("\t|\t[1] Balance Inquiry                         |\n");
        printf("\t|\t[2] Deposit                                 |\n");
        printf("\t|\t[3] Withdraw                                |\n");
        printf("\t|\t[4] Exit                                    |\n");
        printf("\t+---------------------------------------------------+\n\n");
        printf("\tEnter your choice: ");
        while (scanf("%d", &choice) != 1 || choice > 4 || choice < 1) {
            printf("\tInvalid input. Enter numbers from 1-4 only.\n");
            while (getchar() != '\n');
            printf("\tEnter your choice: ");
        }

        switch (choice) {
            case 1: // Balance Inquiry
                displayBalance(index);
                break;

            case 2: // Deposit Money
                depositMoney(index);
                break;

            case 3:// Withdraw Money
                withdrawMoney(index);
                break;

            case 4: // Exit
        	{
        		int choice;
        		
            	printf("\tAre you sure you want to exit this account?\n\n\t[1] Yes\n\t[2] No\n\n");
            	printf("\tChoice: ");
            	while(scanf("%d", &choice) != 1 || choice < 1 || choice > 2) {
            		printf("\tInvalid input. Enter either 1 or 2.\n");
			        while (getchar() != '\n');
			        printf("\tChoice: ");
				}
				
				if(choice == 1) {
					printf("\n\tThank you for using the program.\n");
					return 0;
				}
        	}
        }
    } while (1);

    return 0;
}
