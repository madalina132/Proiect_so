#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
typedef struct
{
 char signature[2];
 int file_size;
 int reserved;
 int data_offset;
}Antet;

typedef struct
{
    int size;
int latime;
int lungime;
int x_pixel;
int y_pixel;
int user_id;
int date;
char drept_user;

}Info_antet;
struct stat st;
int main(int argc,char*argv[])
{
    char buff[1000];
    int rez3;
    int rez;
    int rez2;
    int rez4;
    if(argc==2)
    {    
        //printf("verif2");
        printf("\n");
        rez=open(argv[1],O_RDWR);

        if(rez==-1)
        {
            perror("de ce nu merge ?");
            exit(-1);
        }
        else
        {
        
           Antet antet;
           Info_antet info;
         rez2= read(rez, &antet, sizeof(Antet));
            if(rez2==-1)
             {
                //printf("verif2");
                printf("\n");
                exit(-1);
             }    
              rez3= read(rez, &info, sizeof(Info_antet));
            if(rez3==-1)
             {
                //printf("verif2");
                printf("\n");
                exit(-1);
             }  
              rez4=creat("statistica.txt",S_IRUSR|S_IWUSR);
              if(rez4==-1)
              {
                perror("de ce nu s a creat fisierul ?");
                exit(-1);
              }
              else
              {
                  stat(argv[1],&st);
                  printf("user:%d\n",st.st_size);
                  printf("verif2");
                  printf("\n");
                sprintf(buff,"%d",info.latime);
                printf("%s",buff);
                 printf("\n");
                   write(rez4,buff,strlen(buff));
                 sprintf(buff,"%d",info.lungime);
                printf("%s",buff);
                 printf("\n");
                   write(rez4,buff,strlen(buff));
                 sprintf(buff,"%d",info.x_pixel);
                printf("%s",buff);
                 printf("\n");
                   write(rez4,buff,strlen(buff));
                   sprintf(buff,"%d",info.y_pixel);
                printf("%s",buff);
                  write(rez4,buff,strlen(buff));
                 printf("\n");
                write(rez4,argv[1],strlen(argv[1]));
               // write(rez4,buff,strlen(buff));eeee
                  printf("da");
              }
             printf(" numele :%s",argv[1]);
             printf("\n");
             printf("latime:%d\n",info.latime);
             printf("lungime:%d\n",info.lungime);  
               printf("pixel:%d\n",info.x_pixel);
                printf("pixel:%d\n",info.y_pixel);
                printf("data:%d\n",info.date);
                 printf("drept:%d\n",info.drept_user);

        }
    }
    else{
         printf("Usage ./program imagine.bmp");

    }

}