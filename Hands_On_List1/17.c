#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, char* argv[])
{
   struct
   {
      int ticket_no;
   }database;

   database.ticket_no = 1;
   int fd = open(argv[1], O_CREAT | O_EXCL | O_RDWR , S_IRUSR | S_IWUSR);
   if(fd == -1)
   {
	perror("error");
   }

   write(fd, &database, sizeof(database));
   close(fd);

   fd = open(argv[1],O_RDONLY);
   read(fd, &database, sizeof(database));
   printf("Ticket no : %d\n",database.ticket_no);
   close(fd);

   struct flock lock;
   fd = open(argv[1], O_RDWR);
   printf("Before entering into critical section");
   fcntl(fd, F_SETLKW, &lock);
   printf("Inside the critical section\n");

   read(fd, &database, sizeof(database));
   printf("Current ticket number: %d\n",database.ticket_no);
   database.ticket_no++;
 
   lseek(fd, 0, SEEK_SET);
   write(fd, &database, sizeof(database));

   printf("Press enter to unlock\n");
   getchar();

   lock.l_type = F_UNLCK;
   fcntl(fd, F_SETLK, &lock);
   
   printf("Exited critical section\n");
   
}
