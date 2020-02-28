#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <string.h>
#include "parser.h"
#include <assert.h>
#include "shellEngine.h"

pid_t ppid;
int execcmdnextpipe(char* com[buffer],int f0[2],int f){//cette fonction execute la commande et cree un pipe pour la prochaine commande
  int f1[2];
  pipe(f1);
  int dip=fork();

  switch (dip){
    case -1 :
        perror("fork"); 
        exit(-1);
    case 0 :
    
      if(f0[0]!=0)
        {
          close(0);
          dup(f0[0]);
          if(f0[1]!=1)
          close(f0[1]);
          close(f0[0]);
        }
        if(f!=0)
          close(f);
        
      close(1);
      dup(f1[1]);
      close(f1[1]);
      close(f1[0]);
       if(execvp(com[0],com)!=0){
          perror("erreur commande\n");
          exit(-1); 
      }

    default : ;  
    if(f0[0]!=0)
    {
      close(f0[0]); 
      close(f0[1]);
    }
  }

  f0[0]=f1[0];
  f0[1]=f1[1];
  return 0; 
}    
int execcmdlastpipe(char* com[buffer],int f0[2],int f){//cette fonction execute la derniere commande avec le pipe f0 si il existe sinon ca sera une execution sans pipe normal
  
  switch (fork()){
    case -1 :
        perror("fork"); 
        exit(-1);
    case 0 :
        if(f0[0]!=0)
        {
          close(0);
          dup(f0[0]);
          close(f0[0]);
          if(f0[1]!=1)
          close(f0[1]);
        }
        if(f!=0)
        {
          close(1);
          dup(f);
          close(f);
        }
        if(execvp(com[0],com)!=0){
          perror("erreur commande\n");
          exit(-1); 
      }
          
    default :;
  }
  return 0;
}    

int execlignepipe(char* cmd,int f0[2],_Bool nextpipe,int f1){//fonction execute la commande cmd et determine si cest le dernier pipe en fonction de la valeur de nextpipe passé en parametre
 
  char* com[buffer];
  int retour =0;
  Lire_commande(cmd,com," ");
  if (strcmp(com[0], "cd")==0){
   if(f0[0]!=0)
        {
          close(f0[1]);
          close(f0[0]);
          f0[0]=0;
	  f0[1]=1;}
   
     if (com[1]==(char *)0)
     {
       chdir(getenv("HOME"));
     }
    else if (com[2]==(char *)0)
    {
       if(chdir(com[1])!=0)
      {
        printf("erreur commande cd dossier inexistant\n");
        retour=1;
      }
    }
    else 
    {
      perror("trop d'argument pour cd\n");
      retour=1;
    } 
  }
  else if(strcmp(com[0], "getenv")==0){
    if (com[1]!=(char *)0 && com[2]==(char *)0 )
    {
      char* var=getenv(com[1]);
      if(var!=NULL){
        printf("%s\n",var);
      }else{
        printf("la variable d'environnement '%s' n'est pas définie!",com[1]);
        retour=1;
      }
    }else
    {
      printf("erreur d'argument pour la fonction getenv!");
      retour=1;
    }
  }
  else if(strcmp(com[0], "setenv")==0)
  {
    if (com[1]==(char *)0)
    {
      com[0]="env";
      if(nextpipe)
      {
        execcmdnextpipe(com,f0,f1);
      }else
      {
      execcmdlastpipe(com,f0,f1);
      }
    }else if(com[3]==(char *)0 && com[2]!=(char *)0)
    {
      if(f0[0]!=0)
        {
          close(f0[1]);
          close(f0[0]);
          f0[0]=0;
	        f0[1]=1;
          }
   
      setenv(com[1],com[2],1);
    } else{
      printf("erreur de parametre %s",com[0]);
      retour=1;
    }
    
  }
  else
  {
    if(nextpipe){
      execcmdnextpipe(com,f0,f1);
    }else
    {
      execcmdlastpipe(com,f0,f1);
    }
  }
    return retour;            
}




int  execpip(char* cmd,int f0,int f1){//execute cmd et fait le traitement si il y a un pipe
  char *com[buffer];
  int nb_pip;
  int vrcpu;
  int vretour=0;
  int f[2] = { f0, 1 };
  nb_pip=Lire_commande(cmd,com,"|");//ps ax | ls
  for (int i=0;i<nb_pip;i++){
    //printf(">%s\n",com[i]);
    if(execlignepipe(com[i],f,i<nb_pip-1,f1)!=0)
      vretour=1;
      
  }
  if(f[0]!=0){
    close(f[0]);
    if(f[1]!=1)
    close(f[1]);
  }
  if(f1!=0)
  close(f1);
  while(waitpid(0, &vrcpu, 0)!=-1)
    if (vrcpu!=0)
    {
      vretour=1;  
    }    
  return vretour;
}


int execligneredirect(char* cmd)//execute une commande qui contien des redirection et retour 0 si tout les commande sont bien passé sinon retourne 1
{
  int nbl;
  int nbr;
  int retour=0;
  char* redil[buffer];
  char* redir[buffer];
  char* nomfichier[buffer];
  int f0,f1;
  f0=0; //leture
  f1=0; // lecrtiture instalisation a zero
  nbl=Lire_commande(cmd,redil,"<");
  for (int j=0;j<nbl;j++)
  {
    nbr=Lire_commande(redil[j],redir,">");
    for(int l=0;l<nbr-1;l++)
    {
      if(f1!=0){
        close(f1);
        
      }
      Lire_commande(redir[l+1],nomfichier," ");
      f1=open(nomfichier[0],O_WRONLY | O_CREAT | O_TRUNC,0777);
    }
   
  }
   if(nbl>1)
    {
      
      Lire_commande(redil[nbl-1],nomfichier," ");
      f0=open(nomfichier[0],O_RDONLY,0777);
      if(f0<0)
      {
        printf("erreur ouverture du fichier %s\n",nomfichier[0]);
        f0=0;
      }

    }
  Lire_commande(redil[0],redil,">");
  retour=execpip(redil[0],f0,f1);
    if(f1>0){
        close(f1);
      }   
  return retour;       
}
int execor(char* cmd,int exec){
  char *com[buffer];
  int nb_or,retour;
  nb_or=Lire_commande(cmd,com,"||");
  int i=0;
  if (exec==0){
    exec=execligneredirect(com[i]); 
  }
  i++;
  while(i<nb_or && exec!=0){
    exec=execligneredirect(com[i]);
    
    i++;
  }
  return exec;
}
int execand(char* cmd){
  char *com[buffer];
  int nb_and,vrcpu;
  int vretour=0;
  int i=0;
  int exec=0;
  nb_and=Lire_commande(cmd,com,"&&");
   for (int i=0;i<nb_and;i++){
      exec=execor(com[i],exec);
  }
 
  return exec;
}
int execpvirg(char* cmd){
  char *com[buffer];
    int nb_pvirg;
    nb_pvirg=Lire_commande(cmd,com,";");
  for (int i=0;i<nb_pvirg;i++){
      execand(com[i]);
      
  }
  return 0;
}

void stopexecquit(int sig) {
    pid_t pid = getpid();

    assert(sig == SIGQUIT);
   
    if (ppid != pid) 

      _exit(0);
}
void stopexecint(int sig) {
    pid_t pid = getpid();

    assert(sig == SIGINT);
   
    if (ppid != pid) 
      _exit(0);

}
int execbg(char* cmd){
  int retour =0;
  char *com[10];
  int nb;
  Lire_commande(cmd,com,"&&");
  if(com[0]==(char *)0)return 1;
  if (strstr(com[0],"&")!=NULL){
    Lire_commande(com[0],com,"&");
    Lire_commande(com[0],com," ");
    retour =1;
    switch (fork()){
      case -1 :
          perror("fork"); 
          exit(-1);
      case 0 :
        setpgid(getpid(),getpid());
        if(execvp(com[0],com)!=0){
          perror("erreur commande\n");
          exit(-1); 
        }
            
      default :;
  }

  }
  
  return retour;
}
void mon_shell(){
    int no, status,nbpipe;
    char cmd[100];
    char *com[buffer];
    ppid=getpid();
    signal(SIGQUIT,stopexecquit);
    signal(SIGINT,stopexecint);
    printf("Voici mon shell, taper Q pour sortir\n");
    printf("> ");
    gets(cmd);
    while (strcmp(cmd,"Q")!=0){
        if(execbg(cmd)==0)
          execpvirg(cmd);
          
          printf("\n> ");
          
          gets(cmd);
    }
}