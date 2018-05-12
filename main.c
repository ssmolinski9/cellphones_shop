#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define TRUE 1
#define FALSE 0
#define HUGE 50
#define MID 25
#define SMALL 10

/*
	Name: Cellphones shop
	Author: Sebastian Smolinski
	Date: 27-12-17
	Version: 1.0
*/


/* Structures pool */ 
typedef struct Phone {
	int id, ownerid;
	char model[HUGE];
	char brand[HUGE];
	char weight[SMALL]; 
	char standby[MID]; 
	char price[SMALL]; 
	char condition[MID]; 
	struct Phone *next;  
} Phone;

typedef struct Customer {
	int id; 
	char name[HUGE]; 
	char surname[HUGE]; 
	int totalamount;
	struct Customer *next; 
} Customer; 

/* Functions prototypes pool */ 
Phone *loadPhonesList();
Customer *loadCustomersList();
int addListElementHandler(Phone **, Customer **);
int editListElementHandler(Phone **, Customer **);
int deleteListElementHandler(Phone **, Customer **);
int deleteManyListElementsHandler(Phone **);
int sellPhoneHandler(Phone **, Customer**);  
int sortListHandler(Phone **);
int showCustomerDetailsHandler(Phone **, Customer **);
void savePhonesList(Phone*);
void saveCustomersList(Customer*);
void appendPhoneElement(Phone**, Phone*);
void appendCustomerElement(Customer**, Customer*);
void editPhoneElement(Phone **, int);
void editCustomerElement(Customer **, int);
void deletePhoneElement(Phone **, int); 
void deleteCustomerElement(Customer **, int); 
void swapPhoneListElements(Phone *, Phone *);
void printPhonesList(Phone*);
void printCustomersList(Customer*);
int readString(char *, int, char *);

/* Main function */
int main(int argc, char *argv[]) {
	/* Variables pool */
	int option; 
	Phone *phones = NULL; 
	Customer *customers = NULL; 
	
	/* Load data to lists */
	phones = loadPhonesList(); 
	customers = loadCustomersList();
	
	/* Main loop */
	do {
		/* Menu printer and handler */ 
		printf("1. Add record\n"); 
		printf("2. Edit record\n"); 
		printf("3. Delete record\n"); 
		printf("4. Print records\n"); 
		printf("5. Sort cellphones list\n"); 
		printf("6. Delete records from cellphone list\n"); 
		printf("7. Sell cellphone\n"); 
		printf("8. Show client's transaction list\n");
		printf("9. Save and exit\n"); 
		printf("Select option: "); 
		
		scanf("%d", &option); 
		fseek(stdin, 0, SEEK_END); 
		
		system("CLS");
		switch(option) {
			case 1:
				addListElementHandler(&phones, &customers);
				break; 
			case 2:
				editListElementHandler(&phones, &customers);
				break;
			case 3:
				deleteListElementHandler(&phones, &customers); 
				break;
			case 4:
				printPhonesList(phones);
				printCustomersList(customers);
				break; 
			case 5:
				sortListHandler(&phones); 
				break; 
			case 6:
				deleteManyListElementsHandler(&phones); 
				break; 
			case 7:
				sellPhoneHandler(&phones, &customers); 
				break;
			case 8:
				showCustomerDetailsHandler(&phones, &customers); 
				break;
		}
	} while(option != 9 && option != 9);
	
	/* Save data and clean up */
	savePhonesList(phones);
	saveCustomersList(customers);
	
	free(phones);
	free(customers);
	
	phones = NULL;
	customers = NULL;
	
	return 0;
}

/* Loads data from file to Phones list */
Phone *loadPhonesList() {
	Phone *head = NULL; 
	Phone *holder = (Phone*)malloc(sizeof(Phone)); 
	
	FILE *ptr;
	ptr = fopen("data1.bin", "rb");
	
	if(!ptr) {
		return NULL; 
	} else {
		while(TRUE) {
			fread(holder, sizeof(Phone), 1, ptr);
			if(feof(ptr) != 0) break;
			appendPhoneElement(&head, holder);
		}
	}
	
	fclose(ptr);
	free(holder);
	holder = NULL;
	
	return head; 
}

/* Loads data from file to Customers list */
Customer *loadCustomersList() {
	Customer *head = NULL; 
	Customer *holder = (Customer*)malloc(sizeof(Customer)); 
	
	FILE *ptr; 
	ptr = fopen("data2.bin", "rb"); 
	if(!ptr) {
		return NULL; 
	} else {
		while(TRUE) {
			fread(holder, sizeof(Customer), 1, ptr); 
			if(feof(ptr) != 0) break; 
			appendCustomerElement(&head, holder); 
		}
	}
	
	fclose(ptr); 
	free(holder);
	holder = NULL; 
	
	return head; 
}

/* Saves data from Phones list to the file */
void savePhonesList(Phone *head) {
	FILE *ptr; 
	ptr = fopen("data1.bin", "wb"); 
	if(!ptr) {
		printf("Error! Something with data save went wrong!\n");
	} else {
		while(head) {
			fwrite(head, sizeof(Phone), 1, ptr); 
			head = head->next; 
		}
	}
	
	fclose(ptr);
}

/* Saves data from Customers list to the file */
void saveCustomersList(Customer *head) {
	FILE *ptr; 
	ptr = fopen("data2.bin", "wb"); 
	if(!ptr) {
		printf("Error! Something with data save went wrong!\n");
	} else {
		while(head) {
			fwrite(head, sizeof(Customer), 1, ptr); 
			head = head->next; 
		}
	}
	
	fclose(ptr);
}

/* Handles element adding action */
int addListElementHandler(Phone **headp, Customer **headc) {
	int option; 
	
	do {
		system("CLS");
		printf("1. Add new record to cellphones list\n");
		printf("2. Add new record to clients list\n");
		printf("Select option: ");
		
		scanf("%d", &option); 
		fseek(stdin, 0, SEEK_END); 
	} while(option != 1 && option != 2);
	
	if(option == 1) {
		char model[HUGE]; 
		char brand[HUGE];
		char weight[SMALL]; 
		char standby[MID];
		char price[SMALL]; 
		char condition[MID];
		int id; 
		
		if(readString(brand, HUGE, "Brand") != 0) return;
		if(readString(model, HUGE, "Model") != 0) return;
		if(readString(weight, SMALL, "Weight (in g)") != 0) return;
		if(readString(standby, MID, "Standby time (in minutes)") != 0) return;
		if(readString(price, SMALL, "Price (in EUR)") != 0) return;
		if(readString(condition, MID, "Condition") != 0) return;
		
		Phone *data = (Phone*)malloc(sizeof(Phone));
		strcpy(data->model, model);
		strcpy(data->brand, brand);
		strcpy(data->weight, weight);
		strcpy(data->standby, standby);
		strcpy(data->price, price);
		strcpy(data->condition, condition);
		
		Phone *copy = *headp; 
		
		if(copy == NULL) id = 0;
		else {
			id = copy->id; 
			while(copy->next) {
				if(copy->next->id > id) 
					id = copy->next->id; 
				copy = copy->next; 
			}
		}
		
		data->id = id + 1; 
		data->ownerid = 0; 
		
		appendPhoneElement(headp, data);
		free(data); 
		data = NULL; 
		
		return id; 
	} else if(option == 2) {
		char name[HUGE]; 
		char surname[HUGE]; 
		int id; 
		
		if(readString(name, HUGE, "Name") != 0) return; 
		if(readString(surname, HUGE, "Surname") != 0) return;
		
		Customer *data = (Customer*)malloc(sizeof(Customer)); 
		strcpy(data->name, name);
		strcpy(data->surname, surname); 
		
		Customer *copy = *headc; 
		
		if(copy == NULL) id = 0;
		else {
			id = copy->id; 
			while(copy->next) {
				if(copy->next->id > id) 
					id = copy->next->id; 
				copy = copy->next; 
			}
		}
		
		data->id = id + 1; 
		data->totalamount = 0;
		
		appendCustomerElement(headc, data);
		free(data); 
		data = NULL;
		
		return id;
	}
}

/* Handles element editing action */
int editListElementHandler(Phone **headp, Customer **headc) {
	int option; 
	
	do {
		system("CLS");
		printf("1. Edit record from cellphones list\n");
		printf("2. Edit record from clients list\n");
		printf("Select option: ");
		
		scanf("%d", &option); 
		fseek(stdin, 0, SEEK_END); 
	} while(option != 1 && option != 2);
	
	if(option == 1) {
		printPhonesList(*headp); 
		printf("Select index: "); 
		
		scanf("%d", &option); 
		fseek(stdin, 0, SEEK_END);
		
		editPhoneElement(headp, option);
	} else if(option == 2) {
		printCustomersList(*headc); 
		printf("Select index: "); 
		
		scanf("%d", &option); 
		fseek(stdin, 0, SEEK_END); 
		
		editCustomerElement(headc, option);
	}
	
	return option;
}

/* Handles element deleting action */
int deleteListElementHandler(Phone **headp, Customer **headc) {
	int option; 
	
	do {
		system("CLS");
		printf("1. Delete record from cellphones list\n");
		printf("2. Delete record from customers list\n");
		printf("Select option: ");
		
		scanf("%d", &option); 
		fseek(stdin, 0, SEEK_END); 
	} while(option != 1 && option != 2);
	
	if(option == 1) {
		printPhonesList(*headp); 
		printf("Select index: "); 
		
		scanf("%d", &option); 
		fseek(stdin, 0, SEEK_END); 
		
		deletePhoneElement(headp, option); 
	} else if(option == 2) {
		printCustomersList(*headc); 
		printf("Select index: "); 
		
		scanf("%d", &option); 
		fseek(stdin, 0, SEEK_END); 
		
		deleteCustomerElement(headc, option); 
	}
	
	return option;
}

/* Handles list sorting action */ 
int sortListHandler(Phone **headp) {
	int option; 
	
	do {
		system("CLS"); 
		printf("1. Sort list by brand\n"); 
		printf("2. Sort list by model\n");
		printf("3. Sort list by price\n"); 
		printf("Select option: "); 
		
		scanf("%d", &option); 
		fseek(stdin, 0, SEEK_END); 
	} while(option != 1 && option != 2 && option != 3); 
	
	int i, j, counter = 0; 
	Phone *copy = *headp; 
	
	if(*headp == NULL) 
		return; 
	
	while(copy) {
		counter++; 
		copy = copy->next; 
	}
	
	copy = *headp; 
	
	for(i = 0; i < counter; i++) {
		for(j = 0; j < (counter-i-1); j++) {
			if(option == 1) {
				if(strcmp(copy->brand, copy->next->brand) > 0) {
					swapPhoneListElements(copy, copy->next);
				}
				
				copy = copy->next; 
			} else if (option == 2) {
				if(strcmp(copy->model, copy->next->model) > 0) {
					swapPhoneListElements(copy, copy->next); 
				}
				
				copy = copy->next; 
			} else if (option == 3) {
				uintmax_t num = strtoumax(copy->price, NULL, 10);
				uintmax_t numn = strtoumax(copy->next->price, NULL, 10); 
				
				if(num > numn) {
					swapPhoneListElements(copy, copy->next);
				}
				
				copy = copy->next; 
			}
		}
		
		copy = *headp; 
	}
}

/* Handles element details showing */ 
int showCustomerDetailsHandler(Phone **headp, Customer **headc) {
	int customerid; 
	printCustomersList(*headc); 
	printf("Select client index: ");
	scanf("%d", &customerid); 
	fseek(stdin, 0, SEEK_END); 
	
	Customer *copy = *headc; 
	while(copy) {
		if(copy->id == customerid) {
			Phone *copy2 = *headp; 
			while(copy2) {
				if(copy2->ownerid == customerid) {
					printf("%d) %s %s, weight: %s g, standby: %s min, condition: %s, price: %s EUR\n", copy2->id, copy2->brand, copy2->model, copy2->weight, copy2->standby, copy2->condition, copy2->price);
				}
				
				copy2 = copy2->next; 
			}
			
			printf("\n");
			break; 
		}
		
		copy = copy->next; 
	}
	
	return customerid; 
}

/* Handles mass deleting elements from the Phone list */ 
int deleteManyListElementsHandler(Phone **head) {
	
	Phone *copy;
	Phone *start = NULL;
	Phone *end = NULL;
	
	int option; 
	
	do {
		system("CLS");
		printf("1. Delete records from a given brand\n");
		printf("2. Delete records from a given model\n");
		printf("3. Delete records from a given price\n");
		printf("Select option: ");
		
		scanf("%d", &option); 
		fseek(stdin, 0, SEEK_END); 
	} while(option != 1 && option != 2 && option != 3);
	
	if(option == 1) {
		char brand[HUGE]; 
		readString(brand, HUGE, "Delete records from a given brand"); 
		
		while(strcmp((*head)->brand, brand) == 0) {
			(*head) = (*head)->next;
		}
		
		copy = *head; 
		
		while(copy->next) {
			if(strcmp(copy->next->brand, brand) == 0) {
				start = copy; 
				copy = start; 
				while(strcmp(copy->next->brand, brand) == 0) {
					copy = copy->next; 
					if(copy->next == NULL)
						break; 
				}
				
				end = copy->next; 
				copy = start; 
				copy->next = end;
			} else {
				copy = copy->next;
			}
		}
	} else if (option == 2) {
		char model[HUGE]; 
		readString(model, HUGE, "Delete records from a given model");
		
		while(strcmp((*head)->model, model) == 0) {
			(*head) = (*head)->next;
		}
		
		copy = *head; 
		
		while(copy->next) {
			if(strcmp(copy->next->model, model) == 0) {
				start = copy; 
				copy = start; 
				while(strcmp(copy->next->model, model) == 0) {
					copy = copy->next; 
					if(copy->next == NULL)
						break; 
				}
				
				end = copy->next; 
				copy = start; 
				copy->next = end; 
			} else {
				copy = copy->next;
			}
		}
	} else if (option == 3) {
		char price[SMALL]; 
		readString(price, SMALL, "Delete records from a given price");
		
		while(strcmp((*head)->price, price) == 0) {
			(*head) = (*head)->next;
		}
		
		copy = *head; 
		
		while(copy->next) {
			if(strcmp(copy->next->price, price) == 0) {
				start = copy; 
				copy = start; 
				while(strcmp(copy->next->price, price) == 0) {
					copy = copy->next; 
					if(copy->next == NULL)
						break; 
				}
				
				end = copy->next; 
				copy = start; 
				copy->next = end; 
			} else {
				copy = copy->next;
			}
		}
	}
	
	return option;
}

/* Handles sell phone action */
int sellPhoneHandler(Phone **headp, Customer **headc) {
	int phoneid;
	int customerid;  
	
	printPhonesList(*headp);
	printf("Select index: ");
	scanf("%d", &phoneid); 
	fseek(stdin, 0, SEEK_END); 
	
	printCustomersList(*headc); 
	printf("Select customer index: ");
	scanf("%d", &customerid); 
	fseek(stdin, 0, SEEK_END); 
	
	Phone *copy = *headp; 
	Customer *copy2 = *headc;
	while(copy) {
		if(copy->id == phoneid) {
			if(copy->ownerid != 0)
				return; 
				
			copy->ownerid = customerid;
			
			while(copy2) {
				if(copy2->id == customerid) {
					copy2->totalamount += strtol((copy->price), NULL, 10); 
					break;
				}
				
				copy2 = copy2->next; 
			}
			
			break; 
		}
		
		copy = copy->next; 
	}
	
	return customerid;
}

/* Appends element to the end of Phones list */ 
void appendPhoneElement(Phone **head, Phone *data) {
	Phone *element = (Phone*)malloc(sizeof(Phone)); 
	Phone *last = *head; 
	
	strcpy(element->brand, data->brand); 
	strcpy(element->model, data->model); 
	strcpy(element->weight, data->weight);
	strcpy(element->standby, data->standby);
	strcpy(element->price, data->price);
	strcpy(element->condition, data->condition);
	
	element->ownerid = data->ownerid;
	element->id = data->id; 
	element->next = NULL; 
	
	if(*head == NULL) {
		*head = element; 
	} else {
		while(last->next)
			last = last->next; 
		last->next = element; 
	}
}

/* Appends element to the end of Customers list */ 
void appendCustomerElement(Customer **head, Customer *data) {
	Customer *element = (Customer*)malloc(sizeof(Customer)); 
	Customer *last = *head; 
	
	strcpy(element->name, data->name); 
	strcpy(element->surname, data->surname); 
	
	element->totalamount = data->totalamount;
	element->id = data->id;
	element->next = NULL; 
	
	if(*head == NULL) {
		*head = element; 
	} else {
		while(last->next)
			last = last->next; 
		last->next = element; 
	}
}

/* Edits element from the Phones list */
void editPhoneElement(Phone **head, int id) {
	Phone *copy = *head; 
	
	char model[HUGE]; 
	char brand[HUGE];
	char weight[SMALL]; 
	char standby[MID];
	char price[SMALL]; 
	char condition[MID];
	
	if(readString(brand, HUGE, "Brand") != 0) return;
	if(readString(model, HUGE, "Model") != 0) return;
	if(readString(weight, SMALL, "Weight (in g)") != 0) return;
	if(readString(standby, MID, "Standby (in minutes)") != 0) return;
	if(readString(price, SMALL, "Price (in EUR)") != 0) return;
	if(readString(condition, MID, "Condition") != 0) return;
	
	while(copy) {
		if(copy->id == id)
			break; 
			
		copy = copy->next; 
	}
	
	if(copy == NULL) {
		printf("Selected incorrect index!\n"); 
		return; 
	}
	
	strcpy(copy->brand, brand);
	strcpy(copy->model, model);
	strcpy(copy->weight, weight);
	strcpy(copy->standby, standby);
	strcpy(copy->price, price);
	strcpy(copy->condition, condition);
}

/* Edits element from the Customers list */
void editCustomerElement(Customer **head, int id) {
	Customer *copy = *head; 
	char name[HUGE]; 
	char surname[HUGE];
	
	if(readString(name, HUGE, "Customer's name") != 0) return;
	if(readString(surname, HUGE, "Customer's surname") != 0) return;
	
	while(copy && copy->id != id) {
		copy = copy->next; 
	}
	
	if(copy->id != id) {
		printf("Selected incorrect index!\n"); 
		return; 
	}
	
	strcpy(copy->name, name);
	strcpy(copy->surname, surname);
}

/* Deletes element from Phones list */
void deletePhoneElement(Phone **head, int id) {
	Phone *copy = *head; 
	
	if(copy->id == id) {
		*head = (*head)->next; 
	} else {
		while(copy->next) {
			if(copy->next->id == id)
				break; 
				
			copy = copy->next; 
		} 
		
		if(copy->next == NULL) {
			printf("Selected incorrect index!\n");
			return; 
		}
		
		copy->next = copy->next->next; 
	}
}

/* Deletes element from Customers list */
void deleteCustomerElement(Customer **head, int id) {
	Customer *copy = *head; 
	
	if(copy->id == id) {
		*head = (*head)->next; 
	} else {
		while(copy->next) {
			if(copy->next->id == id)
				break; 
				
			copy = copy->next; 
		}
		
		if(copy->next == NULL) {
			printf("Selected incorrect index!\n");
			return; 
		}
		
		copy->next = copy->next->next; 
	}
}

/* Prints Phones list */
void printPhonesList(Phone *head) {
	if(head == NULL) {
		printf("Cellphones list is empty!\n");
		return; 
	}
	
	printf("Cellphones list:\n");
	while(head) {
		if(head->ownerid == 0)
			printf("%d) %s %s, weight: %s g, standby: %s min, condition: %s, price: %s EUR\n", head->id, head->brand, head->model, head->weight, head->standby, head->condition, head->price);
		head = head->next; 
	}
	
	printf("\n");
}

/* Prints Customers list */
void printCustomersList(Customer *head) {
	if(head == NULL) {
		printf("Customers list is empty!\n");
		return; 
	}
	
	printf("Customers list:\n");
		
	while(head) {
		printf("%d) %s %s, money spent: %d EUR\n", head->id, head->name, head->surname, head->totalamount);
		head = head->next; 
	}
	
	printf("\n");
}

/* Swaps two elements of Phones list */ 
void swapPhoneListElements(Phone *first, Phone *second) {
	char model[HUGE]; 
	char brand[HUGE];
	char weight[SMALL]; 
	char standby[MID];
	char price[SMALL]; 
	char condition[MID];
	int ownerid; 
	
	strcpy(brand, first->brand);
	strcpy(model, first->model);
	strcpy(weight, first->weight);
	strcpy(standby, first->standby);
	strcpy(price, first->price);
	strcpy(condition, first->condition);
	ownerid = first->ownerid; 
	
	strcpy(first->brand, second->brand);
	strcpy(first->model, second->model);
	strcpy(first->weight, second->weight);
	strcpy(first->standby, second->standby);
	strcpy(first->price, second->price);
	strcpy(first->condition, second->condition);
	first->ownerid = second->ownerid; 
	
	strcpy(second->brand, brand); 
	strcpy(second->model, model); 
	strcpy(second->weight, weight); 
	strcpy(second->standby, standby); 
	strcpy(second->price, price); 
	strcpy(second->condition, condition); 
	second->ownerid = ownerid;
}

/* Reads string from stdin and puts it in the array */ 
int readString(char *container, int maxchars, char *message) {
	char reader; 
	int i = 0; 
	
	printf("%s: ", message); 
	
	while((container[i] = getchar()) != '\n') {i++;}
	if(i >= maxchars-1) {
		printf("Typed text is too long!\n"); 
		return 1; 
	}
	
	container[i] = '\0'; 
	return 0; 
}
