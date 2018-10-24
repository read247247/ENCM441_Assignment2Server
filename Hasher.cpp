//
//  Hasher.cpp
//  CPSC441-Assignment2_Server
//
//  Created by Nathan Godard on 2018-10-19.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#include "Hasher.hpp"

#include <string.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

/*
 A few notes on efficiency with hash1:
 This will become extremely inefficient as the number of stored words increases. A better solution might be to implement a minimal BST of String/int pairs, with the sorting done by alphabetic order of the Strings. I have a lot to do this week, so have implemented this in the way that required the least extra code on my part.
 */

/*
 The first unique word recieved will hash to 0, the second to 1, and so on
 NOTE: This hash can hold 16^8 - 1 words in the dictionary due to the implementation of converting the hashed values to a string at the end. This should not be a issue, as that is approximately 21,000 times the number of words in the english langauge
 */
void hash1(char* rcv_message, int rcv_message_length, char* message, int message_length){
    static std::vector<std::string> words;  //The dictionary of words
    std::vector<long> hash_values;       //This will be returned in the form of a string of hex values
    
    bool found = false;     //Tracking whether current word is already in the dictionary
    int first_char_in_next_word_index = 0;
    int num_chars_in_word = 0;
    
    bzero(message, message_length);
    
    int i = 0;
    while(rcv_message[i] == ' '){   //Advance to first character that is not a space (Don't want to accidentally be counting a leading space as part of a word)
        i++;
        if(i >= rcv_message_length){
            break;
        }
    }
    first_char_in_next_word_index = i;
    
    //BASIC IDEA: Count how many letters are in the next word. Create a new string, from first_char_in_next_word_index to the end of the word. Check if the string is in the dictionary. If it is not, add it to the dictionary. Add the index of the string to hash_values. Advance to beginning of next word. Repeat.
    for( ; i < rcv_message_length; i++){
        if(rcv_message[i] == ' ' || i == rcv_message_length - 1){  //Reached the end of a word
            std::string temp = std::string(&rcv_message[first_char_in_next_word_index], num_chars_in_word);    //Grab the current word
            found = false;
            for(int j = 0; j < words.size(); j++){  //Check if the word is already in our dictionary
                if(temp.compare(words.at(j)) == 0){
                    hash_values.push_back(j);       //Store the index in our list to return
                    found = true;
                    break;
                }
            }
            
            if(!found){ //If the word wasn't in the dictionary, add it
                words.push_back(temp);
                hash_values.push_back((long)words.size() - 1);   //Following our convention of the first word hashing to 0
            }
            
            while(rcv_message[i] == ' '){   //Advance to next character that is not a space
                i++;
                if(i >= rcv_message_length){
                    break;
                }
            }
            
            first_char_in_next_word_index = i;
            num_chars_in_word = 0;
        } //End if
        else {
            num_chars_in_word++;    //The current character is not the end of a word
        }
    } //End loop (we have reached the end of the input)
    
    std::stringstream stream;
    
    for(long n: hash_values){    //Convert hashed values to hex and build them into a string to return
        stream << std::setfill (' ') << std::setw(10) << "0x" << std::hex << n << " ";
    }
    std::string* result = new std::string( stream.str() );
    
    //pass return string into message
    if(result -> length() > message_length){
        strcpy(message, "Your input was under the character limit, but had too many words. Try again");
    } else{
        strcpy(message, result->c_str());
    }
}

//Adds the ascii values for each character in a word.
void hash2(char* rcv_message, int rcv_message_length, char* message, int message_length){
    std::vector<int> results;
    
    for(int i = 0; i < rcv_message_length; i++){
        while(rcv_message[i] == ' '){       //Move cursor to first character of a word
            i++;
            if(i >= rcv_message_length){
                break;
            }
        }
        
        int result_for_this_word = 0;
        for(int j = i; rcv_message[j] != ' ' && j < rcv_message_length; j++, i++){      //Sum characters in the word
            result_for_this_word += rcv_message[j];
        }
        results.push_back(result_for_this_word);        //Add hash to results
    }
    std::stringstream stream;
    
    for(int n: results){    //Build hased values into string
        stream << std::setfill (' ') << std::setw(10) << "0x" << std::hex << n << " ";
    }
    std::string* result = new std::string( stream.str() );
    
    //pass string into message
    if(result -> length() > message_length){
        strcpy(message, "Your input was under the character limit, but had too many words. Try again");
    } else{
        strcpy(message, result->c_str());
    }
}

/*
 Takes the ascii value of each character, and multiplies it by the character's position in the word, with the first character as 1. Truncates the resulting hex value to 8 digits (the vast majority of hashes do not require truncation)
 
So for the word "The", (ASCII value of 'T') * 1 + (ASCII value of 'h') * 2 + (ASCII value of 'e') * 3 = 84 * 1 + 104 * 2 + 101 * 3 = 595 base 10, which is 0x253.
 
 This method requires only a simple modification of hash2, and dramatically reduces the number of collisions, since it takes the position of each character into account. I am sure collisions exist, but I have been unable to find them.
 */
void myhash(char* rcv_message, int rcv_message_length, char* message, int message_length){
    std::vector<int> results;
    
    for(int i = 0; i < rcv_message_length; i++){
        while(rcv_message[i] == ' '){
            i++;
            if(i >= rcv_message_length){
                break;
            }
        }
        
        int result_for_this_word = 0;
        for(int j = i; rcv_message[j] != ' ' && j < rcv_message_length; j++, i++){
            result_for_this_word += (rcv_message[j] + 1) * j;
        }
        results.push_back(result_for_this_word);
    }
    std::stringstream stream;
    
    for(int n: results){    //Build hased values into string
        stream << std::setfill (' ') << std::setw(10) << "0x" << std::hex << n << " ";
    }
    std::string* result = new std::string( stream.str() );
    
    //pass string into message
    if(result -> length() > message_length){
        strcpy(message, "Your input was under the character limit, but had too many words. Try again");
    } else{
        strcpy(message, result->c_str());
    }
}
