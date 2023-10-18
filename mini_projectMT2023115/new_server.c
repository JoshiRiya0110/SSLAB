#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "recordHeaders.h"
#include <fcntl.h>
#include <pthread.h>

#define PORT 8096
#define MAX_CLIENTS 5

///Function Declarations 
void serverTask(int nsd);

//Admin function Declaration
bool checkAdminLogin(Admin currUser);
void processAdminMenu(int nsd, int select);
bool addStudent(int nsd, Student record);
Student displayStudent(int id);
bool addFaculty(int nsd, Professor record);
Professor displayFaculty(int id);
void activateStudent(int nsd, int id);
void blockStudent(int nsd, int id);
void modifyStudentDetails(int nsd, int id);
void modifyFacultyDetails(int nsd, int id);

//Professor Function Declaration
bool checkProfessorLogin(Professor currUser);
void processProfessorMenu(int nsd, int select);
void displayOfferedCourses(int nsd);
void addNewCourse(int nsd, int profId);
void removeCourse(int nsd);
void updateCourse(int nsd);
void changeFacultyPassword(int nsd, int id);

//Student declarations
int checkStudentLogin(Student currUser);
void processStudentMenu(int nsd, int select);
void changeStudentPassword(int nsd, int id);
void displayAllCourse(int nsd);
void enrollToCourse(int nsd);
void dropCourse(int nsd);
void displayEnrolledCourseDetails(int nsd);


//###############################Function definations########################################


void serverTask(int nsd)
{
    int select,type,option, currUserID;
	bool loginResult;
	//int check;   //remove
	while(1){
		read(nsd,&option,sizeof(option));                            // client is sending this--> write(sock_fd,&option,sizeof(int));
		printf("Option : %d\n",option);

		if(option==1){
                        Admin currUser1;

                        read(nsd,&currUser1,sizeof(Admin)); 	     //Read Admin Structure and verify login
                        currUserID = currUser1.userID;
                        loginResult = checkAdminLogin(currUser1);
                        write(nsd,&loginResult,sizeof(loginResult));
		}
		else if(option==2){

						Professor currUser2;
							
						read(nsd,&currUser2,sizeof(Professor));     // write(sock_fd,&currUser,sizeof(Professor));
						currUserID = currUser2.userID;
						loginResult = checkProfessorLogin(currUser2);
						write(nsd, &loginResult, sizeof(loginResult));
		}
		else if(option==3){

						Student currUser3;
						int check;
						int id;

						read(nsd, &currUser3, sizeof(Student));
						currUserID = currUser3.userID;
						check = checkStudentLogin(currUser3);
						write(nsd, &check, sizeof(check));
						if(check == 3){  //It implies user login for first time, Needs to change password
							read(nsd, &id, sizeof(id));
							printf(" Id read = %d\n", id); //remove
							changeStudentPassword(nsd, currUser3.userID);
						}
                        loginResult = (check != 0) ? true : false; 
		}
		else{
						loginResult = false;
						write(nsd, &loginResult, sizeof(loginResult));
		}

		if(loginResult) //user verified, plz process user functions next
		{
			printf("Exiting first while login check loop\n");
			break;
		}
	}

	while(1){

		printf("waiting for select\n");   //remove
		read(nsd,&select,sizeof(int));
		printf("Selected option is %d \n", select); //remove

		if(option == 1)
		{
            printf("server task part 1\n");
            if(select != 9){ //select option to logout and exit in client
				processAdminMenu(nsd, select);
			}
            else{
                break;
            }
		}	
		else if(option == 2)
		{
			printf("server task part2\n");
			if(select !=6){ //select option to logout and exit in client
				processProfessorMenu(nsd, select);
			}
			else{
				break;
			}
		}
		else if(option == 3)
		{
			printf("server task part3\n");
			if(select != 6){  //select option to logout and exit in client
				processStudentMenu(nsd, select);
			}
			else{
				break;
			}
			
		}

	}
	close(nsd);
	write(1,"Ended client session.....\n",sizeof("Ended client session.....\n"));
	return;
}

//*********************************************ADMIN FUNCTIONS*****************************************************


void processAdminMenu(int nsd, int select)
{
    printf("Inside process adminMenu\n");
	bool result;
	switch(select)
	{
		case 1:
				Student newUser1;
               	read(nsd, &newUser1, sizeof(Student));
               	result = addStudent(nsd, newUser1);
               	write(nsd, &result, sizeof(result));
				break;

		case 2:
				int id1;
				Student newUser2;
				read(nsd, &id1, sizeof(id1));
                printf("read st id = %d", id1); //debugg
				newUser2 = displayStudent(id1);
				write(nsd, &newUser2, sizeof(Student));
				break;
				
		case 3:
				Professor newUser3;
            	read(nsd,&newUser3,sizeof(Professor));
                result=addFaculty(nsd,newUser3);
                write(nsd,&result,sizeof(result));
                break;

		case 4:
				int id2;
				Professor newUser4;
				read(nsd,&id2,sizeof(id2));
				newUser4=displayFaculty(id2);
				write(nsd,&newUser4,sizeof(Professor));
				break;

		case 5:
				int id3;
				read(nsd,&id3, sizeof(id3));
				activateStudent(nsd, id3);
				break;

		case 6:
				int id4;
				read(nsd, &id4, sizeof(id4));
				blockStudent(nsd, id4);
				break;

		case 7:
				int id5;
				read(nsd,&id5,sizeof(id5));
				modifyStudentDetails(nsd, id5);
				break;

		case 8:
				int id6;
				read(nsd, &id6, sizeof(id6));
				modifyFacultyDetails(nsd, id6);
				break;

        case 9:  //Check has been done in servertask
                //Case for logout and exit
                break;
        default:
                break;
	}
}

bool checkAdminLogin(Admin currUser){

	printf("Entered function to check for admin login" ); //debugg

    bool loginResult;
	Admin adminUser;

	int userid = currUser.userID;
	userid = userid - 1000;  // To resolve data access from file

	printf("\n userid value : %d\n", userid);  //debugg

	int fd=open("AUfile",O_RDWR,0744);
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid)*sizeof(Admin);    	     //nth record
	lock.l_len=sizeof(Admin);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(userid)*sizeof(Admin),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&adminUser,sizeof(Admin));

	// printf("pass temp %s:::  pass temp len %ld\n",temp.password,strlen(temp.password));
	// printf("pass currUser %s::: pass currUser len %ld\n",currUser.password,strlen(currUser.password));
	// currUser.password[strlen(currUser.password)-1] = '\0';

    //Compare the user and database password
	int comp = strcmp(adminUser.password,currUser.password);
	
    loginResult = (comp == 0) ? true : false;

    //Freeing the held lock					
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return loginResult;
}

bool addStudent(int nsd, Student record){

	int fd=open("SUfile",O_RDWR,0744);
	bool result;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(Student);    //nth record
	lock.l_len=sizeof(Student);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	Student endUser;
	int val = lseek(fd,(-1)*sizeof(Student),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(Student));

    //Incrementing the student Count
	record.userID=endUser.userID+1;

	printf("last user id %d\n",endUser.userID); //debugg
	printf("new user id %d\n",record.userID);//debugg


	//printf("new user userid %d\n",record.userID); //debugg

	int j = write(fd,&record,sizeof(Student));

    //Unsuccessful write can return -1(failure) / 0(no of bytes written)
    result  = (j > 0) ? true : false; 
	
    //Sendig to client for printing new userId
    write(nsd, &record, sizeof(record));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

Student displayStudent(int id){

	printf("Entered display student" );
	int userid = id;
	userid = userid - 1000; 

	//printf("\n i value : %d id == %d\n",i, id);  //debugg

	int fd=open("SUfile",O_RDWR,0744);
	
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid)*sizeof(Student);    	     //nth record
	lock.l_len=sizeof(Student);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(userid)*sizeof(Student),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Student));			

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return temp;

}


bool addFaculty(int nsd, Professor record)
{
	int fd = open("PUfile",O_RDONLY, 0744);
	bool result;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(Professor);    //nth record
	lock.l_len=sizeof(Professor);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	Professor endUser;
	int val = lseek(fd,(-1)*sizeof(Professor),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(Professor));
	record.userID=endUser.userID+1;


	printf("new user userid %d\n",record.userID);
	int j=write(fd,&record,sizeof(Professor));
	if(j!=0)	result=true;
	else	result=false;
	

	write(nsd, &record, sizeof(record));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}



Professor displayFaculty(int id)
{
    printf("Entered function to check for admin" );
	int i=id;
	i = i - 1000;
	//printf("\n i value : %d\n",i);
	int fd=open("PUfile",O_RDWR,0744);
	
	Professor temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(Professor);    	     //nth record
	lock.l_len=sizeof(Professor);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(Professor),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Professor));						

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return temp;

}



void activateStudent(int nsd, int id)
{
	printf("Entered function to activate student" );
	int status;
	int i=id;
	i = i - 1000;
	//printf("\n i value : %d\n",i);
	int fd=open("SUfile",O_RDWR,0744);
	
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(Student);    	     //nth record
	lock.l_len=sizeof(Student);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(Student),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Student));	

	if(temp.isAccessAllowed == 1)
	{
		status = 1;
	}
	else
	{
		status = 0;
		temp.isAccessAllowed = 1;					
	}

	lseek(fd,(i)*sizeof(Student),SEEK_SET);
	int check = write(fd,&temp,sizeof(Student));
	write(nsd, &check, sizeof(check));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	write(nsd, &temp, sizeof(temp));
    write(nsd, &status, sizeof(status));

	close(fd);
}

void blockStudent(int nsd, int id)
{
	printf("Entered function to block admin" );
	int status;
	int i=id;
	i = i - 1000;
	//printf("\n i value : %d\n",i);
	int fd=open("SUfile",O_RDWR,0744);
	
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(Student);    	     //nth record
	lock.l_len=sizeof(Student);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(Student),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Student));	

	if(temp.isAccessAllowed ==0)
	{
		status = 1;
	}
	else
	{
		status = 0;
		temp.isAccessAllowed = 0;					
	}

	lseek(fd,(i)*sizeof(Student),SEEK_SET);
	int check = write(fd,&temp,sizeof(Student));
	write(nsd, &check, sizeof(check));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	write(nsd, &temp, sizeof(temp));
    write(nsd, &status, sizeof(status));

	close(fd);
}




void  modifyStudentDetails(int nsd, int id)
{
	int status, choice;
	char tempName[30], tempAddress[30], tempMail[30];
	int tempAge;
	int i=id;
	i = i - 1000;
	//printf("\n i value : %d\n",i);
	int fd=open("SUfile",O_RDWR,0744);
	
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(Student);    	     //nth record
	lock.l_len=sizeof(Student);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(Student),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Student));	

	read(nsd, &choice, sizeof(choice));
	switch(choice)
        {
                case 1:
                        read(nsd, &tempName, sizeof(tempName));
						strcpy(temp.username,tempName);
                        break;
                case 2:
                        read(nsd, &tempAddress, sizeof(tempAddress));
						strcpy(temp.address,tempAddress);
                        break;
                case 3:
                        read(nsd, &tempMail, sizeof(tempMail));
						strcpy(temp.mail,tempMail);
                        break;
                case 4:
                        read(nsd, &tempAge, sizeof(tempAge));
						temp.age = tempAge;
                        break;
                default:
                        printf("Wrong choice\n");
                        break;
        }

	lseek(fd,(i)*sizeof(Student),SEEK_SET);
	int check = write(fd,&temp,sizeof(temp));

	write(nsd, &check, sizeof(check));
	write(nsd, &temp, sizeof(temp));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
}



void modifyFacultyDetails(int nsd, int id)
{
	int status, choice;
	char tempName[30], tempAddress[30], tempMail[30];
	int tempAge;
	int i=id;
	i = i - 1000;
	//printf("\n i value : %d\n",i);
	int fd=open("PUfile",O_RDWR,0744);
	
	Professor temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(Professor);    	     //nth record
	lock.l_len=sizeof(Professor);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(Professor),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Professor));	

	read(nsd, &choice, sizeof(choice));
	switch(choice)
        {
                case 1:
                        read(nsd, &tempName, sizeof(tempName));
						strcpy(temp.username,tempName);
                        break;
                case 2:
                        read(nsd, &tempAddress, sizeof(tempAddress));
						strcpy(temp.address,tempAddress);
                        break;
                case 3:
                        read(nsd, &tempMail, sizeof(tempMail));
						strcpy(temp.mail,tempMail);
                        break;
                case 4:
                        read(nsd, &tempAge, sizeof(tempAge));
						temp.age = tempAge;
                        break;
                default:
                        printf("Wrong choice\n");
                        break;
        }

	lseek(fd,(i)*sizeof(Professor),SEEK_SET);
	int check = write(fd,&temp,sizeof(temp));

	write(nsd, &check, sizeof(check));
	write(nsd, &temp, sizeof(temp));

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);

}

//*****************************************************Professor functions****************************************

bool checkProfessorLogin(Professor currUser){
	int userid = currUser.userID-1000;
	int fd=open("PUfile",O_RDONLY,0744);
	bool result;
	Professor temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid)*sizeof(Professor);    	     //nth record
	lock.l_len=sizeof(Professor);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd, F_SETLKW, &lock);	//lock the selected record
	//getchar();

	lseek(fd,(userid)*sizeof(Professor),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Professor));


	if(!strcmp(temp.password,currUser.password)){
		result=true;
	}
	else{
		result=false;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}


void processProfessorMenu(int nsd, int select)
{
	switch(select)
	{
		case 1:
				displayOfferedCourses(nsd);
				break;

		case 2:
				int id;
				read(nsd,&id,sizeof(id));
				addNewCourse(nsd, id);
				break;

		case 3:
				removeCourse(nsd);
				break;

		case 4:
				updateCourse(nsd);
				break;

		case 5: int userId;
            	read(nsd,&userId,sizeof(userId));
                changeFacultyPassword(nsd, userId);
				break;

		case 6: //Logout
				break;

		default:
				break;
	}
}


void displayOfferedCourses(int nsd)
{
		int profId;
		read(nsd, &profId, sizeof(profId));

		Professor temp;
		int fd1 = open("PUfile",O_RDWR,0744);
		int i1 =profId;
		i1 = i1 - 1000;

		int fl1;
		struct flock lock1;
		lock1.l_type = F_RDLCK;
		lock1.l_whence=SEEK_SET;
		lock1.l_start=(i1)*sizeof(Professor);    	     //nth record
		lock1.l_len=sizeof(Professor);	             //sizeof(record)
		lock1.l_pid=getpid();

		fl1=fcntl(fd1,F_SETLKW,&lock1);	//lock the selected record
		//getchar();

		lseek(fd1,(i1)*sizeof(Professor),SEEK_SET);  //changing the file pointer to the selected record
		read(fd1,&temp,sizeof(Professor));

		int count = 0;
		for(int i=0 ; i<4 ; i++)
		{
			if(temp.courseID[i] != -1)
				count++;
		}
		printf("count %d\n",count);
		write(nsd, &count, sizeof(count));

		printf("After write\n");

		for(int i=0 ; i<4 ; i++)
		{
			printf("%d\n",temp.courseID[i]);
			
			if(temp.courseID[i] != -1)
			{
				Courses tempCourse;

				int fd2 = open("CUfile",O_RDWR,0744);

				int i2=temp.courseID[i];
				i2 = i2 - 1000;

				printf("i2 %d\n", i2);


				int fl2;
				struct flock lock2;
				lock2.l_type = F_RDLCK;
				lock2.l_whence=SEEK_SET;
				lock2.l_start=(i2)*sizeof(Courses);    	     //nth record
				lock2.l_len=sizeof(Courses);	             //sizeof(record)
				lock2.l_pid=getpid();

				fl2=fcntl(fd2,F_SETLKW,&lock2);	//lock the selected record
				//getchar();


				lseek(fd2,(i2)*sizeof(Courses),SEEK_SET);  //changing the file pointer to the selected record
				read(fd2,&tempCourse,sizeof(Courses));

				printf("tempcourse id %d\n",tempCourse.courseID);
				printf("tempcourse name %s\n",tempCourse.name);


				write(nsd,&tempCourse,sizeof(tempCourse));
				printf("tempcourse id %d\n",tempCourse.courseID);
				printf("tempcourse name %s\n",tempCourse.name);


				lock2.l_type=F_UNLCK;
				fcntl(fd2,F_SETLK,&lock2);

				close(fd2);
			}
		}

		lock1.l_type=F_UNLCK;
		fcntl(fd1,F_SETLK,&lock1);
		close(fd1);
}

void addNewCourse(int nsd, int profId)
{

	Professor temp;
	int fd1 = open("PUfile",O_RDWR,0744);
	int i=profId;
	i = i - 1000;

	int fl1;
	struct flock lock1;
	lock1.l_type = F_WRLCK;
	lock1.l_whence=SEEK_SET;
	lock1.l_start=(i)*sizeof(Professor);    	     //nth record
	lock1.l_len=sizeof(Professor);	                 //sizeof(record)
	lock1.l_pid=getpid();

	fl1=fcntl(fd1,F_SETLKW,&lock1);	//lock the selected record
	//getchar();

	lseek(fd1,(i)*sizeof(Professor),SEEK_SET);  //changing the file pointer to the selected record
	read(fd1,&temp,sizeof(Professor));
	//i have got the structure for professor who is adding this new course id inside temp

	int flag = 0;
	for(int k=0 ; k<4 ; k++)			
	{
		if(temp.courseID[k] == -1)
		{
			flag = 1;
			break;
		}
	}
	printf("flag %d\n",flag);
	printf("Professor id %d\n",temp.userID);
	write(nsd, &flag, sizeof(flag));
	
	if(flag == 1)
	{
		//agar professor ke paas course offer karne ki space hai toh

		Courses course;
		//client ne ye course ka description bheja hai ab course id generate karna hai
		read(nsd, &course, sizeof(Courses));


		int fd2=open("CUfile",O_RDWR,0744);
		bool result;

		int fl2;
		struct flock lock2;
		lock2.l_type = F_WRLCK;
		lock2.l_whence=SEEK_END;
		lock2.l_start=(-1)*sizeof(Courses);    //nth record
		lock2.l_len=sizeof(Courses);	             //sizeof(record)
		lock2.l_pid=getpid();

		fl2=fcntl(fd2,F_SETLKW,&lock2);	//lock the selected record
		//getchar();

		Courses endCourse;
		int val = lseek(fd2,(-1)*sizeof(Courses),SEEK_END);  //changing the file pointer to the selected record
		read(fd2,&endCourse,sizeof(Courses));
		course.courseID=endCourse.courseID+1;
		

		printf("Course id %d\n",course.courseID);

		int j=write(fd2,&course,sizeof(Courses));

		if(j!=0)	
			result=true;
		else
			result=false;

		for(int k=0 ; k<4 ; k++)			
		{
			if(temp.courseID[k] == -1)
			{
				temp.courseID[k] = course.courseID;

				lseek(fd1,(i)*sizeof(Professor),SEEK_SET);
				write(fd1,&temp,sizeof(Professor));

				//we are sending the new course we added in order to print the course id on the client side
				write(nsd, &result, sizeof(result));
				write(nsd, &course, sizeof(Courses));

				break;
			}
		}
		
		lock2.l_type=F_UNLCK;
		fcntl(fd2,F_SETLK,&lock2);
		close(fd2);
	}

	lock1.l_type=F_UNLCK;
	fcntl(fd1,F_SETLK,&lock1);

	close(fd1);	

}

void removeCourse(int nsd)
{
	int profID;
	int courseId;
	bool result;

	read(nsd, &profID ,sizeof(profID));
	read(nsd, &courseId, sizeof(courseId));

	int i = profID - 1000;

	int fd1=open("PUfile",O_RDWR,0744);

	if(fd1 == -1)
	{
		result = false;
		write(nsd, &result, sizeof(result));
		return;
	}
	
	Professor temp;

	int fl1;
	struct flock lock1;
	lock1.l_type = F_WRLCK;
	lock1.l_whence=SEEK_SET;
	lock1.l_start=(i)*sizeof(Professor);    	     //nth record
	lock1.l_len=sizeof(Professor);	             //sizeof(record)
	lock1.l_pid=getpid();

	fl1=fcntl(fd1,F_SETLKW,&lock1);	//lock the selected record
	//getchar();

	lseek(fd1,(i)*sizeof(Professor),SEEK_SET);  //changing the file pointer to the selected record
	read(fd1,&temp,sizeof(Professor));						


	for(int i=0 ; i<4 ; i++)
	{
		if(temp.courseID[i] == courseId)
		{
			temp.courseID[i] = -1;
		}
	}

	lseek(fd1, (i)*sizeof(Professor),SEEK_SET);
	write(fd1,&temp,sizeof(temp));

	lock1.l_type=F_UNLCK;
	fcntl(fd1,F_SETLK,&lock1);

	close(fd1);

	Courses course;

	int fd2=open("CUfile",O_RDWR,0744);
	if(fd2 == -1)
	{
		result = false;
		write(nsd, &result, sizeof(result));
		return;
	}
	int fl2;
	i = courseId - 1000;
	struct flock lock2;
	lock2.l_type = F_WRLCK;
	lock2.l_whence=SEEK_SET;
	lock2.l_start=(i)*sizeof(Courses);    	     //nth record
	lock2.l_len=sizeof(Courses);	             //sizeof(record)
	lock2.l_pid=getpid();

	fl2=fcntl(fd2,F_SETLKW,&lock2);	//lock the selected record
	//getchar();

	lseek(fd2,(i)*sizeof(Courses),SEEK_SET);  //changing the file pointer to the selected record
	read(fd2,&course,sizeof(Courses));						

	course.isRemoved = 1;
	
	lseek(fd2, (i)*sizeof(Courses),SEEK_SET);
	write(fd2,&course,sizeof(Courses));

	lock2.l_type=F_UNLCK;
	fcntl(fd2,F_SETLK,&lock2);

	close(fd2);

	result = true;
	write(nsd, &result, sizeof(result));
	return;

}

void updateCourse(int nsd)
{
	int courseId;
	int choice;
	bool result;
	int i;
	
	read(nsd, &courseId, sizeof(courseId));
	read(nsd, &choice, sizeof(choice));


	Courses course;

	int fd=open("CUfile",O_RDWR,0744);
	if(fd == -1)
	{
		result = false;
		write(nsd, &result, sizeof(result));
		return;
	}
	int fl2;
	i = courseId - 1000;
	struct flock lock2;
	lock2.l_type = F_WRLCK;
	lock2.l_whence=SEEK_SET;
	lock2.l_start=(i)*sizeof(Courses);    	     //nth record
	lock2.l_len=sizeof(Courses);	             //sizeof(record)
	lock2.l_pid=getpid();

	fl2=fcntl(fd,F_SETLKW,&lock2);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(Courses),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&course,sizeof(Courses));						


	switch(choice)
        {
                case 1:
                        char name[40];
                        read(nsd, &name, sizeof(name));
						strcpy(course.name,name);
                        break;
                case 2:
                        int seats;
                        read(nsd, &seats, sizeof(seats));
						course.totalSeats = seats;
                        break;
                case 3:
                        char department[30];
                        read(nsd, &department, sizeof(department));
						strcpy(course.department,department);
                        break;
                case 4:
                        int credit;
                        read(nsd, &credit, sizeof(credit));
						course.credits = credit;
                        break;
                default:
                        break;
        }

	

	lseek(fd, (i)*sizeof(Courses),SEEK_SET);
	write(fd,&course,sizeof(Courses));

	lock2.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock2);

	close(fd);

	result = true;
	write(nsd, &result, sizeof(result));


}

void changeFacultyPassword(int nsd, int id)
{
	char changePass[10];
	int check;
	read(nsd, &changePass, sizeof(changePass));
	id = id-1000;

	int fd=open("PUfile",O_RDWR,0744);
	
	Professor temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(id)*sizeof(Professor);    	     //nth record
	lock.l_len=sizeof(Professor);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(id)*sizeof(Professor),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Professor));	

	strcpy(temp.password,changePass);

	lseek(fd,(id)*sizeof(Professor),SEEK_SET);
	check = write(fd,&temp,sizeof(temp));

	write(nsd, &check, sizeof(check));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
}


//###########################################################################

//**********************************************Student functions****************************************

void processStudentMenu(int nsd, int select)
{
	bool result;
	switch(select)
	{
		case 1:
				printf("Process student menu");
				displayAllCourse(nsd);
				break;

		case 2: enrollToCourse(nsd);
				break;

		case 3: dropCourse(nsd);
				break;

		case 4: displayEnrolledCourseDetails(nsd);
				break;

		case 5:
				int id;
            	read(nsd,&id,sizeof(id));
                changeStudentPassword(nsd, id);
                break;

		case 6: //Exit
				break;
		default: 
				break;
	}
}


int checkStudentLogin(Student currUser){

	int userid = currUser.userID;
	userid = userid-1000;
	int fd = open("SUfile",O_RDWR,0744);
	int result;
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (userid)*sizeof(Student);     //nth record
	lock.l_len = sizeof(Student);	             //sizeof(record)
	lock.l_pid = getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(userid)*sizeof(Student), SEEK_SET);  //changing the file pointer to the selected record
	read(fd, &temp, sizeof(Student));

	if(!strcmp(temp.password, currUser.password))
	{
		if(!strcmp(temp.password,"iiitb")){
			//User logging-in for the first time so promt to change password
			result = 3;
		}
		else if(temp.isAccessAllowed == 1){
			result = 1;
		}
		else {
			//User is blocked by admin
			result = 2; 
		}		
	}
	else{
		result=0;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

void displayAllCourse(int nsd)
{
	int noOfCourses;
	Courses endCourse;

	int fd = open("CUfile",O_RDWR,0744);
	
	int fl1;
	struct flock lock1;
	lock1.l_type = F_RDLCK;
	lock1.l_whence=SEEK_SET;
	lock1.l_start=0;    	     //locking from start
	lock1.l_len=lseek(fd,(-1)*sizeof(Courses),SEEK_END);	             //sizeof(record)
	lock1.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock1);


	lseek(fd,(-1)*sizeof(Courses),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endCourse,sizeof(Courses));

	noOfCourses=endCourse.courseID+1 - 1000;

	write(nsd, &noOfCourses, sizeof(noOfCourses));

	for(int i=0 ; i<noOfCourses ; i++)
	{
		Courses tempCourse;

		//Reading records one by one
		lseek(fd,(i)*sizeof(Courses),SEEK_SET);
		read(fd,&tempCourse,sizeof(Courses));

		write(nsd, &tempCourse, sizeof(Courses));			
	}

	lock1.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock1);
	close(fd);	

}

void enrollToCourse(int nsd)
{
	int courseId;
	int userId;

	//read userId and CourseId from client to enroll
	read(nsd, &userId, sizeof(userId));
	read(nsd, &courseId, sizeof(courseId));


	Student temp;
	int fd1 = open("SUfile",O_RDWR,0744);
	int i= userId;
	i = i - 1000;

	int fl1;
	struct flock lock1;
	lock1.l_type = F_WRLCK;
	lock1.l_whence=SEEK_SET;
	lock1.l_start=(i)*sizeof(Student);    	     //nth record
	lock1.l_len=sizeof(Student);	                 //sizeof(record)
	lock1.l_pid=getpid();

	fl1=fcntl(fd1,F_SETLKW,&lock1);	//lock the selected record
	//getchar();

	lseek(fd1,(i)*sizeof(Student),SEEK_SET);  //changing the file pointer to the selected record
	read(fd1,&temp,sizeof(Student));

	int flag = 0;
	//Iterating through student structure to see if he has exhausted no of courses he can take
	for(int k=0 ; k<4 ; k++)			
	{
		if(temp.courseID[k] == -1)
		{
			flag = 1;
			break;
		}
	}


	if(flag != 1){
		flag = 2; //determines cannot take new course
	}


	for(int k=0 ; k<4 ; k++)			
	{
		if(temp.courseID[k] == courseId){
			flag = 3; //already enrolled
			break;
		}
	}

	
	if(flag == 1)
	{
		//Student has space to accomodate new course
		int fd2=open("CUfile",O_RDWR,0744);

		Courses course;
		int i2 = courseId - 1000;

		int fl2;
		struct flock lock2;
		lock2.l_type = F_WRLCK;
		lock2.l_whence=SEEK_SET;
		lock2.l_start=(i2)*sizeof(Courses);    //nth record
		lock2.l_len=sizeof(Courses);	             //sizeof(record)
		lock2.l_pid=getpid();

		fl2=fcntl(fd2,F_SETLKW,&lock2);	//lock the selected record
		//getchar();

		lseek(fd2,(i2)*sizeof(Courses),SEEK_SET); 
		read(fd2, &course, sizeof(Courses));

		if((course.courseID == courseId) && (course.noOfAvailableSeats > 0) && (course.isRemoved != 1)){

			course.noOfAvailableSeats = course.noOfAvailableSeats-1;

			lseek(fd2,(i2)*sizeof(Courses),SEEK_SET);
			write(fd2,&course,sizeof(Courses));

			for(int k=0 ; k<4 ; k++)			
			{
				if(temp.courseID[k] == -1)
				{
					//add courseid to student
					temp.courseID[k] = courseId;

					lseek(fd1,(i)*sizeof(Student),SEEK_SET);
					write(fd1,&temp,sizeof(Student));
					break;
				}
			}
		}
		else{
			flag = 0;
		}
		
		lock2.l_type=F_UNLCK;
		fcntl(fd2,F_SETLK,&lock2);
		close(fd2);
	}
	
	//sending client info about course enrollment
	write(nsd, &flag, sizeof(flag));
	
	lock1.l_type=F_UNLCK;
	fcntl(fd1,F_SETLK,&lock1);

	close(fd1);	

}

void dropCourse(int nsd)
{
	int dropCourseId;
	int userId;

	//read userId and CourseId from client to enroll
	read(nsd, &userId, sizeof(userId));
	read(nsd, &dropCourseId, sizeof(dropCourseId));


	Student temp;
	int fd1 = open("SUfile",O_RDWR,0744);
	int i= userId;
	i = i - 1000;

	int fl1;
	struct flock lock1;
	lock1.l_type = F_WRLCK;
	lock1.l_whence=SEEK_SET;
	lock1.l_start=(i)*sizeof(Student);    	     //nth record
	lock1.l_len=sizeof(Student);	             //sizeof(record)
	lock1.l_pid=getpid();

	fl1=fcntl(fd1,F_SETLKW,&lock1);	//lock the selected record
	//getchar();

	lseek(fd1,(i)*sizeof(Student),SEEK_SET);  //changing the file pointer to the selected record
	read(fd1,&temp,sizeof(Student));

	int flag = 0;
	for(int k=0 ; k<4 ; k++)			
	{
		if(temp.courseID[k] == dropCourseId)
		{
			flag = 1;
			break;
		}
	}

	if(flag != 1){
		flag = 2; //determines selceted course is not part of student;
	}
	
	if(flag == 1)
	{
		//Course part of student we can remove it !!

		int fd2=open("CUfile",O_RDWR,0744);

		Courses course;
		int i2 = dropCourseId - 1000;

		int fl2;
		struct flock lock2;
		lock2.l_type = F_WRLCK;
		lock2.l_whence=SEEK_SET;
		lock2.l_start=(i2)*sizeof(Courses);    //nth record
		lock2.l_len=sizeof(Courses);	       //sizeof(record)
		lock2.l_pid=getpid();

		fl2=fcntl(fd2,F_SETLKW,&lock2);	//lock the selected record
		//getchar();

		
		lseek(fd2,(i2)*sizeof(Courses),SEEK_SET); 
		read(fd2, &course, sizeof(Courses));

		course.noOfAvailableSeats = course.noOfAvailableSeats+1;

		lseek(fd2,(i2)*sizeof(Courses),SEEK_SET);
		int j = write(fd2,&course,sizeof(Courses));
		int z = 0;

		for(int k=0 ; k<4 ; k++)			
		{
			if(temp.courseID[k] == dropCourseId)
			{
				//remove courseid from student
				temp.courseID[k] = -1;

				lseek(fd1,(i)*sizeof(Student),SEEK_SET);
				z = write(fd1,&temp,sizeof(Student));	
				break;
			}
		}
		
		if(j > 0 || z > 0)	//Check if write were successful ?
			flag = 1;
		else
			flag = 0;

		lock2.l_type=F_UNLCK;
		fcntl(fd2,F_SETLK,&lock2);

		close(fd2);
	}
	
	//sending client info about course enrollment
	write(nsd, &flag, sizeof(flag));
	
	lock1.l_type=F_UNLCK;
	fcntl(fd1,F_SETLK,&lock1);

	close(fd1);	

}

void displayEnrolledCourseDetails(int nsd)
{
	int studentId;
	read(nsd, &studentId, sizeof(studentId));

	Student temp;
	int fd1 = open("SUfile",O_RDWR,0744);
	int i1  = studentId;
	i1 = i1 - 1000;

	int fl1;
	struct flock lock1;
	lock1.l_type = F_RDLCK;
	lock1.l_whence=SEEK_SET;
	lock1.l_start=(i1)*sizeof(Student);    	     //nth record
	lock1.l_len=sizeof(Student);	             //sizeof(record)
	lock1.l_pid=getpid();

	fl1=fcntl(fd1,F_SETLKW,&lock1);	//lock the selected record
	//getchar();

	lseek(fd1,(i1)*sizeof(Student),SEEK_SET);  //changing the file pointer to the selected record
	read(fd1,&temp,sizeof(Student));

	int noOfEnrolledCourses = 0;
	for(int i=0 ; i<4 ; i++)
	{
		if(temp.courseID[i] != -1)
			noOfEnrolledCourses++;
	}

	write(nsd, &noOfEnrolledCourses, sizeof(noOfEnrolledCourses));

	for(int i=0 ; i<4 ; i++)
	{		
		//Fetch course data only for enrolled courses
		if(temp.courseID[i] != -1)
		{
			Courses tempCourse;
			int i2=temp.courseID[i];
			i2 = i2 - 1000;

			int fd2 = open("CUfile",O_RDWR,0744);

			int fl2;
			struct flock lock2;
			lock2.l_type = F_RDLCK;
			lock2.l_whence=SEEK_SET;
			lock2.l_start=(i2)*sizeof(Courses);    	     //nth record
			lock2.l_len=sizeof(Courses);	             //sizeof(record)
			lock2.l_pid=getpid();

			fl2=fcntl(fd2,F_SETLKW,&lock2);	//lock the selected record
			//getchar();

			lseek(fd2,(i2)*sizeof(Courses),SEEK_SET);  //changing the file pointer to the selected record
			read(fd2,&tempCourse,sizeof(Courses));

			//send client course data
			write(nsd,&tempCourse,sizeof(tempCourse));

			lock2.l_type=F_UNLCK;
			fcntl(fd2,F_SETLK,&lock2);

			close(fd2);
		}
	}

	lock1.l_type=F_UNLCK;
	fcntl(fd1,F_SETLK,&lock1);
	close(fd1);	
}



void changeStudentPassword(int nsd, int id)
{
	char changePass[10];
	int check;
	read(nsd, &changePass, sizeof(changePass));
	id = id-1000;

	int fd=open("SUfile",O_RDWR,0744);
	
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(id)*sizeof(Student);    	     //nth record
	lock.l_len=sizeof(Student);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(id)*sizeof(Student),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Student));	

	strcpy(temp.password,changePass);

	lseek(fd,(id)*sizeof(Student),SEEK_SET);
	check = write(fd,&temp,sizeof(temp));

	write(nsd, &check, sizeof(check));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
}


//********************************************************************************************************
//*******************************main Function********

void *handle_client(void* arg)
{
   int client_sockfd = *((int*)arg);
   char buffer[1024];
   int read_size;

   //Handle client communication
    // ************   while((read_size = recv(client_socfd, buffer, sizeof(buffer),0)) > 0)
    //    {
	serverTask(client_sockfd); // Handles the incoming function request's from client's
	//buffer[read_size] = '\0';
	//printf("Received message from client: %s\n",buffer);

	//send a response back to the client
	//const char* response = "Message received";
	//send(client_socfd, response, strlen(response), 0);
    // ***********   }

   if(read_size == 0)
   {
        //client disconnected
        printf("Client disconnected\n");
        fflush(stdout);
   }
   else if(read_size == -1)
   {
	    perror("recv");
   }

   close(client_sockfd);
   free(arg);
   return NULL;
}

int main()
{
   int server_sockfd;
   int client_sockfd;
   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;
   pthread_t thread_id;
   socklen_t client_address_len = sizeof(client_addr);

   //create a server socket and store its fd

   server_sockfd = socket(AF_INET,SOCK_STREAM,0);
   if(server_sockfd == -1)
   {
        perror("Error in socket creation");
	    return 0;
   }
   
   //Initialize server address
   memset(&server_addr, 0, sizeof(server_addr));
   
   //Type of socket created ==> INADDR_ANY is an IP address that is used when we don't want to bind a socket to any specific IP.
   //When we don't know the IP address of our machine, we can use the special IP address INADDR_ANY. 
   //It allows our server to receive packets that have been targeted by any of the interfaces

   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY;
   server_addr.sin_port = htons(PORT);   


   //Bind the server socket
   if(bind(server_sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1)
   {
        perror("Bind Failed");
        close(server_sockfd);
        return 0;
   }

   //Listen for incoming connections
   if(listen(server_sockfd, MAX_CLIENTS)==-1)
   {
        perror("Listen failed");
        close(server_sockfd);
        return 0;
   }

   printf("Server listening on port %d...\n",PORT);

   while(1)
   {

	//Accept a new client connections
        client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_addr, &client_address_len);
        if(client_sockfd == -1)
        {
            perror("Accept failed");
            continue;
        }
        
        printf("New connection accepted from %s: %d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

        //create a thread to handle the client

        int *client_socket_ptr = (int *)malloc(sizeof(int));
        *client_socket_ptr = client_sockfd;
            
        if(pthread_create(&thread_id, NULL, handle_client, (void *)client_socket_ptr) ==-1)
        {
            perror("Thread creation failed");
            close(client_sockfd);
            free(client_socket_ptr);
        }
   }

   close(server_sockfd);
   return 0;

}
