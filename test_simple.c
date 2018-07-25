#include "assert.h"
#include <stdlib.h>

void follower_normal (int pid, int num) {
    int currentTerm = 0;
    int lastIndex = 0;
    int lastTerm = 0;

    int cmd;

    int commitIndex = 0;
    int lab_election = 0;
    int lab_normal = 0;

    int retry;
    int timeout;
    int election;

    int old_term = 0;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0;
    
    while (1) {
            retry = rand();
            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));                        
            while (retry) {
                // receive command from client
                cmd = rand();
                if (cmd == 0) {   // Empty command (HeartBeat)
                    lab_normal = 1;
        
                    // assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                    old_term = currentTerm;
                    old_lab_election = lab_election;
                    old_commit = commitIndex;
                    old_lab_normal = lab_normal;
                    old_LLI = lastIndex;
            
                    // send(term, leaderId, prevLogIndex, entries[], leaderCommit) with empty entries
                }
                else {
                    lastIndex = lastIndex + 1;
        
                    lab_normal = 1;
        
                    // assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                    old_term = currentTerm;
                    old_lab_election = lab_election;
                    old_commit = commitIndex;
                    old_lab_normal = lab_normal;
                    old_LLI = lastIndex;
                        
                    // send(term, leaderId, prevLogIndex, entries[], leaderCommit)
                }
                retry = rand();
            }
        
            lab_normal = 2;
     
            // assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;
        
            
            commitIndex = commitIndex + 1;
    }
}

int main() {
    follower_normal(0,5);
}