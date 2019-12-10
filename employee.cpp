#include <iostream>
#include <vector>

using std::string;

class Employee
{
    private:
    string username, password, email;
    
    public:
    int Login(bool logincounter)
    {
        
    }
};

int main()
{
	char choice;
   	bool userLogin;
    Employee employees;
   	employees.Login(userLogin);
	do
	{
        if(userLogin = 1)
		{
            // Menu
		    std::cout << "******Menu******" << std::endl;
		    std::cout << "Enter your option\n"
			<< "1 - Add a new record\n"
			<< "2 - Search record for employee id\n"
			<< "4 - Display employees in a specific department\n"
			<< "5 - Display all employees\n"
			<< "6 - Update existing employee\n"
			<< "Q - Quit the program\n" 
			<< "*******************" << std::endl;
		    std::cin >> choice;
            
            switch (choice)
            {
                case '1':
                    "Add a new record\n";
                    break;
                case 'Q':
                    std::cout << "Quitting" << std::endl;
                default:
                    break;
            }
        }else if(userLogin != 1) {
            std::cout << "Please login" << std::endl;
        }
	} while (choice != 'Q');
    
    return 0;
}
