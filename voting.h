/******************************************************************************************************************
* @file voting.h
* @brief Voting Machine
*
* Course: CPE2600
* Section: 111
* Assignment: CPE Lab 13
* Author: Reagan Burkemper
*
* Description: Voting maching that implements a machine that gathers votes from different terminals with usernames.
*               

* Date: 12/10/2024
*
* Compile Instructions: 
*   To compile & run:
*   - compile with: 
*       - make clean
*       - make
*   - To run server terminal run: ./server
*   - To run the voter terminals run: ./voter
*   
*****************************************************************************************************************/

#ifndef VOTING_H
#define VOTING_H

#define MAX_VOTERS 100
#define MAX_USERNAME_LEN 50
#define MAX_CANDIDATES 4
#define MAX_CANDIDATE_NAME_LEN 50

typedef struct {
    char candidate_names[MAX_CANDIDATES][MAX_CANDIDATE_NAME_LEN];
    char username[MAX_VOTERS][MAX_USERNAME_LEN];
    int votes[MAX_CANDIDATES];
    int voter_count;
    int pids[MAX_VOTERS];
    int time_left;
    int voting_open;
} VotingData;

#endif
