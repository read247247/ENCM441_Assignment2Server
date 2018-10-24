//
//  Server.hpp
//  CPSC441 Assignment2
//
//  Created by Nathan Godard on 2018-10-12.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <string.h>

#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>

#define MAXMESSAGELENGTH 280

class Server{
public:
    void run_server(const int PORTNUMBER);
    void initialize_address(const int PORTNUMBER);
    int create_socket();
    int bind_socket();
    int set_to_listen();
    int accept_connection();
    size_t send_msg();
    size_t rcv_from_client();
    static void closeAndExit(const int sock);
private:
    struct sockaddr_in server;
    int sock;
    int status;
    int accepted_connection;
    char message[MAXMESSAGELENGTH];
    char rcv_message[MAXMESSAGELENGTH];
};
#endif /* Server_hpp */
