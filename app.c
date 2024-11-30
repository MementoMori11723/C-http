#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>

void main(){
  // First we create a socket that we can use to listen for incoming connections
  int s = socket(AF_INET, SOCK_STREAM, 0);

  // We bind the socket to the port 0x401f
  struct sockaddr addr = {
    AF_INET,
    0x401f,
    0
  };

  // We listen for incoming connections by binding the socket to the address
  bind(s, &addr, sizeof(addr));
  listen(s, 10);

  // We accept the incoming connection
  int client_fd = accept(s, 0, 0);
  char buffer[256] = {0};

  // We read the incoming request
  recv(client_fd, buffer, 256, 0);
  char* f = buffer + 5;
  *strchr(f, ' ') = 0;

  // We open the file and send it to the client
  int fd = open(f, O_RDONLY);
  sendfile(client_fd, fd, 0, 256); 

  // We close the file & the client connection & the socket
  close(fd);
  close(client_fd);
  close(s);
}
