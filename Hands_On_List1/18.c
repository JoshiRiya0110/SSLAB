#include<stdio.h>
#include<fcntl.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
   if(argc!=2)
	printf("Missing arguments ");
   struct
   {
	int train_num;
	int ticket_count;
   }database[3];

   for(int i=0; i<3; i++)
   {
	database[i].train_num = i+1;
        database[i].ticket_count = 0;
   }
   
   int fd  = open(argv[1], O_CREAT | O_EXCL | O_RDWR , S_IRUSR | S_IWUSR);
   write(fd, database, sizeof(database));
   fd = open(argv[1], O_RDWR);
   printf("Select train number: 1, 2, 3\n");
   int input;
   scanf("%d",&input);
   struct flock lock;
   lock.l_type = F_WRLCK;
   lock.l_whence = SEEK_SET;
   lock.l_start = (input-1)*sizeof(database);

   lock.l_len = sizeof(database);
   lock.l_pid = getpid();
   
   lseek(fd,(input-1)*sizeof(database),SEEK_SET);
   read(fd, &database, sizeof(database));
   printf("Before entering critical section\n");

   fcntl(fd, F_SETLKW, &lock);
   printf("Current ticket count: %d",database->ticket_count);
   database->ticket_count++;
  
   lseek(fd,-1*sizeof(database),SEEK_CUR);
   write(fd,&database,sizeof(database));
   printf("To book ticket, press enter\n");
   getchar();
   getchar();
   lock.l_type = F_UNLCK;
   fcntl(fd, F_SETLK, &lock);
   printf("Ticket booked with number %d\n",database->ticket_count);
}
