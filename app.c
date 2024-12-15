#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/select.h>
#include <stdbool.h>

#define MAX_BUFFER 5000
#define MAX_THREADS 25
#define MAX_QUEUE 250

typedef struct {
  int client_fd;
} client_t;

volatile bool stop_server = false;

client_t client_queue[MAX_QUEUE];
int queue_front = 0, queue_rear = 0;
pthread_mutex_t queue_lock;
pthread_cond_t queue_cond;

void check_enter_key() {
  char buf;
  if (read(STDIN_FILENO, &buf, 1) == 1 && buf == '\n') {
    stop_server = true;
  }
}

ssize_t safe_send(int client_fd, const void *data, size_t size) {
  ssize_t bytes_sent = 0;
  while (bytes_sent < size) {
    ssize_t ret = send(client_fd, data + bytes_sent, size - bytes_sent, 0);
    if (ret < 0) {
      perror("Send failed");
      return ret;
    }
    bytes_sent += ret;
  }
  return bytes_sent;
}

void *handle_client(void *arg) {
  while (!stop_server) {
    pthread_mutex_lock(&queue_lock);

    while (queue_front == queue_rear && !stop_server) {
      pthread_cond_wait(&queue_cond, &queue_lock);
    }

    if (stop_server) {
      pthread_mutex_unlock(&queue_lock);
      break;
    }

    int client_fd = client_queue[queue_front].client_fd;
    queue_front = (queue_front + 1) % MAX_QUEUE; // Move front to the next request
    pthread_mutex_unlock(&queue_lock);

    char buffer[MAX_BUFFER] = {0};
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
      perror("Recv failed or client disconnected");
      close(client_fd);
      continue;
    }

    char *f = buffer + 5;
    char *end = strchr(f, ' ');
    if (end) *end = 0;

    if (strcmp(f, "") == 0 || strcmp(f, "/") == 0) {
      f = "index.html";
    }

    char file_path[512];
    snprintf(file_path, sizeof(file_path), "pages/%s", f);

    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
      char *response = "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 86\r\n"
        "\r\n"
        "<html><body><h1>404 Not Found</h1><p>The requested file does not exist.</p></body></html>";
      if (safe_send(client_fd, response, strlen(response)) < 0) {
        perror("Send failed");
      }
    } else {
      char *header = "HTTP/1.1 200 OK\r\n\r\n";
      if (safe_send(client_fd, header, strlen(header)) < 0) {
        perror("Send failed");
      }

      char file_buffer[MAX_BUFFER];
      ssize_t bytes_read;
      while ((bytes_read = read(fd, file_buffer, sizeof(file_buffer))) > 0) {
        if (safe_send(client_fd, file_buffer, bytes_read) < 0) {
          break;
        }
      }
      close(fd);
    }

    close(client_fd);
  }

  return NULL;
}

void *accept_clients(void *arg) {
  int server_fd = *((int *)arg);
  while (!stop_server) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
      perror("Accept failed");
      continue;
    }

    pthread_mutex_lock(&queue_lock);

    if ((queue_rear + 1) % MAX_QUEUE == queue_front) {
      printf("Queue is full... Rejecting connection\n");
      close(client_fd);
    } else {
      client_queue[queue_rear].client_fd = client_fd;
      queue_rear = (queue_rear + 1) % MAX_QUEUE;
      pthread_cond_signal(&queue_cond);
    }

    pthread_mutex_unlock(&queue_lock);
  }

  return NULL;
}

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("Socket creation failed");
    exit(1);
  }

  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(8000),
    .sin_addr.s_addr = INADDR_ANY
  };

  if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Bind failed");
    close(server_fd);
    exit(1);
  }

  if (listen(server_fd, 10) < 0) {
    perror("Listen failed");
    close(server_fd);
    exit(1);
  }

  printf("Server is up and running\n");

  pthread_mutex_init(&queue_lock, NULL);
  pthread_cond_init(&queue_cond, NULL);

  pthread_t threads[MAX_THREADS];
  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, handle_client, NULL) != 0) {
      perror("Error creating thread");
      exit(1);
    }
  }

  pthread_t accept_thread;
  if (pthread_create(&accept_thread, NULL, accept_clients, &server_fd) != 0) {
    perror("Error creating accept thread");
    exit(1);
  }

  while (!stop_server) {
    check_enter_key();
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  pthread_join(accept_thread, NULL);

  pthread_mutex_destroy(&queue_lock);
  pthread_cond_destroy(&queue_cond);
  close(server_fd);

  printf("Server stopped.\n");
  return 0;
}
