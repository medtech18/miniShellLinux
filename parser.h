#ifndef PARSER_H
#define PARSER_H

void reset_tab(char* tab[10]); //reinitiliser le tableau donnee en paramettre
char * split_string( char *s, const char *delimiter);//fonction recuperer du site stackoverflow.com elle fait le meme fonctionnement que strtock sauf que ca prend en charge une chaine de carractere comme delimiteur 
                                                    // direct link :https://stackoverflow.com/questions/34675966/strtok-when-the-delimiter-is-string
int Lire_commande(char* commande,char* tab[10],char* sep);

#endif
