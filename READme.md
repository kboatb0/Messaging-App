# Chat Application
This is a simple chat application built in C++ using sockets. It allows multiple clients to connect to a server and send/receive messages in real-time.

## Features
### Server:
Listens for incoming client connections.
Accepts multiple clients and assigns them unique usernames.
Broadcasts messages from one client to all other connected clients.

### Client:
Connects to the server and sends messages.
Receives and displays messages from other clients.
Automatically reconnects if the connection to the server is lost.

## Getting Started
### Prerequisites
C++ compiler (e.g., Visual Studio)
Windows OS (the application uses Windows-specific socket libraries)

### Installation
Clone the repository:
Open the project in your C++ IDE (e.g., Visual Studio).
Build and run the Server project.
Build and run the Client project.
Enter a username when prompted and start chatting!

### Usage
The server will listen on port 8080.
Multiple clients can connect to the server using localhost:8080.
Clients can send messages, which will be broadcasted to all connected clients.