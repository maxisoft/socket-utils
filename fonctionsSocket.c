/*
 **********************************************************
 *
 *  Programme : fonctionsSocket.c
 *
 *  ecrit par : LP.
 *
 *  resume :    fonction d'initialisation des sockets en modes 
 *              connecte et non-connecte
 *
 *  date :      25 / 01 / 06
 *
 ***********************************************************
 */

// definitions
#include "fonctionsSocket.h"

/*
 **********************************************************
 *
 *  function  : socketServeur
 *
 *  resume :    creer la socket du serveur et la retourne
 *              socketServeur(numero de port)
 *
 ***********************************************************
 */
int socketServeur(unsigned short port) {          
  int sock,          /* descripteur de la socket */
      err;	     /* code d'erreur */

  struct sockaddr_in nom;	     /* adresse de la sochet */
  
  socklen_t      size_addr;  	/* taille de l'adresse d'une socket */
  
  size_addr = sizeof(struct sockaddr_in);
  
  /* creation de la socket, protocole TCP */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socketServeur : erreur de socket");
    return ERR_SOCKET;
  }
  
  /* 
   * initialisation de l'adresse de la socket 
   */
  nom.sin_family = AF_INET;
  nom.sin_port = htons(port);
  nom.sin_addr.s_addr = INADDR_ANY;
  bzero(nom.sin_zero, 8);
  
  /*
   * mode SO_REUSEADDR pour eviter les: "address already in use"
   */
  int on = 1;
  err = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  if (err < 0)
    perror("socketServeur : erreur setsockopt");
  	 
  
  /* 
   * attribution de l'adresse a la socket
   */
  err = bind(sock, (struct sockaddr *)&nom, size_addr);
  if (err < 0) {
    perror("socketServeur : erreur sur le bind");
    return ERR_BIND;
  }
  
  /* 
   * Utilisation en socket de controle, puis attente de demandes de 
   * connexion.
   */
  err = listen(sock, 1);
  if (err < 0) {
    perror("socketServeur : erreur dans listen");
    return ERR_LISTEN;
  }
  
  /*
   * retourne le descripteur de socket
   */
  return sock;
}

/*
 **********************************************************
 *
 *  fonction : socketClient
 *
 *  resume :    fonction de connexion d'une socket au serveur
 *              socketClient(nom de machine serveur, numero de port)
 *
 ***********************************************************
 */

int socketClient(char  *nomMachine, unsigned short port) {
  int sock,               /* descripteur de la socket */
      err;                /* code d'erreur */

  struct hostent*    host; /* description de la machine serveur */
  struct sockaddr_in nom;  /* adresse de la socket du serveur */

  socklen_t size_addr_in = sizeof(struct sockaddr_in);


  /* 
   * creation d'une socket, domaine AF_INET, protocole TCP 
   */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socketClient : erreur sur la creation de socket");
    return ERR_SOCKET;
  }
  
  /* initialisation de l'adresse de la socket */
  nom.sin_family = AF_INET;
  bzero(nom.sin_zero, 8);
  nom.sin_port = htons(port);

  /* 
   * recherche de l'adresse de la machine
   */
  host = gethostbyname (nomMachine);
  if (host == NULL) {   
    perror("socketClient : erreur gethostbyname");
    return ERR_GETHOST;
  }
  
  /*
   * recopie de l'adresse IP
   */
  nom.sin_addr.s_addr = ((struct in_addr *) (host->h_addr_list[0]))->s_addr;
					     
  /* 
   * connection au serveur 
   */
  err = connect(sock, (struct sockaddr *)&nom, size_addr_in);
  if (err < 0) {
    perror("socketClient : erreur a la connection de socket");
    return ERR_CONNECT;
  }
  
  /*
   * on retourne le descripteur de socket
   */
  return sock;
}
 

/*


/*******************************************
 *
 * fonction d'initialisation d'une socket non connectee locale
 *
 *******************************************/
int socketUDP (unsigned short port) {
  int sock,          /* descripteur de la socket */
       err;	     /* code d'erreur */

  struct sockaddr_in nom;	     /* adresse de la sochet */
  
  socklen_t       size_addr;  	/* taille de l'adresse d'une socket */
  
  
  size_addr = sizeof(struct sockaddr_in);
  
  /* creation de la socket, protocole UDP */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socketUDP : erreur de socket");
    return ERR_SOCKET;
  }
  
  /* 
   * initialisation de l'adresse de la socket 
   */
  nom.sin_family = AF_INET;
  nom.sin_port = htons(port);
  nom.sin_addr.s_addr = INADDR_ANY;
  bzero(nom.sin_zero, 8);
  
  /*
   * mode SO_REUSEADDR pour eviter les: "address already in use"
   */
  int on = 1;
  err = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  if (err < 0) 
    perror("serveur : erreur setsockopt");
  
  /* 
   * attribution de l'adresse a la socket
   */
  err = bind(sock, (struct sockaddr *)&nom, size_addr);
  if (err < 0) {
    perror("socketUDP : erreur sur le bind");
    return ERR_BIND ;
  }
  
  /*
   * retourne le descripteur de socket
   */
  return sock;
}

/*******************************************
 *
 * fonction de creation d'une structure d'adresse
 *
 *   attention, dans cette fonction l'adresse est allouee dynamiquement
 *   il faut eventuellement penser a la liberer
 *
 *******************************************/
struct sockaddr_in* initAddr(char* nom_machine, unsigned short port) {
  struct sockaddr_in* paddr;
  struct hostent* host;

  /* allocation de la structure d'adresse */
  paddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  if (paddr == NULL) {
    perror("initAddr : erreur malloc");
    return NULL;
  }
  
  /* cherche l'adresse de la machine 
   */
  host = gethostbyname(nom_machine);
  if (host == NULL) {
    printf("initAddr : erreur gethostbyname");
    return NULL;
  }

  /*
   * recopie de l'adresse IP
   */
  paddr->sin_addr.s_addr = ((struct in_addr *) (host->h_addr_list[0]))->s_addr;
      
  /* initialisation de l'adresse de la socket */
  paddr->sin_family = AF_INET;
  bzero(paddr->sin_zero, 8);
  paddr->sin_port = htons(port);

  return paddr;
}


/*******************************************
 *
 * fonction pour obtenir la taille d'une adresse
 *
 *******************************************/
int sizeAddr() {
  return sizeof(struct sockaddr_in);
}
