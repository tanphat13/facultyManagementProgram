#include "User.h"
// Constructor
User::User() {
	idUser = "";
	passUser = "";
}
//Destructor - Log out
User::~User() {
	idUser.clear();
	passUser.clear();
	typeUser.clear();
}
// Function to log in
int User::LogIn(string id, string pass) {
	idUser = id;
	passUser = pass;
	ifstream checkAcc;
	checkAcc.open("user.csv", ios::in);
	while (!checkAcc.eof()) {
		int i = 0;
		bool flag1 = true;
		string info;
		getline(checkAcc, info);
		while (info[i] != ',') {
			if (info[i] != id[i]) {
				flag1 = false;
				break;
			}
			else i++;
		}
		if (i != id.length()) flag1 = false;
		if (flag1 == false) continue;
		else {
			int j = 0;
			while (info[i + 1 + j] != ',') {
				if (info[i + 1 + j] != pass[j]) {
					return 0;
				}
				else j++;
			}
			if (j != pass.length()) return 0;
			int k = 0;
			while (i + j + k + 2 < info.length()) {
				k++;
			}
			typeUser = info.substr(i + j + 2, k);
			checkAcc.close();
			if (typeUser[0] == 'a') return 1;
			else if (typeUser[0] == 't') return 2;
			else if (typeUser[0] == 's') return 3;
		}
	}
	return 0;
}
// Function to take the data into a 2-dimensional array
string** User::getdata(const char* file, int& row, int& col) {
	ifstream readFile;
	string** s = new string * [MAX];
	for (int i = 0; i < MAX; i++) {
		s[i] = new string[MAX];
	}
	readFile.open(file, ios::in);
	row = 0;
	bool flag = false;
	int count = 0;
	while (!readFile.eof()) {
		bool blank = true;
		string data;
		getline(readFile, data);
		int k = 0, h = 0;
		col = 0;
		while (k < data.size()) {
			while (data[k] != ',') {
				k++;
				if (k == data.size()) break;
			}
			s[row][col] = data.substr(h, k - h);
			h = k + 1;
			k++;
			col++;
			if (flag == false) count++;
		}
		flag = true;
		for (int i = 0; i < count; i++) { // if there is no data in whole line, mean only commas in line, it will break
			if (s[row][i] != "") blank = false;
		}
		if (blank) break;
		row++;
	}
	col = count;
	readFile.close();
	return s;
}
// Function to write back data from array into file
void User::setdata(string** s, const char* file, int row, int col) {
	ofstream writeFile;
	writeFile.open(file, ios::out);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col - 1; j++) {
			writeFile << s[i][j] << ",";
		}
		writeFile << s[i][col - 1];
		if (i == row - 1) break;
		writeFile << endl;
	}
	writeFile.close();
	return;
}
// Function to find user by ID
bool User::FindUser(string id, int& line) {
	fstream user("user.csv", fstream::in | fstream::out);
	string s, temp;
	int size;
	line = 0;
	while (!user.eof()) {
		int i = 0;
		getline(user, s);
		while (s[i] != ',') {
			i++;
		}
		if (s.substr(0, i) == id) {
			user.close();
			return true;
		}
		line++;
	}
	cout << "The ID you input does not exist in the system" << endl;
	user.close();
	return false;
}
// Function to add new user
bool User::addMem() {
	string id, role;
	bool checkRole = false;
	cin.ignore();
	cout << "New ID: ";
	getline(cin, id);
	while (!checkRole) {
		cout << "role: ";
		getline(cin, role);
		if (role != "student" && role != "teacher") {
			cout << "Wrong type of user! Try again" << endl;
			continue;
		}
		checkRole = true;
	}
	string** s;
	const char* fileName;
	int row = 0, col = 0;
	s = getdata("user.csv", row, col);
	for (int i = 0; i < row; i++) {
		if (s[i][0] == id) {
			cout << "This user has already existed" << endl;
			return false;
		}
	}
	s[row][0] = id;
	s[row][1] = id;
	s[row][2] = role;
	setdata(s, "user.csv", row + 1, col);
	cout << "Add user successful" << endl;
	if (role == "student") s = getdata("student.csv", row, col);
	else if (role == "teacher") s = getdata("teacher.csv", row, col);
	string nameUser, dateofBirth, address, tele;
	cout << "Name of user: ";
	getline(cin, nameUser);
	if (nameUser == "") nameUser = "x";
	cout << "Date of Birth (dd/mm/yy): ";
	getline(cin, dateofBirth);
	if (dateofBirth == "") dateofBirth = "x";
	if (col == 6) {
		cout << "Telephone number: ";
		getline(cin, tele);
		if (tele == "") tele = "x";
	}
	cout << "Address: ";
	getline(cin, address);
	if (address == "") address = "x, x";
	string subAdd[5];
	int j = 0, k = 0, h = 0;
	while (k < address.size()) {
		while (address[k] != ',') {
			k++;
			if (k == address.size()) break;
		}
		subAdd[j] = address.substr(h, k - h);
		j++;
		h = k + 1;
		k++;
	}
	s[row][0] = id;
	s[row][1] = nameUser;
	s[row][2] = dateofBirth;
	if (col == 6) s[row][3] = "\"" + tele + "\"";
	for (int i = 0; i < j; i++) {
		s[row][col - j + i] = subAdd[i];
		if (i == 0) s[row][col - j + i] = "\"" + s[row][col - j + i];
		if (i == j - 1) s[row][col - j + i] = s[row][col - j + i] + "\"";
	}
	if (col == 6) setdata(s, "teacher.csv", row + 1, col);
	else setdata(s, "student.csv", row + 1, col);
	cout << "Add user's information successful!" << endl;
	delete s;
	return true;
}
// Function to remove user
void User::removeMem(string id) {
	string** arr1;
	string type;
	int userrow, usercol, line;
	line = 0;
	arr1 = getdata("user.csv", userrow, usercol);
	bool flag = true;
	do {
		if (id == "") {
			cout << "Input user's ID you want to remove: ";
			cin.ignore();
			getline(cin, id);
			if (!FindUser(id, line)) {
				cout << "Input again!!!" << endl;
				flag = false;
			}
		}
		else {
			FindUser(id, line);
		}
	} while (!flag);
	if (id[0] == 'S') type = "teacher";
	else type = "student";
	int mark0;
	for (int i = 0; i < userrow; i++) {
		if (id == arr1[i][0]) {
			mark0 = i;
			break;
		}
	}
	for (int i = 0; i < userrow - 1; i++) {
		if (i >= mark0) {
			for (int j = 0; j < usercol; j++) {
				arr1[i][j] = arr1[i + 1][j];
			}
		}
	}
	setdata(arr1, "user.csv", userrow - 1, usercol);
	cout << "Remove user successful!" << endl;
	string** narr;
	int row, col, mark;
	mark = 0;
	if (type == "teacher") {
		narr = getdata("teacher.csv", row, col);
		for (int i = 0; i < row; i++) {
			if (id == narr[i][0]) {
				mark = i;
				break;
			}
		}
		for (int i = 0; i < row - 1; i++) {
			if (i >= mark) {
				for (int j = 0; j < col; j++) {
					narr[i][j] = narr[i + 1][j];
				}
			}
		}
		setdata(narr, "teacher.csv", row - 1, col);
	}
	else if (type == "student") {
		narr = getdata("student.csv", row, col);
		for (int i = 0; i < row; i++) {
			if (id == narr[i][0]) {
				mark = i;
				break;
			}
		}
		for (int i = 0; i < row - 1; i++) {
			if (i >= mark) {
				for (int j = 0; j < col; j++) {
					narr[i][j] = narr[i + 1][j];
				}
			}
		}
		setdata(narr, "student.csv", row - 1, col);
	}
	cout << "Delete user's information successful!" << endl;
}
// Function to view information
void User::viewInfo(const char* file) {
	string** s;
	int row, col, pos = 1;
	s = getdata(file, row, col);
	for (int i = 0; i < row; i++) {
		if (s[i][0] == idUser) {
			for (int j = 0; j < col - 1; j++) {
				cout << s[i][j] << "(" << pos << ")" << '\t';
				pos++;
			}
			cout << s[i][col - 1] << "(" << pos << ")" << endl;
		}
		else
			continue;
	}
	bool choice;
	cout << "Do you want to modify your information ? Press 1 to modify or 0 to back to menu: ";
	cin >> choice;
	if (!choice) return;
	do {
		cout << "Choose the information you want to modify: ";
		cin >> pos;
		while (pos == 1) {
			cout << "You cannot modify your ID!" << endl;
			cout << "Choose again or 0 to back to menu: ";
			cin >> pos;
			if (pos == 0) return;
		}
		modiInfo(file, s, row, col, pos);
		note("Modify information");
		cout << "Do you want to modify other information ? Press 1 to continue or 0 to back to menu: ";
		cin >> choice;
	} while (choice);
	return;
}
// Function to modify information
void User::modiInfo(const char* file, string** s, int row, int col, int pos) {
	if (pos > col) return;
	int i;
	string temp, newInfo;
	cout << "Please input new information: ";
	cin.ignore();
	getline(cin, newInfo);
	for (i = 0; i < row; i++) {
		if (s[i][0] == idUser) {
			temp = s[i][pos - 1];
			break;
		}
	}
	if (temp[0] == '\"' && temp[temp.size() - 1] == '\"') newInfo = "\"" + newInfo + "\"";
	else if (temp[0] == '\"') newInfo = "\"" + newInfo;
	else if (temp[temp.size() - 1] == '\"') newInfo = " " + newInfo + "\"";
	s[i][pos - 1] = newInfo;
	setdata(s, file, row, col);
}
// Function for user to change password
void User::changePass() {
	string** s;
	int row, col, line;
	s = getdata("user.csv", row, col);
	string newpass;
	bool check = FindUser(idUser, line);
	cout << "Input new password for this user: ";
	cin.ignore();
	getline(cin, newpass);
	s[line][1] = newpass;
	setdata(s, "user.csv", row, col);
	cout << "Change password success!" << endl;
	delete s;
	return;
}
// Function to open course
bool User::openCourse() {
	string idCourse, nameCourse, numofCre, numofStu;
	cin.ignore();
	cout << "ID course: ";
	getline(cin, idCourse);
	cout << "Name course: ";
	getline(cin, nameCourse);
	cout << "Number of credit: ";
	getline(cin, numofCre);
	cout << "Number of student: ";
	getline(cin, numofStu);
	string** s;
	int row, col, count = 0;
	s = getdata("course.csv", row, col);
	for (int i = 0; i < row; i++) {
		if (s[i][0] == idCourse && s[i][1] == idUser) {
			cout << "This course has already existed!" << endl;
			return false;
		}
		if (s[i][1] == idUser) count++;
		else continue;
	}
	if (count >= 5) {
		cout << "You have already open 5 course!" << endl;
		return false;
	}
	s[row][0] = idCourse;
	s[row][1] = idUser;
	s[row][2] = nameCourse;
	s[row][3] = numofCre;
	s[row][4] = numofStu;
	setdata(s, "course.csv", row + 1, col);
	delete s;
	return true;
}
// Function to view GPA
void User::viewGPA(string courseID) {
	cin.ignore();
	cout << "ID course: ";
	getline(cin, courseID);
	string** s1, ** s2;
	int row1, col1, row2, col2;
	int line[MAX];
	int size = 0;
	s1 = getdata("grade.csv", row1, col1);
	s2 = getdata("student.csv", row2, col2);
	for (int i = 0; i < row1; i++) {
		if (s1[i][0] == courseID) {
			if (s1[i][2] == idUser) {
				line[size] = i;
				size++;
			}
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < row2; j++) {
			if (s1[line[i]][1] == s2[j][0]) {
				cout << s2[j][0] << '\t' << s2[j][1] << '\t' << s1[line[i]][3] << '\t' << s1[line[i]][4] << endl;
			}
		}
	}
}
// Function to moddify
void User::modiGPA() {
	string courseID;
	cin.ignore();
	cout << "ID course: ";
	getline(cin, courseID);
	string** s;
	int row, col, j = 0;
	int line[MAX];
	bool flag = true;
	string stuID, newGPA;
	s = getdata("grade.csv", row, col);
	for (int i = 0; i < row; i++) {
		if (s[i][0] == courseID) {
			if (s[i][2] == idUser) {
				line[j] = i;
				j++;
			}
		}
	}
	cout << "Modify mode:" << endl;
	do {
		bool check = false;
		cout << "ID student: ";
		getline(cin, stuID);
		cout << "new GPA score: ";
		getline(cin, newGPA);
		for (int i = 0; i < j; i++) {
			if (s[line[i]][1] == stuID) {
				s[line[i]][3] = newGPA;
				cout << s[line[i]][1] << '\t' << s[line[i]][3] << '\n';
				check = true;
			}
		}
		if (check == false) cout << "There is no student has ID " << stuID << endl;
		else {
			cout << "Modiied" << endl;
			setdata(s, "grade.csv", row, col);
		}
		cout << "Do you want to continue modifying ? Press 1 to continue or 0 to exit mode: ";
		cin >> flag;
		cin.ignore();
	} while (flag);
	delete s;
	return;
}
// Function to register course
bool User::regisCourse() {
	string** s1, ** s2;
	string idCourse, idTeacher, Sem;
	int row1, col1, row2, col2, size = 0;
	int line[MAX];
	s1 = getdata("course.csv", row1, col1);
	s2 = getdata("grade.csv", row2, col2);
	cout << "Semester: ";
	cin.ignore();
	getline(cin, Sem);
	stringstream geek(Sem);
	int SemInt = 0;
	geek >> SemInt;
	for (int i = 0; i < row1; i++) {
		if (s1[i][3] != "0") {
			line[size] = i;
			size++;
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < col1 - 1; j++) {
			cout << s1[line[i]][j] << '\t';
		}
		cout << s1[line[i]][col1 - 1] << endl;
	}
	int numOfStudent;
	bool openedCourse, LimitCre, SemChecker;
	cout << "Input ID course: ";
	getline(cin, idCourse);
	cout << "Input ID teacher: ";
	getline(cin, idTeacher);
	numOfStudent = checkCourseFull(s2, row2, idUser, idCourse, idTeacher, Sem);
	if (numOfStudent == -1) {
		cout << "You have already registered this course" << endl;
		cout << "Fait to register!!" << endl;
		return false;
	}
	for (int i = 0; i < row1; i++) {
		stringstream geek(s1[i][4]);
		int MaxStudent = 0;
		geek >> MaxStudent;
		if (s1[i][0] == idCourse && s1[i][1] == idTeacher && MaxStudent == numOfStudent) {
			cout << "This course is full!" << endl;
			cout << "Fail to register!!" << endl;
			return false;
		}
	}
	openedCourse = checkCourseOpen(s1, row1, idCourse, idTeacher);
	LimitCre = checkCredit(s1, s2, row1, row2, idUser, idCourse, idTeacher, Sem);
	if (!openedCourse || !LimitCre) {
		cout << "Fail to register!!" << endl;
		return false;
	}
	else {
		s2[row2][0] = idCourse;
		s2[row2][1] = idUser;
		s2[row2][2] = idTeacher;
		s2[row2][3] = "-1";
		s2[row2][4] = Sem;
		setdata(s2, "grade.csv", row2 + 1, col2);
		return true;
	}
	return false;
}
//Function to remove course
bool User::removeCourse() {
	int graderow, gradecol, courserow, coursecol;
	string** s1;
	s1 = getdata("grade.csv", graderow, gradecol);
	string** s2;
	s2 = getdata("course.csv", courserow, coursecol);
	int size = 0;
	int line[MAX];
	string sem;
	cout << "Input semester: ";
	cin.ignore();
	getline(cin, sem);
	for (int i = 0; i < graderow; i++) {
		if ((idUser == s1[i][1]) && (sem == s1[i][4])) {
			line[size] = i;
			size++;
		}
	}
	cout << "List of your registed course in this semester: " << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < courserow; j++) {
			if ((s1[line[i]][0] == s2[j][0]) && (s1[line[i]][2] == s2[j][1])) {
				cout << s2[j][0] << " " << s2[j][1] << " " << s2[j][2] << endl;
			}
		}
	}
	int mark = 0;
	string IDcourse, IDteacher;
	bool flag = false;
	do {
		cout << "Input course's ID you want to remove: ";
		getline(cin, IDcourse);
		cout << "Input teacher's ID of the course: ";
		getline(cin, IDteacher);
		for (int i = 0; i < size; i++) {
			if ((IDcourse == s1[line[i]][0]) && (IDteacher == s1[line[i]][2])) {
				if (s1[line[i]][3] != "-1") {
					cout << "You are not allowed to remove this course!!!" << endl;
					return false;
				}
				flag = true;
				break;
			}
		}
		if (!flag) cout << "Not match, input again!!!" << endl;
	} while (!flag);
	for (int i = 0; i < graderow; i++) {
		if ((IDcourse == s1[i][0])
			&& (idUser == s1[i][1])
			&& (IDteacher == s1[i][2])
			&& (sem == s1[i][4])) {
			mark = i;
			break;
		}
	}
	for (int i = 0; i < graderow - 1; i++) {
		if (i >= mark) {
			for (int j = 0; j < gradecol; j++) {
				s1[i][j] = s1[i + 1][j];
			}
		}
	}
	setdata(s1, "grade.csv", graderow - 1, gradecol);
	return true;
}
// Function to check the course is open or not
bool User::checkCourseOpen(string** s, int row, string idCourse, string idTeacher) {
	for (int i = 0; i < row; i++) {
		if (s[i][0] == idCourse && s[i][1] == idTeacher && s[i][3] == "0") {
			cout << "This course is not opened" << endl;
			return false;
		}
	}
	return true;
}
// Function to check the course is full or not
int User::checkCourseFull(string** s, int row, string idUser, string idCourse, string idTeacher, string Sem) {
	int countInt = 0; // Count number of student by integer number
	string countString; // variable to convert interger number
	for (int i = 0; i < row; i++) {
		/*stringstream geek(s[i][4]);
		int Sem = 0;
		geek >> Sem;*/
		if (s[i][0] == idCourse && s[i][2] == idTeacher && s[i][4] == Sem)  countInt++;
		if (s[i][0] == idCourse && s[i][1] == idUser && s[i][2] == idTeacher && s[i][4] == Sem) {
			return -1;
		}
	}
	return countInt;
}
// Function to check whether student registered 20 credit or not
bool User::checkCredit(string** s1, string** s2, int row1, int row2, string idUser, string idCourse, string idTeacher, string Sem) {
	int countCre = 0, size = 0;
	string countString;
	int line[MAX];
	for (int i = 0; i < row2; i++) {
		/*stringstream geek(s2[i][4]);
		int Sem = 0;
		geek >> Sem;*/
		if (s2[i][1] == idUser && Sem == s2[i][4]) {
			line[size] = i;
			size++;
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < row1; j++) {
			if (s1[j][0] == s2[line[i]][0] && s1[j][1] == s2[line[i]][2]) {
				stringstream geek(s1[j][3]);
				int Cre = 0;
				geek >> Cre;
				countCre += Cre;
			}
		}
	}
	if (countCre == 20) {
		cout << "You have already registed 20 Credit in this semester!" << endl;
		return false;
	}
	int countCreNew = countCre;
	for (int i = 0; i < row2; i++) {
		if (s1[i][0] == idCourse && s1[i][1] == idTeacher) {
			stringstream geek(s1[i][3]);
			int Cre = 0;
			geek >> Cre;
			countCreNew += Cre;
		}
	}
	if (countCreNew > 20) {
		cout << "You cannot registered this course due to exceeding the limit of credit!" << endl;
		cout << "Your number of currently registered credit is " << countCre << endl;
		return false;
	}
	return true;
}
// Function to access to view profile mode
void User::viewProfile() {
	string** grade;
	string** course;
	string sem;
	int graderow, gradecol, courserow, coursecol, choice;
	grade = getdata("grade.csv", graderow, gradecol);
	course = getdata("course.csv", courserow, coursecol);
	cout << "View profile mode: " << endl;
	do {
		cout << "0. Exit" << '\t' << "1. view GPA" << '\t' << "2. view registered course" << '\t' << "3. view course in the semester" << '\n';
		cout << "Choose task: ";
		cin >> choice;
		switch (choice) {
		case 1: { // view GPA
			cout << "Input semester you want to see GPA: ";
			cin.ignore();
			getline(cin, sem);
			for (int i = 0; i < graderow; i++) {
				if ((idUser == grade[i][1]) && (sem == grade[i][4])) {
					for (int j = 0; j < courserow; j++) {
						if (grade[i][0] == course[j][0] && grade[i][2] == course[j][1]) {
							cout << grade[i][0] << " " << course[j][2] << " GPA: " << grade[i][3] << endl;
						}
					}
				}
			}
			note("View GPA");
			break;
		}
		case 2: { // view registered course
			int size = 0;
			int line[MAX];
			string sem;
			cout << "Input semester: ";
			cin.ignore();
			getline(cin, sem);
			for (int i = 0; i < graderow; i++) {
				if ((idUser == grade[i][1]) && (sem == grade[i][4])) {
					line[size] = i;
					size++;
				}
			}
			cout << "List of your registed course in this semester: " << endl;
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < courserow; j++) {
					if ((grade[line[i]][0] == course[j][0]) && (grade[line[i]][2] == course[j][1])) {
						cout << course[j][0] << " " << course[j][1] << " " << course[j][2] << endl;
					}
				}
			}
			note("View registered course");
			break;
		}
		case 3: { // view course in semester
			string temp[200];
			bool flag;
			int k = 0;
			for (int i = 0; i < graderow; i++) {
				flag = true;
				if (sem == grade[i][4]) {
					for (int j = 0; j < 200; j++) {
						if (grade[i][0] + grade[i][2] == temp[j]) {
							flag = false;
						}
					}
					if (flag) {
						temp[k] = grade[i][0] + grade[i][2];
						k++;
					}
				}
			}
			cout << "Total courses in semester " << sem << " :" << endl;
			for (int i = 0; i < courserow; i++) {
				for (int j = 0; j < k; j++) {
					if (course[i][0] + course[i][1] == temp[j]) {
						cout << course[i][0] << " " << course[i][1] << " " << course[i][2];
						if (course[i][3] == "0") cout << " Course Closed";
						else if (course[i][3] != "0") cout << " Credits: " << course[i][3];
						cout << endl;
					}
				}
			}
			note("View course in semester");
			break;
		}
		default: {
			cout << "No task!" << endl;
			break;
		}
		}
	} while (choice != 0);
	note("Exit view profile mode");
}
// Function for admin delete user which be searched by name or address
void User::AdminDelete(string* arr, int size) {
	string IDdelete;
	int line;
	bool flag1, flag2;
	flag1 = false, flag2 = true;
	do {
		do {
			cout << "Input ID you want to remove: ";
			cin.ignore();
			getline(cin, IDdelete);
			if (!FindUser(IDdelete, line)) {
				cout << "Input again!!!" << endl;
				flag2 = false;
			}
		} while (!flag2);
		for (int i = 0; i < size; i++) {
			if (arr[i] == IDdelete) {
				flag1 = true;
				removeMem(IDdelete);
				break;
			}
		}
		if (!flag1) cout << "Not Match, input again!!!" << endl;
	} while (!flag1);
	note("Delete user have been found");
}
// Function for admin to search by name or address
bool User::AdminSearch() {
	string* arr = new string[MAX];
	string** student;
	string** teacher;
	int sturow, stucol, tearow, teacol, size = 0, line;
	string info;
	student = getdata("student.csv", sturow, stucol);
	teacher = getdata("teacher.csv", tearow, teacol);
	cout << "Please input the information you want to find: ";
	cin.ignore();
	getline(cin, info);
	for (int i = 0; i < sturow; i++) {
		for (int j = 0; j < stucol; j++) {
			if ((info == student[i][j]) || ("\"" + info == student[i][j]) || ((" " + info + "\"") == student[i][j])) {
				arr[size] = student[i][0];
				size++;
				for (int k = 0; k < stucol - 1; k++) {
					cout << student[i][k] << ",";
				}
				cout << student[i][stucol - 1] << endl;
			}
		}
	}
	for (int i = 0; i < tearow; i++) {
		for (int j = 0; j < teacol; j++) {
			if ((info == teacher[i][j]) || ("\"" + info == teacher[i][j]) || (" " + info + "\"" == teacher[i][j])) {
				arr[size] = teacher[i][0];
				size++;
				for (int k = 0; k < teacol - 1; k++) {
					cout << teacher[i][k] << ",";
				}
				cout << teacher[i][teacol - 1] << endl;
			}
		}
	}
	bool check;
	cout << "Do you want to delete a user you have found ? Press 1 to continue or 0 to exit: ";
	cin >> check;
	if (check == 1) {
		note("Search a user by information");
		AdminDelete(arr, size);
		return false;
	}
	return true;
}
// Function for student to find course by name
void User::FindCourseByName() {
	string** course;
	string name;
	int row, col;
	bool flag = false;
	course = getdata("course.csv", row, col);
	cout << "Input course's name you want to find: ";
	cin.ignore();
	getline(cin, name);
	cout << "Course List: " << endl;
	for (int i = 0; i < row; i++) {
		if (name == course[i][2]) {
			flag = true;
			for (int j = 0; j < col; j++) {
				cout << course[i][j] << " ";
			}cout << endl;
		}
	}
	if (!flag) {
		cout << "There is no course match with your search!!!" << endl;
		return;
	}
}
// Funtion to note change during execution
void User::note(string funct) {
	string** s;
	int row, col;
	s = getdata("Note.csv", row, col);
	s[row][0] = idUser;
	if (idUser == "") s[row][0] = "NULL";
	s[row][1] = funct;
	if (funct == "Stop program") {
		row++;
		s[row][0] = "----------";
		s[row][1] = "----------";
	}
	setdata(s, "Note.csv", row + 1, col);
}