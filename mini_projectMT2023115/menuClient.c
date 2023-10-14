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
#define PORT 8080

int option,currUserID;


void attemptAdminLogin(int sock_fd);
void attemptProfessorLogin(int sock_fd);
void attemptStudentLogin(int sock_fd);

void add_student(int sock_fd);
void display_student_details(int sock_fd);
void add_faculty(int sock_fd);
void display_faculty_details(int sock_fd);
void activate_student(int sock_fd);
void block_student(int sock_fd);
void modify_student_details(int sock_fd);
void modify_faculty_details(int sock_fd);

void display_offered_courses(int sock_fd);
void add_new_course(int sock_fd);
void remove_course(int sock_fd);
void update_course(int sock_fd);
void change_password(int sock_fd);

void display_all_courses(int sock_fd);
void enroll_to_course(int sock_fd);
void drop_course(int sock_fd);
void display_enrolled_course_details(int sock_fd);

void chooseOption(int sock_fd){

	printf(" ---------------WELCOME BACK TO ACADEMIA :: COURSE REGISTRATION----------------\n");
        printf("Choose an option: \n");
	printf("| 1 : Admin Login     |\n");
	printf("| 2 : Professor Login |\n");
	printf("| 3 : Student Login   |\n");
        printf(" ----------------------------------------------\n");

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



void attemptStudentLogin(int sock_fd){
	bool result;
	Student currUser;

	write(1,"Student ID : ",sizeof("Student ID : "));
	scanf("%d",&currUser.userID);

	write(1,"Password : ",sizeof("Password : "));
	scanf("%s",currUser.password);


	//to the server
	write(sock_fd,&option,sizeof(int));
	write(sock_fd,&currUser,sizeof(Student));

	read(sock_fd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Invalid login!!\n\n",sizeof("Invalid login!!\n\n"));
		chooseOption(sock_fd);
	}
	else{
		write(1,"Succesfully logged in!!\n\n",sizeof("Succesfully logged in!!\n\n"));
	}
}



void attemptProfessorLogin(int sock_fd){
        bool result; 
        Professor currUser;

        write(1,"Professor ID : ",sizeof("Professor ID : "));
        scanf("%d",&currUser.userID);

        write(1,"Password : ",sizeof("Password : "));
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
        }
        return;
}



void attemptAdminLogin(int sock_fd){
        bool result = true; 
        Admin currUser;

        printf("Admin ID : ");
        scanf("%d",&currUser.userID);
	
	currUserID=currUser.userID;

        printf("Password : ");
	getchar();
	fgets(currUser.password, sizeof(currUser.password), stdin);

	printf("%s\n", currUser.password);
	fflush(stdout);
	printf("Option for server %d",option);

        //to the server
        //write(sock_fd,&option,sizeof(int));
        //write(sock_fd,&currUser,sizeof(Admin));

       // read(sock_fd,&result,sizeof(result)); //from the server


        if(!result){
                printf("Invalid login!!\n\n");
                chooseOption(sock_fd); 
        }
        else{
                write(1,"Succesfully logged in!!\n\n",sizeof("Succesfully logged in!!\n\n"));
        }

        return;
}



void showMenu(int sock_fd){
	int select;
	if(option==1){
		write(1," ...............WELCOME TO ADMIN MENU..................\n",sizeof(" ...............WELCOME TO ADMIN MENU................\n"));
		write(1,"| 1 : Add Student                                      |\n",sizeof("| 1 : Add Student                                      |\n"));
		write(1,"| 2 : View Student Details                             |\n",sizeof("| 2 : View Student Details                             |\n"));
		write(1,"| 3 : Add Faculty                                      |\n",sizeof("| 3 : Add Faculty                                      |\n"));
		write(1,"| 4 : View Faculty Details                             |\n",sizeof("| 4 : View Faculty Details                             |\n"));
		write(1,"| 5 : Activate Student                                 |\n",sizeof("| 5 : Activate Student                                 |\n"));
		write(1,"| 6 : Block Student                                    |\n",sizeof("| 6 : Block Student                                    |\n"));
		write(1,"| 7 : Modify Student Details                           |\n",sizeof("| 7 : Modify Student Details                           |\n"));
		write(1,"| 8 : Modify Faculty Details                           |\n",sizeof("| 8 : Modify Faculty Details                           |\n"));
		write(1,"| 9 : Logout and Exit                                  |\n",sizeof("| 9 : Logout and Exit                                  |\n"));
		
		write(1," ......................................................\n",sizeof(" ......................................................\n"));

		write(1,"Choose an option : ",sizeof("Choose an option : "));
		scanf("%d",&select);
		printf("Option : %d\n",select);

		switch(select){
		case 1 :
			add_student(sock_fd);
			break;
		case 2 :
			display_student_details(sock_fd);
			break;
		case 3 :
			add_faculty(sock_fd);
			break;
		case 4 :
			display_faculty_details(sock_fd);
			break;
		case 5 :
			activate_student(sock_fd);
			break;
		case 6 :
			block_student(sock_fd);
			break;
		case 7 :
			modify_student_details(sock_fd);
			break;
		case 8 :
			modify_faculty_details(sock_fd);
			break;
		case 9 :
			write(sock_fd,&select,sizeof(int));
			write(1,"Thank you\n",sizeof("Thank you\n"));
			exit(0);
		default :
			write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
			showMenu(sock_fd);
			break;
		}
	}
	else if(option==2){
		write(1," ...............WELCOME TO FACULTY MENU..................\n",sizeof(" ...............WELCOME TO FACULTY MENU..................\n"));
		write(1,"| 1 : View Offering Courses                              |\n",sizeof("| 1 : View Offering Courses                              |\n"));
		write(1,"| 2 : Add New Course                                     |\n",sizeof("| 2 : Add New Course                                     |\n"));
		write(1,"| 3 : Remove Course From Catalog                         |\n",sizeof("| 3 : Remove Course From Catalog                         |\n"));
		write(1,"| 4 : Update Course Details                              |\n",sizeof("| 4 : Update Course Details                              |"));
		write(1,"| 5 : Change Password                                    |\n",sizeof("| 5 : Change Password                                    |\n"));
		write(1,"| 6 : Logout and Exit                                    |\n",sizeof("| 6 : Logout and Exit                                    |\n"));
		write(1," ........................................................\n",sizeof(" ........................................................\n"));

		write(1,"Choose an option : ",sizeof("Choose an option : "));
		scanf("%d",&select);
		printf("Option : %d\n",select);

		switch(select){
		case 1 :
			display_offered_courses(sock_fd);
			break;
		case 2 :
			add_new_course(sock_fd);
			break;
		case 3 :
			remove_course(sock_fd);
			break;
		case 4 :
			update_course(sock_fd);
			break;
		case 5 :
			change_password(sock_fd);
			break;
		case 6 :
			write(sock_fd,&select,sizeof(int));
			write(1,"Thank you\n",sizeof("Thank you\n"));
			exit(0);
		default :
			write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
			showMenu(sock_fd);
			break;
		}
	}
	else{
		write(1," ...............WELCOME TO STUDENT MENU..................\n",sizeof(" ...............WELCOME TO STUDENT MENU..................\n"));
                write(1,"| 1 : View All Courses                                   |\n",sizeof("| 1 : View All Courses                                   |\n"));
                write(1,"| 2 : Enroll New Course                                  |\n",sizeof("| 2 : Enroll New Course                                  |\n"));
                write(1,"| 3 : Drop Course                                        |\n",sizeof("| 3 : Drop Course                                        |\n"));
                write(1,"| 4 : View Enrolled Course Details                       |\n",sizeof("| 4 : View Enrolled Course Details                       |"));
                write(1,"| 5 : Change Password                                    |\n",sizeof("| 5 : Change Password                                    |\n"));
                write(1,"| 6 : Logout and Exit                                    |\n",sizeof("| 6 : Logout and Exit                                    |\n"));
                write(1," ........................................................\n",sizeof(" ........................................................\n"));

		write(1,"Choose an option : ",sizeof("Choose an option : "));
                scanf("%d",&select);
                printf("Option : %d\n",select);

                switch(select){
                case 1 :
                        display_all_courses(sock_fd);
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
                        change_password(sock_fd);
                        break;
                case 6 :
                        write(sock_fd, &select,sizeof(int));
                        write(1,"Thank you\n",sizeof("Thank you\n"));
                        exit(0);
                default :
                        write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
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
   scanf("%s",user.username);

   write(1, "Enter student age : ",sizeof("Enter student age : "));
   scanf("%d\n",&user.age);

   write(1, "Enter student address : ",sizeof("Enter student address : "));
   scanf("%s",user.address);

   write(1, "Enter student mail : ",sizeof("Enter student mail : "));
   scanf("%s",user.mail);

   write(1, "Enter student LoginId : ",sizeof("Enter student LoginId : "));
   scanf("%s",user.loginID);

   //send to server
   write(sock_fd, &select,sizeof(int));
   write(sock_fd, &user, sizeof(user));

   //from server
   read(sock_fd, &result, sizeof(result));
   if(result)
   {
	write(1, "Student added successfully ", sizeof("Student added successfully "));
	printf("Student id generated is %d ", user.userID);
   }
   else
   {
	write(1, "Student cannot be added *** ", sizeof("Student cannot be added ***"));
   }
   showMenu(sock_fd);
}




void display_student_details(int sock_fd)
{
   int id;
   Student user;
   write(1, "Enter student ID : ",sizeof("Enter student ID : "));
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




void add_faculty(int sock_fd)
{
   Professor user;
   bool result;
   int select = 1;

   write(1, "Enter faculty name : ",sizeof("Enter faculty name : "));
   scanf("%s",user.username);

   write(1, "Enter faculty age : ",sizeof("Enter faculty age : "));
   scanf("%d\n", &user.age);

   write(1, "Enter faculty address : ",sizeof("Enter faculty address : "));
   scanf("%s",user.address);

   write(1, "Enter faculty mail : ",sizeof("Enter faculty mail : "));
   scanf("%s",user.mail);

   write(1, "Enter faculty LoginId : ",sizeof("Enter faculty LoginId : "));
   scanf("%s",user.loginID);

   //send to server
   write(sock_fd, &select,sizeof(int));
   write(sock_fd, &user, sizeof(user));

   //from server
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




void display_faculty_details(int sock_fd)
{
   int id;
   Professor user;
   write(1, "Enter faculty ID : ",sizeof("Enter faculty ID : "));
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

}



void activate_student(int sock_fd){}
void block_student(int sock_fd){}
void modify_student_details(int sock_fd){}
void modify_faculty_details(int sock_fd){}

void display_offered_courses(int sock_fd){}
void add_new_course(int sock_fd){}
void remove_course(int sock_fd){}
void update_course(int sock_fd){}
void change_password(int sock_fd){}

void display_all_courses(int sock_fd){}
void enroll_to_course(int sock_fd){}
void drop_course(int sock_fd){}
void display_enrolled_course_details(int sock_fd){}


//**************************************Main code*************************
int main() {
    int client_socfd = 10;  
    chooseOption(client_socfd);    
//     showMenu(client_socfd);






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
