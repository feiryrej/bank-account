#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ACC_DETAILS 20

FILE *file; // file for account details
FILE *file1; // file for transaction history

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


void updateHistoryFile(int index, const char* transactionType, float amount, int month, int day, int year) {
    file1 = fopen("history.txt", "a");

    if (strcmp(transactionType, "Deposited") == 0) {
        fprintf(file1, "\n| %-13s | %-8s | %0.2d/%0.2d/%d | %-13.2f |               | %-13.2f |",
            accounts[index].accountNumber, accounts[index].accountPin, month, day, year, amount, accounts[index].accountBalance);
        fprintf(file1, "\n+---------------+----------+------------+---------------+---------------+---------------+");
    } else if (strcmp(transactionType, "Withdrawn") == 0) {
        fprintf(file1, "\n| %-13s | %-8s | %0.2d/%0.2d/%d |               | %-13.2f | %-13.2f |",
            accounts[index].accountNumber, accounts[index].accountPin, month, day, year, amount, accounts[index].accountBalance);
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
void withdrawMoney(int index, int month, int day, int year) {
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
	        
		        updateHistoryFile(index, "Withdrawn", amount, month, day, year); // Pass the correct transaction type "Withdrawn"
		        break; 
	        }
	    }
	    
	    choice = repeatTransaction();
	    if(choice == 2) return;
	} while(choice == 1);
}


// Function for Deposit Money
void depositMoney(int index, int month, int day, int year) {
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
	
	    updateHistoryFile(index, "Deposited", amount, month, day, year); // Pass the correct transaction type "Withdrawn"     
	    choice = repeatTransaction();
	    if(choice == 2) return;
	} while(choice == 1);
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
	        printf("\nMaximum number of accounts reached. Cannot add more accounts.\n\n");
	        printf("\n");
	        return;
		}
	
	    int newAccountNo; // takes an integer input from the user
	    char accNo[6]; // the character array storage for a valid account number 
	
	    printf("\n");
	    printf("\t\t+-------------------------------------------------+\n");
	    printf("\t\t|                CREATE ACCOUNT                   |\n");
	    printf("\t\t+-------------------------------------------------+\n");
	    printf("\n\tEnter a new account number: ");
	    // takes an integer account number input from the user to be checked 
	    while(scanf("%d", &newAccountNo) != 1) {
	        printf("\tInvalid input. Try again.\n");
	            
	        while(getchar() != '\n');
	        printf("\tEnter a new account number: ");
	    }
	    // checks if the account number inputted by the user is a 5-digit number
	    if(newAccountNo > 99999 || newAccountNo < 10000) {
	        printf("\tInvalid input. Your account number should consist of 5 digits only.\n");
	        return createAccount();
	    }
	    // converts the int input to string and stores 
	    itoa(newAccountNo, accNo, 10); // syntax: itoa(int value, char str, int base) 
	
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
	
	    int newPin;
	    char pin[5]; // the character array storage for a valid pin
		
	    printf("\n\tEnter a new PIN for the account: ");
	    // takes an integer account pin input from the user to be checked 
	    while(scanf("%d", &newPin) != 1 || newPin > 9999 || newPin < 1000) {
	        printf("\tInvalid input. Try again.\n");
	            
	        while(getchar() != '\n');
	        printf("\tEnter a new pin for the account: ");
	    }
	    itoa(newPin, pin, 10);
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
            printf("\t\t+-------------------------------------------------+\n");
	    	printf("\t\t|                DELETE ACCOUNT                   |\n");
	    	printf("\t\t+-------------------------------------------------+\n");
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
       	 	
            if(choice == 2) return;
			
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


// Function for getting the month
int getDateMonth() {
    int month;

    printf("\tMonth: ");
    while(scanf("%d", &month) != 1 || month < 1 || month > 12) {
        printf("\tInvalid input. Enter a valid number for month.\n");

        while(getchar() != '\n');
        printf("\tMonth: ");
    }

    return month;
}


// Function getting the valid day range for the corresponding month
int getDateDay(int month) {
    int day;

    printf("\tDay: ");
    if(month == 4 || month == 6 || month == 9 || month == 11) {
        while(scanf("%d", &day) != 1 || day < 1 || day > 30) {
            printf("\tInvalid input. Enter days from 1-30 only.\n");

            while(getchar() != '\n');
            printf("\tDay: ");
        }
    }
    else if(month == 2) {
        while(scanf("%d", &day) != 1 || day < 1 || day > 28) {
            printf("\tInvalid input. Enter days from 1-28 only.\n");

            while(getchar() != '\n');
            printf("\tDay: ");
        }
    }
    else {
        while(scanf("%d", &day) != 1 || day < 1 || day > 31) {
            printf("\tInvalid input. Enter days from 1-31 only.\n");

            while(getchar() != '\n');
            printf("\tDay: ");
        }
    }

    return day;
}


// Function for getting the year
int getDateYear() {
    int year;

    printf("\tYear: ");
    while(scanf("%d", &year) != 1 || year < 1000 || year > 9999) {
        printf("\tInvalid input. Enter a valid year.\n");

        while(getchar() != '\n');
        printf("Year: ");
    }

    return year;
}
    
    
// Main menu function
void mainMenu(int index) {
    int choice;
    int month, day, year;

    printf("\n\t+-------------------------------------------------------+\n");
    printf("\t| Enter the date [MM/DD/YYYY]:                          |\n");
    printf("\t+-------------------------------------------------------+\n");
    printf("\t| Example: 06/18/2023                                   |\n");
    printf("\t+-------------------------------------------------------+\n\n");
    month = getDateMonth();
    day = getDateDay(month);
    year = getDateYear();
    printf("\n");

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
                depositMoney(index, month, day, year);
                break;

            case 3:// Withdraw Money
                withdrawMoney(index, month, day, year);
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
				
				if(choice == 1) return;
				break;
        	}

            default:
                printf("\n\t Invalid choice. Please enter a valid option (1-4).\n\n");
                break;
        }
    } while (1);
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

    int answer;
    do {
	    printf("\n");
	    printf("\t+---------------------------------------------------+\n");
	    printf("\t| Welcome to PUP-On-Line Banking Systems            |\n");
	    printf("\t+---------------------------------------------------+\n");
	    printf("\t|\t[1] Open Existing Account                   |\n");
	    printf("\t|\t[2] Create New Account                      |\n");
	    printf("\t|\t[3] Delete Account                          |\n");
	    printf("\t|\t[4] Exit                                    |\n");
	    printf("\t+---------------------------------------------------+\n\n");
	    printf("\tEnter your choice: ");
	    while (scanf("%d", &answer) != 1 || answer > 4 || answer < 1) {
	        printf("\tInvalid input. Enter numbers from 1-3 only.\n");
	        while (getchar() != '\n');
	        printf("\tEnter your choice: ");
	    }
	
	    switch (answer) {
	        case 1: // Open Existing Account
	            mainMenu(accountNoChecker());
	            break;
	
	        case 2: // Create New Account
	            createAccount();
	            break;
	            
	        case 3: // Delete Account
	            deleteAccount();
	            break;
	            
	    	case 4: // exit
	    	{
	    		int choice; 
	    		
	    		printf("\tAre you sure you want to exit the program?\n\n\t[1] Yes\n\t[2] No\n\n");
	    		printf("\tChoice: ");
                while(scanf("%d", &choice) != 1 || choice < 1 || choice > 2) {
                	printf("\tInvalid input.\n");
                	
                	while(getchar() != '\n');
                	printf("\tChoice: ");
				}
                
                if(choice == 1) {
                    printf("\n\tThank you for using the program.\n");
                    return 0;
                }
	    		break;
	    	}
		}
	} while(1);
	
	printf("\tThank you for using the program.\n");
	
    return 0;
}
