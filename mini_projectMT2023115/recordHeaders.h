typedef struct Admin{
        int userID;
        char username[30];
        char password[10];
}Admin;

typedef struct Professor{
        int userID;
        char username[30];
        char password[10];
        char address[30];
        char loginID[20];
	char mail[30];
        int age;
	int courseID[4];
}Professor;


typedef struct Courses{
	int courseID;
	char name[40];
	int totalSeats;
	int credits;
	int noOfAvailableSeats;
	char department[30];
	int profUserID;
        int isRemoved;

}Courses;

typedef struct Student{
        int userID;
        int isAccessAllowed;
        char username[30];
        char password[10];
        char address[30];
        char loginID[20];
        int age;
	char mail[30];
	int courseID[4];
}Student;

