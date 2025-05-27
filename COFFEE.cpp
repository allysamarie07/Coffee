/*
	Date Made: 12/29/23
	Purpose: Coffee Machine.
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <conio.h>

// Function prototypes
void Welcome();
void Report(int water, int milk, int coffee, float money); //Reports the available ingredients.
void Resources(int drinkType, int* water, int* milk, int* coffee, int* waterRefill, int* milkRefill, int* coffeeRefill); // Checks the resources available and replenishes it.
void Payment(float* money, float cost, float payment); //Calculate the change as well as the bills/coins dispensed.
void makeCoffee(int drinkType); //A simulation of serving the coffee.
void Save(int water, int milk, int coffee, float money, int drinksMade, float totalSales, float overallProfit, int waterRefill, int milkRefill, int coffeeRefill); //Saves the data inside a file "coffee.txt".
void Load(int* water, int* milk, int* coffee, float* money, int* drinksMade, float* totalSales, float* overallProfit, int* waterRefill, int* milkRefill, int* coffeeRefill); //Loads/reads previous data "coffee.txt".
int isInteger(const char *payment);//Checks if the payment entered are all integers.

int main(void) {
	int water, milk, coffee, waterRefill, milkRefill, coffeeRefill;
	int drinksMade; //Tallies the drinks served
	float money, payment, totalSales, overallProfit;
	int costumer = 1, reset = 0;
	clrscr();
	Load(&water, &milk, &coffee, &money, &drinksMade, &totalSales, &overallProfit, &waterRefill, &milkRefill, &coffeeRefill); //Load data from the file
	Welcome();
		while (1) {
			char choice[20];
			printf("\"Serving Costumer Number %d\"\n", costumer);
			printf("What would you like? (espresso/latte/cappuccino): ");
			scanf("%s", choice);

			for(int i = 0; choice[i]; i++){
			choice[i] = tolower(choice[i]); //Lowercase all characters to avoid errors.
			}

				if (strcmp(choice, "off") == 0) {
					// Turn off the Coffee Machine
					printf("Program Terminated");
					Save(water, milk, coffee, money, drinksMade, totalSales, overallProfit, waterRefill, milkRefill, coffeeRefill);
					break;
				} else if (strcmp(choice, "report") == 0) {
					// Display report
					Report(water, milk, coffee, money);
				} else if (strcmp(choice, "reset") == 0){
					//Resets the values of each data.
					reset++;
					if (reset >= 3){
						printf("DATA RESET\n");
						water = 1000;
						milk = 1000;
						coffee = 500;
						money = 1000.0;
						drinksMade = 0;
						totalSales = 0;
						overallProfit = 0;
						waterRefill = 0;
						milkRefill = 0;
						coffeeRefill = 0;
						reset = 0;
					}else{
						printf("Invalid choice. Please try again.\n");
					}
				} else if (strcmp(choice, "collect") == 0){
					if(money > 1000){
						printf("Money Collected: %.2f Php\n", money - 1000);
						printf("Starting amount: 1000 PhP\n");
						money = 1000;
					}else{
						printf("There is no money to be collected.\n");
					}
				} else if (strcmp(choice, "espresso") == 0 || strcmp(choice, "latte") == 0 || strcmp(choice, "cappuccino") == 0) {
					char extraC;
					if (scanf("%c", &extraC) == 1 && extraC == '\n'){
						// Check resources sufficiency
						int drinkType;
						float cost, profit;
						printf("You have selected ");
						if (strcmp(choice, "espresso") == 0) {
							drinkType = 1; // Espresso
							printf("Espresso.\n");
							cost = 100.0; // Espresso cost
							profit = cost - 60; //Profit
						} else if (strcmp(choice, "latte") == 0) {
							drinkType = 2; // Latte
							printf("Latte\n");
							cost = 140.0; // Latte cost
							profit = cost - 100; //Profit
						} else {
							drinkType = 3; // Cappuccino
							printf("Cappuccino\n");
							cost = 120.0; // Cappuccino cost
							profit = cost - 80; //Profit
						}
						Resources(drinkType, &water, &milk, &coffee, &waterRefill, &milkRefill, &coffeeRefill);
						printf("That would be %.2f Php\n", cost);
						 do {
							printf("Please insert payment: ");
							fflush(stdin);
							if (scanf("%f", &payment) == 1 && isInteger((char *)&payment)) {
								// Check for additional characters in the input
								char extra;
								if (scanf("%c", &extra) == 1 && extra == '\n') {
									// Payment is a valid integer and no extra characters
									if (payment > 1000) {
										printf("Payment must not exceed 1000 pesos\n");
									} else if (payment < cost) {
										printf("Sorry, that's not enough money. Money refunded.\n");
									} else if (money < (payment - cost)) {
										printf("Please enter exact amount as there is no change.\n");
									} else {
										break;
									}
								} else {
									printf("Invalid input. Please enter a valid numeric amount.\n");
									// Clear the input buffer
									while (getchar() != '\n');
								}
							} else {
								if(fmod(payment, 1.0) != 0 && !isInteger((char *)&payment)){
									printf("Sorry. Machine does not accept centavo coins.\n");
								}else{
									printf("Invalid input. Please enter a valid numeric amount.\n");
								}
								// Clear the input buffer
								while (getchar() != '\n');
								payment = 0;
							}
						} while (1);
						Payment(&money, cost, payment);

						// Make coffee
						makeCoffee(drinkType);
						overallProfit += profit;
						totalSales += cost;
						costumer++;
						drinksMade++;
					}else {
						printf("Error. Select only one item at a time.\n");
						// Clear the input buffer
						while (getchar() != '\n');
					}
			} else {
				printf("Invalid choice. Please try again.\n");
			}
			printf("\n");
		}
	getch();
	return (0);
}
void Welcome(){
	printf("WELCOME TO OUR COFFEE MAKER!\n\n");
}

void Report(int water, int milk, int coffee, float money) { //Reports the available ingredients.
	printf("\nWater: %d mL\n", water);
	printf("Milk: %d mL\n", milk);
	printf("Coffee: %d g\n", coffee);
	printf("Money: %.2f Php\n", money);
}

void Resources(int drinkType, int* water, int* milk, int* coffee, int* waterRefill, int* milkRefill, int* coffeeRefill) { // Checks the resources available and replenishes it.
	int requiredWater = 0, requiredMilk = 0, requiredCoffee = 0;
	// Define resource requirements for each drink type
	if (drinkType == 1) {
		requiredWater = 60;
		requiredMilk = 0;
		requiredCoffee = 20;
	} else if (drinkType == 2) {
		requiredWater = 60;
		requiredMilk = 215;
		requiredCoffee = 15;
	} else {
		requiredWater = 60;
		requiredMilk = 150;
		requiredCoffee = 15;
	}
	// Check sufficiency of resources
	if (*water >= requiredWater && *milk >= requiredMilk && *coffee >= requiredCoffee){
		*water -= requiredWater;
		*milk -= requiredMilk;
		*coffee -= requiredCoffee;
	}else{
		if (*water < requiredWater) {
			printf("Sorry, there is not enough water to make this drink.\n");
		}
		if (*milk < requiredMilk){
			printf("Sorry, there is not enough milk to make this drink.\n");
		}
		if (*coffee < requiredCoffee){
			printf("Sorry, there is not enough coffee to make this drink.\n");
		}
		printf("\nReplenish Resources?(Yes/No): ");
		while(1){
			char choice[20];
			scanf("%s", choice);
			char extra;
			if (scanf("%c", &extra) == 1 && extra == '\n') {
				for(int i = 0; choice[i]; i++){
				choice[i] = tolower(choice[i]);
				}
				if (strcmp(choice, "yes") == 0){
					if (*water < requiredWater){
						printf("Water replenished:%dmL\n", 1000 - *water);
						*water = 1000;
						*waterRefill += 1;
						
					}
					if (*milk < requiredMilk) {
						printf("Milk replenished:%dmL\n", 1000 - *milk);
						*milk = 1000;
						*milkRefill += 1;
					}
					if (*coffee < requiredCoffee) {
						printf("Coffee replenished:%dg\n", 500 - *coffee);
						*coffee = 500;
						*coffeeRefill += 1;
					}		
					*water -= requiredWater;
					*milk -= requiredMilk;
					*coffee -= requiredCoffee;
					break;
				}else{
					printf("Machine will not proceed unless replenished: ");
					// Clear the input buffer
				}
			}else{
				printf("Machine will not proceed unless replenished: ");
				// Clear the input buffer
				while (getchar() != '\n');
			}
		}
    }
}

void Payment(float* money, float cost, float payment) { //Calculate the change as well as the bills/coins dispensed.
	float change = payment - cost;
	int bill500 = 0, bill200 = 0, bill100 = 0, bill50 = 0;
	int coin20 = 0, coin10 = 0, coin5 = 0, coin1 = 0;
	   if (payment == cost) {
			*money += cost;
	   }else{
			if (change >= 500) {
			bill500 = change / 500;
			change -= bill500 * 500;
			printf("500 Php: %d\n", bill500);
			}

			if (change >= 200) {
			bill200 = change / 200;
			change -= bill200 * 200;
			printf("200 Php: %d\n", bill200);
			}

			if (change >= 100) {
			bill100 = change / 100;
			change -= bill100 * 100;
			printf("100 Php: %d\n", bill100);
			}

			if (change >= 50) {
			bill50 = change / 50;
			change -= bill50 * 50;
			printf("50 Php: %d\n", bill50);
			}
			
			if (change >= 20) {
			coin20 = change / 20;
			change -= coin20 * 20;
			printf("20 Php: %d\n", coin20);
			}
			
			if (change >= 10) {
			coin10 = change / 10;
			change -= coin10 * 10;
			printf("10 Php: %d\n", coin10);
			}

			if (change >= 5) {
			coin5 = change / 5;
			change -= coin5 * 5;
			printf("5 Php: %d\n", coin5);
			}

			if (change >= 1) {
			coin1 = change / 1;
			change -= coin1 * 1;
			printf("1 Php: %d\n", coin1);
			}
			printf("Change (Php): %.2f\n", payment - cost);
			*money += cost;
		}
}

void makeCoffee(int drinkType) { //A simulation of serving the coffee.
    printf("Making coffee...\n");

    printf("Here is your ");
    if (drinkType == 1) {
        printf("espresso. Enjoy!\n");
    } else if (drinkType == 2) {
        printf("latte. Enjoy!\n");
    } else {
        printf("cappuccino. Enjoy!\n");
    }
	printf("    (  )   (   )  )\n");
	printf("     ) (   )  (  (\n");
	printf("     ( )  (    ) )\n");
	printf("     _____________\n");
	printf("    <_____________> ___\n");
	printf("    |             |/ _ \\\n");
	printf("    |               | | |\n");
	printf("    |               |_| |\n");
	printf(" ___|             |\\___/\n");
	printf("/    \\___________/    \\\n");
	printf("\\_____________________/\n");
}

int isInteger(const char *payment) {   //Checks if the payment entered are all integers.
    for (int i = 0; payment[i] != '\0'; i++) {
        if (!isdigit(payment[i])) {
            return 0; // Not an integer
        }
    }
    return 1; // Integer
}

void Save(int water, int milk, int coffee, float money, int drinksMade, float totalSales, float overallProfit, int waterRefill, int milkRefill, int coffeeRefill) { //Function that saves the data into a file.
    FILE *file = fopen("coffee.txt", "w");
    if (file != NULL) {
		fprintf(file, "%9d %10d %11d %12.2f %13d %18.2f %21.2f %20d %19d %21d\n", water, milk, coffee, money, drinksMade, totalSales, overallProfit,waterRefill, milkRefill, coffeeRefill);
		fprintf(file, "Water(mL) | Milk(mL) | Coffee(g) | Money(Php) | Drinks Made | Total Sales(Php) | Overall Profit(Php) | # Water of Refills | # Milk of Refills | # Coffee of Refills");
		fclose(file);
    } else {
        printf("Error: Unable to save file.\n");
    }
}

void Load(int* water, int* milk, int* coffee, float* money, int* drinksMade, float* totalSales, float* overallProfit, int* waterRefill, int* milkRefill, int* coffeeRefill) { //Function that reads/loads the file saved.
    FILE *file = fopen("coffee.txt", "r");
    if (file != NULL) {
		fscanf(file, "%d %d %d %f %d %f %f %d %d %d", water, milk, coffee, money, drinksMade, totalSales, overallProfit, waterRefill, milkRefill, coffeeRefill);
		fclose(file);
    } else {
        printf("Error: Unable to read file.\n");
		FILE *file = fopen("coffee.txt", "w");
		printf("New File Created.\n");
		*water = 1000;
        *milk = 1000;
        *coffee = 500;
        *money = 1000.0;
		*drinksMade = 0;
		*totalSales = 0;
		*overallProfit = 0;
		*waterRefill = 0;
		*milkRefill = 0;
		*coffeeRefill = 0;
		fclose(file);
	}
}