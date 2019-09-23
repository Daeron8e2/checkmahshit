/*
 * GROCERIES.C SOURCE CODE
 * Written by Jamon Jordan.
 * To store groceries in a grocery list database.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void clearInput(void);
void addNewGrocery(void);
void listAll(void);
void deleteGrocery(void);
void modifyGrocery(void);
int prompt(void);

struct grocery {
    int number;
    char name[20];
    int items;
    float price;
    struct grocery *next;
    };
struct grocery *firstg, *currentg, *newg;
int anum = 0;

int main()
{
/* datafile variable is used to open and reference the file */
    FILE *datafile;
    char *filename = "groceries.dat"; // file saved in groceries.dat
    char ch;
    firstg = NULL;
    
    datafile = fopen(filename,"r");
    if(datafile)    // file opened; doesn't exist otherwise
    {
        firstg = (struct grocery *)malloc(sizeof(struct grocery));
        currentg = firstg;
        while(1)    // the while loop only ends when the last record is read from disk
        {
            newg = (struct grocery *)malloc(sizeof(struct grocery));
            fread(currentg, sizeof(struct grocery),1,datafile);
            if(currentg->next == NULL)
                break;
            currentg->next = newg;
            currentg = newg;
        }
        fclose(datafile);
        anum = currentg->number;
    }
    /*
     * List all the possible options for the user to 
     * interact with the list
     */
    do
    {
        puts("\nA - Add a new grocery");
        puts("L - List all groceries");
        puts("D - Delete a grocery");
        puts("M - Modify grocery entry");
        puts("Q - Quit this program\n");
        printf("\tYour choice: ");
        ch = getchar();
        ch = toupper(ch);
        switch(ch)
        {
            case 'A':
                puts("\nAdd grocery item\n");
                clearInput();
                addNewGrocery();
                break;
            case 'L':
                puts("\nList the groceries\n");
                listAll();
                break;
            case 'D':
                puts("\nDelete a grocery\n");
                deleteGrocery();
                break;
            case 'M':
                puts("\nModify grocery entry\n");
                modifyGrocery();
                break;
            case 'Q':
                puts("Quit\n");
            default:
                clearInput();
                break;
        }
    } 
    while(ch != 'Q');
    
/* 
 * This section of code is used to store the linked
 * list on disk, in the file groceries.dat 
 */
    currentg = firstg;
    if(currentg == NULL)
        return 0;   // no data to write
                    // End of program
    datafile = fopen(filename,"w"); // datafile variable used to open and reference the file
    if(datafile == NULL)
    {
        printf("Error writing to %s\n", filename);
        return 1;
    }
            // write each record to disk
    while(currentg != NULL)
    {
        fwrite(currentg, sizeof(struct grocery), 1, datafile);
        currentg = currentg->next;
    }
    fclose(datafile);
    return 0;
    
    return 0;
}

/*
 * Clears any text from the input stream
 */

void clearInput(void)
{
    while ((getchar())!='\n');
}

/* Function to add a new grocery */

void addNewGrocery(void)
{
    newg = (struct grocery *)malloc(sizeof(struct grocery));
    
    if(firstg==NULL)
        firstg = currentg = newg;
    else
    {
        currentg = firstg;  // first record made current
                            // loop through all groceries:
                            
        while(currentg->next != NULL)
            currentg = currentg->next;
                // The last grocery is found
        currentg->next = newg;  // save the address of new
        currentg = newg;        // make current record new
    }
    
/* Filling in the new structure */

    anum++;
    printf("%27s: %5i\n","Grocery Item Number",anum);
    currentg->number = anum;
    
    printf("%27s: ","Enter the grocery's name");
    scanf("%s",currentg->name);
    
    printf("%27s: ","Enter the number of items");
    scanf("%d",&currentg->items);
    
    printf("%27s: $","Enter the item's price per item");
    scanf("%f",&currentg->price);
    
    currentg->next = NULL;
    
    clearInput();
}

/* function to list all of the elements in the list */
void listAll(void)
{
    if(firstg==NULL)
        puts("There are no records to print!");
    else{
	printf("%6s %-15s %-15s %6s\n", "Gr#","Name","Amount","Price");
        currentg=firstg;
        do{
            printf("%5d: %-15s %-15d $%.2f\n",
                currentg->number,
                currentg->name,
                currentg->items,
                currentg->price);
        }
        while((currentg=currentg->next) != NULL);
    }
    clearInput();
}

/* delete any of the grocery items already on the list */

void deleteGrocery(void)
{
    int record;
    struct grocery *previousg;

/* if there are no records, then nothing is done */
    if(firstg==NULL)
    {
        puts("There are no groceries to delete!");
        clearInput();
        return;
    }
    listAll();		/* show all records first */
    printf("Enter grocery number to delete: ");
    scanf("%d",&record);

    currentg = firstg; /* points to start of list */
    /* while loop is used to scan through list until NULL at the end */
    while(currentg != NULL)
    {
        if(currentg->number == record)
        {
            if(currentg == firstg)	/* special condition */
                firstg=currentg->next;
            else
                previousg->next = currentg->next;
            free(currentg);
            printf("Grocery number %d deleted!\n",record);
            clearInput();
            return;
        }
        else
        {
            previousg = currentg;
            currentg = currentg->next;
        }
    }
    printf("Grocery number %d was not found!\n",record);
    puts("Nothing deleted.");
}

void modifyGrocery(void)
{
    int record;

    if(firstg==NULL)
    {
        puts("There are no records to modify!");
        return;
    }
    
    listAll();		/* show all records */
    printf("\nEnter grocery number to modify or change: ");
    scanf("%d",&record);

/* 
 * While loop to cycle through each element in the 
 * structure and replace them with new data
 */
    currentg = firstg;
    while(currentg != NULL)
    {
        if(currentg->number == record)
        {
            printf("Grocery #%d:\n",currentg->number);
            printf("Current Name: %s\n",currentg->name);
            prompt();
                scanf("%s",currentg->name);
            printf("Items to purchase: %d\n",currentg->items);
            prompt();
                scanf("%d",&currentg->items);
            printf("Amount per item: %8.2f\n",currentg->price);
            prompt();
                scanf("%f",&currentg->price);
            clearInput();
            return;
        }
        else
        {
            currentg = currentg->next;
        }
    }
    printf("Grocery %d was not found!\n",record);
    clearInput();
}

int prompt(void)
{
		printf("Enter new: ");
		return(1);
}
