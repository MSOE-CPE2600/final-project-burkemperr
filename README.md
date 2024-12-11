# Lab Week 13
## Final Project

### Overview
Voting Machine project that gathers votes from different terminals using usernames. The vote lasts for 45 seconds. The server program keeps track of the votes and voter terminal PIDS, so it can start and closing the vote. The voter program is where users vote on their favoite subject. The same username is not allowed to vote more than once. The program uses shared memory for inter-process communication.
##### Purpose
Besides being a lab assignment the purpose of this program is to create a voting system where multiple users/voter processes are able to vote concurrently. The shared memory is so that data can be shared between the server and voter processes.

#### Sample Output    
    Enter username: 
##### input: 
    bob
##### output:
    Vote for your favorite subject!
    Subjects:
    1. Math
    2. English
    3. Science
    4. History
    Enter your vote (1-4): 
##### input:
    4
##### output:
    Vote submitted successfully!
##### When the terminal runs out of time output:
    Voting terminal exiting...