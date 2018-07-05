#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

#define FOLLOWER 1
#define CANDIDATE 2
#define LEADER 3

typedef struct _msg_reqVote {
    int term;
    int candidateId;
    int lastLogIndex;
    int lastLogTerm;
} msg_reqVote;

msg_reqVote init_reqVote(int num) {
    msg_reqVote m;
    m.term = rand();
    m.candidateId = rand() % num;
    m.lastLogIndex = rand();
    m.lastLogTerm = rand();
}

int filter_reqVote (msg_reqVote m, int term) {
    if (m.term == term)
        return 1;
    return 0;
}

typedef struct _msg_vote {
    int term;
    int vote;
} msg_vote;

msg_vote init_vote() {
    msg_vote m;
    m.term = rand();
    m.vote = rand() % 2;
}

int filter_vote(msg_vote m, int term) {
    if (m.term == term && m.vote == 1)
        return 1;
    return 0;
}

typedef struct _msg_AE {
    int term;
    int leaderId;
    int prevLogIndex;
    int prevLogTerm;
    int leaderCommit;
    int entries[100];
} msg_AE;

msg_AE init_AE() {
    msg_AE m;
    m.term = rand();
    m.leaderId = rand();
    m.prevLogIndex = rand();
    m.prevLogTerm = rand();
    m.leaderCommit = rand();
}

int filter_AE(msg_AE m, int currentTerm, int lastIndex, int lastTerm) {
    if (m.term = currentTerm && m.prevLogIndex == lastIndex && m.prevLogTerm == lastTerm)
        return 1;
    return 0;
}

typedef struct _msg_AE_ack {
    int term;
    int success;
} msg_AE_ack;

msg_AE_ack init_AE_ack() {
    msg_AE_ack m;
    m.term = rand();
    m.success = rand() % 2;
}

int filter_AE_ack(msg_AE_ack m, int term) {
    if (m.term == term && m.success == 1)
        return 1;
    return 0;
}

void Raft(int pid, int num) {
    int state = FOLLOWER;
    int currentTerm = 0;
    int votedFor = pid;
    int log[1000];

    int lastIndex = 0;
    int lastTerm = 0;

    int cmd;

    int commitIndex = 0;
    int lastApplied = 0;

    int nextIndex[num];
    int matchIndex[num];

    msg_reqVote mbox_reqVote[2*num];
    int num_mbox_reqVote = 0;
    msg_reqVote m_reqVote;

    msg_vote mbox_vote[2*num];
    int num_mbox_vote = 0;
    msg_vote m_vote;

    msg_AE mbox_AE[2*num];
    int num_mbox_AE = 0;
    msg_AE m_AE;

    msg_AE_ack mbox_AE_ack[2*num];
    int num_mbox_AE_ack = 0;
    msg_AE_ack m_AE_ack;

    int lab_election = 1;
    int lab_normal = 1;

    int retry;
    int timeout;
    int voting = 0;
    int election;

    int old_lab_election = 0;
    int old_lab_normal = 0;
    int old_term = 0;
    int old_LI = 0;
    int old_commit = 0;

    while(1) {
        if (state == CANDIDATE) {
            currentTerm++;
            
            votedFor = pid; // vote for itself
            
            lab_election = 1;
            
            // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
            // old_term = currentTerm;
            // old_lab_election = lab_election;
            // old_commit = commitIndex;
            // old_lab_normal = old_lab_normal;
            // old_LLI = lastLogIndex;

            // Ask for votes
            // send(term, c_id, lastLogIndex, lastLogTerm)

            lab_election = 2;

            // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
            // old_term = currentTerm;
            // old_lab_election = lab_election;
            // old_commit = commitIndex;
            // old_lab_normal = old_lab_normal;
            // old_LLI = lastLogIndex;

            // Empty mbox
            memset(mbox_vote,0,sizeof(mbox_vote));
            num_mbox_vote = 0;

            // receive votes
            retry = rand() % 2;
            while(retry) {
                m_vote = init_vote();

                if (m_vote.term > currentTerm) {
                    state = FOLLOWER;
                    currentTerm = m_vote.term;
                    break;
                }

                if(filter_vote(m_vote, currentTerm)) {
                    mbox_vote[num_mbox_vote] = m_vote;
                    num_mbox_vote++;
                }

                if(num_mbox_vote >= num/2)
                    break;
  
                retry = rand() % 2;
            }

            timeout = rand() % 2;
            if (timeout) {
                state = CANDIDATE;
            }

            if(num_mbox_vote >= num/2)
                state = LEADER;
        }

        if (state == FOLLOWER) {
            lab_election = 1;

            // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
            // old_term = currentTerm;
            // old_lab_election = lab_election;
            // old_commit = commitIndex;
            // old_lab_normal = old_lab_normal;
            // old_LLI = lastLogIndex;

            // Empty mbox
            memset(mbox_reqVote,0,sizeof(mbox_reqVote));
            num_mbox_reqVote = 0;

            // receive
            retry = rand() % 2;
            while (retry) {
                msg_reqVote m_reqVote = init_reqVote(num);
                
                if (m_vote.term > currentTerm) {
                    state = FOLLOWER;
                    currentTerm = m_vote.term;
                    break;
                }

                if (filter_reqVote(m_reqVote,currentTerm)) {
                    mbox_reqVote[num_mbox_reqVote] = m_reqVote;
                    num_mbox_reqVote++;

                    if (num_mbox_vote >= 1)
                        break;

                    retry = rand() % 2;
                }
            }

            timeout = rand() % 2;
            if (timeout) {
                state = CANDIDATE;
            }

            if (num_mbox_reqVote >= 1) {
                lab_election = 2;

                // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                // old_term = currentTerm;
                // old_lab_election = lab_election;
                // old_commit = commitIndex;
                // old_lab_normal = old_lab_normal;
                // old_LLI = lastLogIndex;

                /*
                if (mbox_reqVote[0].lastLogTerm > log.lastTerm) {
                    send vote with success = 1;
                    votedFor = mbox_reqVote[0].candidateId;
                }
                else {
                    if (mbox_reqVote[0].lastLogTerm < log.lastTerm)
                        send vote with success = 0;
                    else {
                        if (mbox_reqVote[0].lastLogIndex > log.lastIndex) {
                            send vote with success = 1;
                            votedFor = mbox_reqVote[0].candidateId;
                        }
                        else
                            send vote with success = 0;
                    }
                }
                */
            }
        }

        if (state == LEADER) {
            lab_election = 1;

            // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
            // old_term = currentTerm;
            // old_lab_election = lab_election;
            // old_commit = commitIndex;
            // old_lab_normal = old_lab_normal;
            // old_LLI = lastLogIndex;
            
            // Empty mbox
            memset(mbox_reqVote,0,sizeof(mbox_reqVote));
            num_mbox_reqVote = 0;
            
            // receive
            retry = rand() % 2;
            while (true) {
                msg_reqVote m_reqVote = init_reqVote(num);
                            
                if (m_reqVote.term > currentTerm) {
                    state = FOLLOWER;
                    currentTerm = m_reqVote.term;
                    break;
                }
            
                if (filter_reqVote(m_reqVote,currentTerm)) {
                    mbox_reqVote[num_mbox_reqVote] = m_reqVote;
                    num_mbox_reqVote++;
        
                    if (num_mbox_vote >= 1)
                        break;
            
                    retry = rand() % 2;
                }
                if(!retry) break
            }
            
            // Leader can't timeout
            
            if (num_mbox_reqVote >= 1) {
                lab_election = 2;

                // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                // old_term = currentTerm;
                // old_lab_election = lab_election;
                // old_commit = commitIndex;
                // old_lab_normal = old_lab_normal;
                // old_LLI = lastLogIndex;
                            
            /*
            if (mbox_reqVote[0].lastLogTerm > log.lastTerm) {
                send vote with success = 1;
                votedFor = mbox_reqVote[0].candidateId;
            }
            else {
                if (mbox_reqVote[0].lastLogTerm < log.lastTerm)
                    send vote with success = 0;
                else {
                    if (mbox_reqVote[0].lastLogIndex > log.lastIndex) {
                        send vote with success = 1;
                        votedFor = mbox_reqVote[0].candidateId;
                    }
                    else
                        send vote with success = 0;
                }
            }
            */
            }
        }

        while (state != CANDIDATE) {
            if (state == LEADER) {
                if (voting == 0) {
                    lab_normal = 1;

                    // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                    // old_term = currentTerm;
                    // old_lab_election = lab_election;
                    // old_commit = commitIndex;
                    // old_lab_normal = old_lab_normal;
                    // old_LI = lastIndex;  
                    
                    // Follower exits the loop spontaneously for leader election (for voting)
                    election = rand() % 2;
                    if(election) {
                        break;
                    }
    
                    // receive command from client (send can happen any number of times before a receive for the leader)
                    retry = rand() % 2;
                    while (retry) {
                        cmd = rand();
                        if (cmd == 0) {   // Empty command (HeartBeat)
                            // send(term, leaderId, prevLogIndex, entries[], leaderCommit) with empty entries
                        }
                        else {
                            lastIndex++;
                            // send(term, leaderId, prevLogIndex, entries[], leaderCommit)
                        }
                        retry = rand() % 2
                    }
    
                    lab_normal = 2;

                    // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                    // old_term = currentTerm;
                    // old_lab_election = lab_election;
                    // old_commit = commitIndex;
                    // old_lab_normal = old_lab_normal;
                    // old_LI = lastIndex;
    
                    // Empty mbox
                    memset(mbox_AE_ack,0,sizeof(mbox_AE_ack));
                    num_mbox_AE_ack = 0;
    
                }
                // After viting come back here, before the receive but after clearing the mbox (because we want to preserve state)
                
                // for the case when voting
                lab_normal = 2;

                // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                // old_term = currentTerm;
                // old_lab_election = lab_election;
                // old_commit = commitIndex;
                // old_lab_normal = old_lab_normal;
                // old_LI = lastIndex;

                retry = rand() % 2;
                while (retry) {
                    m_AE_ack = init_AE_ack();

                    if (m_AE_ack.term > currentTerm) {
                        state = FOLLOWER;
                        currentTerm = m_AE_ack.term;
                        break;
                    }

                    if (m_AE_ack.success == 0 && m_AE_ack.term == currentTerm) {
                        // set next index one by one
                    }

                    if (filter_AE_ack(m_AE_ack, currentTerm)) {
                        mbox_AE_ack[num_mbox_AE_ack] = m_AE_ack;
                        num_mbox_AE_ack++;
    
                        if (num_mbox_AE_ack >= num/2)
                            break;
    
                        retry = rand() % 2;
                    }
                }

                // Leader can't time out

                // Follower exits the loop spontaneously for leader election (for voting)
                election = rand() % 2;
                if(election) {
                    voting = 1;
                    break;
                }

                if (num_mbox_AE_ack >= num/2) {
                    commitIndex++;
                }
                else { // is this okay? (if not enough msgs rcvd, then have a fresh election)
                    state = CANDIDATE;
                    break;
                }
            }

            if (state == FOLLOWER) {
                if (voting == 0) {
                    lab_normal = 1;

                    // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                    // old_term = currentTerm;
                    // old_lab_election = lab_election;
                    // old_commit = commitIndex;
                    // old_lab_normal = old_lab_normal;
                    // old_LLI = lastLogIndex; 

                    // Follower exits the loop spontaneously for leader election (for voting)
                    election = rand() % 2;
                    if(election) {
                        break;
                    }
                    
                    // Empty mbox
                    memset(mbox_AE,0,sizeof(mbox_AE));
                    num_mbox_AE = 0;
    
                    // receive
                    retry = rand() % 2;
                    while (retry) {
                        m_AE = init_AE();
    
                        if (m_AE_ack.term > currentTerm) {
                            state = FOLLOWER;
                            currentTerm = m_AE_ack.term;
                            break;
                        }
    
                        if (filter_AE(m_AE, currentTerm, lastIndex, lastTerm)) {
                            mbox_AE[num_mbox_AE] = m_AE;
                            num_mbox_AE++;
        
                            if (num_mbox_AE >= 1)
                                break;
        
                            retry = rand() % 2;
                        }
                    }
    
                    timeout = rand() % 2;
                    if (timeout) {
                        state = CANDIDATE;
                        break;
                    }
    
                    // Follower exits the loop spontaneously for leader election (for voting)
                    election = rand() % 2;
                    if(election) {
                        voting = 1;
                        break;
                    }
                }

                if (num_mbox_AE_ack >= 1) {
                    lastIndex++;
                    
                    if (mbox_AE[0].leaderCommit >= commitIndex) {
                        commitIndex = mbox_AE[0].leaderCommit;
                        // Commit all terms till min(commitIndex, lastIndex)
                    }

                    lab_normal = 2;

                    // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                    // old_term = currentTerm;
                    // old_lab_election = lab_election;
                    // old_commit = commitIndex;
                    // old_lab_normal = old_lab_normal;
                    // old_LLI = lastLogIndex; 

                    // send(term, success) to leader
                }
            }
        }
    }
}

int main() {
    Raft(0,5);
    return 0;
}