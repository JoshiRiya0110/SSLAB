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


#define PORT 8093
#define MAX_CLIENTS 5

bool checkAdmin(Admin currUser);
bool checkProfessor(Professor currUser);
int checkStudent(Student currUser);
void processAdminMenu(int nsd, int select);
void processProfessorMenu(int nsd, int select);
void processStudentMenu(int nsd, int select);



//**************************************************************************************ADMIN FUNCTIONS*****************************************************

bool add_student(int nsd, Student record){
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
	record.userID=endUser.userID+1;

	printf("last user id %d\n",endUser.userID);
	printf("new user id %d\n",record.userID);


	//printf("new user userid %d\n",record.userID);
	int j=write(fd,&record,sizeof(Student));
	if(j!=0)	result=true;
	else	result=false;
	

	write(nsd, &record, sizeof(record));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}




Student display_student(int id){

	printf("Entered function to check for admin" );
	int i=id;
	i = i - 1000;
	printf("\n i value : %d\n",i);
	int fd=open("SUfile",O_RDWR,0744);
	
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(Student);    	     //nth record
	lock.l_len=sizeof(Student);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(Student),SEEK_SET);  //changing the file pointer to the selected record
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



Professor displayFaculty(int id){

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
	printf("Entered function to check for admin" );
	int status;
	int i=id;
	i = i - 1000;
	//printf("\n i value : %d\n",i);
	int fd=open("SUfile",O_RDWR,0744);
	
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
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
	printf("Entered function to check for admin" );
	int status;
	int i=id;
	i = i - 1000;
	//printf("\n i value : %d\n",i);
	int fd=open("SUfile",O_RDWR,0744);
	
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
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
	lock.l_type = F_RDLCK;
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
	lock.l_type = F_RDLCK;
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

//*****************************************************Professor functions**********************************************************

void displayOfferedCourses(int nsd)
{
		printf("displayofferedcourses\n");
		int profId;
		read(nsd, &profId, sizeof(profId));

		//now i have to fetch this particular profid structure
		Professor temp;
		int fd1 = open("PUfile",O_RDWR,0744);
		int i1  =profId;
		i1 = i1 - 1000;
		printf("i1 %d\n",i1);

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

				printf("tempcourse id %d",tempCourse.courseID);
				printf("tempcourse name %s",tempCourse.name);


				write(nsd,&tempCourse,sizeof(tempCourse));
				printf("tempcourse id %d",tempCourse.courseID);
				printf("tempcourse name %s",tempCourse.name);


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
	lock1.l_type = F_RDLCK;
	lock1.l_whence=SEEK_SET;
	lock1.l_start=(i)*sizeof(Professor);    	     //nth record
	lock1.l_len=sizeof(Professor);	             //sizeof(record)
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

		//printf("new user userid %d\n",course.userID);
		//maine jo client ne course tsructure bheja tha usko file me likh diya
		lseek(fd2,sizeof(Courses),SEEK_END); 
		int j=write(fd2,&course,sizeof(Courses));




		if(j!=0)	
			result=true;
		else
			result=false;


		//ab mujhe professor ke structure ke andar courseid array me ye partcular course id daalni hai
		for(int k=0 ; k<4 ; k++)			
		{
			if(temp.courseID[k] == -1)
			{
				temp.courseID[k] = course.courseID;

				//ab mujhe professor ka modified structure vapis se file ke andar likhna hai
				lseek(fd1,(i)*sizeof(Professor),SEEK_SET);
				write(fd1,&temp,sizeof(Professor));

				//we are sending the new course we added in order to print the course id on the client side
				write(nsd, &result, sizeof(result));
				write(nsd, &course, sizeof(Courses));


				printf(" 575  %d\n",temp.courseID[k]);
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

//*****************************************************Student functions*************************************************************

void changePassword(int nsd, int id)
{
	char changePass[10];
	int check;
	read(nsd, &changePass, sizeof(changePass));
	id = id-1000;

	int fd=open("SUfile",O_RDWR,0744);
	
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
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


//**************************************************************************************************************************************


void serverTask(int nsd){

	int msgLength,select,type,option,accType,currUserID;
	bool result;
	while(1){
		read(nsd,&option,sizeof(option));                            // client is sending this--> write(sock_fd,&option,sizeof(int));
		printf("Option : %d\n",option);

		if(option==1){

                        Admin currUser1;
                        
                        msgLength=read(nsd,&currUser1,sizeof(Admin)); 	     // write(sock_fd,&currUser,sizeof(Admin));
                        currUserID=currUser1.userID;
                        printf("Username : %d\n",currUser1.userID);
                        printf("Password : %s\n",currUser1.password);                           // we should not print the password
						printf("called func above");
                        result=checkAdmin(currUser1);
						printf("called func below %d\n",result);
                        write(nsd,&result,sizeof(result));
		}

		else if(option==2){

						Professor currUser2;
							
						msgLength=read(nsd,&currUser2,sizeof(Professor));     // write(sock_fd,&currUser,sizeof(Professor));
						printf("Username : %d\n",currUser2.userID);
						printf("Password : %s\n",currUser2.password);
						currUserID=currUser2.userID;
						result=checkProfessor(currUser2);
						write(nsd,&result,sizeof(result));
		}
		else if(option==3){

						Student currUser3;
						int check;
						int id;

						msgLength=read(nsd,&currUser3,sizeof(Student));
						currUserID=currUser3.userID;
						printf("Username : %d\n",currUser3.userID);
						check = checkStudent(currUser3);
						write(nsd,&check,sizeof(check));
						read(nsd, &id, sizeof(id));
						if(check == 3)
							changePassword(nsd, id);
		}
		else{

						result=false;
						write(nsd,&result,sizeof(result));
		}
		if(result)
		{
						break;
		}
	}



	while(1){

		read(nsd,&select,sizeof(int));

		if(option == 1)
		{
			processAdminMenu(nsd, select);
			//break;
		}
			
		else if(option == 2)
		{
			printf("server task part2\n");
			processProfessorMenu(nsd, select);
			//break;
		}

		else
		{
			processStudentMenu(nsd, select);
			//break;
		}
		
	/*	if(option==1 || option==2){
			if(select==1){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=depositMoney(accType,currUserID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(select==2){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=withdrawMoney(accType,currUserID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(select==3){
				float amt;
				amt=getBalance(accType,currUserID);
				write(nsd,&amt,sizeof(float));
			}
			else if(select==4){
				char pwd[10];
				read(nsd,pwd,sizeof(pwd));
				result=alterPassword(accType,currUserID,pwd);
				write(nsd,&result,sizeof(result));
			}
			else if(select==5){
				if(option==1){
					normalUser user1=getNormalUser(currUserID);
					write(nsd,&user1,sizeof(normalUser));
				}
				else if(option==2){
					jointUser user2=getJointUser(currUserID);
					write(nsd,&user2,sizeof(jointUser));
				}
			}
			else if(select==6)	break;
		}
		else if(option==3){
			read(nsd,&type,sizeof(int));
			if(select==1){
				if(type==1){
					normalUser newUser1;
					read(nsd,&newUser1,sizeof(normalUser));
					result=addNormalUser(newUser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					jointUser newUser2;
					read(nsd,&newUser2,sizeof(jointUser));
					result=addJointUser(newUser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==2){
				if(type==1){
					int delUserID1;
					read(nsd,&delUserID1,sizeof(int));
					result=deleteNormalUser(delUserID1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					int delUserID2;
					read(nsd,&delUserID2,sizeof(int));
					result=deleteJointUser(delUserID2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==3){
				if(type==1){
					normalUser modUser1;
					read(nsd,&modUser1,sizeof(normalUser));
					result=modifyNormalUser(modUser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					jointUser modUser2;
					read(nsd,&modUser2,sizeof(jointUser));
					result=modifyJointUser(modUser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==4){
				if(type==1){
					normalUser searchUser1;
					int userID1;
					read(nsd,&userID1,sizeof(int));
					searchUser1=getNormalUser(userID1);
					write(nsd,&searchUser1,sizeof(normalUser));
				}
				else if(type==2){
					jointUser searchUser2;
					int userID2;
					read(nsd,&userID2,sizeof(int));
					searchUser2=getJointUser(userID2);
					write(nsd,&searchUser2,sizeof(jointUser));
				}
			}
			else if(select==5)	break;
		}
*/
	}
	//close(nsd);
	//write(1,"Ended client session.....\n",sizeof("Ended client session.....\n"));
	return;
}


void processAdminMenu(int nsd, int select)
{
	bool result;
	switch(select)
	{
		case 1:
				Student newUser1;
               	read(nsd,&newUser1,sizeof(Student));
               	result=add_student(nsd, newUser1);
               	write(nsd,&result,sizeof(result));
				break;

		case 2:
				int id1;
				Student newUser2;
				read(nsd,&id1,sizeof(id1));
				newUser2=display_student(id1);
				write(nsd,&newUser2,sizeof(Student));
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
	}
}

void processProfessorMenu(int nsd, int select)
{
	switch(select)
	{
		case 1:
				printf("ProcessProfessorMenu\n");
				displayOfferedCourses(nsd);
				break;
		case 2:
				int id;
				read(nsd,&id,sizeof(id));
				addNewCourse(nsd, id);
				break;
		case 3:

		case 4:
		case 5:
	}
}

void processStudentMenu(int nsd, int select)
{
	bool result;
	switch(select)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
				int profId;
            	read(nsd,&profId,sizeof(profId));
                changePassword(nsd,profId);
                break;


	}
}


bool checkAdmin(Admin currUser){

	printf("Entered function to check for admin" );
	int i=currUser.userID;
	i = i - 1000;
	printf("\n i value : %d\n",i);
	int fd=open("AUfile",O_RDWR,0744);
	
	bool result;
	Admin temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(Admin);    	     //nth record
	lock.l_len=sizeof(Admin);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(Admin),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Admin));
	// printf("pass temp %s:::  pass temp len %ld\n",temp.password,strlen(temp.password));
	// printf("pass currUser %s::: pass currUser len %ld\n",currUser.password,strlen(currUser.password));
	// currUser.password[strlen(currUser.password)-1] = '\0';
	int comp = strcmp(temp.password,currUser.password);
	// printf("Comp :: %d\n",comp);
	
	if(comp==0)
	{
			result=true;
			printf("finding result %d",result);
	}
	else
	{
		result=false;
	}						

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}


bool checkProfessor(Professor currUser){
	int i=currUser.userID-1000;
	int fd=open("PUfile",O_RDONLY,0744);
	bool result;
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


	if(!strcmp(temp.password,currUser.password))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

int checkStudent(Student currUser){
	int i = currUser.userID;
	i = i-1000;
	int fd = open("SUfile",O_RDWR,0744);
	int result;
	Student temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(Student);    	     //nth record
	lock.l_len=sizeof(Student);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(Student),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(Student));

	// currUser.password[strlen(currUser.password)-1] = '\0';
	 printf("len1 %ld\n", strlen(temp.password));
	 printf("len2 %ld\n",strlen(currUser.password));


	//currUser.password[strlen(currUser.password)] = '\0';
	int comp = strcmp(temp.password,currUser.password);
	printf("Comp :: %d\n",comp);
	
	// if(comp==0)
	// {
	// 		result=true;
	// 		printf("finding result %d",result);
	// }
	// else
	// {
	// 	result=false;
	// }		



	if(!strcmp(temp.password,currUser.password) )
	{
		if(!strcmp(temp.password,"iiitb"))
		{
			result = 3;
		}
		else if(temp.isAccessAllowed == 1)
		{
			result = 1;
		}
		
		else
		{
			result = 2;
		}		
		
	}
	else
	{
		result=0;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

//**************************************************************************************
void *handle_client(void* arg)
{
   int client_socfd = *((int*)arg);
   char buffer[1024];
   int read_size;

   //Handle client communication
// ************   while((read_size = recv(client_socfd, buffer, sizeof(buffer),0)) > 0)
//    {
	serverTask(client_socfd);
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

   close(client_socfd);
   free(arg);
   return NULL;

}





int main()
{
   int server_socfd;
   int client_socfd;
   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;
   pthread_t thread_id;
   socklen_t client_address_len = sizeof(client_addr);

   //create a server socket and store its fd

   server_socfd = socket(AF_INET,SOCK_STREAM,0);
   if(server_socfd == -1)
   {
	perror("Error in socket creation");
	return 0;
   }
   

   //Initialize server address
   memset(&server_addr, 0, sizeof(server_addr));
   

   /*type of socket created, INADDR_ANY is an IP address that is used when we don't want to bind a socket to any specific IP.When we don't know the IP address of our machine, we can use the special IP address INADDR_ANY. It allows our server to receive packets that have been targeted by any of the interfaces*/

   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY;
   server_addr.sin_port = htons(PORT);   


   //Bind the server socket
   if(bind(server_socfd,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1)
   {
	perror("Bind Failed");
	close(server_socfd);
	return 0;
   }


   //Listen for incoming connections
   if(listen(server_socfd, MAX_CLIENTS)==-1)
   {
	perror("Listen failed");
        close(server_socfd);
	return 0;
   }


   printf("Server listening on port %d...\n",PORT);

   while(1)
   {

	//Accept a new client connections

	client_socfd = accept(server_socfd,(struct sockaddr *)&client_addr, &client_address_len);
	if(client_socfd == -1)
	{
	   perror("Accept failed");
	   continue;
	}
	
	printf("New connection accepted from %s: %d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));


	//create a thread to handle the client

	int *client_socket_ptr = (int *)malloc(sizeof(int));
	*client_socket_ptr = client_socfd;
        
	if(pthread_create(&thread_id, NULL, handle_client, (void *)client_socket_ptr) ==-1)
	{
	   perror("Thread creation failed");
	   close(client_socfd);
	   free(client_socket_ptr);
	}
   }

   close(server_socfd);
   return 0;

}
