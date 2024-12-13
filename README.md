# C-HTTP

A lightweight and minimal HTTP server written in C that serves static files. The server stops when the **Enter** key is pressed. This project includes Docker and Docker Compose to easily run the server in a containerized environment.

## Features

- **Lightweight & Fast**: Built from scratch in C with minimal dependencies.
- **Static File Serving**: Can serve HTML, CSS, JS, and other static files.
- **Graceful Shutdown**: The server stops when the **Enter** key is pressed.
- **Handles 404s**: If a requested file is not found, a `404 Not Found` response is sent.
- **Dockerized**: Use Docker and Docker Compose to easily set up and run the server in a containerized environment.
- **Customizable Port**: The server listens on port 11000 by default, but this can be changed.

## Requirements

### For Local Development:

- **GCC or any other C compiler** to build the server (if not using Docker).
- **Docker** and **Docker Compose** to run the server in a containerized environment.

### For Docker Setup:

- Docker and Docker Compose installed on your machine. You can install them by following the instructions in the [official Docker documentation](https://docs.docker.com/get-docker/).

## How to Run

### Option 1: Run with Docker

1. **Clone the Repository**:

   First, clone the repository to your local machine:

   ```bash
   git clone http://github.com/MementoMori11723/C-http.git C-HTTP
   cd C-HTTP
   ```

2. **Build and Run the Docker Container**:

   Use Docker Compose to build and run the container:

   ```bash
   docker-compose up --build -d
   ```

   This will automatically build the Docker image and start the server in a container, listening on `http://localhost:11000`.

3. **Stop the Docker Container**:

   To stop the server running in Docker, press the **Enter** key inside the terminal where the container is running. Alternatively, you can use:

   ```bash
   docker-compose down
   ```

   This will stop and remove the container.

### Option 2: Run Locally (Without Docker)

1. **Clone the Repository**:

   ```bash
   git clone http://github.com/MementoMori11723/C-http.git C-HTTP
   cd C-HTTP
   ```

2. **Build the Project**:

   Use `make` to compile the project:

   ```bash
   make
   ```

3. **Start the Server**:

   To start the server, run:

   ```bash
   ./http_server
   ```

   The server will start and listen on `http://localhost:11000` by default.

4. **Stop the Server**:

   To stop the server, press the **Enter** key. Alternatively, you can use:

   ```bash
   make stop
   ```

## How to Test

Once the server is running, you can test it in several ways:

### Using a Web Browser:

- Open your preferred browser and navigate to `http://localhost:11000/`. You should see the content served by the server. If a file is requested and exists, the server will return the content with a `200 OK` status.

### Using `wget`:

- You can also test the server using `wget`. For example, to request the `about.html` file:

  ```bash
  wget http://localhost:11000/about.html
  ```

### Using `curl`:

- Similarly, you can use `curl` to make requests to the server:

  ```bash
  curl http://localhost:11000/about.html
  ```

In both cases, if the requested file is found, it will be returned with a `200 OK` status. If the file is not found, a `404 Not Found` response will be sent.

## What It Does

### Server Functionality:

- **Listening**: The server listens on port 11000 by default for incoming HTTP requests.
- **Serving Static Files**: It serves files located in the directory from which it is run. For example, if you request `/about.html`, it will look for an `about.html` file in the current directory.
- **File Response**:
  - If the requested file exists, the server sends it with a `200 OK` status.
  - If the file does not exist, the server responds with a `404 Not Found` status.
- **Shutdown Mechanism**: The server stops when the **Enter** key is pressed. This provides a simple mechanism to stop the server without needing to force quit.

### File Structure:

The server can serve any static files in the directory it is run from. Ensure that any HTML, CSS, JavaScript, or other content you want to serve is located in the same directory as the server, or modify the code to specify a path to another directory.

## Troubleshooting

- **Server Not Starting**: Ensure that you have successfully compiled the server with `make`. If there are any errors during compilation, they should be displayed in the terminal.
- **Docker Issues**: If using Docker and the server isn't starting, ensure that Docker is installed and running correctly. You can check this with `docker --version` and `docker-compose --version`.
- **Port Conflicts**: If port `11000` is already in use, you may need to modify the code or configuration to use a different port.
- **File Not Found**: If the file you are requesting is not found, make sure it exists in the server's working directory. If you're using a different directory, adjust the file path in the code accordingly.

## Contributing

If you'd like to contribute to this project, feel free to fork the repository and submit a pull request. Please ensure that your contributions follow the basic code style and conventions used in the repository.

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-name`).
3. Make your changes.
4. Commit your changes (`git commit -am 'Add new feature'`).
5. Push to the branch (`git push origin feature-name`).
6. Open a pull request.

## Future Improvements

- Add support for HTTPS (SSL/TLS).
- Support for dynamic content (e.g., PHP, CGI).
- Add logging for requests and errors.
- Implement more robust handling for various HTTP methods (e.g., POST, PUT).
- Enable configurable server port and host through command-line arguments or a configuration file.
