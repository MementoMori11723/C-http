# C-HTTP

A Lightweight and Minimal HTTP Server Written in C

## How to run
```bash
$ make
```

## How to test
```bash
$ wget localhost:7926/index.html
```

## What are we doing?  
We start by creating a socket and binding it to a specific port, which allows the server to listen for incoming connections. Once the server is running, it:  

1. **Listens** for incoming requests on the bound port.  
2. **Accepts** the connection when a client reaches out.  
3. **Reads** the HTTP request data sent by the client.  
4. **Processes** the request to determine the appropriate response.  
5. **Sends** back the response, typically including an HTTP status code, headers, and the requested content (e.g., an HTML file).  

This simple workflow demonstrates the foundational steps of an HTTP server.
