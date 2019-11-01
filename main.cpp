#include "User.h"

int main() {
	string id, pass;
	User userCur;
	bool Login = 1;
	do {
		int flag = 0;
		while (flag == 0) { // Log in
			cout << "Your ID: ";
			getline(cin, id);
			cout << "Password: ";
			getline(cin, pass);
			flag = userCur.LogIn(id, pass);
			if (flag == 0) { // Announce fail to access
				cout << "Wrong ID or Password! Please try again." << endl;
				continue;
			}
		}
		userCur.note("Log in");
		cout << "You are "; // Access Granted and announce type of user
		if (flag == 1) cout << "admin!" << endl;
		if (flag == 2) cout << "teacher!" << endl;
		if (flag == 3) cout << "student!" << endl;
		int choice;
		switch (flag) {
		case 1: { // Administrator
			do { // Do task here
				cout << "0. Log out" << endl
					<< "1. Change password" << endl
					<< "2. Find user by ID" << endl
					<< "3. Find user by name or address" << endl
					<< "4. Add new user" << endl
					<< "5. Remove a user" << endl;
				cout << "Choose the function: ";
				cin >> choice;
				switch (choice) {
				case 0: {
					userCur.note("Log out");
					userCur.~User();
					break;
				}
				case 1: {
					userCur.changePass();
					userCur.note("Change password");
					break;
				}
				case 2: {
					string id;
					int line;
					cout << "ID user: ";
					cin.ignore();
					getline(cin, id);
					userCur.FindUser(id, line);
					cout << "ID " << id << " is found at line " << line + 1 << endl;
					userCur.note("Find user by ID");
					break;
				}
				case 3: {
					bool check;
					check = userCur.AdminSearch();
					if (check) userCur.note("Search a user by information");
					break;
				}
				case 4: {
					bool check;
					check = userCur.addMem();
					if (check) userCur.note("Add new user");
					break;
				}
				case 5: {
					string id;
					userCur.removeMem(id);
					userCur.note("Remove user");
					break;
				}
				default: {
					cout << " No function! Please try again" << endl;
					break;
				}
				}
			} while (choice != 0);
			break;
		}
		case 2: { // Teacher
			do { // Do task here
				cout << "0. Log out" << endl
					<< "1. Change password" << endl
					<< "2. View/Modify information" << endl
					<< "3. Open course" << endl
					<< "4. Summarize score in course" << endl
					<< "5. Modified student in opened course" << endl;
				cout << "Choose the function: ";
				cin >> choice;
				switch (choice) {
				case 0: {
					userCur.note("Log out");
					userCur.~User();
					break;
				}
				case 1: {
					userCur.changePass();
					userCur.note("Change password");
					break;
				}
				case 2: {
					userCur.viewInfo("teacher.csv");
					userCur.note("View information");
					break;
				}
				case 3: {
					bool check;
					check = userCur.openCourse();
					if (check) userCur.note("Open new course");
					break;
				}
				case 4: {
					string courseID;
					userCur.viewGPA(courseID);
					userCur.note("Summarize score in course");
					break;
				}
				case 5: {
					userCur.modiGPA();
					userCur.note("Modify score");
					break;
				}
				default: {
					cout << " No function! Please try again" << endl;
					break;
				}
				}
			} while (choice != 0);
			break;
		}
		case 3: { // Student
			do { // Do task here
				cout << "0. Log out" << endl
					<< "1. Change password" << endl
					<< "2. View/Modify information" << endl
					<< "3. Register course" << endl
					<< "4. Remove couse" << endl
					<< "5. View profile" << endl;
				cout << "Choose the function: ";
				cin >> choice;
				switch (choice) {
				case 0: {
					userCur.note("Log out");
					userCur.~User();
					break;
				}
				case 1: {
					userCur.changePass();
					userCur.note("Change password");
					break;
				}
				case 2: {
					userCur.viewInfo("student.csv");
					userCur.note("View information");
					break;
				}
				case 3: {
					bool check;
					check = userCur.regisCourse();
					if (check) userCur.note("Register new course");
					break;
				}
				case 4: {
					bool check;
					check = userCur.removeCourse();
					if (check) userCur.note("Remove course");
					break;
				}
				case 5: {
					userCur.note("Access to view profile mode");
					userCur.viewProfile();
					break;
				}
				default: {
					cout << " No function! Please try again" << endl;
					break;
				}
				}
			} while (choice != 0);
			break;
		}
		}
		cin.ignore();
		cout << "Do you want to stop the program ? Input 1 to continue or 0 to stop: ";
		cin >> Login;
		cin.ignore();
	} while (Login);
	userCur.note("Stop program");
	return 0;
}