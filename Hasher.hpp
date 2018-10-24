//
//  Hasher.hpp
//  CPSC441-Assignment2_Server
//
//  Created by Nathan Godard on 2018-10-19.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#ifndef Hasher_hpp
#define Hasher_hpp

#include <stdio.h>
#include <string.h>

void hash1(char* rcv_message, int rcv_message_length, char* message, int message_length);
void hash2(char* rcv_message, int rcv_message_length, char* message, int message_length);
void myhash(char* rcv_message, int rcv_message_length, char* message, int message_length);

#endif /* Hasher_hpp */
