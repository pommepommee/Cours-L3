#include <stdlib.h>     /* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/socket.h> /* Pour socket */
#include <arpa/inet.h>  /* Pour sockaddr_in, IPPROTO_TCP */
#include <stdio.h>      /* Pour perror */
#include <unistd.h>     /* Pour close */
#include <string.h>     /* Pour memset */
#include <time.h>

#include <pthread.h>
#include "includes.h"
#include "message.h"

int main(int argc, char *argv[]) {
  struct sockaddr_in adresse;
  struct sockaddr p1, p2;
  /*size_t taille;*/
  requete_t requete;
  reponse_t reponse;
  socklen_t len = sizeof(struct sockaddr_in);
  socklen_t p_len = sizeof(struct sockaddr);
  int sockfdTCP, sockfdUDP, sock_one, sock_two;
  /*char *msg, *msgenvoi;
  msgenvoi = "";*/

  /* Vérification des arguments */
  if(argc != 2) {
    fprintf(stderr, "Usage : %s port\n", argv[0]);
    fprintf(stderr, "Où :");
    fprintf(stderr, "\n\tport : le numéro de port d'écoute du serveur\n");
    exit(EXIT_FAILURE);
  }

  /*
   *
   * PARTIE UDP
   *
   */
   /* Création de la socket */
   if((sockfdUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
     perror("Erreur lors de la création de la socket ");
     exit(EXIT_FAILURE);
   }

   /* Création de l'adresse du serveur */
   memset(&adresse, 0, sizeof(struct sockaddr_in));
   adresse.sin_family = AF_INET;
   adresse.sin_port = htons(atoi(argv[1]));
   adresse.sin_addr.s_addr = htonl(INADDR_ANY);

   /* Nommage de la socket */
   if(bind(sockfdUDP, (struct sockaddr*)&adresse, sizeof(struct sockaddr_in)) == -1) {
     perror("Erreur lors du nommage de la socket ");
     exit(EXIT_FAILURE);
   }

   if(getsockname(sockfdUDP,(struct sockaddr*)&adresse,&len)==-1){
     perror("Erreur getsockname");
     exit(EXIT_FAILURE);
   }

   /* Attente de la requête du joueur 1*/
   printf("Serveur en attente de la connexion du joueur 1.\n");
   if(recvfrom(sockfdUDP, &requete, sizeof(requete), 0, (struct sockaddr*)&p1, &p_len) == -1) {
     perror("Erreur lors de la réception du message ");
     exit(EXIT_FAILURE);
   }

   /* Attente de la requête du joueur 2*/
   printf("Serveur en attente de la connexion du joueur 2\n");
   if(recvfrom(sockfdUDP, &requete, sizeof(requete), 0, (struct sockaddr*)&p2, &p_len) == -1) {
     perror("Erreur lors de la réception du message ");
     exit(EXIT_FAILURE);
   }

  /*
  *
  * PARTIE TCP
  *
  */

  /* Création de la socket */
  if((sockfdTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    perror("Erreur lors de la création de la socket ");
    exit(EXIT_FAILURE);
  }

  /* Création de l'adresse du serveur */
  memset(&adresse, 0, sizeof(struct sockaddr_in));
  adresse.sin_family = AF_INET;
  adresse.sin_addr.s_addr = htonl(INADDR_ANY);
  adresse.sin_port = htons(0);

  /* Nommage de la socket */
  if(bind(sockfdTCP, (struct sockaddr*)&adresse, sizeof(struct sockaddr_in)) == -1) {
    perror("createTCPServerSock : Erreur lors du nommage de la socket ");
    exit(EXIT_FAILURE);
  }

  if(getsockname(sockfdTCP,(struct sockaddr*)&adresse,&len)==-1){
    perror("Erreur getsockname");
    exit(EXIT_FAILURE);
  }

  memset(&reponse,0,sizeof(reponse));
  reponse.type = TYPE_CONNEXION_AUTHORIZED;
  reponse.port = ntohs(adresse.sin_port);
  printf("\nreponse.port : %d\n",reponse.port);
  /* 1ere connexion autorisée */
  if(sendto(sockfdUDP, &reponse, sizeof(reponse), 0, (struct sockaddr*)&p1, p_len) == -1) {
    perror("Erreur lors de la reponse 1 ");
    exit(EXIT_FAILURE);
  }
  /* 2ere connexion autorisée */
  if(sendto(sockfdUDP, &reponse, sizeof(reponse), 0, (struct sockaddr*)&p2, p_len) == -1) {
    perror("Erreur lors de la reponse 2 ");
    exit(EXIT_FAILURE);
  }
  /* Fermeture de la socket UDP*/
  if(close(sockfdUDP) == -1) {
    perror("Erreur lors de la fermeture de la socket ");
    exit(EXIT_FAILURE);
  }

  /* Mise en mode passif de la socket */
  if(listen(sockfdTCP, 1) == -1) {
    perror("Erreur lors de la mise en mode passif ");
    exit(EXIT_FAILURE);
  }

  /* Attente d'une connexion */
  if((sock_one = accept(sockfdTCP, NULL, NULL)) == -1) {
    perror("Erreur lors de la demande de connexion TCP du joueur 1");
    exit(EXIT_FAILURE);
  }
  printf("Joueur 1 connecté TCP !\n");
  if((sock_two = accept(sockfdTCP, NULL, NULL)) == -1) {
    perror("Erreur lors de la demande de connexion TCP du joueur 2");
    exit(EXIT_FAILURE);
  }
  printf("Joueur 2 connecté TCP !\n");

  /*do{

    if(read(sock_one, &taille, sizeof(size_t)) == -1) {
      perror("Erreur lors de la lecture de la taille du message ");
      exit(EXIT_FAILURE);
    }
    if((msg = (char*)malloc(sizeof(char) * taille)) == NULL) {
      perror("Erreur lors de l'allocation mémoire pour le message ");
      exit(EXIT_FAILURE);
    }
    if(read(sock_one, msg, sizeof(char) * taille) == -1) {
      perror("Erreur lors de la lecture de la taille du message ");
      exit(EXIT_FAILURE);
    }
    printf("Serveur : message recu '%s'.\n", msg);
    if(strcmp(msg,"Au revoir")==0){

      taille = strlen("Au revoir") + 1;
      if(write(sock_one, &taille, sizeof(size_t)) == -1) {
        perror("Erreur lors de l'envoi de la taille du message ");
        exit(EXIT_FAILURE);
      }
      if(write(sock_one, "Au revoir", sizeof(char) * taille) == -1) {
        perror("Erreur lors de l'envoi du message ");
        exit(EXIT_FAILURE);
      }
      printf("Serveur : Au revoir envoyé.\n");
    }else{


    taille = strlen(msgenvoi) + 1;
    if(write(sock_one, &taille, sizeof(size_t)) == -1) {
      perror("Erreur lors de l'envoi de la taille du message ");
      exit(EXIT_FAILURE);
    }
    if(write(sock_one, msgenvoi, sizeof(char) * taille) == -1) {
      perror("Erreur lors de l'envoi du message ");
      exit(EXIT_FAILURE);
    }
    printf("Serveur : message envoyé.\n");
  }

}while(strcmp(msgenvoi,"Au revoir"));*/

  /* Fermeture des sockets */
  if(close(sock_one) == -1) {
    perror("Erreur lors de la fermeture de la socket de communication ");
    exit(EXIT_FAILURE);
  }
  if(close(sockfdTCP) == -1) {
    perror("Erreur lors de la fermeture de la socket de connexion ");
    exit(EXIT_FAILURE);
  }

  /* Liberation mémoire */
  /*free(msg);*/

  printf("Serveur terminé.\n");

  return EXIT_SUCCESS;
}