#define MAX 1000

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <sstream>

using namespace std;

class User
{
private:
	string typeUser;
	string idUser;
	string passUser;
public:
	User();
	~User();
	int LogIn(string id, string pass);
	string checktype() {
		return typeUser;
	}
	string** getdata(const char* file, int& row, int& col);
	void setdata(string** s, const char* file, int row, int col);
	bool FindUser(string id, int& line);
	bool addMem();
	void removeMem(string id);
	void viewInfo(const char* file);
	void modiInfo(const char* file, string** s, int row, int col, int pos);
	void changePass();
	bool openCourse();
	void viewGPA(string courseID);
	void modiGPA();
	bool regisCourse();
	bool checkCourseOpen(string** s, int row, string idCourse, string idTeacher);
	int checkCourseFull(string** s, int row, string idUser, string idCourse, string idTeacher, string Sem);
	bool checkCredit(string** s1, string** s2, int row1, int row2, string idUser, string idCourse, string idTeacher, string Sem);
	bool removeCourse();
	bool AdminSearch();
	void AdminDelete(string* arr, int line);
	void FindCourseByName();
	void viewProfile();
	void note(string funct);
};
