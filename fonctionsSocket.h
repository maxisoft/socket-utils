/*
 **********************************************************
 *
 *  Entete : fonctionsSocket.h
 *
 *  ecrit par : LP.
 *
 *  resume :    prototype des fonctions socket
 *
 *  date :      25 / 01 / 06
 *
 ***********************************************************
 */

/* include generaux */
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

/* include socket */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

// codes erreur des fonctions
#define ERR_SOCKET  -1
#define ERR_BIND    -2
#define ERR_LISTEN  -3
#define ERR_GETHOST -4
#define ERR_CONNECT -5

// mode connecte
int socketServeur(unsigned short port);
int socketClient(char *nomMachine, unsigned short port);

// mode non connecte
struct sockaddr_in* initAddr(char* nom_machine, unsigned short port);
int socketUDP(unsigned short port);
int sizeAddr();

