#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>



int main()
  {
    char nazwa[100];
    char plik_map[] ="./plik_map";
    int id,fd,fd2;
    void *wsk = NULL;
    struct stat sfile;

    id=fork();
    if(id==0)
      {
        printf("Proces potomny wyswietlajacy plik\n");
        execlp("display","display","-update","1",plik_map,NULL);

      }
    else if(id>0)
      {
        while(1)
        {
        printf("Podaj nazwe pliku do przeslania:\n");
        scanf("%s", nazwa);
        fd=open(nazwa,O_RDONLY);
        stat(nazwa, &sfile); //pobranie statystyk o pliku
        fd2=open(plik_map, O_RDWR | O_CREAT); //otwarcie pliku zmapowanego
        ftruncate(fd2,sfile.st_size); //zmiana rozmiaru pliku
        wsk=mmap(NULL,sfile.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
        read(fd,wsk,sfile.st_size);
        msync(wsk,sfile.st_size,MS_SYNC);
        close(fd);
        close(fd2);
        }
      }
    else
      {
        printf("Fork nie powiodl sie\n");
        return -1;
      }
      munmap(wsk,sfile.st_size);
      return 0;

  }
