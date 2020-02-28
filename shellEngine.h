#ifndef SHELLENGINE_H
#define SHELLENGINE_H
#define buffer 10

int execcmdredirect(char* cmd,char* fichier,int mode,int cl);

int execcmdnextpipe(char* com[10],int f0[2],int f);

int execcmdlastpipe(char* com[buffer],int f0[2],int f);

int execlignepipe(char* cmd,int f0[2],_Bool nextpipe,int f1);

int execpip(char* cmd,int f0,int f1);

int execligneredirect(char* cmd);

int execor(char* cmd,int exec);

int execpvirg(char* cmd);

void stopexecquit(int sig);

void stopexecint(int sig) ;

int execbg(char* cmd);

void mon_shell();

#endif
