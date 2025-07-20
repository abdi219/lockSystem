#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <stdexcept>

using namespace std;

class Student
{
private:
  string name;
  int rollNo;
  float gpa;
  string address;
  string email;

public:
  Student() {}

  Student(string n, int r, float g, string a, string e)
  {
    if (r <= 0)
      throw invalid_argument("Roll number must br positive");
    if (g < 0.0 || g > 4.0)
      throw out_of_range("GPA must be between 0.0 and 4.0.");
    name = n;
    rollNo = r;
    gpa = g;
    address = a;
    email = e;
  }

  void display()
  {
    cout << "Name: " << name << endl;
    cout << "Roll No: " << rollNo << endl;
    cout << "GPA: " << gpa << endl;
    cout << "Address: " << address << endl;
    cout << "Email: " << email << endl;
  }

  int getRollNo()
  {
    return rollNo;
  }

  string getName()
  {
    return name;
  }

  string getAllData()
  {
    return name + "," + to_string(rollNo) + "," + to_string(gpa) + "," + address + "," + email + "\n";
  }
};

class User
{
protected:
  string username;
  string password;

public:
  virtual void menu(Student students[], int &size) = 0;
  string getUsername()
  {
    return username;
  }
};

class Admin : public User
{
public:
  Admin(string u, string p)
  {
    username = u;
    password = p;
  }

  bool login(string u, string p)
  {
    return (username == u && password == p);
  }

  void savePasswordToFile(string newPass)
  {
    ofstream fout("admin_pass.txt");
    fout << newPass;
    fout.close();
  }

  void changePassword()
  {
    string oldPass, newPass;
    cout << "Enter current password: ";
    cin >> oldPass;
    if (oldPass == password)
    {
      cout << "Enter new password: ";
      cin >> newPass;
      password = newPass;
      savePasswordToFile(newPass);
      cout << "Password changed successfully!" << endl;
    }
    else
    {
      cout << "Incorrect current password." << endl;
    }
  }

  void searchStudent(Student students[], int size)
  {
    int choice;
    cout << "Search by:\n1. Roll No\n2. Name\nEnter choice: ";
    cin >> choice;

    if (choice == 1)
    {
      int r;
      cout << "Enter Roll No: ";
      cin >> r;
      for (int i = 0; i < size; i++)
      {
        if (students[i].getRollNo() == r)
        {
          students[i].display();
          return;
        }
      }
      cout << "Not found." << endl;
    }
    else if (choice == 2)
    {
      string name;
      cout << "Enter Name: ";
      cin.ignore();
      getline(cin, name);
      for (int i = 0; i < size; i++)
      {
        if (students[i].getName() == name)
        {
          students[i].display();
          return;
        }
      }
      cout << "Not found." << endl;
    }
    else
    {
      cout << "Invalid choice." << endl;
    }
  }

  void saveToFile(Student students[], int size)
  {
    ofstream fout("students.txt");
    for (int i = 0; i < size; i++)
    {
      fout << students[i].getAllData();
    }
    fout.close();
    cout << "Data saved to file!" << endl;
  }

  void addStudent(Student students[], int &size)
  {
    if (size >= 10)
    {
      cout << "Can't add more students. Limit reached!" << endl;
      return;
    }

    string name, address, email;
    int roll;
    float gpa;

    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Roll No: ";
    cin >> roll;
    cout << "Enter GPA: ";
    cin >> gpa;
    cin.ignore();
    cout << "Enter Address: ";
    getline(cin, address);
    cout << "Enter Email: ";
    getline(cin, email);

    try
    {
      students[size++] = Student(name, roll, gpa, address, email);
      cout << "Student added successfully!\n";
    }
    catch (const exception &e)
    {
      cerr << "Error: " << e.what() << endl;
    }
  }

  void removeStudent(Student students[], int &size)
  {
    int roll;
    cout << "Enter Roll No to remove: ";
    cin >> roll;
    bool found = false;
    for (int i = 0; i < size; i++)
    {
      if (students[i].getRollNo() == roll)
      {
        found = true;
        for (int j = i; j < size - 1; j++)
        {
          students[j] = students[j + 1];
        }
        size--;
        cout << "Student removed successfully.\n";
        break;
      }
    }
    if (!found)
    {
      cout << "Student not found.\n";
    }
  }

  void updateStudent(Student students[], int size)
  {
    int roll;
    cout << "Enter Roll No to update: ";
    cin >> roll;

    for (int i = 0; i < size; i++)
    {
      if (students[i].getRollNo() == roll)
      {
        float gpa;
        string address, email;
        cout << "Enter new GPA: ";
        cin >> gpa;
        cin.ignore();
        cout << "Enter new Address: ";
        getline(cin, address);
        cout << "Enter new Email: ";
        getline(cin, email);

        students[i] = Student(students[i].getName(), roll, gpa, address, email);
        cout << "Student updated successfully!\n";
        return;
      }
    }
    cout << "Student not found.\n";
  }

  void viewAll(Student students[], int size)
  {
    for (int i = 0; i < size; i++)
    {
      cout << "------------------------" << endl;
      students[i].display();
    }
    cout << "------------------------" << endl;
  }

  void menu(Student students[], int &size) override
  {
    int choice;
    do
    {
      cout << "\nAdmin Menu:\n";
      cout << "1. View Students\n2. Search Student\n3. Change Password\n4. Save to File\n";
      cout << "5. Add Student\n6. Remove Student\n7. Update Student\n8. Exit\nEnter choice: ";
      cin >> choice;

      switch (choice)
      {
      case 1:
        viewAll(students, size);
        break;
      case 2:
        searchStudent(students, size);
        break;
      case 3:
        changePassword();
        break;
      case 4:
        saveToFile(students, size);
        break;
      case 5:
        addStudent(students, size);
        break;
      case 6:
        removeStudent(students, size);
        break;
      case 7:
        updateStudent(students, size);
        break;
      case 8:
        cout << "Exiting Admin Menu." << endl;
        break;
      default:
        cout << "Invalid choice." << endl;
      }
    } while (choice != 8);
  }
};

class Guest : public User
{
public:
  Guest()
  {
    username = "guest";
  }

  void searchStudent(Student students[], int size)
  {
    string name;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    for (int i = 0; i < size; i++)
    {
      if (students[i].getName() == name)
      {
        students[i].display();
        return;
      }
    }
    cout << "Not found." << endl;
  }

  void viewAll(Student students[], int size)
  {
    for (int i = 0; i < size; i++)
    {
      cout << "------------------------" << endl;
      students[i].display();
    }
    cout << "------------------------" << endl;
  }

  void menu(Student students[], int &size) override
  {
    int choice;
    do
    {
      cout << "\n👤 Guest Menu:\n";
      cout << "1. View Students\n2. Search Student\n3. Exit\nEnter choice: ";
      cin >> choice;

      switch (choice)
      {
      case 1:
        viewAll(students, size);
        break;
      case 2:
        searchStudent(students, size);
        break;
      case 3:
        cout << "Exiting Guest Menu." << endl;
        break;
      default:
        cout << "Invalid choice." << endl;
      }
    } while (choice != 3);
  }
};

class LockSystem
{
private:
  string correctPassword;
  Student students[10];
  int studentCount;
  Admin adminUser;
  Guest guestUser;

public:
  LockSystem(string pass)
      : correctPassword(pass), adminUser("admin", loadPasswordFromFile())
  {

    studentCount = 6;
    students[0] = Student("Abdullah Faisal", 062, 3.5, "Lahore", "abdi134@gmail.com");
    students[1] = Student("Hammad Tahir", 047, 3.8, "Karachi", "hammad122@gmail.com");
    students[2] = Student("Bisma Shahzad", 035, 3.2, "Islamabad", "bisma199@gmail.com");
    students[3] = Student("Abeeha Noor", 023, 3.9, "Rawalpindi", "abeeha235@gmail.com");
    students[4] = Student("Moosa Rashid", 016, 2.8, "Multan", "moosa835@gmail.com");
    students[5] = Student("Ali Ahmed", 034, 3.7, "Faisalabad", "ali143@gmail.com");
  }

  string loadPasswordFromFile()
  {
    ifstream fin("admin_pass.txt");
    string pass;
    getline(fin, pass);
    fin.close();
    return pass.empty() ? "1234" : pass;
  }

  void logAttempt(string user, bool success)
  {
    ofstream fout("log.txt", ios::app);
    fout << user << " - " << (success ? "Success" : "Failed") << endl;
    fout.close();
  }

  void start()
  {
    string role;
    cout << "Login as (admin/guest): ";
    cin >> role;

    if (role == "admin")
    {
      string u;
      cout << "Username: ";
      cin >> u;

      if (u != adminUser.getUsername())
      {
        cout << "Invalid username.\n";
        logAttempt(u, false);
        return;
      }

      int attempts = 3;
      while (attempts--)
      {
        string p;
        cout << "Password: ";
        cin >> p;

        if (adminUser.login(u, p))
        {
          logAttempt(u, true);
          adminUser.menu(students, studentCount);
          return;
        }
        else
        {
          cout << "Wrong password! Attempts left: " << attempts << endl;
          logAttempt(u, false);
        }
      }
      cout << "Too many wrong attempts! Locking system for 10 seconds..." << endl;
      Sleep(10000);
    }
    else if (role == "guest")
    {
      logAttempt("guest", true);
      guestUser.menu(students, studentCount);
    }
    else
    {
      cout << "Unknown role." << endl;
    }
  }
};

int main()
{
  LockSystem lock("1234");
  lock.start();
  return 0;
}