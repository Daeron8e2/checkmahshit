/* 
 * Database Management System
 * Employee.cpp souce code
 * Written by Jamon Jordan.
 * To store, retrieve and alter
 * employees stored in a database file.
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <list>
#define FILENAME "employee_data.txt" // Filename definition

using std::string; using std::cout; using std::endl;

int logID; // Global variable to store ID of logged in user

// Function Declerations
void wait();
void departmentMenu();

class Employee  // Base class
{
    protected:
        int mID, mDept;
        string mUser, mPass;
    public:
        // Constructor
        Employee(int id = 0, string name = "", string pass = "", int dept = 0) 
            : mID(id), mUser(name), mPass(pass), mDept(dept) { }
        // Login function to retrieve data from file
        bool Login(bool log, Employee& emp)
        {
            std::ifstream datafile(FILENAME);
            string tempUser, tempPass, name, pass;
            int id, dept;
            int counter = 0;
            do
            {
                datafile.clear();
                datafile.seekg(0, std::ios::beg);
                cout << "Enter username: "; std::cin >> tempUser;
                cout << "Enter password: "; std::cin >> tempPass;
                while(datafile>>emp.mID>>emp.mUser>>emp.mPass>>emp.mDept){
                    if(emp.mUser == tempUser && emp.mPass == tempPass){
                        log = true;
                        logID = emp.mID;  // Save the ID of the user who is logged in
                        cout << "Login successful." << endl;
                        break;
                    }
                }
                if(log == true)
                    break;
                cout << "Invalid login, try again." << endl;
                counter++;
            }while(counter < 3);
            
            // Prints an error message if something went wrong
            if(log == false)
                std::cerr << "Your login was invalid, returning to menu." << endl;

            return log;
        }
        void fileCreation(Employee& emp)
        {
            std::fstream datafile; // File object created and file opened
            int id, dept;
            string name, pass;
            datafile.open(FILENAME, std::ios::in);
            // Asks you to enter a password 
            if(!datafile.is_open()){
                cout << "The file could not be found." << endl;
                cout << "Since you are the first employee \n"
                    << "to use this database, you must create \n" 
                    << "an admin login for yourself" << endl;
                cout << "Enter your username: ";    std::cin >> emp.mUser;
                cout << "Enter your password: ";    std::cin >> emp.mPass;
                emp.mID = 1, emp.mDept = 3;
                datafile.open(FILENAME, std::ios::app);   // open the file in append mode
                datafile << emp.mID << " " << emp.mUser << " " << emp.mPass << " " << emp.mDept << std::endl;
            }
            datafile.close();
        }
};

/*
 * The three types of employees are seperated
 * into three derived classes. Each class has its
 * own member functions and data variables.
 */

class HumanResources : public Employee // HR class acts as the admin type
{
public:
    using Employee::Employee;
    void Menu() // Menu function to choose an option
    {
        char ch;
        do{
            cout << "\n** Human Resources Employee Menu **" << endl
                << "Choose an option:" << endl
                << "A - Add new employee" << endl
                << "D - Delete an employee" << endl
                << "M - Modify employee details" << endl
                << "S - Search for an employee" << endl
                << "L - List all employees" << endl
                << "Q - Quit this program\n" << endl
                << "\tYour choice: ";
            std::cin >> ch;
            ch = toupper(ch);
            switch(ch) // Switch function to load correct member function based on user choice
            {
                case 'A':
                    cout << "\nAdd a new employee\n";
                    createNewEmployee();
                    break;
                case 'D':
                    cout << "\nDelete an employee\n";
                    deleteEmployee();
                    break;
                case 'M':
                    cout << "\nModify an employee\n";
                    modifyEmployee();
                    break;
                case 'S':
                    cout << "\nSearch for an employee\n";
                    searchEmployee();
                    break;
                case 'L':
                    cout << "\nListing employees\n";
                    listAll();
                    wait();
                    break;
                case 'Q':
                    cout << "Quit\n";
                default:
                    break;
            }
        }while(ch != 'Q');
    }
    void createNewEmployee() // This allows the Human Resources employee to add an employee to the database
    {
        std::fstream file; // Instantiate a file object
        int id, dept, ch;
        string name, pass, passTest;
        int count = 0;

        file.open(FILENAME, std::ios::in);  // Open the File
        if(!file.is_open()) // Checks to see if the file is open
        {
            std::cerr << "Error writing to " << FILENAME << endl; // Outputs an error message
            return; // Returns to the menu
        }
        // While loop sets the ID equal to the last id + 1.
        while(file>>id>>name>>pass>>dept) { 
            ++id;
        }
        // Ask the user for the input
        cout << "Enter the username: "; std::cin >> name;
        cout << "Enter the password: "; std::cin >> pass;
        cout << "Confirm the password: "; std::cin >> passTest;
        while(passTest != pass) // Runs if the user input the password incorrectly
        {
            cout << "\tPasswords don't match.\n"
                << "\tEnsure that you enter the \n"
                << "\tpassword correctly.\n"
                << "Try again." << endl;
            cout << "Enter the password: "; std::cin >> pass;
            cout << "Confirm the password: "; std::cin >> passTest;
        }
        cout << "Choose the department this employee will fall under: " << endl;
        cout << "1 - General Employee\n2 - Management Employee\n3 - Human Resources Employee" << endl;
        cout << "Your choice: "; std::cin >> ch;
        // Close the file and open in append mode 
        file.close();
        file.open(FILENAME, std::ios::app);
        switch(ch)  // Sets the department based on the selected one
        {
            case 1:
                dept = 1;
                file << id << " " << name << " " << pass << " " << dept << std::endl;
                cout << "Successfully saved to disk" << endl;
                break;
            case 2:
                dept = 2;
                file << id << " " << name << " " << pass << " " << dept << std::endl;
                cout << "Successfully saved to disk" << endl;
                break;
            case 3:
                dept = 3;
                file << id << " " << name << " " << pass << " " << dept << std::endl;
                cout << "Successfully saved to disk" << endl;
                break;
            default:
                cout << "Only choose either 1, 2, or 3.\nQuitting to Menu" << endl;
                break;
        }
    }
    void deleteEmployee() // Member function used to delete an employees details
    {
        std::ifstream file; // File object created
        std::list<HumanResources> employees; // A list of objects is created to store all the lines in the file.
        int id, dept, choice;
        string name, pass;
        file.open(FILENAME, std::ios::in);
        if(!file.is_open()) // Checks to see if the file is open
        {
            std::cerr << "Error writing to " << FILENAME << endl; // Outputs an error message
            return; // Returns to the menu
        }
        while(file>>id>>name>>pass>>dept) // Adding the objects to the list
        {
            employees.push_back(HumanResources(id, name, pass, dept));
        }
        file.close();

        cout << "Enter the ID of the user you want to delete" << endl;
        listAll();  // List all the empployees in the database for the user
        cout << "Your choice: "; std::cin >> choice;
        // While checks to see if the user entered his own ID
        while(choice == logID)
        {
            cout << "You cannot delete your own credentials, try again. " << endl; // Forces user to try a different ID
            listAll();
            cout << "Your choice: "; std::cin >> choice;
        }
        // Create a variable to iterate through the list
        auto itr = employees.begin();
        std::ofstream out(FILENAME); // Instantiate a file output object
        /* 
         * Loop that adds all the list members to the file, skipping the deleted one
         * and decrementing the ID of all the list items that follow.
         */
        while(itr != employees.end())
        {
            if(itr->mID == choice)
            {
                itr++;
                while(itr != employees.end())
                {
                    id--;
                    itr->mID = id;
                    out << itr->mID << " " << itr->mUser << " " << itr->mPass << " " << itr->mDept << std::endl;
                    itr++;
                }
                break;
            }
            out << itr->mID << " " << itr->mUser << " " << itr->mPass << " " << itr->mDept << std::endl;
            itr++;
        }
        out.close(); // close the file before ending the function
    }
    void modifyEmployee()
    {
        std::ifstream datafile; // File object created
        std::list<HumanResources> employees; // A list of objects is created to store all the lines in the file.
        int id, dept, choice;
        string name, pass;
        datafile.open(FILENAME, std::ios::in); // Open the file
        if(!datafile.is_open()) // Checks to see if the file is open
        {
            std::cerr << "Error writing to " << FILENAME << endl; // Outputs an error message
            return; // Returns to the menu
        }
        while(datafile>>id>>name>>pass>>dept) // Fill the list with the employees
        {
            employees.push_back(HumanResources(id, name, pass, dept));
        }
        datafile.close();
        cout << "Enter the ID of the user you want to modify" << endl;
        listAll();
        cout << "Your choice: "; std::cin >> choice;

        auto itr = employees.begin();
        std::ofstream out(FILENAME); // Create a output file that will overwrite the current one
        // Loop that finds the employee that the user wants to modify
        for(itr = employees.begin(); itr != employees.end();)
        {
            if(itr->mID == choice) // Once found, allows the user to modify the different details
            {
                cout << "Make your changes:" << endl;
                cout << "Employee number: " << itr->mID << endl;
                cout << "\nCurrent username: " << itr->mUser << endl;;
                cout << "Enter new: ";  std::cin >> itr->mUser;
                cout << "Current password: " << itr->mPass << endl;
                cout << "Enter new: ";  std::cin >> itr->mPass;
                cout << "Current department: ";

                if(itr->mDept == 1)
                    cout << "General Employee" << endl;
                else if(itr->mDept == 2)
                    cout << "Management" << endl;
                else if(itr->mDept == 3)
                    cout << "Human Resources" << endl;

                cout << "\nChoose a department\n 1 - General Employee\n 2 - Management\n 3 - Human Resources" << endl;
                cout << "Enter new: ";  std::cin >> itr->mDept;
            }
            out << itr->mID << " " << itr->mUser << " " << itr->mPass << " " << itr->mDept << std::endl;
            itr++;
        }
        out.close();
    }
    void searchEmployee() // Allows the user to search for a specific user by username or by id
    {
        std::ifstream file;
        HumanResources hrEmp; // Create a human resources object to hold the necessary data
        string username, password, searchUser;
        int id, searchID, dept, ch;
        bool correct = false;
        // Ask the user if he wants to search by id or username
        cout << "\nSearch for a user by username or id?" << endl;
        cout << "1 - Username\n2 - ID" << endl;
        cout << "Your choice: ";
        std::cin >> ch;
        file.open(FILENAME, std::ios::in);
        
        // Check what the user wants to search for
        if(ch == 1) // if the user wants to enter a username, then run this
        {
            cout << "Enter the username of the employee: "; std::cin >> searchUser;
            while(file>>id>>username>>password>>dept) // If the user is found then take the necessary data
            {
                if(searchUser == username)
                {
                    hrEmp.mID = id; hrEmp.mUser = username; 
                    hrEmp.mPass = password; hrEmp.mDept = dept;
                    correct = true;
                    break;
                }
            }
        }else if(ch == 2) // else if the user wants to search using an id, then this
        {
            cout << "Enter the employee ID of the employee: "; std::cin >> searchID;
            while(file>>id>>username>>password>>dept)
            {
                if(searchID == id) // If the user is found then take the necessary data
                {
                    cout << "User found, printing details." << endl;
                    hrEmp.mID = id; hrEmp.mUser = username; 
                    hrEmp.mPass = password; hrEmp.mDept = dept;
                    correct = true;
                    break;
                }
            }
        }else{ // if the choice doesn't exist then return to the menu
            std::cerr << "You didn't choose one of the possible choices.\nReturning to menu." << endl;
            return;
        }

        if(correct == false)
        {
            std::cerr << "The employee was not found" << endl;
            wait();
            return;
        }

        // Display the users details on the screen
        cout << "\nID - " << hrEmp.mID << endl
            << "Username - " << hrEmp.mUser << endl
            << "Departmant - ";

        if(hrEmp.mDept == 1)
            cout << "General Employee" << endl;
        else if(hrEmp.mDept == 2)
            cout << "Management" << endl;
        else if(hrEmp.mDept == 3)
            cout << "Human Resources" << endl;
        
        wait();
    }
    void listAll() // This function lists all the employees in the database
    {
        std::ifstream datafile(FILENAME);
        Employee employee;
        string username, password;
        int id, dept;
        while(datafile>>id>>username>>password>>dept)
        {
            std::cout << id << " - " << username << std::endl;
        }
        cout << std::endl;
        datafile.close();
    }
};

class Management : public Employee
{
public:
using Employee::Employee;
    void Menu()
    {
        char ch;
        do{
            cout << "\n** Management Employee **" << endl
                << "\tChoose an option" << endl
                << "L - List all employees" << endl
                << "S - Search for an employee" << endl
                << "Q - Quit this program\n" << endl
                << "\tYour choice: ";
            std::cin >> ch;
            ch = toupper(ch);
            switch(ch)
            {
                case 'L':
                    cout << "\nListing employees\n";
                    listAll();
                    break;
                case 'S':
                    cout << "\nSearching for a specific employee\n";
                    searchEmployee();
                    break;
                case 'Q':
                    cout << "Quit\n";
                default:
                    break;
                }
            }while(ch != 'Q');
    }
    void searchEmployee() // Allows the user to search for a specific user by username or by id
    {
        std::ifstream file;
        Management manEmp; // Create a human resources object to hold the necessary data
        string username, password, searchUser;
        int id, searchID, dept, ch;
        bool correct = false;
        // Ask the user if he wants to search by id or username
        cout << "\nSearch for a user by username or id?" << endl;
        cout << "1 - Username\n2 - ID" << endl;
        cout << "Your choice: ";
        std::cin >> ch;
        file.open(FILENAME, std::ios::in);
        
        // Check what the user wants to search for
        if(ch == 1) // if the user wants to enter a username, then run this
        {
            cout << "Enter the username of the employee: "; std::cin >> searchUser;
            while(file>>id>>username>>password>>dept) // If the user is found then take the necessary data
            {
                if(searchUser == username)
                {
                    manEmp.mID = id; manEmp.mUser = username; 
                    manEmp.mPass = password; manEmp.mDept = dept;
                    correct = true;
                    break;
                }
            }
        }else if(ch == 2) // else if the user wants to search using an id, then this
        {
            cout << "Enter the employee ID of the employee: "; std::cin >> searchID;
            while(file>>id>>username>>password>>dept)
            {
                if(searchID == id) // If the user is found then take the necessary data
                {
                    cout << "User found, printing details." << endl;
                    manEmp.mID = id; manEmp.mUser = username; 
                    manEmp.mPass = password; manEmp.mDept = dept;
                    correct = true;
                    break;
                }
            }
        }else{ // if the choice doesn't exist then return to the menu
            std::cerr << "You didn't choose one of the possible choices.\nReturning to menu." << endl;
            return;
        }

        if(correct == false)
        {
            std::cerr << "The employee was not found" << endl;
            wait();
            return;
        }

        // Display the users details on the screen
        cout << "\nID - " << manEmp.mID << endl
            << "Username - " << manEmp.mUser << endl
            << "Departmant - ";

        if(manEmp.mDept == 1)
            cout << "General Employee" << endl;
        else if(manEmp.mDept == 2)
            cout << "Management" << endl;
        else if(manEmp.mDept == 3)
            cout << "Human Resources" << endl;
        
        wait();
    }
    void listAll()
    {
        std::ifstream datafile(FILENAME);
        Employee employee;
        string username, password;
        int id, dept;
        while(datafile >> id >> username >> password >> dept)
        {
            std::cout << id << " - " << username << std::endl;
        }
        cout << std::endl;
        datafile.close();
        wait();
    }
};

class GenEmployee : public Employee
{
public:
using Employee::Employee;
    void Menu()
    {
        char ch;
        do{
            cout << "\n** General Employee Menu **" << endl
                << "Choose an option" << endl
                << "L - List your details" << endl
                << "Q - Quit the program\n" << endl
                << "\tYour choice: ";
                std::cin >> ch;
                ch = toupper(ch);
                switch(ch)
                {
                    case 'L':
                        searchEmployee();
                        break;
                    case 'Q':
                        cout << "Quit\n";
                    default:
                        break;
                }
            }while(ch != 'Q');
    }
    void searchEmployee()
    {
        GenEmployee genEmp;
        std::ifstream datafile; // File object created and file opened
        int id, dept;
        string name, pass;
        datafile.open(FILENAME, std::ios::in);
        // Save details in object
        while(datafile>>id>>name>>pass>>dept)
        {   
            if(id == logID){
                genEmp.mID = id; genEmp.mUser = name; 
                genEmp.mPass = pass; genEmp.mDept = dept;
                break;
            }
        }
        // Display employee details
        cout << "\nYour employee details are: " << endl
        << "ID - " << genEmp.mID << endl
        << "Username - " << genEmp.mUser << endl
        << "Departmant - General Employee" << endl
        << "Have a wonderful day" << endl;
        wait();
    }
};

int main()
{
    Employee emp;
    char choice;
    bool logState = false; // stores whether the user has logged in or not
    emp.fileCreation(emp);
    // loop that runs until the user has logged in
    do{
        cout << "======= Login Menu =======" << endl
            << "L - Login" << endl
            << "Q - Quit" << endl
            << "---------------------------" << endl;
        std::cin >> choice;
        choice = toupper(choice);
        switch(choice){
            case 'L':
                logState = emp.Login(logState, emp);
                
                if(logState == true)
                    break;

                cout << "You have not been logged in, try again." << endl;
                continue;
            case 'Q':
                cout << "Quitting!" << endl;
                break;
            default:
                cout << "Something went wrong." << endl;
            }
    }while(logState == false && choice != 'Q');

    departmentMenu();

    wait();
    return 0;
}

// Function that sorts the logged in user, into one of the three departments
void departmentMenu()
{
    std::ifstream file;
    int id, dept;
    string name, pass;
    HumanResources hrDept;
    Management manDept;
    GenEmployee empDept;

    file.open(FILENAME, std::ios::in);
    while(file>>id>>name>>pass>>dept)
    {
        if(id == logID)
            break;
    }
    // Only access the class for the department the user is in.
    if(dept == 1)
        empDept.Menu();
    else if(dept == 2)
        manDept.Menu();
    else if(dept == 3)
        hrDept.Menu();
    else 
        cout << "Something went wrong. Quitting." << endl; // if department is not found, then quit.

    file.close();
}

// Function that waits for the user to press enter
void wait()
{
    cout << "Press Enter to continue..." << std::endl;
    std::cin.ignore(10, '\n');
    std::cin.get();
}