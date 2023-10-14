#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include "recordHeaders.h"


int getNewUserID1();
int getNewUserID2();
int getNewUserID3();

int getNewUserID1(){
    int fd=open("SUfile",O_RDONLY,0744);             // Normal User
    Student record;
    lseek(fd,-sizeof(Student),SEEK_END);
    read(fd,&record,sizeof(Student));
    close(fd);
    return record.userID+1;
}

int getNewUserID2(){
    int fd=open("PUfile",O_RDONLY,0744);             // Joint User
    Professor record;
    lseek(fd,-sizeof(Professor),SEEK_END);
    read(fd,&record,sizeof(Professor));
    close(fd);
    return record.userID+1;
}

int getNewUserID3(){
    int fd=open("AUfile",O_RDWR,0744);             // Admin
    Admin record;
    lseek(fd,-sizeof(Admin),SEEK_END);
    read(fd,&record,sizeof(Admin));
    close(fd);
    return record.userID+1;
}

void addAdmin()
{
   int fd=open("AUfile",O_RDWR | O_CREAT,0744);
    int choice=0;
    Admin newAdmin;
    printf("Please enter the name of the admin: ");
    scanf(" %[^\n]",newAdmin.username);
    printf("Please enter the password(max 10 characters): ");
    scanf(" %[^\n]",newAdmin.password);
    newAdmin.userID=1000;
    printf("Your userID is : %d\n",newAdmin.userID);
    write(fd,&newAdmin,sizeof(Admin));
    printf("Do you want to continue(0/1)? ");
    scanf("%d",&choice);
    while(choice){
        printf("Please enter the name of the admin: ");
        scanf(" %s",newAdmin.username);
        printf("Please enter the password(max 10 characters): ");
        scanf(" %s",newAdmin.password);
        newAdmin.userID=getNewUserID3();
        printf("Your userID is : %d\n",newAdmin.userID);
        write(fd,&newAdmin,sizeof(Admin));
        printf("Do you want to continue(0/1)? ");
        scanf("%d",&choice);
    }
    close(fd);

}

void addStudent()
{

    int fd=open("SUfile",O_RDWR | O_CREAT,0744);
    int choice=1;
    Student user;
    user.userID=1000;

    write(1, "Enter student name : ",sizeof("Enter student name : "));
    fflush(stdin);
    scanf("%s",user.username);

    write(1, "Enter student age : ",sizeof("Enter student age : "));
    scanf("%d",&user.age);

    write(1, "Enter student address : ",sizeof("Enter student address : "));
    scanf("%s",user.address);

    write(1, "Enter student mail : ",sizeof("Enter student mail : "));
    scanf("%s",user.mail);

    write(1, "Enter student LoginId : ",sizeof("Enter student LoginId : "));
    scanf("%s",user.loginID);

    
    user.isAccessAllowed = 1;
    strcpy(user.password,"iiitb");

    printf("Your userID is : %d\n",user.userID);
    write(fd,&user,sizeof(Student));
    printf("Do you want to continue(0/1)? ");
    scanf("%d",&choice);
    while(choice){
        user.userID=getNewUserID1();

        write(1, "Enter student name : ",sizeof("Enter student name : "));
        scanf("%s",user.username);

        write(1, "Enter student age : ",sizeof("Enter student age : "));
        scanf("%d",&user.age);

        write(1, "Enter student address : ",sizeof("Enter student address : "));
        scanf("%s",user.address);

        write(1, "Enter student mail : ",sizeof("Enter student mail : "));
        scanf("%s",user.mail);

        write(1, "Enter student LoginId : ",sizeof("Enter student LoginId : "));
        scanf("%s",user.loginID);

        user.isAccessAllowed = 1;
        strcpy(user.password,"iiitb");

        printf("Your userID is : %d\n",user.userID);


        write(fd,&user,sizeof(Student));
        printf("Do you want to continue(0/1)? ");
        scanf("%d",&choice);
    }
    close(fd);
}
   
void addProfessor()
{
    int fd=open("PUfile",O_RDWR | O_CREAT,0744);
    int choice=1;
    Professor newPUser;
    newPUser.userID=1000;

    write(1, "Enter faculty name : ",sizeof("Enter faculty name : "));
    scanf("%s", newPUser.username);

    write(1, "Enter faculty age : ",sizeof("Enter faculty age : "));
    scanf("%d", &newPUser.age);

    write(1, "Enter faculty address : ",sizeof("Enter faculty address : "));
    scanf("%s", newPUser.address);

    write(1, "Enter faculty mail : ",sizeof("Enter faculty mail : "));
    scanf("%s", newPUser.mail);

    write(1, "Enter faculty LoginId : ",sizeof("Enter faculty LoginId : "));
    scanf("%s", newPUser.loginID);

    strcpy(newPUser.password,"iiitb");

    newPUser.courseID[0] = -1;
    newPUser.courseID[1] = -1;
    newPUser.courseID[2] = -1;
    newPUser.courseID[3] = -1;


    printf("Your userID is : %d\n",newPUser.userID);
    write(fd,&newPUser,sizeof(Professor));
    printf("Do you want to continue(0/1)? ");
    scanf("%d",&choice);
    while(choice){
        newPUser.userID=getNewUserID2();

        write(1, "Enter faculty name : ",sizeof("Enter faculty name : "));
        scanf("%s", newPUser.username);

        write(1, "Enter faculty age : ",sizeof("Enter faculty age : "));
        scanf("%d", &newPUser.age);

        write(1, "Enter faculty address : ",sizeof("Enter faculty address : "));
        scanf("%s", newPUser.address);

        write(1, "Enter faculty mail : ",sizeof("Enter faculty mail : "));
        scanf("%s", newPUser.mail);

        write(1, "Enter faculty LoginId : ",sizeof("Enter faculty LoginId : "));
        scanf("%s", newPUser.loginID);

        strcpy(newPUser.password,"iiitb");

        newPUser.courseID[0] = -1;
        newPUser.courseID[1] = -1;
        newPUser.courseID[2] = -1;
        newPUser.courseID[3] = -1;

        printf("Your userID is : %d\n",newPUser.userID);

        write(fd,&newPUser,sizeof(Professor));
        printf("Do you want to continue(0/1)? ");
        scanf("%d",&choice);
    }
    close(fd);

}

void addDummyCourse()
{
        int fd=open("CUfile",O_RDWR | O_CREAT,0744);
        Courses course;
        course.courseID = 1000;
        course.isRemoved = 0;

        write(1, "Enter course name : ",sizeof("Enter course name : "));
        fflush(stdin);
        scanf("%s",course.name);   
        
        write(1, "Enter total no of seats: ",sizeof("Enter total no of seats: "));
        scanf("%d",&course.totalSeats);
        getchar();

        write(1, "Enter credits offered : ",sizeof("Enter credits offered : "));
        fflush(stdin);
        scanf("%d",&course.credits);
        
        write(1, "Number of seats available : ",sizeof("Number of seats available : "));
        fflush(stdin);
        scanf("%d",&course.noOfAvailableSeats);

        write(1, "Enter department : ",sizeof("Enter department : "));
        fflush(stdin);
        scanf("%s",course.department);

        course.profUserID = 0;

        close(fd);
        
}

 
int main(){

    int option;
    while(1)
    {
        printf("Enter your choice:\n");
        printf("1. Admin  2. Professor  3. Student  4. Dummy Course  5. Exit \n");
        scanf("%d", &option);
        switch(option)
        {
            case 1:
                addAdmin();
                break;
            case 2:
                addProfessor();
                break;
            case 3:
                addStudent();
                break;
            case 4:
                addDummyCourse();
                break;
            case 5:
                exit(0);
            default:
                printf("Select correct option \n");
        }	
    }

    return 0;
}
