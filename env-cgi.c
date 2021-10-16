#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>

char json[100];
char *Pjson=&json[0];
static FILE *f;

void addTime(void)
{
    time_t timer;
    char Time[30];
    time(&timer);
    char *t=ctime(&timer);
    if(t[strlen(t)-1]=='\n')
        t[strlen(t)-1]=' ';
    sprintf(Time,"\"time\":\"%s\"",t);
    Pjson=strcat(Pjson,Time);
}

int containsChar(char *string, char ch)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if(*(string+i)==ch)
            return i;
    }
    return 0;
}

void split_query(char *string){
    
    char *part1;
    char *part2;
    int index=1;
    char j[10]=" ";
    char *pj=&j[0];
    while (index)
    {
        char t1[10]="";
        char t2[10]="";
        char *text1=&t1[0];
        char *text2=&t2[0];
        part1=memchr(string,'=',strlen(string));

        if((index=containsChar(part1,'&')))
            part2=memchr((part1+index),'&',strlen(part1));
        else
            part2=memchr((part1+index),'=',strlen(part1));

        strncat(text1,string,(strlen(string)-strlen(part1)));
        strncat(text2,(string+(strlen(string)-strlen(part1))+1),(strlen(part1)-strlen(part2)-1));
        sprintf(string,"%s",(part2+1));
        sprintf(pj,"\"%s\": \"%s\",",text1,text2);
        Pjson=strcat(Pjson,pj);
    }
}

int main (int argc, char *argv[]) 
{ 
    char *envPtr;

    printf ("Content-type: text/html\n"); 
    printf ("\n"); 
    printf ("<HTML>\n"); 
    printf ("<HEAD>\n"); 
    printf ("<TITLE>CGI Environment Variable</TITLE>\n"); 
    printf ("</HEAD>\n"); 
    printf ("<BODY>\n"); 

    envPtr= getenv ("REQUEST_METHOD"); 
    printf ("REQUEST_METHOD= "); 
    if (!envPtr) 
        printf ("<NULL-POINTER>\n"); 
    else 
        printf ("%s\n", envPtr); 
    printf ("<br>\n");

    envPtr = getenv("CONTENT_LENGTH");
    long len = strtol(envPtr, NULL, 10);
    envPtr = malloc(len + 1);
    printf("query= ");
    if (!envPtr)
    {
        printf ("<NULL-POINTER>\n"); 
    }
    else
    {
        fgets(envPtr, len + 1, stdin);
        printf ("q=%s",envPtr);
        split_query(envPtr);
        addTime();
        printf ("<br>\n");
        printf("{%s}\n",Pjson);
        f=fopen("/var/www/html/info.json","w");
        if(f)
        {
            fprintf(f,"{%s}",Pjson);
            fclose(f);
        }
    }
    free(envPtr);

    printf ("<br>\n");
    printf ("</BODY>\n");
    printf("<script>");
    printf("window.addEventListener('load',()=>{");
    printf("window.location.replace(\"http://rpi-arnev/index.html\")})");
    printf("</script>");
    printf ("</HTML>\n");
    return (EXIT_SUCCESS); 
}