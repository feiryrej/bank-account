#include <stdio.h>  // for standard input output functions printf, scanf, fprintf, fscanf, fopen, fclose
#include <string.h> // for functions responsible for string handling e.g. strcmp & strlen
#include <stdlib.h> // for itoa (integer to ascii)
#include <time.h>   // for time function

/* ito yung macro definition na magd-define ng constant 20 for MAX_ACC_DETAILS
 * sya yung magd-determine kung hanggang ilan lang yung kayang i-hold na
 * account details ng both program at ng .txt file */
#define MAX_ACC_DETAILS 50 

FILE *file; // file pointer for accounts.txt file
FILE *file1; // file pointer for history.txt file

/* very important GLOBAL variable, why?
 * ito yung magm-monitor kung ilan na yung accounts
 * stored inside the program and inside the .txt file
 * this will increment every time magc-create ng account (line 419)
 * at magd-decrement naman sya every time magd-delete ng account (line 511) */
int indexAccounts = 0; 

// Structure to represent a bank account
struct accountDetails {
    char accountNumber[6];  // Holds the account number (up to 5 digits)
    char accountPin[5];     // Stores the account PIN (up to 4 digits)
    float accountBalance;   // Stores the account balance
} accounts[MAX_ACC_DETAILS];

/* ito yung function na responsible for updating the accounts.txt file
 * ginagamit/cin-call natin sya mostly kapag may binago si user
 * e.g. nag-delete ng account (line 512), nag-create (line 420), etc
 * ayun yung instances na nagagamit natin sya, tapos ic-call natin sya
 * para mag-reflect yung changes na yun na ginawa ni user sa accounts.txt file
 * MOST IMPORTANTLY this is a case where FILE HANDLING happens */
void updateAccountsFile() {
    file = fopen("accounts.txt", "w"); // here, we open the accounts.txt file in write mode ("w") para sure na mao-overwrite yung file with updated details
    /* this for loop will iterate until ma-reach nya yung indexAccounts
     * (see lines 14 to 18 to recall the use of indexAccounts variable) */
    for(int i = 0; i < indexAccounts; i++) {
    	// ip-print nya isa isa yung bawat account details (updated) doon sa accounts.txt file using *fprintff*
        fprintf(file, "%s %s %0.2f\n", accounts[i].accountNumber, accounts[i].accountPin, accounts[i].accountBalance); 
    }
    fclose(file); 
}


/* ito yung function responsible for updating the history.txt file
 * iu-update nya naman yung file na nagc-contain ng transaction history
 * ip-print nya sa history.txt file yung transaction history in table format
 * IMPORTANT: another case where FILE HANDLING happens */
void updateHistoryFile(int index, const char* transactionType, float amount, const char* date) {
    file1 = fopen("history.txt", "a"); // opens the "history.txt" file in append mode ("a")
	
    if (strcmp(transactionType, "Deposited") == 0) {  // If the transactionType is "Deposited," the code inside this if block will execute.
        fprintf(file1, "\n| %-13s | %-8s | %s | %-13.2f |               | %-13.2f |",
                accounts[index].accountNumber, accounts[index].accountPin, date, amount, accounts[index].accountBalance);
            fprintf(file1, "\n+---------------+----------+------------+---------------+---------------+---------------+");
    } else if (strcmp(transactionType, "Withdrawn") == 0) { // If the transactionType is "Withdrawn," the code inside this else if block will execute.
        fprintf(file1, "\n| %-13s | %-8s | %s |               | %-13.2f | %-13.2f |",
                accounts[index].accountNumber, accounts[index].accountPin, date, amount, accounts[index].accountBalance);
            fprintf(file1, "\n+---------------+----------+------------+---------------+---------------+---------------+");
    }

    fclose(file1);
}


// Function to check the pin entered by the user
void pinChecker(int index) {
	/* dito kukuha muna tayo ng integer input from the user
	 * na gagamitin natin to check if that input is valid or not
	 * dahil kapag string input kaagad ang kinuha natin
	 * mahihirapan tayong i-check kung valid input sya or not */
	int numberInput;
	/* ito naman yung character array (string) na magr-receive at magc-contain kay integer input
	 * dahil ic-convert natin si integer input into string */
    char pin[5]; 

    printf("\tEnter PIN: ");
    // takes an integer PIN number input from the user to be checked using the while loop
    while(scanf("%d", &numberInput) != 1) {
    	printf("\tInvalid input. Try again.\n");

    	while(getchar() != '\n');
    	printf("\tEnter PIN: "); 
	}
	// checks if the PIN inputted by the user is a 4-digit number
	if(numberInput > 9999 || numberInput < 1000) {
		printf("\tInvalid input. Your PIN should consist of 4 digits only.\n");
		return pinChecker(index); // kapag hindi 4-digit number, papaulit nya lang si user mag-enter ng valid PIN
	}
	/* kapag nag-enter si user ng valid PIN number
	 * ic-convert na natin ngayon yung integer PIN number na yun
	 * into a string PIN number using the *itoa* function
	 * si *itoa* ay declared under the standard library 
	 * syntax: itoa(int value, char str, int base) */
	itoa(numberInput, pin, 10); 
	
	/* after nating i-convert si integer PIN into a string PIN
	 * ic-compare na natin sya ngayon doon sa pin nung corresponding account number which is also a string
	 * kaya tayo nag-string compare (str cmp) to see if the PIN for an account number is correct */
    if (strcmp(pin, accounts[index].accountPin) != 0) { 
        printf("\tThe pin you have entered is incorrect. Try again.\n");
        /* != 0 ang condition meaning kapag hindi same ang PIN na ininput ni user, 
		 * magr-recursion tayo using this function call
		 * kung saan tatawagin ulit natin si pinChecker para ipaulit kay user ang process
		 * until mag-enter sya ng tamang PIN */
        return pinChecker(index); // passes the index para sure na yung chin-check nating pin ay same index with the account number
    }
}


// Function to check the account number entered by the user
int accountNoChecker() {
    char accNo[6]; // the character array storage for a valid account number
    int found = 0; 
    /* gagamitin natin tong index variable to determine
     * kung ano yung index ng account number na pinili ni user
     * and this will be useful for the entire program
     * kasi ipapasa natin si index sa ibat ibang functions
     * e.g. deposit money, balance inq, withdraw money, pin checker
     * para ma-make sure natin na nasa tamang index (account number) tayo nagm-manipulate ng data
     * at hindi natin mapakialaman yung ibang existing account details */
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
            found = 1; // sets found to 1, meaning nahanap natin si account number sa database, nage-exist sya 
            index = i; // is-set natin si index kay i (i = kung saan natin nahanap yung account number)
            pinChecker(i); // the function calls the pinChecker function with the found index (i) to check the associated PIN
        }
    }

    if (!found) { // after the loop, if the found flag is still 0, it means the entered account number does not match any account in the accounts array
        printf("\tAccount Number not found. Try again.\n\n");
        return accountNoChecker(); // uses recursion kung saan ic-call ulit natin si account no. checker function para magpa-input ulit kay user
    }

    return index; // function returns the index of the matching account to the caller (line 634)
}


// Function to ask the user if they want to repeat the transaction or not
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
    return answer; // will return a value of either 1 or 2 (yes or no) to the caller of the function 
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
	    
	    choice = repeatTransaction(); // calls repeat transaction tapos yung ir-return nyang answer ay is-store natin to variable choice
	    if(choice == 2) return; // kapag 2 (no) ang choice ni user, balik lang doon sa caller (line 546) at mags-stop na yung loop ng function na to
	} while(choice == 1); // hanggat 1 (yes) yung choice ni user, repeat lang ang function na to
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
	    
	    choice = repeatTransaction(); // calls repeat transaction tapos yung ir-return nyang answer ay is-store natin to variable choice
	    if(choice == 2) return; // kapag 2 (no) ang choice ni user, balik lang doon sa caller at mags-stop na yung loop ng function na to
	} while(choice == 1); // hanggat 1 (yes) yung choice ni user, repeat lang ang function na to
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
	         
	        choice = repeatTransaction(); // calls repeat transaction tapos yung ir-return nyang answer ay is-store natin to variable choice
	    	if(choice == 2) return; // kapag 2 (no) ang choice ni user, balik lang doon sa caller at mags-stop na yung loop ng function na to
	} while(choice == 1); // hanggat 1 (yes) yung choice ni user, repeat lang ang function na to
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
		printf("\tInvalid amount. Initial deposit should at least be P500 and will not exceed to 1 million.\n");
		return createDeposit();
	}
    printf("\tInitial deposit acknowledged.\n");

    return deposit; // returns the valid deposit amount to the caller (line 400)
}


// Function for creating a new account which will reflect in the .txt file
void createAccount() {
	int createAgain;
	
	do{
		/* monitors if the indexAccounts (taga-monitor kung ilan na yung accounts natin), 
		 * is greater than or equal to max_acc_details (max number of accounts na kayang i-hold ng program at file)
		 * at kapag nag-true, meaning sobra na sa capacity ng program yung number of accounts
		 * kaya hindi na hahayaan ni program na magpa-create pa ng new account */
	    if(indexAccounts >= MAX_ACC_DETAILS) {
	        printf("\n");
	        printf("\nMaximum number of accounts reached. Cannot add more accounts.\n\n");
	        printf("\n");
	        return; // magr-return sa caller (line 638) since hindi na tayo papayag na magpa-create pa ng new account
		}
	
	    int newAccountNo; // takes an integer input from the user
	    char accNo[6]; // the character array storage for a valid account number 
	
	    printf("\n");
	    printf("\t\t\t\tCREATE NEW ACCOUNT\n");
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
	    // converts the int input to string and stores that to the character array that we declared at line 338
	    itoa(newAccountNo, accNo, 10); // syntax: itoa(int value, char str, int base) 
		
		/* after nating makuha yung valid account from the user
		 * mags-string compare tayo
		 * ic-compare natin sa accounts array kung may same na account number na
		 * if meron, meaning may nage-exist nang ganung acc number
		 * and we will  set found to 1 at magj-jump to line 372*/
	    int found = 0;
	    for(int i = 0; i < MAX_ACC_DETAILS; i++) {
	        if(strcmp(accounts[i].accountNumber, accNo) == 0) {
	            found = 1;
	            break;
	        }
	    }
		
		// if found, meaning may ganun nang account number kaya bawal nang gamitin pa yun
	    if(found) {
	        printf("\tAccount Number already exists. Try again.\n");
	        /* uses recursion para ipaulit kay user yung process ng 
			 * pag-input ng valid account number na walang ka-same
			 * sa existing account numbers na meron tayo inside the array at inside the accounts.txt file */
	        return createAccount(); 
	    }
	    printf("\tAccount Number acknowledged.\n");
		
		/* after nating makakuha na ng valid account number from user
		 * proceed tayo sa pagkuha ng pin for that account number */
	    int newPin;
	    char pin[5]; // the character array storage for a valid pin
	
	    printf("\n\tEnter a new PIN for the account: ");
	    // takes an integer account pin input from the user to be checked 
	    while(scanf("%d", &newPin) != 1 || newPin > 9999 || newPin < 1000) {
	        printf("\tInvalid input. Try again.\n");
	            
	        while(getchar() != '\n');
	        printf("\tEnter a new pin for the account: "); // papaulit kay user yung process if invalid pin ang ininput
	    }
	    // converts the int input to string and stores that to the character array that we declared at line 384
	    itoa(newPin, pin, 10); 
	    printf("\tPIN acknowledged.\n");
		
		/* after nating makakuha ng valid pin para doon sa account number
		 * magpapa-input na tayo ngayon ng initial deposit ni user */
	    float initialBalance = createDeposit(); // calls the create deposit function and stores the returned value to "initialBalance" variable
	
		/* dito magi-iterate tayo sa array until makahanap tayo ng index
		 * kung saan empty pa o wala pang account number, account pin, at account balance
		 * after natin mahanap yun, is-set natin yung indexAccounts variable sa value na yun (see line 408)
		 * at doon na natin is-store yung panibagong account details na crineate ni user (see lines 413-415) */
	    for(int i = 0; i < MAX_ACC_DETAILS; i++) {
	        if(strlen(accounts[i].accountNumber) == 0) {
	            indexAccounts = i;
	            break;
	        }
	    }
	
	    strcpy(accounts[indexAccounts].accountNumber, accNo); // uses strcpy (string copy) to copy the user input account number to the accounts array
	    strcpy(accounts[indexAccounts].accountPin, pin); // uses strcpy (string copy) to copy the user input PIN to the accounts array
	    accounts[indexAccounts].accountBalance = initialBalance; 
		
		/* increment natin si indexAccounts variable 
		 * to signal na nadagdagan ng isang account yung total number of accounts natin */
	    indexAccounts++; 
	    updateAccountsFile(); // calls the update accounts file function para i-update yung accounts.txt file at makita natin doon yung newly created account
	        
	    printf("\n\tAccount created succesfully.\n\n");
	    
	    createAgain = repeatTransaction(); // calls repeat transaction tapos yung ir-return nyang answer ay is-store natin to variable choice
	    if(createAgain == 2) return; // kapag 2 (no) ang choice ni user, balik lang doon sa caller at mags-stop na yung loop ng function na to
	} while(createAgain == 1); // hanggat 1 (yes) yung choice ni user, repeat lang ang function na to
}


// Function for deleting an existing account
void deleteAccount() {
    int deleteAgain;
    
    do {
    	/* monitors if the indexAccounts (taga-monitor kung ilan na yung accounts natin) is 0 
		 * kapag nag-true sya (zero yung number of accounts natin sa accounts.txt file)
		 * ibig sabihin, walang laman o wala tayong account details
		 * at wala tayong id-delete, so magr-return lang sya sa caller (line 443) 
		 * at hindi magp-proceed sa pag-delete ng account */
        if(indexAccounts == 0) {
            printf("\n");
            printf("\n\tDatabase empty. No existing account found.\n\n");
            printf("\n");
            return;
        }
        else { // else statement kapag hindi 0 ang indexAccounts meaning may laman na account details ang accounts.txt file
            int numberInp; // kuha muna tayo ulit ng number input from the user para i-check natin...
            char deleteAcc[6]; // tapos is-store yun sa character array na to using itoa function
            int accInd; // index nung account na gusto nating i-delete

            printf("\n");
            printf("\t\t\t\tDELETE AN ACCOUNT\n");
            printf("\n\tEnter the account number of the account you want to delete: ");
            // check lang ulit kung valid ba yung account number na ininput ni user 
            while(scanf("%d", &numberInp) != 1 || numberInp < 10000 || numberInp > 99999) {
                printf("\tInvalid input. Enter a valid account number.\n");

                while(getchar() != '\n');
                printf("\tEnter the account number of the account you want to delete: ");
            }
            // after checking, convert na natin yung integer input into string at i-store kay deleteAcc character array (string) 
            itoa(numberInp, deleteAcc, 10);
			
			/* after getting a valid account number input from the user,
			 * magi-iterate tayo sa accounts array para maghanap kung may
			 * may same na account number yung array natin doon sa ininput ni user
			 * using string compare */ 
            int found = 0;
            for(int i = 0; i < MAX_ACC_DETAILS; i++) {
                if(strcmp(accounts[i].accountNumber, deleteAcc) == 0) {
                    found = 1; // sets found to 1, meaning yung account na yun ay nage-exist sa accounts.txt file natin
                    accInd = i; // sets the accInd to i (i = kung saan natin nahanap yung existing account number)
                    /* calls pin checker para i-check syempre kung tama yung pin while also passing the index
                     * and if the pin is correct, the program flow will jump to line 493 */
                    pinChecker(i); 
                    break;
                }
            }
            
            /* kapag walang nahanap na same account number mula ron 
             * sa accounts array natin, the account doesnt exist */
            if(!found) {
                printf("\tAccount Number not found. Try again.\n");
                /* magr-recursion, tatawagin lang ulit si deleteacc function para 
				 * ipaulit kay user magpa-input ng existing account number */
                return deleteAccount(); 
            }

            printf("\n");
    		
    		/* after nating ma-check
    		// asks the user for confirmation kung talagang gusto na nilang i-delete yung account */
    		int choice;
            printf("\n\tAre you sure you want to delete this account?\n\n\t[1] Yes\n\t[2] No\n\n");
            printf("\tEnter your choice: ");
			while (scanf("%d", &choice) != 1 || choice > 2 || choice < 1) {
	            printf("\tInvalid input. Enter numbers from 1-2 only.\n");
	            while (getchar() != '\n');
	            printf("\tEnter your choice: ");
       	 	}
       	 	
            if(choice == 2) return; // kapag 2 (no) ang choice ni user, return lang tayo sa caller (line 642) at hindi magp-proceed sa deletion of account
			
			// kapag 1 (yes) naman ang choice ni user, proceed tayo sa deletion of account using swapping
            for(int i = 0; i < indexAccounts; i++) {
                if(i >= accInd) {
                    accounts[i] = accounts[i + 1];
                }
            }

            indexAccounts--; // decrements the indexAccounts variable to signal na nabawasan ng account yung accounts array natin
            updateAccountsFile(); // then iu-update natin yung accounts.txt file with this function call para mag-reflect doon na may nabawas ngang account

            printf("\tAccount deleted successfully.\n\n");

            deleteAgain = repeatTransaction(); // calls repeat transaction tapos yung ir-return nyang answer ay is-store natin to variable choice
	    	if(deleteAgain == 2) return; // kapag 2 (no) ang choice ni user, balik lang doon sa caller at mags-stop na yung loop ng function na to
        }
    } while(deleteAgain == 1); // hanggat 1 (yes) yung choice ni user, repeat lang ang function na to
}


// Main menu function
void mainMenu(int index) {
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
				
				if(choice == 1) return; // kapag nag 1 si user (yes), return lang tayo sa caller (line 634)
				break;
        	}

            default:
                printf("\n\t Invalid choice. Please enter a valid option (1-4).\n\n");
                break;
        }
    } while (1);
}


int main() {
	// IMPORTANT: another case where FILE HANDLING happens
    file = fopen("accounts.txt", "r"); // open natin si accounts.txt in read mode to retrieve the account details
	
	/* check natin if the file is existing or not
	 * kapag wala tayong accounts.txt file sa folder, magr-return ng null si fopen function at ilalagay nya kay "file" pointer (see line 584)
	 * at kapag "file" pointer == NULL nga, hindi magp-proceed ang program sa inner parts since hindi mahanap ang file */
    if (file == NULL) {
        printf("\t\t+-------------------------------------------------+\n");
        printf("\t\t|             Unable to open file.                |\n");
        printf("\t\t+-------------------------------------------------+\n");
        return 1; // returns 1 and ends the program immediately 
    }
	
	// kung nage-exist naman si accounts.txt file, proceed lang sa main program
    printf("\t\t+-------------------------------------------------+\n");
    printf("\t\t|              File opened successfully.          |\n");
    printf("\t\t+-------------------------------------------------+\n");
		
    // retrieve natin yung data from accounts.txt file using for loop at fscanf function
    for (int i = 0; i < MAX_ACC_DETAILS; i++) {
        fscanf(file, "%s %s %f\n", accounts[i].accountNumber, accounts[i].accountPin, &accounts[i].accountBalance);
        
        /* this one checks valid account details from the accounts.txt file
         * kapag yung na-scan nating data from the file is not 0,
         * ibig sabihin nun, may valid account number na nage-exist
         * at ii-increment natin si indexAccounts at ma-monitor nya kung ilan ang valid accounts from the file */
        if(strlen(accounts[i].accountNumber) != 0) {
            indexAccounts++;
        }
    }
    fclose(file); // end of file handling (for now)

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
                    printf("\nThank you for using the program.\n");
                    return 0;
                }
	    		break;
	    	}
		}
	} while(1);
	
	printf("\tThank you for using the program.\n");
	
    return 0;
}
