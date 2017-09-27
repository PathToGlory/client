#include <netdb.h>
#include <netinet/in.h>
#include <assert.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

int main() {
  int port = 9999;

  in_addr ipv4addr;
  assert(inet_pton(AF_INET, "0.0.0.0", &ipv4addr) == 1 && "Wrong formated ip address");

  hostent* server = gethostbyaddr(&ipv4addr, sizeof(ipv4addr), AF_INET);
  assert(server != nullptr && "No such host");

  int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  assert(sockfd != -1 && "Socket initialization failed");

  sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memcpy(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(port);

  assert(connect(sockfd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) != -1 &&
         "Error during connection");

  char buffer[256];
  memset(buffer, 0, sizeof(buffer));
  fgets(buffer, 255, stdin);

  while (strcmp(buffer, "quit")) {
    assert(write(sockfd, buffer, strlen(buffer)) && "Error writing socket");

    memset(buffer, 0, sizeof(buffer));
    assert(read(sockfd, buffer, 255) && "Error reading socket");

    printf("Server send: %s", buffer);

    memset(buffer, 0, sizeof(buffer));
    fgets(buffer, 255, stdin);
  }

  return 0;
}
