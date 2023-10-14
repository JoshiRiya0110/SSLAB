#include<stdio.h>
#include<stdlib.h>

typedef struct Admin{
        int userID; 
        char username[30];
        char password[10];
}Admin;

int main()
{
	Admin user;
	printf("Enter userId : ");
	scanf("%d",&user.userID);
	printf("Enter password: ");
	scanf("%s", user.username);  

	printf("Hello");
}
