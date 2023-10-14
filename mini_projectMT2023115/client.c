#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "recordHeaders.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8093

int option,currUserID;
int globalUserId;


void attemptAdminLogin(int sock_fd);
void attemptProfessorLogin(int sock_fd);
void attemptStudentLogin(int sock_fd);

void add_student(int sock_fd);
void displayStudentDetails(int sock_fd);
void addFaculty(int sock_fd);
void displayFacultyDetails(int sock_fd);
void activateStudent(int sock_fd);
void blockStudent(int sock_fd);
void modifyStudentDetails(int sock_fd);
void modifyFacultyDetails(int sock_fd);

void displayOfferedCourses(int sock_fd);
void addNewCourse(int sock_fd, int profID);
void removeCourse(int sock_fd);
void updateCourse(int sock_fd);
void changePassword(int sock_fd, int id);

void display_all_courses(int sock_fd);
void enroll_to_course(int sock_fd);
void drop_course(int sock_fd);
void display_enrolled_course_details(int sock_fd);
void showMenu(int sock_fd);




void chooseOption(int sock_fd){

	printf(" ---------------WELCOME BACK TO ACADEMIA :: COURSE REGISTRATION----------------\n");
        printf("|Choose an option:                                                             |\n");
	printf("| 1 : Admin Login                                                              |\n");
	printf("| 2 : Professor Login                                                          |\n");
	printf("| 3 : Student Login                                                            |\n");
        printf(" ------------------------------------------------------------------------------\n");

	printf("Your choice: ");
	scanf("%d",&option);

	//printf("Option : %d\n",option);
	//option=option-2608;

	switch(option){
		case 1 :
			attemptAdminLogin(sock_fd);
			break;
		case 2 :
			attemptProfessorLogin(sock_fd);
			break;
		case 3 :
			attemptStudentLogin(sock_fd);
			break;
		default :
			printf("Invalid option!!\n\n");
			//chooseOption(sock_fd);
			break;
	}
	return;
}




// void attemptStudentLogin(int sock_fd){
// 	bool result;
// 	Student currUser;

// 	printf("Student ID : ");
// 	scanf("%d",&currUser.userID);

// 	write(1,"Password : ",sizeof("Password : "));
// 	scanf("%s",currUser.password);


// 	//to the server
// 	write(sock_fd,&option,sizeof(int));
// 	write(sock_fd,&currUser,sizeof(Student));

// 	read(sock_fd,&result,sizeof(result)); //from the server

// 	if(!result){
// 		write(1,"Invalid login!!\n\n",sizeof("Invalid login!!\n\n"));
// 		chooseOption(sock_fd);
// 	}
// 	else{
// 		write(1,"Succesfully logged in!!\n\n",sizeof("Succesfully logged in!!\n\n"));
// 	}
// }


void attemptStudentLogin(int sock_fd){
        int result; 
        Student currUser;

        printf("Student ID : ");
        scanf("%d",&currUser.userID);
	
	currUserID=currUser.userID;

        printf("Password : ");
	// getchar();
	// fgets(currUser.password, sizeof(currUser.password), stdin);
        fflush(stdin);
        scanf("%s",currUser.password);

	printf("%s\n", currUser.password);
	fflush(stdout);
	printf("Option for server %d\n",option);

        //to the server
        write(sock_fd,&option,sizeof(sock_fd));
        write(sock_fd,&currUser,sizeof(Student));

        read(sock_fd,&result,sizeof(result)); //from the server


        if(result == 0)
        {
                printf("Invalid login!!\n\n");
                chooseOption(sock_fd); 
        }
        else if(result == 1)
        {
                printf("Succesfully logged in!!\n\n");
                globalUserId = currUser.userID;
                showMenu(sock_fd);
        }
        else if(result == 2)
        {
                printf("You are blocked....Kindly contact the administrator\n\n");
        }
        else
        {
                changePassword(sock_fd,globalUserId);
        }
        return;
}


void attemptProfessorLogin(int sock_fd){
        bool result; 
        Professor currUser;

        write(1,"Professor ID : ",sizeof("Professor ID : "));
        scanf("%d",&currUser.userID);

        write(1,"Password : ",sizeof("Password : "));
        fflush(stdin);
        scanf("%s",currUser.password);

        //to the server
        write(sock_fd,&option,sizeof(int));
        write(sock_fd,&currUser,sizeof(Professor));

        read(sock_fd,&result,sizeof(result)); //from the server

        if(!result){
                write(1,"Invalid login!!\n\n",sizeof("Invalid login!!\n\n"));
                chooseOption(sock_fd);
        }
        else{
                write(1,"Succesfully logged in!!\n\n",sizeof("Succesfully logged in!!\n\n"));
                globalUserId = currUser.userID;
                showMenu(sock_fd);
        }
        return;
}



void attemptAdminLogin(int sock_fd){
        bool result; 
        Admin currUser;

        printf("Admin ID : ");
        fflush(stdin);
        scanf("%d",&currUser.userID);


        printf("Password : ");
	fflush(stdin);
        scanf("%s",currUser.password);

	printf("%s\n", currUser.password);
	fflush(stdout);
	printf("Option for server %d",option);



        printf("selected userid %d\n",currUser.userID);

        //to the server
        write(sock_fd,&option,sizeof(sock_fd));
        write(sock_fd,&currUser,sizeof(Admin));

        read(sock_fd,&result,sizeof(result)); //from the server


        if(!result){
                printf("Invalid login!!\n\n");
                chooseOption(sock_fd); 
        }
        else{
                printf("Succesfully logged in!!\n\n");
                globalUserId = currUser.userID;
                showMenu(sock_fd);
        }

        return;
}





void showMenu(int sock_fd){
	int select;
	if(option==1){
		printf(" ...............WELCOME TO ADMIN MENU..................\n");
		printf("| 1 : Add Student                                      |\n");
		printf("| 2 : View Student Details                             |\n");
		printf("| 3 : Add Faculty                                      |\n");
		printf("| 4 : View Faculty Details                             |\n");
		printf("| 5 : Activate Student                                 |\n");
		printf("| 6 : Block Student                                    |\n");
		printf("| 7 : Modify Student Details                           |\n");
		printf("| 8 : Modify Faculty Details                           |\n");
		printf("| 9 : Logout and Exit                                  |\n");
		
		printf(" ......................................................\n");

		printf("Choose an option : ");
		scanf("%d",&select);
		printf("Option : %d\n",select);
                write(sock_fd, &select, sizeof(select));

		switch(select){
		case 1 :
			add_student(sock_fd);
			break;
		case 2 :
                        
                        displayStudentDetails(sock_fd);
			break;
		case 3 :
			addFaculty(sock_fd);
			break;
		case 4 :
			displayFacultyDetails(sock_fd);
			break;
		case 5 :
			activateStudent(sock_fd);
			break;
		case 6 :
			blockStudent(sock_fd);
			break;
		case 7 :
			modifyStudentDetails(sock_fd);
			break;
		case 8 :
			modifyFacultyDetails(sock_fd);
			break;
		case 9 :
			write(sock_fd,&select,sizeof(int));
			printf("Thank you\n");
			exit(0);
		default :
			printf("Invalid option!!\n\n");
			showMenu(sock_fd);
			break;
		}
	}
	else if(option==2){
		printf(" ...............WELCOME TO FACULTY MENU..................\n");
		printf("| 1 : View Offering Courses                              |\n");
		printf("| 2 : Add New Course                                     |\n");
		printf("| 3 : Remove Course From Catalog                         |\n");
		printf("| 4 : Update Course Details                              |\n");
		printf("| 5 : Change Password                                    |\n");
		printf("| 6 : Logout and Exit                                    |\n");
		printf(" ........................................................\n");

		printf("Choose an option : ");
		scanf("%d",&select);
		printf("Option : %d\n",select);
                write(sock_fd, &select, sizeof(select));

		switch(select){
		case 1 :
			displayOfferedCourses(sock_fd);
			break;
		case 2 :
			addNewCourse(sock_fd,globalUserId);
			break;
		case 3 :
			removeCourse(sock_fd);
			break;
		case 4 :
			updateCourse(sock_fd);
			break;
		case 5 :
			changePassword(sock_fd, globalUserId);
			break;
		case 6 :
			write(sock_fd,&select,sizeof(int));
			printf("Thank you\n");
			exit(0);
		default :
			printf("Invalid option!!\n\n");
			showMenu(sock_fd);
			break;
		}
	}
	else{
		printf(" ...............WELCOME TO STUDENT MENU..................\n");
                printf("| 1 : View All Courses                                   |\n");
                printf("| 2 : Enroll New Course                                  |\n");
                printf("| 3 : Drop Course                                        |\n");
                printf("| 4 : View Enrolled Course Details                       |\n");
                printf("| 5 : Change Password                                    |\n");
                printf("| 6 : Logout and Exit                                    |\n");
                printf(" ........................................................\n");

		printf("Choose an option : ");
                scanf("%d",&select);
                printf("Option : %d\n",select);
                write(sock_fd, &select, sizeof(select));

                switch(select){
                case 1 :
                        display_all_courses(sock_fd); //poori course file display karo
                        break;
                case 2 :
                        enroll_to_course(sock_fd);
                        break;
                case 3 :
                        drop_course(sock_fd);
                        break;
                case 4 :
                        display_enrolled_course_details(sock_fd);
                        break;
                case 5 :
                        changePassword(sock_fd,globalUserId);
                        break;
                case 6 :
                        write(sock_fd, &select,sizeof(int));
                        printf("Thank you\n");
                        exit(0);
                default :
                        printf("Invalid option!!\n\n");
                        showMenu(sock_fd);
                        break;
		}
	}
}


//*************Admin related functions***************


void add_student(int sock_fd)
{
   Student user;
   bool result;
   int select = 1;

   write(1, "Enter student name : ",sizeof("Enter student name : "));
   fflush(stdin);
   scanf("%s",user.username);
   
   

   write(1, "Enter student age : ",sizeof("Enter student age : "));
   scanf("%d",&user.age);
   getchar();

   write(1, "Enter student address : ",sizeof("Enter student address : "));
   fflush(stdin);
   scanf("%s",user.address);
   

   write(1, "Enter student mail : ",sizeof("Enter student mail : "));
   fflush(stdin);
   scanf("%s",user.mail);
   

   write(1, "Enter student LoginId : ",sizeof("Enter student LoginId : "));
   fflush(stdin);
   scanf("%s",user.loginID);

   user.isAccessAllowed = 1;
   strcpy(user.password,"iiitb");
   

   //send to server
   write(sock_fd, &user, sizeof(Student));

   //from server
   read(sock_fd, &user,sizeof(Student));
   read(sock_fd, &result, sizeof(result));

   if(result)
   {
	write(1, "Student added successfully ", sizeof("Student added successfully "));
        
	printf("Student id generated is %d\n", user.userID);
   }
   else
   {
	write(1, "Student cannot be added *** ", sizeof("Student cannot be added ***"));
   }
   showMenu(sock_fd);
}



void displayStudentDetails(int sock_fd)
{
   int id;
   Student user;

   printf("Enter student ID : ");
   fflush(stdin);
   scanf("%d", &id);
   
   //send to server
   write(sock_fd, &id, sizeof(id));

   //from server
   read(sock_fd, &user, sizeof(user));

   printf("Student name      : %s\n",user.username);
   printf("Student age       : %d\n",user.age);
   printf("Student address   : %s\n",user.address);
   printf("Student mail      : %s\n",user.mail);
   printf("Student LoginId   : %s\n",user.loginID);
   
}




void addFaculty(int sock_fd)
{
        Professor user;
        bool result;

        printf("Enter faculty name : ");
        fflush(stdin);
        scanf("%s",user.username);
        

        printf("Enter faculty age : ");
        fflush(stdin);
        scanf("%d", &user.age);

        printf("Enter faculty address : ");
        fflush(stdin);
        scanf("%s",user.address);


        printf("Enter faculty mail : ");
        fflush(stdin);
        scanf("%s",user.mail);
        

        printf("Enter faculty LoginId : ");
        fflush(stdin);
        scanf("%s",user.loginID);

        user.courseID[0] = -1;
        user.courseID[1] = -1;
        user.courseID[2] = -1;
        user.courseID[3] = -1;

        //send to server
        write(sock_fd, &user, sizeof(user));

        //from server
        read(sock_fd, &user,sizeof(user));
        read(sock_fd, &result, sizeof(result));
        if(result)
        {
                write(1, "Faculty added successfully ", sizeof("Faculty added successfully "));
                printf("Faculty id generated is %d ", user.userID);
        }
        else
        {
                write(1, "Faculty cannot be added *** ", sizeof("Faculty cannot be added ***"));
        }
        showMenu(sock_fd);
}



void displayFacultyDetails(int sock_fd)
{
        int id;
        Professor user;
        printf("Enter faculty ID : ");
        scanf("%d", &id);

        //send to server
        write(sock_fd, &id, sizeof(id));

        //from server
        read(sock_fd, &user, sizeof(user));

        printf("Faculty name      : %s\n",user.username);
        printf("Faculty age       : %d\n",user.age);
        printf("Faculty address   : %s\n",user.address);
        printf("Faculty mail      : %s\n",user.mail);
        printf("Faculty LoginId   : %s\n",user.loginID);
        
        for(int i=0; i<4; i++)
        {
                printf("Course id: %d\n",user.courseID[i]);
        }

}


void activateStudent(int sock_fd)
{
        int id;
        int status;
        int check;
        Student user;
        printf("Enter student ID : ");
        scanf("%d",&id);

        //send to server
        write(sock_fd, &id, sizeof(id));

        //from server
        read(sock_fd, &check, sizeof(check));
        read(sock_fd, &user, sizeof(user));
        read(sock_fd, &status, sizeof(status));

        if(check == -1)
        {
                printf("This operation failed !!!....Try again....\n");
                return;
        }


        if(status == 1)
        {
                printf("Student with userID %d was already in active state.\n",user.userID);
        }
        else if(user.isAccessAllowed == 1)
        {
                printf("Student with userID %d has been activated.\n",user.userID);
        }
     
}


void blockStudent(int sock_fd)
{
        int id;
        int status;
        int check;
        Student user;
        printf("Enter student ID : ");
        scanf("%d",&id);

        //send to server
        write(sock_fd, &id, sizeof(id));

        //from server
        read(sock_fd, &check, sizeof(check));
        read(sock_fd, &user, sizeof(user));
        read(sock_fd, &status, sizeof(status));

        
        if(check == -1)
        {
                printf("This operation failed !!!....Try again....\n");
                return;
        }

        if(status ==1)
        {
                printf("Student with userID %d is already in blocked state.\n",user.userID);
        }
        else if(user.isAccessAllowed == 0)
        {
                printf("Student with userID %d has been blocked.\n",user.userID);
        }
       
}


void modifyStudentDetails(int sock_fd)
{
        int id;
        int choice;
        int check;
        Student user;
        char tempName[30], tempAddress[30], tempMail[30];
	int tempAge;
        printf("Enter the student ID : \n");
        scanf("%d",&id);

        printf("Enter the field number which you want to modify : \n");
        printf("1. Name\n");
        printf("2. Address\n");
        printf("3. Mail\n");
        printf("4. Age\n");

        fflush(stdin);

        scanf("%d",&choice);

        //send to server
        write(sock_fd, &id, sizeof(id));
        write(sock_fd, &choice, sizeof(choice));

        switch(choice)
        {
                case 1:
                        fflush(stdin);
                        scanf("%s",tempName);
                        write(sock_fd, &tempName, sizeof(tempName));
                        break;
                case 2:
                        fflush(stdin);
                        scanf("%s",tempAddress);
                        write(sock_fd, &tempAddress, sizeof(tempAddress));
                        break;
                case 3:
                        fflush(stdin);
                        scanf("%s",tempMail);
                        write(sock_fd, &tempMail, sizeof(tempMail));
                        break;
                case 4:
                        fflush(stdin);
                        scanf("%d",&tempAge);
                        write(sock_fd, &tempAge, sizeof(tempAge));
                        break;
                default:
                        printf("Wrong choice\n");
                        break;
        }

        // //send to server
        // write(sock_fd, &id, sizeof(id));
        // write(sock_fd, &val, sizeof(val));
        

        //from server
        read(sock_fd, &check, sizeof(check));
        if(check == -1)
        {
                printf("This operation failed !!!....Try again....\n");
                return;
        }

        read(sock_fd, &user, sizeof(user));

        printf("Modified details of user with userID %d\n",id);
        printf("Name : %s\n",user.username);
        printf("Address : %s\n",user.address);
        printf("Mail : %s\n",user.mail);
        printf("Age : %d\n",user.age);

}


void modifyFacultyDetails(int sock_fd)
{
        int id;
        int val;
        int check;
        Professor user;
        char tempName[30], tempAddress[30], tempMail[30];
	int tempAge;
        printf("Enter the Faculty ID : \n");
        scanf("%d",&id);

        printf("Enter the field number which you want to modify : \n");
        printf("1. Name\n");
        printf("2. Address\n");
        printf("3. Mail\n");
        printf("4. Age\n");
        scanf("%d",&val);

        //send to server
        write(sock_fd, &id, sizeof(id));
        write(sock_fd, &val, sizeof(val));


        switch(val)
        {
                case 1:
                        fflush(stdin);
                        scanf("%s",tempName);
                        write(sock_fd, &tempName, sizeof(tempName));
                        break;
                case 2:
                        fflush(stdin);
                        scanf("%s",tempAddress);
                        write(sock_fd, &tempAddress, sizeof(tempAddress));
                        break;
                case 3:
                        fflush(stdin);
                        scanf("%s",tempMail);
                        write(sock_fd, &tempMail, sizeof(tempMail));
                        break;
                case 4:
                        fflush(stdin);
                        scanf("%d",&tempAge);
                        write(sock_fd, &tempAge, sizeof(tempAge));
                        break;
                default:
                        printf("Wrong choice\n");
                        break;
        }

        // //send to server
        // write(sock_fd, &id, sizeof(id));
        // write(sock_fd, &val, sizeof(val));

        //from server
        read(sock_fd, &check, sizeof(check));
        if(check == -1)
        {
                printf("This operation failed !!!....Try again....\n");
                return;
        }

        read(sock_fd, &user, sizeof(user));


        printf("Modified details of user with userID %d\n",id);
        printf("Name : %s\n",user.username);
        printf("Address : %s\n",user.address);
        printf("Mail : %s\n",user.mail);
        printf("Age : %d\n",user.age);
}

//**********************************************Professor Functions*********************************************************

void displayOfferedCourses(int sock_fd) // these are the courses that are present under the current faculty
{
        Professor prof;
        Courses course;

        int noOfCourses;
        
        //send to server
        write(sock_fd, &globalUserId, sizeof(globalUserId));

        //from server
        read(sock_fd, &noOfCourses, sizeof(noOfCourses));
        for(int i=0 ; i<noOfCourses ; i++)
        {
                read(sock_fd, &course, sizeof(course));
                fflush(stdout);

                printf("Course Name : %s\n",course.name);
                fflush(stdout);

                printf("Course ID : %d\n",course.courseID);
                fflush(stdout);

                printf("Total Seats : %d\n",course.totalSeats);
                fflush(stdout);

                printf("Number of credits : %d\n",course.credits);
                fflush(stdout);

                printf("Number of available seats : %d\n",course.noOfAvailableSeats);
                fflush(stdout);

                printf("Department : %s\n",course.department);
                fflush(stdout);

                printf("\n\n\n");

        }
        showMenu(sock_fd);
}



void addNewCourse(int sock_fd, int profId)
{
        Courses course;
        bool result;
        //int select = 1;

        write(1, "Enter course name : ",sizeof("Enter course name : "));
        fflush(stdin);
        scanf("%s",course.name);   
        
        write(1, "Enter total no of seats: ",sizeof("Enter total no of seats: "));
        fflush(stdin);
        scanf("%d",&course.totalSeats);

        write(1, "Enter credits offered : ",sizeof("Enter credits offered : "));
        fflush(stdin);
        scanf("%d",&course.credits);
        
        write(1, "Number of seats available : ",sizeof("Number of seats available : "));
        fflush(stdin);
        scanf("%d",&course.noOfAvailableSeats);

        write(1, "Enter department : ",sizeof("Enter department : "));
        fflush(stdin);
        scanf("%s",course.department);

        course.isRemoved = 0;
        


        course.profUserID = profId;
        

        //send to server
        write(sock_fd, &profId, sizeof(profId));
        
        

        int profSpaceAvailability;

        //from server
        read(sock_fd, &profSpaceAvailability, sizeof(int));
        if(profSpaceAvailability == 0)
        {
                printf("You can teach only 4 courses at max..\n");
                
        }
        else
        {
                write(sock_fd, &course, sizeof(course));

                read(sock_fd, &result, sizeof(result));
                read(sock_fd, &course,sizeof(course));
                

                if(result)
                {
                        write(1, "Course added successfully ", sizeof("Course added successfully "));
                        
                        printf("Course id generated is %d\n", course.courseID);
                }
                else
                {
                        write(1, "Course cannot be added *** ", sizeof("Course cannot be added ***"));
                }
        }
        showMenu(sock_fd);
}


void removeCourse(int sock_fd)
{
        int removeCourseID;
        bool result;
        printf("Enter the course id you want to remove: ");
        scanf("%d",&removeCourseID);

        //to server
        write(sock_fd, &globalUserId, sizeof(globalUserId));
        write(sock_fd, &removeCourseID, sizeof(removeCourseID));

        read(sock_fd, &result, sizeof(result));
        if(result)
                printf("The course has been successfully removed....");
        else
                printf("The course could not be removed due to some error...");
        showMenu(sock_fd);
}

void updateCourse(int sock_fd)
{
        bool result;
        int updateCourseId;
        int choice;

        printf("Enter the course ID : ");
        scanf("%d",&updateCourseId);

        write(sock_fd,&updateCourseId,sizeof(updateCourseId));

        printf("Choose the option to update: \n");
        printf("1. Course Name\n");
        printf("2. Total No of Seats\n");
        printf("3. Department name\n");
        printf("4. Credits\n");

        switch(choice)
        {
                case 1:
                        char name[40];
                        fflush(stdin);
                        scanf("%s",name);
                        write(sock_fd, &name, sizeof(name));
                        break;
                case 2:
                        int seats;
                        fflush(stdin);
                        scanf("%d",&seats);
                        write(sock_fd, &seats, sizeof(seats));
                        break;
                case 3:
                        char department[30];
                        fflush(stdin);
                        scanf("%s",department);
                        write(sock_fd, &department, sizeof(department));
                        break;
                case 4:
                        int credit;
                        fflush(stdin);
                        scanf("%d",&credit);
                        write(sock_fd, &credit, sizeof(credit));
                        break;
                default:
                        printf("You entered wrong choice\n");
                        break;
        }

        read(sock_fd, &result, sizeof(result));
        if(result)
                printf("Course with courseID %d has been successfully updated\n",updateCourseId);
        
        else
                printf("Course with courseID %d could not be updated successfully...",updateCourseId);

        showMenu(sock_fd);
}



//**********************************************Student Functions**************************************************************

void changePassword(int sock_fd, int id)
{

        int result;
        while(1)
        {
                char newPass[10];
                printf("Enter the new Password : ");
                fflush(stdin);
                scanf("%s",newPass);

                if(strcmp(newPass,"iiitb")==0)
                {
                        printf("You have entered the default password...please add a new password\n");
                        continue;
                }
                //send to server
                write(sock_fd, &id, sizeof(id));
                write(sock_fd, &newPass, sizeof(newPass));

                //received from the server
                read(sock_fd, &result, sizeof(result));

                if(result != -1)
                        break;
                else
                        printf("Try again !!!!\n");
        }
        

}

void display_all_courses(int sock_fd){}
void enroll_to_course(int sock_fd){}
void drop_course(int sock_fd){}
void display_enrolled_course_details(int sock_fd){}


//**************************************Main code*************************
int main() {
    int client_socfd;
    struct sockaddr_in server_addr;
    char message[1024];

    // Create a socket
    client_socfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("\nclient sock_fd %d\n",client_socfd);
    if (client_socfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_socfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, PORT);
  
	//client_socfd = 10;  
    chooseOption(client_socfd);    
    //showMenu(client_socfd);






    // Send a message to the server
   // const char *message_to_server = "Hello, Server!";
 //   send(client_socfd, message_to_server, strlen(message_to_server), 0);
	
    // Receive a response from the server
   // char server_response[1024];
   // recv(client_socfd, server_response, sizeof(server_response), 0);
 //   printf("Server response: %s\n", server_response);

    // Close the client socket
    close(client_socfd);

    return 0;
}
