// DataStructureProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <json.hpp>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

using json = nlohmann::ordered_json;

struct Employee;

struct Time {
    int Day, Hour, Minute, Second;
};

struct Attendee {
    Employee* self;
    Attendee* next;
};

struct Appointment {
    char Title[50];
    Time StartingTime;
    int Duration;
    Attendee* ListOfAttendees;
    Appointment* next;
};


struct Employee {
    int UniqueID;
    char FirstName[30], LastName[30], EmailAddress[50];
    Appointment* Calendar;
    Employee* next;
    Employee* prev;
};

struct Company {
    Employee* Head, * Tail;
};

bool isEmpty_Company(Company* company) {
    return company->Head == NULL;
}

bool isEmpty_Appointments(Appointment* head) {
    return head == NULL;
}

bool isEmpty_Attendee(Attendee* head) {
    return head == NULL;
}

Company* Initialize_Company() {
    Company* w = new Company;
    w->Head = NULL;
    w->Tail = NULL;

    return w;
}

Attendee* Initialize_Attendee() {
    Attendee* head = new Attendee;
    head = NULL;

    return head;
}

Appointment* Initialize_Appointment() {
    Appointment* head = new Appointment;
    head = NULL;

    return head;
}

Employee* Create_Employee(string EmployeeInfo[])
{
    Employee* curr = new Employee;
    strcpy_s(curr->FirstName, EmployeeInfo[0].c_str());
    strcpy_s(curr->LastName, EmployeeInfo[1].c_str());
    strcpy_s(curr->EmailAddress, EmployeeInfo[2].c_str());

    stringstream ss(EmployeeInfo[3]);
    ss >> curr->UniqueID;

    return curr;
}

Company* Insert_Employee(Company* w, string EmployeeInfo[], Employee*& emp)
{

	Employee* curr = Create_Employee(EmployeeInfo);

	curr->next = NULL;
	curr->prev = NULL;
	curr->Calendar = Initialize_Appointment();

	//cout << "Inserting The Following: " << curr->FirstName << " , " << curr->LastName << " , " << curr->EmailAddress << endl;

	if (isEmpty_Company(w))
	{
		w->Head = curr;
		w->Tail = curr;
        emp = curr;
		return w;
	}
	w->Tail->next = curr;
	curr->prev = w->Tail;
	w->Tail = curr;
    emp = curr;
	return w;
}

Employee* Insert_Appointment(Employee* emp,json j)
{
    Appointment* app = new Appointment;
    
    app->Duration = j["duration"];
    app->StartingTime.Second = j["seconds"];
    app->StartingTime.Minute = j["minutes"];
    app->StartingTime.Hour = j["hours"];
    app->StartingTime.Day = j["day"];

    strcpy_s(app->Title, to_string(j["title"]).c_str());

    app->next = NULL;
    app->ListOfAttendees = Initialize_Attendee();

    if (isEmpty_Appointments(emp->Calendar))
    {
        emp->Calendar = app;
        return emp;
    }

    app->next = emp->Calendar;
    emp->Calendar = app;

    return emp;
}

Attendee* InsertFriend(Attendee* head, Employee* EmployeeSelf)
{
    Attendee* temp = new Attendee;
    

    temp->self = EmployeeSelf;
    temp->next = head;

    return temp;
}

void CreateRelation( Employee* ind, Employee* fri,Appointment* indC, Appointment* indf)
{
    indC->ListOfAttendees = InsertFriend(indC->ListOfAttendees, fri);
    indf->ListOfAttendees = InsertFriend(indf->ListOfAttendees, ind);
}

void displayAttendee(Appointment* a)
{
    Attendee* list = a->ListOfAttendees;

    while (list != NULL)
    {
        cout << list->self->FirstName << endl;;
        list = list->next;
    }
}



void display(Company* Company)
{
    if (isEmpty_Company(Company))
    {
        cout << "Company is emtpy";
        return;
    }
    Employee* cur = Company->Head;
    Appointment* curA = cur->Calendar;
    
    while (cur != NULL)
    {
        curA = cur->Calendar;
        cout << cur->FirstName << "," << cur->LastName << "," << cur->EmailAddress  << endl;
        

        while (curA != NULL)
        {
            cout << curA->Title << endl;
            displayAttendee(curA);
            curA = curA->next;
        }
        cur = cur->next;
        
    }
    
    return;
}



void manageAttendee2(Company* company, Employee* emp)
{
    Employee* temp = company->Head;
    

    while (temp->UniqueID != emp->UniqueID && temp !=NULL)
    {
        Appointment* tempC = temp->Calendar;
        while (tempC != NULL)
        {
            Appointment* tempE = emp->Calendar;
            while (tempE != NULL)
            {
                if (strcmp(tempC->Title, tempE->Title) == 0 )
                    CreateRelation(temp, emp, tempC, tempE);

                tempE = tempE->next;
            }
            tempC = tempC->next;
        }
        temp = temp->next;
    }
}

Company* Parse_Company()
{
    std::ifstream ifs("test.json");
    json j = json::parse(ifs);
    Company* company = Initialize_Company();

    for (auto it : j)
    {
        string emplo[4];

        emplo[0] = it["fname"];
        emplo[1] = it["lname"];
        emplo[2] = it["email"];
        emplo[3] = to_string(it["ID"]);
        Employee* emp = new Employee;

        company = Insert_Employee(company, emplo, emp);

        
        for (auto z : it["Appointment"])
        {
            cout << z << endl;
            emp = Insert_Appointment(emp, z);
        }
        cout << endl;
        manageAttendee2(company, emp);
    }

    
    //display(company);
    
    return company;
}

Employee* CheckIfEmployeeExists(Company* company, Employee* ind)
{

    Employee* cur = company->Head;
    while (cur != NULL)
    {
        if (cur->UniqueID == ind->UniqueID)
        {
            return cur;
        }
        cur = cur->next;
    }

}

void Employee_Carateristics(string EmployeeInfo[]) {

    cin.ignore();
    cout << "Give us the First name: ";
    getline(cin, EmployeeInfo[0]);
    cout << "Give us the Last name: ";
    getline(cin, EmployeeInfo[1]);
    cout << "Give us the email: ";
    cin >> EmployeeInfo[2];
    cout << "Give us the uniqueID: ";
    cin >> EmployeeInfo[3];
    cin.ignore();
    

}

Company* UserGivenEmp(Company* company)
{
    Employee* emp = new Employee;
    string emplo[4];

    Employee_Carateristics(emplo);
    company = Insert_Employee(company, emplo, emp);

    return company;
}

void* UserGivenEmpSearch(Company* company)
{
    Employee* emp = new Employee;
    string emplo[4];

    Employee_Carateristics(emplo);
    emp = Create_Employee(emplo);
    emp = CheckIfEmployeeExists(company, emp);
    
    if (emp == NULL)
    {
        cout << "Employee doesn't exist" << endl;
        return;
    }
    else
        cout << "Employee exists" << endl;

}

void DeleteApp(Employee* EmployeeSelf,Company* company) {

    Employee* curr = company -> Head;

    while (curr != NULL)
    {
        Appointment* list = curr->Calendar;

        while(list != NULL)
        {
            Attendee* temp = list->ListOfAttendees;
            Attendee* prev = NULL;

            if (temp != NULL && temp->self->UniqueID == EmployeeSelf->UniqueID)
            {
                list->ListOfAttendees = temp->next; 
                delete temp;          
                goto out;
            }

            else
            {
                while (temp != NULL && temp->self->UniqueID != EmployeeSelf->UniqueID)
                {
                    prev = temp;
                    temp = temp->next;
                }

                if (temp == NULL)
                    goto out;

                prev->next = temp->next;
                delete temp;

            }
            out:
            list = list->next;
        }

        curr = curr->next;
    }

}

void DeleteUserGivenEmp(Company* w)
{
    Employee* ind = new Employee;
    string emplo[5];

    Employee_Carateristics(emplo);
    ind = Create_Employee(emplo);
    ind = CheckIfEmployeeExists(w, ind);
    
    if (ind == NULL) 
    {
        cout << "Employee doesn't exist" << endl;
        return;
    }
    else
        DeleteApp(ind,w);
 

    if (ind->prev != NULL)
        ind->prev->next = ind->next;

    else
        w->Head = ind->next;


    if (ind->next != NULL)
    {
        ind->next->prev = ind->prev;
    }
    else
        w->Tail = ind->next;


    return;
}

Company* contructMeeting(Company* company)
{
    int counter = 0;
    string s;

    json j;
    cout << "Give us the title: " << endl;
    getline(cin, s);
    j["title"] = s;
    cin.ignore();
    cout << "Give us the day: " << endl;
    cin >> j["day"];
    cout << "Give us the hour: " << endl;
    cin >> j["hours"];
    cout << "Give us the minute: " << endl;
    cin >> j["minutes"];
    cout << "Give us the second: " << endl;
    cin >> j["seconds"];
    cout << "Give us the duration: " << endl;
    cin >> j["duration"];
    system("CLS");
    
    cout << "How many will attend this meeting (minimum 2)" << endl;
    cin >> counter;
    

    for (int i = 0; i < counter; i++)
    {
        retry:
        Employee* temp = new Employee;
        cin.ignore();
        cout << "-----------------------------" << endl;
        cout << "Enter the ID of the employee " << endl;
        
        cin >> temp->UniqueID;
        temp = CheckIfEmployeeExists(company, temp);

        if (temp == NULL)
        {
            cout << "Employee doesn't exist renter" << endl;
            goto retry;
        }
        
        else
        {
            temp = Insert_Appointment(temp, j);
            manageAttendee2(company, temp);
        }
    }
    return company;
}

void removeApp(Employee* emp,string title)
{
    title = '"' + title + '"';
    while (emp != NULL)
    {
        Appointment* temp = emp->Calendar;
        Appointment* prev = NULL;

        cout << title << endl;
        // If head node itself holds
        // the key to be deleted
        
            if (temp != NULL && strcmp(temp->Title, title.c_str()) == 0 )
            {
                emp->Calendar = temp->next; // Changed head
                delete temp;
                goto out;
               
            }

        else
        {  
                while (temp != NULL && strcmp(temp->Title, title.c_str()) != 0 )
                {

                    prev = temp;
                    temp = temp->next;

                }

            // If key was not present in linked list
            if (temp == NULL)
                goto out;

            // Unlink the node from linked list
            prev->next = temp->next;
            delete temp;

        }
        out:
        emp = emp->next;
    }
   
}

void removeAppbyDate(Employee* emp, int day)
{
    
    while (emp != NULL)
    {
        Appointment* tmp2 = emp->Calendar;
        
        // remove initial matching elements
        while (emp->Calendar && emp->Calendar->StartingTime.Day < day)
        {
            Appointment* tmp = emp->Calendar;
            emp->Calendar = emp->Calendar->next;
            free(tmp);
        }

        for (Appointment* current = emp->Calendar; current != NULL; current = current->next)
        {
            while (current->next != nullptr && current->next->StartingTime.Day < day)
            {
                Appointment* tmp = current->next;
                current->next = tmp->next;
                free(tmp);
            }
        }

        emp = emp->next;
    }

}

void deleteApp(Company* company)
{
    string title;
    cin.ignore();
    cout << "Enter the title of the app to delete: ";
    getline(cin, title);

    Employee* curr = company->Head;

    removeApp(curr, title);
    
}

void deleteAppbyDate(Company* company)
{
    int day;
    cin.ignore();
    cout << "Enter the day to delete before: ";
    cin >> day;

    Employee* curr = company->Head;

   removeAppbyDate(curr,day);

}

void outputFile(Company* company)
{
    Employee* curr = company->Head;
    json key3JSON = json::array();
    json j = json::object();
    json key2JSON = json::array();
    json j2 = json::object();
    
    

    while (curr != NULL)
    {
        Appointment* cal = curr->Calendar;


        j["ID"] = curr->UniqueID;
        j["fname"] = curr->FirstName;
        j["lname"] = curr->LastName;
        j["email"] = curr->EmailAddress;
        
        while (cal != NULL)
        {
            j2["title"] = cal->Title;
            j2["day"] = cal->StartingTime.Day;
            j2["hours"] = cal->StartingTime.Hour;
            j2["minutes"] = cal->StartingTime.Minute;
            j2["seconds"] = cal->StartingTime.Second;
            j2["duration"] = cal->Duration;
            key2JSON.push_back(j2);
            cal = cal->next;
        }
        
        j["Appointment"] = key2JSON;
        key2JSON.clear(); 
        key3JSON.push_back(j);
        curr = curr->next;
    }
    ofstream ofs("test2.json", std::ofstream::out);
    ofs << key3JSON.dump(2);
    ofs.close();
}

int main()
{
    Company* comp = new Company;
    

    contructMeeting(comp);
    deleteApp(comp);
    outputFile(comp);

    int x = 0; //to break menu

    comp = Parse_Company();

    int userInput;
    while (x == 0) {

        cout << endl << "Enter the number of what you want to do" << endl;
        cout << "1- Search for Employee in the list " << endl;
        cout << "2- Add Employee to the list " << endl;
        cout << "3- Delete Employee from the list " << endl;
        cout << "4- Add a meeting or construct a meeting" << endl;
        cout << "5- Delete a meeting using it's title" << endl;
        cout << "6- Delete all meeting before X day" << endl;
        cout << "7- Output the company to a new json file" << endl;
        cout << "8- Finish the program" << endl;
        cout << "9- Suggest a friend for individual" << endl << endl;
        cout << "10- Display all Employee in console" << endl << endl;
        cin >> userInput;

        switch (userInput)
        {
        case 1:
            UserGivenEmpSearch(comp);
            break;

        case 2:
            UserGivenEmp(comp);
            break;

        case 3:
            DeleteUserGivenEmp(comp);
            break;

        case 4:
            contructMeeting(comp);
            break;

        case 5:
            deleteApp(comp);
            break;

        case 6:
            deleteAppbyDate(comp);
            break;

        case 7:
            outputFile(comp);
            break;

        case 8:
            x = 1;
            break;

        case 9:
            //Not made yet
            break;

        case 10:
            display(comp);
            break;
        }
    }

    
}

