//citation: Julian Day Calculations (Gregorian Calendar). (n.d.). //https://quasar.as.utexas.edu/BillInfo/JulianDatesG.html 

#define _CRT_SECURE_NO_WARNINGS

#include	<iostream>
#include    <iomanip>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"


using namespace std;

bool ReadFile(string, List*);
bool DeleteRecord(List*, char*);
bool Display(List, int, int);
bool InsertBook(string, List*);
bool SearchStudent(List*, char* id, LibStudent&);
bool computeAndDisplayStatistics(List*);
bool printStuWithSameBook(List*, char*);
bool displayWarnedStudent(List*, List*, List*);
int menu();
bool isDuplicateId(List* list, const char* id);
void updateBookFile(const string& filePath, const string& content);
void printList(List*, ostream&, bool);
int getIntegerInput();
Date seperateDate(string);
int julianDay(Date, Date);

int main() {
	List studentlist;
	menu();
	cout << "\n\n";
	system("pause");
	return 0;
}

//check for duplicate id
bool isDuplicateId(List* list, const char* id) {
	Node* cur = list->head;

	while (cur != nullptr) {
		if (strcmp(cur->item.id, id) == 0) {
			return true; // Found a student with the same ID (duplicate)
		}
		cur = cur->next;
	}

	return false; // Student with the given ID not found (no duplicate)
}

//read student record
bool ReadFile(string filename, List* list) {
	ifstream inFile(filename);
	if (inFile.fail()) {
		cout << "Error opening file: " << filename << endl;
		return false;
	}

	// Temporary variables to store student information while reading
	char tempName[30];
	char tempId[10];
	char tempCourse[3];
	char tempPhone[10];

	string text;
	while (inFile.peek() != EOF && inFile >> text) {
		inFile.ignore(1000, '='); // Ignore characters until '=' is encountered
		inFile >> tempId; // Read student ID

		inFile.ignore(1000, '\n'); // Ignore the rest of the line

		inFile.ignore(1000, '='); // Ignore characters until '=' is encountered
		inFile.ignore(1); // Ignore the space after '='
		inFile.getline(tempName, 30); // Read student name

		inFile.ignore(1000, '='); // Ignore characters until '=' is encountered
		inFile.ignore(1); // Ignore the space after '='
		inFile.getline(tempCourse, 3); // Read course

		inFile.ignore(1000, '='); // Ignore characters until '=' is encountered
		inFile.ignore(1); // Ignore the space after '='
		inFile.getline(tempPhone, 10); // Read phone number

		// Check if the student name already exists in the linked list using the updated function
		LibStudent newStudent;
		strcpy_s(newStudent.name, tempName);
		strcpy_s(newStudent.id, tempId);
		strcpy_s(newStudent.course, tempCourse);
		strcpy_s(newStudent.phone_no, tempPhone);

		if (isDuplicateId(list, tempId)) {
			continue; // Skip this record and move to the next one
		}

		list->insert(newStudent);

	}
	inFile.close();
	cout << list->size() << " records has been succesfully read" << endl;
	return true;
}

void updateBookFile(const string& filePath, const string& content) {
	ofstream file(filePath);
	if (file.is_open()) {
		file << content;
		file.close();
	}
	else {
		cout << "Error opening file for writing." << endl;
	}
}


//delete student record by using student id
bool DeleteRecord(List* list, char* id) {
	if (list->empty()) {
		cout << "No student in the record." << endl;
		return false;
	}

	Node* cur = list->head;
	int position = 1;
	bool found = false;

	// Loop through the linked list to find the student with the given ID
	while (cur != nullptr) {
		if (strcmp(cur->item.id, id) == 0) {
			found = true;
			break;
		}
		cur = cur->next;
		position++;
	}

	if (!found) {
		return false;
	}

	// Remove the student from the list
	list->remove(position);

	// Update student.txt file without the deleted student
	ofstream studFile("student.txt");
	cur = list->head;

	while (cur != nullptr) {
		studFile << "Student Id = " << cur->item.id << endl;
		studFile << "Name = " << cur->item.name << endl;
		studFile << "course = " << cur->item.course << endl;
		studFile << "Phone Number = " << cur->item.phone_no << endl << endl;
		cur = cur->next;
	}
	studFile.close();

	// Update book.txt file
	ifstream bookFile("book.txt");
	string tempContent;

	char stuId[8];
	string line;

	while (bookFile >> stuId) {
		bookFile.ignore();
		bookFile.ignore();
		getline(bookFile, line);

		if (strcmp(stuId, id) != 0) {
			tempContent += string(stuId) + ("  " + line + "\n\n\n");
		}
	}

	bookFile.close();

	// Update book.txt with modified content
	updateBookFile("book.txt", tempContent);

	return true;
}

//This function is to search a student based on id
bool SearchStudent(List* list, char* id, LibStudent& stu) {

	Node* cur;
	if (list->empty()) {//empty list
		cout << "There is no student record.\n";
		return false;
	}
	else {
		//Transverse from the first node to until the last node
		cur = list->head;
		while (cur != NULL) {
			if (strcmp(cur->item.id, id) == 0) {//compare the id 
				stu = cur->item;//return the student information using stu
				cout << "Sucessfully found the student record!\n\n";
				return true;
			}
			cur = cur->next;
		}
		cout << "Unable to found the student id\n";
		return false;
	}

}

//print list fucntion needed by bool Display
void printList(List* list, ostream& out, bool displayBook)
{
	Node* cur;

	if (!list->empty()) {//ensure that list is not empty
		cur = list->head;   // strating to tranverse from first node
		for (int i = 1; i <= list->size(); i++) { //while list size is not reached
			out << endl;
			out << "\nSTUDENT" << i << endl;
			cur->item.print(out);       //print to file

			if (displayBook) {//when displayBook is true
				out << "\nBOOK LIST:" << endl;
				int TotalBook = cur->item.totalbook;
				for (int i = 0; i < TotalBook; i++) {
					out << "\nBook " << i + 1 << "\n" << endl;
					cur->item.book[i].print(out);   //print to file
				}
			}
			cur = cur->next;
			cout << endl;
			cout << string(80, '*') << endl;
		}
		out << string(80, '*') << endl;

	}
	else
	{
		cout << "Unable to print from empty list";
	}
}

//display student record or book info to screen or to file based on user's selection
bool Display(List list, int source, int detail) {
	// source: location to print output
	// detail: determine what to print

	if (list.empty())
	{
		cout << "\n\nCannot print from an empty list\n";
		return false;
	}
	if (source == 1) {//display to file
		ofstream out;

		if (detail == 1) {//display student info and book info
			out.open("student_booklist.txt");
			printList(&list, out, true);
			out << endl;
			cout << "\nSuccessfully display output to student_booklist.txt" << endl;
			cout << "\nSuccessfully display output" << endl;
		}
		else if (detail == 2) {//display student info only
			out.open("student_info.txt");
			printList(&list, out, false);
			out << endl;
			cout << "\nSuccessfully display output to student_info.txt" << endl;
			cout << "\nSuccessfully display output" << endl;
		}
		out.close();
	}
	else if (source == 2) { // display to screen

		if (detail == 1) {// display student info and book info
			printList(&list, cout, true);
			cout << endl;
			cout << "\nSuccessfully display output" << endl;

		}
		else if (detail == 2) {//display student info only
			printList(&list, cout, false);
			cout << endl;
			cout << "\nSuccessfully display output" << endl;
		}

	}
	return true;
}

//compute and display statistic regarding the overdue books and fine
bool computeAndDisplayStatistics(List* list) {
	Node* cur;
	const int courseNum = 5;//total number of course
	const char coursename[courseNum][3] = { "CS", "IA", "IB", "CN", "CT" };
	struct Statistics {
		int Nostudent = 0;
		int TBooksBorrow = 0;
		int TOverdueBooks = 0;
		double TOverdueFine = 0.0;
	};
	Statistics statistics[courseNum];//store the 5 course statistics in the Statistics struct

	if (list->empty()) {
		cout << "There is no student record.\n";
		return false;
	}
	else {
		cur = list->head;
		while (cur != NULL) {

			for (int i = 0; i < courseNum; i++) {
				if (strcmp(coursename[i], cur->item.course) == 0) {
					statistics[i].Nostudent += 1;
					statistics[i].TBooksBorrow += cur->item.totalbook;
					if (cur->item.total_fine > 0) {
						statistics[i].TOverdueFine += cur->item.total_fine;
					}
					for (int j = 0; j < cur->item.totalbook; j++) {
						if (cur->item.book[j].fine > 0) {
							statistics[i].TOverdueBooks += 1;
						}
					}
				}
			}
			cur = cur->next;
		}

		cout << string(115, 95) << endl;
		cout << "\nCourse" << setw(20) << "Number of Students" << setw(25) << "Total Books Borrowed" << setw(25) << "Total Overdue Books" << setw(25) << "Total Overdue Fine(RM)\n";
		cout << string(115, 95) << endl;
		for (int index = 0; index < courseNum; index++) {
			cout << setw(20) << left << coursename[index] << setw(20) << statistics[index].Nostudent << setw(25) << statistics[index].TBooksBorrow << setw(25);
			cout << statistics[index].TOverdueBooks << setw(25) << fixed << setprecision(2) << statistics[index].TOverdueFine << endl;
		}
		cout << string(115, 95) << endl;
		return true;
	}
}

//function to validate integer input 
int getIntegerInput() {

	int input;

	while (true) {

		cin >> input;

		if (cin.fail()) { // Check if input is not integer

			cin.clear(); // Clear error flags 

			cin.ignore(10000, '\n'); // Ignore invalid input 

			cout << "Invalid input.\n\nPlease enter an integer:"; //prompt user to enter integer input

		}

		else {

			cin.ignore(10000, '\n'); // Ignore any extra characters 

			return input;

		}

	}

}

//funciton to print student info who is borrowing the same book (same title)
bool printStuWithSameBook(List* list, char* callNum)
{
	if (list->empty())
	{
		cout << "\nUnable to print an empty list.\n";
		return false;
	}
	else
	{
		int studNum = 0;
		const int maxBook = 15;
		List stuSameBooklist;
		LibStudent student;
		Node* cur = list->head;
		while (cur != NULL) {
			student = cur->item;

			for (int i = 0; i < maxBook; i++) {
				//compare the callNum
				if (strcmp(student.book[i].callNum, callNum) == 0) {
					studNum += 1;
					//insert into stuSameBooklist
					stuSameBooklist.insert(student);
				}

			}
			cur = cur->next;
		}

		cout << "There are " << studNum << " student(s) that borrow the book with call number " << callNum << " as shown below:";
		cout << "\n\n";

		if (!stuSameBooklist.empty()) {//have student in the list
			Node* curstu = stuSameBooklist.head;
			while (curstu != NULL) {
				cout << "Student Id = " << curstu->item.id << endl;
				cout << "Name = " << curstu->item.name << endl;
				cout << "Course = " << curstu->item.course << endl;
				cout << "Phone Number = " << curstu->item.phone_no << endl;
				//to cout the date using the print function in Date struct

				cout << "Borrow Date: "; curstu->item.book[0].borrow.print(cout);
				cout << endl;
				cout << "Due Date: "; curstu->item.book[0].due.print(cout);
				cout << "\n\n";
				curstu = curstu->next;
			}
		}

		return true;
	}
}

//display type1 and type2 student who meets the critera
bool displayWarnedStudent(List* list, List* type1, List* type2)
{
	if (!list->empty())
	{
		Date currday = seperateDate("29/3/2020");  // current day
		cout << "The current date is:  ", currday.print(cout);
		cout << endl;

		LibStudent stud;

		Node* cur = list->head;

		while (cur != NULL)
		{
			stud = cur->item;
			int overdue_book = 0;
			if (stud.total_fine > 0) {
				for (int i = 0; i < stud.totalbook; i++) {
					if (stud.book[i].fine >= 5) {
						overdue_book++;
					}

				}
				if (overdue_book > 2) {
					type1->insert(stud);

					if (stud.total_fine > 50.00) {

						if (overdue_book == stud.totalbook) {
							type2->insert(stud);
							break;
						}
					}
					break;

				}





			}

			cur = cur->next;  //traverse to the next item
		}

		//display student list and book list for type1 and type2
		if (type1->empty()) {
			cout << "\nList for type 1 warning is empty" << endl;
		}
		else
		{
			cout << "\n\nWarning for type 1: " << endl;
			cout << string(80, '=');
			Node* cur1 = type1->head;
			int stuno = 0;

			while (cur1 != NULL)
			{
				int bookno = 0;
				cout << "\nStudent " << ++stuno;
				cur1->item.print(cout);
				cout << "\nBook list for overdue books: " << endl;
				int j = 0;
				while (j < 15) {// 15 is limit for books that can be borrowed
					if (cur1->item.totalbook == 0) // student did not borrow any book
						break; // exit from the loop 
					else  // student did borrow some books
					{
						if (cur1->item.book[j].fine > 0.00) // student have overdue books 
						{
							cout << "\nBook " << ++bookno << endl;
							cur1->item.book[j].print(cout);     // print details of book
						}
					}
					j++;
				}
				cout << endl;
				cout << string(80, '*');
				cur1 = cur1->next; // traverse to the next pointer
			}

		}

		if (type2->empty()) {
			cout << "\n\nList for type 2 warning is empty" << endl;
		}
		else
		{
			cout << "\n\nWarning for type 2: " << endl;
			cout << string(80, '=');
			Node* cur2 = type2->head;
			int stuno = 0;
			while (cur2 != NULL)
			{
				int bookno = 0;
				cout << "\nStudent " << ++stuno;
				cur2->item.print(cout);
				cout << "Book List for overdue books: " << endl;
				int k = 0;
				while (k < 15)    // 15 is limit for books that can be borrowed
				{
					if (cur2->item.totalbook == 0)//the student did not borrow anymore book
						break;
					else
					{
						//the book is overdue
						if (cur2->item.book[k].fine > 0.00)
						{
							cout << "\nBook " << ++bookno << endl;
							cur2->item.book[k].print(cout);
						}
					}
					k++;
				}
				cout << endl;
				cout << string(80, '*');
				cur2 = cur2->next;// traverse to the next pointer
			}
		}
		return true;
	}
	else
	{
		cout << "\nUnable to print an empty list.\n";
		return false;
	}
}

//need to include <string> //this function convert the string Date to int and store in Date struct
Date seperateDate(string date) {
	Date stDate;
	int pos[2] = { 0 };  //to save the position of '/'
	int loc = 0;
	for (int index = 0; index < date.length(); index++) {//to find the '/' position
		if (date[index] == '/') {
			pos[loc] = index;
			loc++;
		}
	}
	stDate.day = stoi(date.substr(0, pos[0]));//stoi is to convert string to int & get substr before the first '/'
	stDate.month = stoi(date.substr(pos[0] + 1, pos[1] - pos[0] - 1));
	stDate.year = stoi(date.substr(pos[1] + 1));

	return stDate;
}

//convert dates to julian date to find number of overdue dates
int julianDay(Date dueD, Date curD) {

	//initialization of variables for overdue date
	double A = 0, B = 0, C = 0, E = 0, F = 0, JulianDay1 = 0, Y = dueD.year, M = dueD.month, D = dueD.day; //Y,M,D represents year month and date 

	//initialization of variables for current date
	double A2 = 0, B2 = 0, C2 = 0, E2 = 0, F2 = 0, JulianDay2 = 0, Y2 = curD.year, M2 = curD.month, D2 = curD.day;//Y2,M2,D2 represents year month and date 

	//initilization for variables for difference of julian days 
	double JulianDiff = 0;

	//formula for conversion of julian day number for begigning of the date at 0 hours(greenwich time)
	// first julian days for overdue date 
	A = Y / 100;
	B = A / 4;
	C = 2 - A + B;
	E = 365.25 * (Y + 4716);
	F = 30.6001 * (M + 1);
	JulianDay1 = C + D + E + F - 1524.5;

	// second julian days for current date 
	A2 = Y2 / 100;
	B2 = A2 / 4;
	C2 = 2 - A2 + B2;
	E2 = 365.25 * (Y2 + 4716);
	F2 = 30.6001 * (M2 + 1);
	JulianDay2 = C2 + D2 + E2 + F2 - 1524.5;

	//find difference between the two julian days to obtain the total overdue days 
	JulianDiff = JulianDay2 - JulianDay1;


	return (int)JulianDiff; //return the difference of julian days for fine calculation purpose
}

//function to insert book info
bool InsertBook(string filename, List* list) {

	if (list->empty()) {
		cout << "No student found in the record.\n";
		return false;
	}
	else {
		char ID[10];
		string autname, borrowD, dueD;

		double fine = 0;
		LibStudent tempStud;
		ifstream in;
		in.open(filename);

		if (in.fail()) {
			cout << filename << " is not found!\n";
			return false;
		}
		else {
			string text;
			while (in >> text) {
				LibBook Book;

				//read every information from the file
				strncpy(ID, &text[0], 9);
				ID[9] = 0;
				in >> autname >> Book.title >> Book.publisher >> Book.ISBN >> Book.yearPublished >> Book.callNum >> borrowD >> dueD;

				//to split the '/' from the authname
				string author;
				int prev = 0, authorNo = 0;
				for (int i = 0; i < autname.length(); i++) {
					if (i != (autname.length() - 1)) {//if haven't reach to the end of the autname
						if (autname[i] == '/') {//found the '/'
							Book.author[authorNo] = new char[50];
							author = autname.substr(prev, i - prev);
							strcpy(Book.author[authorNo], &author[0]);
							authorNo++;//update the number of author
							prev = i + 1;
						}
					}
					else {//if reach to the end of the autname
						author = autname.substr(prev, i - prev + 1);
						Book.author[authorNo] = new char[50];
						strcpy(Book.author[authorNo], &author[0]);
						authorNo++;
					}

				}

				Date currentdate;
				Book.due = seperateDate(dueD);
				Book.borrow = seperateDate(borrowD);
				currentdate = seperateDate("29/3/2020");

				// assume current date is 29/3/2020, find the overdue days 
				int numoverdue = julianDay(Book.due, currentdate);

				//fine
				if (numoverdue > 0) {
					Book.fine = numoverdue * 0.5;
				}

				Node* cur = list->head;

				while (cur != NULL) {   //loop through every element in linked list to find student

					if (strcmp(cur->item.id, ID) == 0) {
						cur->item.totalbook++;
						cur->item.book[cur->item.totalbook - 1] = Book;
						cur->item.calculateTotalFine();
					}
					cur = cur->next;
				}
			}
			in.close();
		}

		cout << "Book inserted successfully to student list.\n";
		return true;
	}


}

//menu driven program
int menu() {
	int selection = 0, choice = 0, choice1 = 0, choice2 = 0, count = 0;
	int source = 0, detail = 0;
	string studId;
	List studentlist;

	do {
		system("CLS");
		cout << "Menu\n " << endl;
		cout << "1.Read File " << endl;
		cout << "2.Delete Record" << endl;
		cout << "3.Search student" << endl;
		cout << "4.Insert book" << endl;
		cout << "5.Display Output" << endl;
		cout << "6.Compute and Display Statistics" << endl;
		cout << "7.Student With Same Book" << endl;
		cout << "8.Display Warned Student" << endl;
		cout << "9.Exit" << endl;
		cout << "Enter your choice: ";
		selection = getIntegerInput();

		switch (selection)
		{
		case 1:
		{
			system("CLS");
			cout << "\nRead File\n";
			cout << string(80, '=') << endl;
			if (!ReadFile("student.txt", &studentlist))
				cout << "Fail to read file" << endl;
			system("pause");

			break;
		}

		case 2:
		{
			system("CLS");
			cout << "\nDelete Record\n";
			cout << string(80, '=') << endl;
			cout << "Note: Remember to read student record before continue\n" << endl;
			char id[10];

			do {
				cout << "\nEnter the student ID to delete: ";
				cin.getline(id, 10);

				if (strlen(id) < 7 || strlen(id) > 9 || strlen(id) == 0)	//max length for student Id is 9
				{
					cout << "\nInvalid studentId. Please type again.\n";
					cout << endl;
				}
			} while (strlen(id) < 7 || strlen(id) > 9 || strlen(id) == 0);

			cout << "Are you sure you want to delete student with ID " << id << "? (Y/N): ";// Confirm deletion with the user
			char confirm;
			cin >> confirm;
			do {
				if (toupper(confirm) == 'Y') {
					if (DeleteRecord(&studentlist, id)) {
						cout << "Student with ID " << id << " successfully deleted." << endl;
					}
					else {
						cout << "Student with ID " << id << " not found or failed to delete." << endl;
					}
				}
				else if (toupper(confirm) == 'N') {
					cout << "Deletion canceled." << endl;
				}
				else {
					cout << "Invalid input! Please enter again.." << endl;
				}
			} while (toupper(confirm) != 'Y' && toupper(confirm) != 'N');
			break;

		}

		case 3:
		{
			system("CLS");
			cout << "\nSEARCH STUDENT\n";
			cout << string(80, '=') << endl;
			cout << "Note: Remember to read student record before continue\n" << endl;
			string Id;
			do {
				cout << "Enter the student id : ";
				getline(cin, Id);

				if (Id.length() > 9 || Id.length() == 0)	//max length for student Id is 9
				{
					cout << "\nInvalid studentId. Please type again.\n";
				}
			} while (Id.length() > 9 || Id.length() == 0);

			LibStudent temp;
			if (!SearchStudent(&studentlist, &Id[0], temp)) {
				cout << "Cannot found the student!\n";

			}
			else
				temp.print(cout);
			system("pause");
			break;
		}
		case 4:
		{
			system("CLS");
			cout << "\nInsert book\n";
			cout << string(80, '=') << endl;
			if (!InsertBook("book.txt", &studentlist))
				cout << "Fail to insert book" << endl;
			system("pause");
			break;
		}
		case 5:

		{
			system("CLS");
			cout << "\nDisplay Output\n";
			cout << string(80, '=') << endl;
			cout << "Note: Remember to read student record before continue" << endl;
			cout << "Note: Remember to insert book record before continue\n" << endl;
			do
			{
				cout << "\nWhere do you want to display the output (1-File/2-Screen):  ";
				source = getIntegerInput();
				if (source != 1 && source != 2) {
					cout << "\nInvalid input!!!" << endl;
					cout << "\nPlease Try Again" << endl;
				}

			} while (source != 1 && source != 2);

			do
			{
				cout << "\nDo you want to display book list for every student (1-YES/2-NO): ";

				detail = getIntegerInput();

				if (detail != 1 && detail != 2) {
					cout << "\nInvalid input!!!" << endl;
					cout << "\nPlease Try Again" << endl;
				}

			} while (detail != 1 && detail != 2);

			if (!Display(studentlist, source, detail))
				cout << "Fail to display file" << endl;
			system("pause");
			break;
		}

		case 6:
		{
			system("CLS");
			cout << "\nCOMPUTE AND DISPLAY STATISTICS\n";
			cout << string(80, '=') << endl;
			cout << "Note: Remember to read student record before continue" << endl;
			cout << "Note: Remember to insert book record before continue\n" << endl;

			if (!computeAndDisplayStatistics(&studentlist))
				cout << "Fail to compute and display statistics for student record " << endl;
			else
				cout << "Printed successfully" << endl;

			system("pause"); break; }

		case 7:
		{
			system("CLS");
			cout << "\nSTUDENT WITH SAME BOOK\n";
			cout << string(80, '=') << endl;
			string num;
			cout << "Note: Remember to read student record before continue" << endl;
			cout << "Note: Remember to insert book record before continue\n" << endl;
			cout << endl;
			cout << "Enter the call number of the book : ";
			getline(cin, num);
			//check for invalid input
			while (num.length() >= 20 || num.length() == 0)
			{
				cout << "\nInvalid callNum!" << endl;
				cout << "Please type again." << endl;
				num = "";                                     // set num back to empty before readng new input
				cout << "\nEnter the call number of the book : ";
				getline(cin, num);
			}
			if (!printStuWithSameBook(&studentlist, &num[0]))
				cout << "There is no students in the record.\n";

			else
				cout << "Successfully printed.\n";
			system("pause");
			break;
		}
		case 8:
		{
			system("CLS");
			List type1;
			List type2;
			cout << "\nDISPLAY WARNED STUDENT\n";
			cout << string(80, '=') << endl;
			cout << "Note: Remember to read student record before continue" << endl;
			cout << "Note: Remember to insert book record before continue\n" << endl;

			if (!displayWarnedStudent(&studentlist, &type1, &type2))
				cout << "Fail to display warned student" << endl;
			system("pause");
			break;
		}

		}

	} while (selection != 9 && selection >= 1 && selection < 9);

	return 0;
}
