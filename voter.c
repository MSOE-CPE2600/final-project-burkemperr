/******************************************************************************************************************
* @file voter.c
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
#include "voting.h"

#define SHM_KEY 12345

void handle_exit(int sig) {
    printf("Voting terminal exiting...\n");
    exit(0);
}

int main() {
    // Access the shared memory segment created by the server
    int shmid = shmget(SHM_KEY, sizeof(VotingData), 0666);
    if (shmid == -1) {
        perror("Shared memory");
        exit(1);
    }
    
    // attaches the shared memory segment to the voter's address space
    VotingData *data = (VotingData *)shmat(shmid, NULL, 0);
    if (data == (void *)-1) {
        perror("Shared memory attach");
        exit(1);
    }
    
    // signal handler for termination signal
    signal(SIGTERM, handle_exit);

    // goes until voting is closed
    while (data->voting_open) {
        char username[MAX_USERNAME_LEN];
        int candidate;

        printf("\nEnter username: ");
        scanf("%s", username);

        int duplicate = 0;
        for (int i = 0; i < data->voter_count; i++) {
            if (strcmp(data->username[i], username) == 0) {
                duplicate = 1;
                break;
            }
        }

        // ensure the same username can't vote twice
        if (duplicate) 
        {
            printf("Username already voted, you may not vote twice. Try again.\n");
            continue;
        }
        // Voting info that the voting terminal sees
        printf("\nVote for your favorite subject!");
        printf("\nSubjects:\n");
        for (int i = 0; i < MAX_CANDIDATES; i++) {
            if (strlen(data->candidate_names[i]) > 0) {
                printf("%d. %s\n", i + 1, data->candidate_names[i]);
            }
        }

        printf("Enter your vote (1-%d): ", MAX_CANDIDATES);
        scanf("%d", &candidate);

        if (candidate < 1 || candidate > MAX_CANDIDATES || strlen(data->candidate_names[candidate - 1]) == 0) {
            printf("Invalid subject. Try again.\n");
            continue;
        }
        // record the vote in shared memory
        strcpy(data->username[data->voter_count], username);
        data->votes[candidate - 1]++;
        data->pids[data->voter_count] = getpid();
        data->voter_count++;

        printf("Vote submitted successfully!\n");
    }

    printf("\nVoting closed!\n");
    shmdt(data);
    return 0;
}
