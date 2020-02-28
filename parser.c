#include<string.h>
#include <stdlib.h>
#include <stdio.h>

void reset_tab(char* tab[10])    //reinitiliser le tableau donnee en paramettre
{
for (int i=0;i<10;i++)
      tab[i]=(char *)0;
}
char * split_string( char *s, const char *delimiter )//fonction recuperer du site stackoverflow.com elle fait le meme fonctionnement que strtock sauf que ca prend en charge une chaine de carractere comme delimiteur 
{                                                    // direct link :https://stackoverflow.com/questions/34675966/strtok-when-the-delimiter-is-string
    static char *p;

    if ( s ) p = s;

    size_t n = strlen( delimiter );

    if ( p && n != 0 )
    {
        while ( memcmp( p, delimiter, n ) == 0 ) p += n;
    }

    if ( !p || !*p ) return p = NULL;

    char *t = p;

    if ( n != 0 ) p = strstr( t, delimiter );

    if ( n == 0 || !p )
    {
        p = t + strlen( t );
    }
    else
    {
        *p = '\0';
        p += n;
    }
    return t;
}

int Lire_commande(char* commande,char* tab[10],char* sep)
{
    int i=0;
    char* token;
    reset_tab(tab);
    char* temp = calloc(strlen(commande)+1, sizeof(char));
    strcpy(temp, commande);

    token = split_string(temp, sep);
    
    while(token!=NULL){
        tab[i]=token;
        i++;
        token = split_string(NULL, sep);
    }
    return i;
}