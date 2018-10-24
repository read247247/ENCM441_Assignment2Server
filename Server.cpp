//
//  Server.cpp
//  CPSC441 Assignment2
//
// TCP-based client example of socket programming.
// This server interacts with the Assignment2 client.
// It hashes strings it recieves from the client word-by-word and returns the has
//
//  Created by Nathan Godard on 2018-10-12.
//  Adapted from code given by Dr. Carey Williamson at http://pages.cpsc.ucalgary.ca/~carey/CPSC441/examples/wordlengthserver.c (Retrieved Oct. 19, 2018)
//

#include "Server.hpp"
#include "Hasher.hpp"

#define DEBUG 0

#define HASH1 0
#define HASH2 1
#define MYHASH 0

void Server::run_server(const int PORTNUMBER) {

    /* Setup socket: error message printouts implemented in member functions */
    initialize_address(PORTNUMBER);
    sock = create_socket();
    if(sock < 0){ exit(1); }    //TCP setup failed
    status = bind_socket();
    if(status < 0){ exit(1); }  //Binding socket to address and port failed
    status = set_to_listen();
    if(status < 0){ exit(1); }  //Setting socket to listen for incoming clients failed
    
    std::cout << "Looks like everything set up correctly! Searching for connection...";
    
    status = accept_connection();
    if(status < 0){ exit(1); }  //Establishing connection failed
    
    /* Loop for listening to client */
    while (1) {
        size_t bytes_rcvd = rcv_from_client();
        
        if(bytes_rcvd <= 0){
            close(accepted_connection);
            break;
        }
#if DEBUG
        std::cout << rcv_message;
#endif
        if (strcmp(rcv_message, "CLOSE CONNECTION") == 0) {
            close(accepted_connection);
            break;
        }
        
#if HASH1
        hash1(rcv_message, (int)bytes_rcvd, message, MAXMESSAGELENGTH);
#elif HASH2
        hash2(rcv_message, (int)bytes_rcvd, message, MAXMESSAGELENGTH);
#elif MYHASH
        myhash(rcv_message, (int)bytes_rcvd, message, MAXMESSAGELENGTH);
#endif
        
#if DEBUG
        std::cout << "about to send\n";
#endif
        send_msg();
#if DEBUG
        std::cout << "sent\n";
#endif
    }
    close(sock);
    return;
}

void Server::initialize_address(const int PORTNUMBER){
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORTNUMBER);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
}

int Server::create_socket(){
    int sock = socket(PF_INET, SOCK_STREAM, 0);
#if DEBUG
    if (sock < 0) {
        std::cout << "Error in socket()\n";
    } else {
        std::cout << "Socket created successfully.\n";
    }
#endif
    return sock;
}

int Server::bind_socket(){
    //Makes the socket reusable if the program crashes
    int status_placeholder = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &status_placeholder, sizeof(status_placeholder)) == -1) {
        std::cerr << "setsockopt failed!!";
        return -1;
    }
    
    int status = bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_in));
#if debug
    if (status < 0) { std::cout << "Error in bind()\n"; }
    else{ std::cout << "Binding completed.\n"; }
#endif
    return status;
}

int Server::set_to_listen(){
    bzero(rcv_message, MAXMESSAGELENGTH);
    bzero(message, MAXMESSAGELENGTH);
    int status = listen(sock, 5);
#if debug
    if (status < 0) {
        std::cout << "Error in listen()\n";
    } else {
        std::cout << "Listening for connection requests...\n";
    }
#endif
    return status;
}

int Server::accept_connection(){
    accepted_connection = accept(sock, NULL, NULL);
#if DEBUG
    if (accepted_connection < 0) {
        std::cout << "Error in accept()\n";
    } else{
        std::cout << "Connection established.\n";
    }
#endif
    return accepted_connection;
}

size_t Server::send_msg(){
    size_t count;
    //message = (char*)"For termination send \"Bye\"\n";
    count = send(accepted_connection, message, MAXMESSAGELENGTH, 0);
#if DEBUG
    if (count < 0) {
        std::cout << "Error in send()\n";
    } else {
        std::cout << "Sent message!";
    }
#endif
    return count;
}

size_t Server::rcv_from_client(){
    ssize_t bytes_rcvd = recv(accepted_connection, rcv_message, MAXMESSAGELENGTH, 0 );
    rcv_message[bytes_rcvd] = '\0';
#if DEBUG
    if (bytes_rcvd <= 0){
        std::cout << "Error in recieving from client\n";
    } else { std::cout << "We have recieved a message (yay)\n"; }
#endif
    return bytes_rcvd;
}

void Server::closeAndExit(const int sock){
    close(sock);
    exit(0);
}
