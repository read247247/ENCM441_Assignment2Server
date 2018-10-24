//
//  main.cpp
//  CPSC441-Assignment2_Server
//
//  Created by Nathan Godard on 2018-10-19.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//
#include "Server.hpp"
#include <iostream>

#define PORTNUMBER 8543

int main(int argc, const char * argv[]) {
    Server* server = new Server();
    server -> run_server(PORTNUMBER);
    return 0;
}
