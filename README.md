# C-HTTP

A Lightweight and Minimal HTTP Server Written in C that serves static files and stops when the Enter key is pressed.

## How to Run

1. Clone this repository:

    ```bash
    git clone http://github.com/MementoMori11723/C-http.git C-HTTP
    cd C-HTTP
    ```

2. Build the project using `make`:

    ```bash
    make
    ```

3. Run the server:

    ```bash
    ./simple_http
    ```

   The server will run on `http://localhost:8000`.

4. To stop the server, press the **Enter** key.

## How to Test

1. Open a browser or use `wget` or `curl` to test the server:

    ```bash
    wget http://localhost:8000/index.html
    ```
    or

    ```bash
    curl http://localhost:8000/index.html
    ```

2. If the requested file is found, it will be served with a `200 OK` response. If the file is not found, a `404 Not Found` response will be sent.

## What It Does

- The server listens on port 8000 for incoming HTTP requests.
- It serves files based on the requested URL.
- If the file exists, it sends the file with a `200 OK` response.
- If the file does not exist, it sends a `404 Not Found` response.
- The server stops when the **Enter** key is pressed.

