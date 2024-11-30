#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <stdbool.h>

bool stop_server = false;

// Function to check for Enter key input
void check_enter_key() {
  char buf;
  if (read(STDIN_FILENO, &buf, 1) == 1 && buf == '\n') {
    stop_server = true;
  }
}

int main() {
  // Create a socket
  int s = socket(AF_INET, SOCK_STREAM, 0);

  // Bind the socket to port 8000
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(8000), // Port 8000
    .sin_addr.s_addr = INADDR_ANY // Bind to all available interfaces
  };

  if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Bind failed");
    close(s);
    exit(1);
  }

  if (listen(s, 10) < 0) {
    perror("Listen failed");
    close(s);
    exit(1);
  }

  printf("Server is running on http://localhost:8000\n");

  fd_set read_fds;
  int max_fd = s;
  struct timeval timeout = {1, 0}; // Timeout for select (1 second)

  // Run until Enter is pressed
  while (!stop_server) {
    FD_ZERO(&read_fds);
    FD_SET(s, &read_fds); // Add server socket to the set
    FD_SET(STDIN_FILENO, &read_fds); // Add stdin (keyboard) to the set

    // Wait for activity on either socket or stdin
    int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

    if (activity < 0) {
      perror("Select error");
      break;
    }

    // Check if Enter key was pressed
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      check_enter_key();
    }

    // Check if there is an incoming connection
    if (FD_ISSET(s, &read_fds)) {
      int client_fd = accept(s, NULL, NULL);
      if (client_fd < 0) {
        perror("Accept failed");
        continue;
      }

      char buffer[256] = {0};
      recv(client_fd, buffer, sizeof(buffer) - 1, 0);

      char *f = buffer + 5; // Extract file name after "GET /"
      char *end = strchr(f, ' ');
      if (end) *end = 0;

      // Check if the file exists
      int fd = open(f, O_RDONLY);
      if (fd < 0) {
        // File not found: Send 404 response
        char *response = "HTTP/1.1 404 Not Found\r\n"
          "Content-Type: text/plain\r\n"
          "Content-Length: 13\r\n"
          "\r\n"
          "404 Not Found";
        send(client_fd, response, strlen(response), 0);
      } else {
        // File found: Send 200 OK response and file content
        char *header = "HTTP/1.1 200 OK\r\n\r\n";
        send(client_fd, header, strlen(header), 0);
        sendfile(client_fd, fd, NULL, 256);
        close(fd);
      }

      close(client_fd);
    }
  }

  printf("Server stopped.\n");

  // Close the server socket
  close(s);
  return 0;
}
