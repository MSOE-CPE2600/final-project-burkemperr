/******************************************************************************************************************
* @file server.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include "voting.h"

#define SHM_KEY 12345 // <- shared memory key used by both server and voter processes

void print_results(VotingData *data) {
    int win_votes = 0;
    bool tie = false; // keeps track if theres a tie or not
    int winner = -1;
    printf("\nVoting Results:\n");
    sleep(10);
    for (int i = 0; i < MAX_CANDIDATES; i++) {
        if (strlen(data->candidate_names[i]) > 0) {
            printf("%s: %d votes\n", data->candidate_names[i], data->votes[i]);
            if(data->votes[i] != 0){
                if (data->votes[i] > win_votes)
                {
                    win_votes = data->votes[i];
                    tie = false;
                    winner = i;
                }
                else if (win_votes == data->votes[i])
                {
                    tie = true;
                }
            }
        }
    }
    // Determine the result
    if (!tie && winner != -1) 
    {
        printf("Winner: %s with %d votes!!!\n", data->candidate_names[winner], win_votes);
    } else if (tie) 
    {
        printf("It was a tie, there is no winner.\n");
    } else 
    {
        printf("No votes were cast.\n");
    }

}

void handle_exit(int sig) {
    exit(0);
}

int main() {
    // create shared memory segment for voting data
    int shmid = shmget(SHM_KEY, sizeof(VotingData), IPC_CREAT | 0666); // system 5 concept, done differently is posx
    if (shmid == -1) {
        perror("Shared memory");
        exit(1);
    }

    // shmat attaches the shared memory segment to the server's address space
    VotingData *data = (VotingData *)shmat(shmid, NULL, 0); // data pointer points to the shared memory segment: contains the VotingData structure
    if (data == (void *)-1) {
        perror("Shared memory attach");
        exit(1);
    }

    memset(data, 0, sizeof(VotingData));
    // updates the voting_open field of the VotingData structure to 1
    data->voting_open = 1;
    data->time_left = 45;

    // initialize candidate names
    strncpy(data->candidate_names[0], "Math", MAX_CANDIDATE_NAME_LEN);
    strncpy(data->candidate_names[1], "English", MAX_CANDIDATE_NAME_LEN);
    strncpy(data->candidate_names[2], "Science", MAX_CANDIDATE_NAME_LEN);
    strncpy(data->candidate_names[3], "History", MAX_CANDIDATE_NAME_LEN);
    
    // signal handler for process termination
    signal(SIGINT, handle_exit);

    printf("Voting started. Time left: 45 seconds\n");

    // the countdown for voting
    while (data->time_left > 0) {
        sleep(1);
        data->time_left--;
        printf("Time left: %d seconds\n", data->time_left);
    }

    data->voting_open = 0;
    printf("\nVoting closed!\n");

    print_results(data);

    for (int i = 0; i < data->voter_count; i++) {
        kill(data->pids[i], SIGTERM);
    }

    // detaches and removes the shared memory segment
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
