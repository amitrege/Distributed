#define FOLLOWER 1
#define CANDIDATE 2
#define LEADER 3

typedef struct _msg_reqVote {
    int term;
    int candidateId;
    int lastLogIndex;
    int lastLogTerm;
} msg_reqVote;

int filter_reqVote (msg_reqVote* m, int term) {
    if (m->term >= term)
        return 1;
    return 0;
}

typedef struct _msg_vote {
    int term;
    int vote;
} msg_vote;

int filter_vote(msg_vote* m, int term) {
    if (m->term >= term && m->vote == 1)
        return 1;
    return 0;
}

typedef struct _msg_AE {
    int term;
    int leaderId;
    int prevLogIndex;
    int prevLogTerm;
    int leaderCommit;
    //int entries[100];
} msg_AE;

int filter_AE(msg_AE* m, int currentTerm, int lastIndex, int lastTerm) {
    if (m->term >= currentTerm && m->prevLogIndex == lastIndex && m->prevLogTerm == lastTerm)
        return 1;
    return 0;
}

typedef struct _msg_AE_ack {
    int term;
    int success;
} msg_AE_ack;

int filter_AE_ack(msg_AE_ack* m, int term) {
    if (m->term >= term && m->success == 1)
        return 1;
    return 0;
}

/*
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

    int lab_election = 0;
    int lab_normal = 0;

    int retry;
    int timeout;
    int election;

    volatile int random;

    int old_term = 0;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0;

    while(1) {
        if (state == CANDIDATE) {
            // Since candidate votes for itself instantaneously, we don't need to consider others asking for its vote
            currentTerm++;

            votedFor = pid; // vote for itself

            // labels should be immidiately before sends/receives
            lab_election = 1;

            // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
            // old_term = currentTerm;
            // old_lab_election = lab_election;
            // old_commit = commitIndex;
            // old_lab_normal = old_lab_normal;
            // old_LLI = lastIndex;

            // Ask for votes
            // send(term, c_id, lastLogIndex, lastLogTerm)

            lab_election = 2;

            // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
            // old_term = currentTerm;
            // old_lab_election = lab_election;
            // old_commit = commitIndex;
            // old_lab_normal = old_lab_normal;
            // old_LLI = lastIndex;

            // Empty mbox
            num_mbox_vote = 0;

            // receive votes
            retry = random;
            while(retry && num_mbox_vote < (num/2)) {
                if (m_vote.term > currentTerm) {
                    state = FOLLOWER;
                    currentTerm = m_vote.term;
                }

                if(filter_vote(m_vote, currentTerm)) {
                    mbox_vote[num_mbox_vote] = m_vote;
                    num_mbox_vote++;
                }

                if(num_mbox_vote >= num/2)
                    break;
  
                retry = random;
            }

            timeout = random;
            if (timeout) {
                state = CANDIDATE;
            }

            if(num_mbox_vote >= num/2) {
                state = LEADER;

                // when entering a different machine
                old_lab_normal = 0;
            }
            else {
                state = CANDIDATE;
            }
        }

        if (state == FOLLOWER) {
            lab_election = 1;

            // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
            // old_term = currentTerm;
            // old_lab_election = lab_election;
            // old_commit = commitIndex;
            // old_lab_normal = old_lab_normal;
            // old_LLI = lastIndex;

            // Empty mbox
            num_mbox_reqVote = 0;

            // receive
            retry = random;
            while (retry && num_mbox_reqVote < 1) {
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

                    retry = random;
                }
            }

            timeout = random;
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
                // old_LLI = lastIndex;

                // when entering a different machine
                old_lab_normal = 0; 

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
/*            }
            else {
                state = CANDIDATE;
            }
        }

        while (state != CANDIDATE) {
            if (state == LEADER) {
                retry = random;
                while (retry) {
                    // receive command from client
                    cmd = random;
                    if (cmd == 0) {   // Empty command (HeartBeat)
                        lab_normal = 1;

                        // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                        // old_term = currentTerm;
                        // old_lab_election = lab_election;
                        // old_commit = commitIndex;
                        // old_lab_normal = old_lab_normal;
                        // old_LLI = lastIndex;

                        // send(term, leaderId, prevLogIndex, entries[], leaderCommit) with empty entries
                    }
                    else {
                        lastIndex++;

                        lab_normal = 1;

                        // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                        // old_term = currentTerm;
                        // old_lab_election = lab_election;
                        // old_commit = commitIndex;
                        // old_lab_normal = old_lab_normal;
                        // old_LLI = lastIndex;

                        // send(term, leaderId, prevLogIndex, entries[], leaderCommit)
                    }
                    retry = rand() % random;
                }

                lab_normal = 2;

                // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                // old_term = currentTerm;
                // old_lab_election = lab_election;
                // old_commit = commitIndex;
                // old_lab_normal = old_lab_normal;
                // old_LLI = lastIndex;

                /*  MBOX to be emptied after getting enough msgs
                // Empty mbox
                memset(mbox_AE_ack,0,sizeof(mbox_AE_ack));
                num_mbox_AE_ack = 0;
                */

/*                retry = random;
                while (retry) {
                    if (m_AE_ack.term > currentTerm) {
                        state = FOLLOWER;
                        currentTerm = m_AE_ack.term;
                        
                        // Just to make sure
                        // assert currentTerm > old_term;

                        // when entering a different machine
                        old_lab_election = 0;
                        
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
    
                        retry = random;
                    }
                }

                // Leader can't timeout

                if (num_mbox_AE_ack >= num/2) {
                    commitIndex++;

                    // Empty mbox
                    num_mbox_AE_ack = 0;
                }
                else { // leader can't get enough messages
                    state = CANDIDATE;

                    // when entering a different machine
                    old_lab_election = 0;

                    break;
                }
            }

            if (state == FOLLOWER) {
                lab_normal = 1;
                // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                // old_term = currentTerm;
                // old_lab_election = lab_election;
                // old_commit = commitIndex;
                // old_lab_normal = old_lab_normal;
                // old_LLI = lastIndex;
                
                // Empty mbox
                num_mbox_AE = 0;

                // receive
                retry = random;
                while (retry) {
                    if (m_AE_ack.term > currentTerm) {
                        state = FOLLOWER;
                        currentTerm = m_AE_ack.term;

                        // Just to make sure
                        // assert currentTerm > old_term;

                        // when entering a different machine
                        old_lab_election = 0;

                        break;
                    }

                    if (filter_AE(m_AE, currentTerm, lastIndex, lastTerm)) {
                        mbox_AE[num_mbox_AE] = m_AE;
                        num_mbox_AE++;
    
                        if (num_mbox_AE >= 1)
                            break;
    
                        retry = random;
                    }
                }

                timeout = random;
                if (timeout) {
                    state = CANDIDATE;

                    // when entering a different machine
                    old_lab_election = 0;

                    break;
                }

                if (num_mbox_AE_ack >= 1) {
                    if (mbox_AE[0].leaderCommit > commitIndex) {
                        commitIndex = mbox_AE[0].leaderCommit;
                        // Commit all terms till min(commitIndex, lastIndex)
                    }

                    lab_normal = 2;

                    // assert currentTerm > old_term || (currentTerm == old_term) ==> lab_election > old_lab_election || (currentTerm == old_term && lab_election == old_lab_election) ==> commitIndex > old_commit || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit) ==> lab_normal > old_lab_normal || (currentTerm == old_term && lab_election == old_lab_election && commitIndex == old_commit && lab_normal == old_lab_normal) ==> lastLogIndex >= old_LLI;            
                    // old_term = currentTerm;
                    // old_lab_election = lab_election;
                    // old_commit = commitIndex;
                    // old_lab_normal = old_lab_normal;
                    // old_LLI = lastIndex;

                    // send(term, success) to leader

                    // need to do this after the send to prove invarinat 
                    lastIndex++;
                }
                else {
                    state = CANDIDATE;

                    // when entering a different machine
                    old_lab_election = 0;

                    break;
                }
            }
        }
    }
}

*/

void leader_normal (int pid, int num) {
    int state = FOLLOWER;
    int currentTerm = 0;
    int votedFor = pid;
    //int log[1000];

    int lastIndex = 0;
    int lastTerm = 0;

    int cmd;

    int commitIndex = 0;
    int lastApplied = 0;

    //int nextIndex[100];
    //int matchIndex[100];

    //msg_reqVote mbox_reqVote[200];
    int num_mbox_reqVote = 0;
    msg_reqVote m_reqVote;

   // msg_vote mbox_vote[200];
    int num_mbox_vote = 0;
    msg_vote m_vote;

    //msg_AE mbox_AE[200];
    int num_mbox_AE = 0;
    msg_AE m_AE;

    //msg_AE_ack mbox_AE_ack[200];
    int num_mbox_AE_ack = 0;
    msg_AE_ack m_AE_ack;

    int lab_election = 0;
    int lab_normal = 0;

    int retry;
    int timeout;
    int election;

    volatile int random;

    int old_term = currentTerm - 1;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0;
    
    while (state != CANDIDATE) {
        retry = random;
        
        while (retry) {
            // receive command from client
            cmd = random;
            if (cmd == 0) {   // Empty command (HeartBeat)
                lab_normal = 1;
    
                assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                old_term = currentTerm;
                old_lab_election = lab_election;
                old_commit = commitIndex;
                old_lab_normal = lab_normal;
                old_LLI = lastIndex;

                // assert((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex == old_LLI));
    
                // send(term, leaderId, prevLogIndex, entries[], leaderCommit) with empty entries
            }
            else {
                lastIndex = lastIndex + 1;
    
                lab_normal = 1;
    
                assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                old_term = currentTerm;
                old_lab_election = lab_election;
                old_commit = commitIndex;
                old_lab_normal = lab_normal;
                old_LLI = lastIndex;
                    
                // send(term, leaderId, prevLogIndex, entries[], leaderCommit)
            }
            retry = random;
        }
    
        lab_normal = 2;
 
        assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
        old_term = currentTerm;
        old_lab_election = lab_election;
        old_commit = commitIndex;
        old_lab_normal = lab_normal;
        old_LLI = lastIndex;
    
        //  MBOX to be emptied after getting enough msgs (however we assume here that send s have been shifted leftward)
        // Empty mbox
        num_mbox_AE_ack = 0;
        
        retry = random;
        while (retry && num_mbox_AE_ack < (num/2)) {
            if (m_AE_ack.term > currentTerm) {
                state = FOLLOWER;
                currentTerm = m_AE_ack.term;
                
                // Just to make sure
                assert(currentTerm > old_term);
    
                // when entering a different machine
                // old_lab_election = 0;
                
                break;
            }
    
            if (m_AE_ack.success == 0 && m_AE_ack.term == currentTerm) {
                // set next index one by one
            }
    
            if (m_AE_ack.term == currentTerm && m_AE_ack.success == 1) {
                // mbox_AE_ack[num_mbox_AE_ack] = m_AE_ack;
                num_mbox_AE_ack = num_mbox_AE_ack + 1;
            }
    
            retry = random;
        }

        // Leader can't timeout
    
        if (num_mbox_AE_ack >= num/2) {
            commitIndex = commitIndex + 1;
        }
        else {
            //currentTerm = currentTerm + 1;
            break;
        }  
    }
}

void follower_normal (int pid, int num) {
    int state = LEADER;
    int currentTerm = 0;
    int votedFor = pid;
    //int log[1000];

    int lastIndex = 0;
    int lastTerm = 0;

    int cmd;

    int commitIndex = 0;
    int lastApplied = 0;

    int leaderCommit;

    //int nextIndex[100];
    //int matchIndex[100];

    //msg_reqVote mbox_reqVote[200];
    int num_mbox_reqVote = 0;
    msg_reqVote m_reqVote;

   // msg_vote mbox_vote[200];
    int num_mbox_vote = 0;
    msg_vote m_vote;

    //msg_AE mbox_AE[200];
    int num_mbox_AE = 0;
    msg_AE m_AE;

    //msg_AE_ack mbox_AE_ack[200];
    int num_mbox_AE_ack = 0;
    msg_AE_ack m_AE_ack;

    int lab_election = 0;
    int lab_normal = 0;

    int retry;
    int timeout;
    int election;

    volatile int random;

    int old_term = currentTerm - 1;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0;

    while (state != CANDIDATE) {
        if (state == LEADER) {
            retry = random;
            
            while (retry) {
                // receive command from client
                cmd = random;
                if (cmd == 0) {   // Empty command (HeartBeat)
                    lab_normal = 1;
        
                    assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                    old_term = currentTerm;
                    old_lab_election = lab_election;
                    old_commit = commitIndex;
                    old_lab_normal = lab_normal;
                    old_LLI = lastIndex;
    
                    // assert((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex == old_LLI));
        
                    // send(term, leaderId, prevLogIndex, entries[], leaderCommit) with empty entries
                }
                else {
                    lastIndex = lastIndex + 1;
        
                    lab_normal = 1;
        
                    assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                    old_term = currentTerm;
                    old_lab_election = lab_election;
                    old_commit = commitIndex;
                    old_lab_normal = lab_normal;
                    old_LLI = lastIndex;
                        
                    // send(term, leaderId, prevLogIndex, entries[], leaderCommit)
                }
                retry = random;
            }
        
            lab_normal = 2;
     
            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;
        
            //  MBOX to be emptied after getting enough msgs (however we assume here that send s have been shifted leftward)
            // Empty mbox
            num_mbox_AE_ack = 0;
            
            retry = random;
            while (retry && num_mbox_AE_ack < (num/2)) {
                if (m_AE_ack.term > currentTerm) {
                    state = FOLLOWER;
                    currentTerm = m_AE_ack.term;
                    
                    // Just to make sure
                    assert(currentTerm > old_term);
        
                    // when entering a different machine
                    // old_lab_election = 0;
                    
                    break;
                }
        
                if (m_AE_ack.success == 0 && m_AE_ack.term == currentTerm) {
                    // set next index one by one
                }
        
                if (filter_AE_ack(&m_AE_ack, currentTerm)) {
                    // mbox_AE_ack[num_mbox_AE_ack] = m_AE_ack;
                    num_mbox_AE_ack = num_mbox_AE_ack + 1;
                }
        
                retry = random;
            }
    
            // Leader can't timeout
        
            if (num_mbox_AE_ack >= num/2) {
                commitIndex = commitIndex + 1;
            }
            else {
                //currentTerm = currentTerm + 1;
                break;
            }  
        }

        if (state == FOLLOWER) {
            retry = random;
            while (retry) {
                lab_normal = 1;
                assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                old_term = currentTerm;
                old_lab_election = lab_election;
                old_commit = commitIndex;
                old_lab_normal = lab_normal;
                old_LLI = lastIndex;
                
                // Empty mbox
                num_mbox_AE = 0;
        
                // receive
                retry = random;
                while (retry && num_mbox_AE < 1) {
                    if (m_AE_ack.term > currentTerm) {
                        state = FOLLOWER;
                        currentTerm = m_AE_ack.term;
        
                        // Just to make sure
                        assert(currentTerm > old_term);
        
                        // when entering a different machine
                        //old_lab_election = 0;
        
                        break;
                    }
        
                    if (filter_AE(&m_AE, currentTerm, lastIndex, lastTerm)) {
                        // mbox_AE[num_mbox_AE] = m_AE;
                        num_mbox_AE = num_mbox_AE + 1;
                    }
        
                    retry = random;
                }
        
                timeout = random;
                if (timeout) {
                    state = CANDIDATE;
        
                    break;
                }
    
                if (num_mbox_AE >= 1) {
                    if (leaderCommit > commitIndex) {
                        commitIndex = leaderCommit;
                        assert(commitIndex > old_commit);
                        // Commit all terms till min(commitIndex, lastIndex)
                    }
    
                    lastIndex = lastIndex + 1;
                }
                else {
                    state = CANDIDATE;
                    break;
                }
            }
    
            lab_normal = 2;
            
            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;
    
            // send(term, success) to leader
    
            commitIndex = commitIndex + 1;
        }
    }
}

void test_2 () { 
    int retry;
    volatile int random;
    int cmd;
    int currentTerm = 0;
    int lab_election = 0;
    int commitIndex = 0;
    int lab_normal = 0;
    int lastIndex = 0;

    int old_term = 0;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0;

    retry = random;
    
    while (retry) {
        // receive command from client
        cmd = random;
        if (cmd == 0) {   // Empty command (HeartBeat)
            lab_normal = 1;

            assert(currentTerm == old_term);
            assert(old_lab_election == lab_election);
            assert(commitIndex == old_commit);
            assert(old_lab_normal == 0 || old_lab_normal == 1);
            assert(lastIndex >= old_LLI);

            //assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;

            // assert((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex == old_LLI));

            // send(term, leaderId, prevLogIndex, entries[], leaderCommit) with empty entries
        }
        else {
            lastIndex = lastIndex + 1;

            lab_normal = 1;

            assert(currentTerm == old_term);
            assert(old_lab_election == lab_election);
            assert(commitIndex == old_commit);
            assert(old_lab_normal == 0 || old_lab_normal == 1);
            assert(lastIndex >= old_LLI);

            //assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;
                
            // send(term, leaderId, prevLogIndex, entries[], leaderCommit)
        }
        retry = random;
    }
}

void test(int pid, int num) {
    int state = FOLLOWER;
    int currentTerm = 0;
    int votedFor = pid;
    //int log[1000];

    int lastIndex = 0;
    int lastTerm = 0;

    int cmd;

    int commitIndex = 0;
    int lastApplied = 0;

    int leaderCommit;

    //int nextIndex[100];
    //int matchIndex[100];

    //msg_reqVote mbox_reqVote[200];
    int num_mbox_reqVote = 0;
    msg_reqVote m_reqVote;

   // msg_vote mbox_vote[200];
    int num_mbox_vote = 0;
    msg_vote m_vote;

    //msg_AE mbox_AE[200];
    int num_mbox_AE = 0;
    msg_AE m_AE;

    //msg_AE_ack mbox_AE_ack[200];
    int num_mbox_AE_ack = 0;
    msg_AE_ack m_AE_ack;

    int lab_election = 0;
    int lab_normal = 0;

    int retry;
    int timeout;
    int election;

    volatile int random;

    int old_term = currentTerm - 1;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0; 

    while (state != CANDIDATE) {
        retry = random;
        while (retry) {
            lab_normal = 1;
            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;
            
            // Empty mbox
            num_mbox_AE = 0;
    
            // receive
            retry = random;
            while (retry && num_mbox_AE < 1) {
                if (m_AE_ack.term > currentTerm) {
                    state = FOLLOWER;
                    currentTerm = m_AE_ack.term;
    
                    // Just to make sure
                    assert(currentTerm > old_term);
    
                    // when entering a different machine
                    //old_lab_election = 0;
    
                    break;
                }
    
                if (filter_AE(&m_AE, currentTerm, lastIndex, lastTerm)) {
                    // mbox_AE[num_mbox_AE] = m_AE;
                    num_mbox_AE = num_mbox_AE + 1;
                }
    
                retry = random;
            }
    
            timeout = random;
            if (timeout) {
                state = CANDIDATE;
    
                break;
            }

            if (num_mbox_AE >= 1) {
                if (leaderCommit > commitIndex) {
                    commitIndex = leaderCommit;
                    assert(commitIndex > old_commit);
                    // Commit all terms till min(commitIndex, lastIndex)
                }

                lastIndex = lastIndex + 1;
            }
            else {
                state = CANDIDATE;
                break;
            }
        }

        lab_normal = 2;
        
        assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
        old_term = currentTerm;
        old_lab_election = lab_election;
        old_commit = commitIndex;
        old_lab_normal = lab_normal;
        old_LLI = lastIndex;

        // send(term, success) to leader

        commitIndex = commitIndex + 1;
    }
}

int candidate_election (int pid, int num) {
    int state = CANDIDATE;
    int currentTerm = 0;
    int votedFor = pid;
    //int log[1000];

    int lastIndex = 0;
    int lastTerm = 0;

    int cmd;

    int commitIndex = 0;
    int lastApplied = 0;

    int leaderCommit;

    //int nextIndex[100];
    //int matchIndex[100];

    //msg_reqVote mbox_reqVote[200];
    int num_mbox_reqVote = 0;
    msg_reqVote m_reqVote;

   // msg_vote mbox_vote[200];
    int num_mbox_vote = 0;
    msg_vote m_vote;

    //msg_AE mbox_AE[200];
    int num_mbox_AE = 0;
    msg_AE m_AE;

    //msg_AE_ack mbox_AE_ack[200];
    int num_mbox_AE_ack = 0;
    msg_AE_ack m_AE_ack;

    int lab_election = 0;
    int lab_normal = 0;

    int retry;
    int timeout;
    int election;

    volatile int random;

    int old_term = currentTerm - 1;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0; 


    while (1) {
        if (state == CANDIDATE) {
            // Since candidate votes for itself instantaneously, we don't need to consider others asking for its vote
            currentTerm = currentTerm + 1;

            votedFor = pid; // vote for itself

            // labels should be immidiately before sends/receives
            lab_election = 1;

            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;

            // Ask for votes
            // send(term, c_id, lastLogIndex, lastLogTerm)

            lab_election = 2;

            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;

            // Empty mbox
            num_mbox_vote = 0;

            // receive votes
            retry = random;
            while(retry && num_mbox_vote < (num/2)) {
                if (m_vote.term > currentTerm) {
                    state = FOLLOWER;
                    currentTerm = m_vote.term;

                    break;
                }

                if(filter_vote(&m_vote, currentTerm)) {
                    // mbox_vote[num_mbox_vote] = m_vote;
                    num_mbox_vote = num_mbox_vote + 1;
                }

                if(num_mbox_vote >= num/2)
                    break;
  
                retry = random;
            }

            timeout = random;
            if (timeout) {
                state = CANDIDATE;
            }

            if(num_mbox_vote >= num/2) {
                state = LEADER;

                // when entering a different machine
                old_lab_normal = 0;
            }
            else {
                // Not required?
            }
        }
    }
} 

void follower_election (int pid, int num) {
    int state = FOLLOWER;
    int currentTerm = 0;
    int votedFor = pid;
    //int log[1000];

    int lastIndex = 0;
    int lastTerm = 0;

    int cmd;

    int commitIndex = 0;
    int lastApplied = 0;

    int leaderCommit;

    //int nextIndex[100];
    //int matchIndex[100];

    //msg_reqVote mbox_reqVote[200];
    int num_mbox_reqVote = 0;
    msg_reqVote m_reqVote;

   // msg_vote mbox_vote[200];
    int num_mbox_vote = 0;
    msg_vote m_vote;

    //msg_AE mbox_AE[200];
    int num_mbox_AE = 0;
    msg_AE m_AE;

    //msg_AE_ack mbox_AE_ack[200];
    int num_mbox_AE_ack = 0;
    msg_AE_ack m_AE_ack;

    int lab_election = 0;
    int lab_normal = 0;

    int retry;
    int timeout;
    int election;

    volatile int random;

    int old_term = currentTerm - 1;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0; 
    while (1) {
        if (state == CANDIDATE) {
            // Since candidate votes for itself instantaneously, we don't need to consider others asking for its vote
            currentTerm = currentTerm + 1;

            votedFor = pid; // vote for itself

            // labels should be immidiately before sends/receives
            lab_election = 1;

            //assert(old_term == 0 || old_term == currentTerm - 1);
            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;

            // Ask for votes
            // send(term, c_id, lastLogIndex, lastLogTerm)

            lab_election = 2;

            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;

            // Empty mbox
            num_mbox_vote = 0;

            // receive votes
            retry = random;
            while(retry && num_mbox_vote < (num/2)) {
                if (m_vote.term > currentTerm) {
                    state = FOLLOWER;
                    currentTerm = m_vote.term;

                    break;
                }

                if(filter_vote(&m_vote, currentTerm)) {
                    // mbox_vote[num_mbox_vote] = m_vote;
                    num_mbox_vote = num_mbox_vote + 1;
                }

                if(num_mbox_vote >= num/2)
                    break;
  
                retry = random;
            }

            timeout = random;
            if (timeout) {
                state = CANDIDATE;
            }

            if(num_mbox_vote >= num/2) {
                state = LEADER;

                // when entering a different machine
                old_term = 0;
                old_lab_election = 0;
                old_commit = 0;
                old_lab_normal = 0;
                old_LLI = 0;
            }
            else {
                // Not required?
            }
        }

        if (state == FOLLOWER) {
            lab_election = 1;

            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;

            // Empty mbox
            num_mbox_reqVote = 0;

            // receive
            retry = random;
            while (retry && num_mbox_reqVote < 1) {
                if (m_vote.term > currentTerm) {
                    state = FOLLOWER;
                    currentTerm = m_vote.term;
                    break;
                }

                if (filter_reqVote(&m_reqVote,currentTerm)) {
                    // mbox_reqVote[num_mbox_reqVote] = m_reqVote;
                    num_mbox_reqVote = num_mbox_reqVote + 1;
                }

                if (num_mbox_vote >= 1) {
                    break;
                }

                retry = random;
            }

            timeout = random;
            if (timeout) {
                state = CANDIDATE;
            }

            if (num_mbox_reqVote >= 1 && state == FOLLOWER) {
                lab_election = 2;

                assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                old_term = currentTerm;
                old_lab_election = lab_election;
                old_commit = commitIndex;
                old_lab_normal = lab_normal;
                old_LLI = lastIndex;

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

                // when entering a different machine
                old_term = 0;
                old_lab_election = 0;
                old_commit = 0;
                old_lab_normal = 0;
                old_LLI = 0; 
            }
            else {
                state = CANDIDATE;

                // when entering a different machine
                old_term = 0;
                old_lab_election = 0;
                old_commit = 0;
                old_lab_normal = 0;
                old_LLI = 0;
            }
        }

        while (state != CANDIDATE) {
            if (state == LEADER) {
                retry = random;
                
                while (retry) {
                    // receive command from client
                    cmd = random;
                    if (cmd == 0) {   // Empty command (HeartBeat)
                        lab_normal = 1;
            
                        assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                        old_term = currentTerm;
                        old_lab_election = lab_election;
                        old_commit = commitIndex;
                        old_lab_normal = lab_normal;
                        old_LLI = lastIndex;
        
                        // assert((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex == old_LLI));
            
                        // send(term, leaderId, prevLogIndex, entries[], leaderCommit) with empty entries
                    }
                    else {
                        lastIndex = lastIndex + 1;
            
                        lab_normal = 1;
            
                        assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                        old_term = currentTerm;
                        old_lab_election = lab_election;
                        old_commit = commitIndex;
                        old_lab_normal = lab_normal;
                        old_LLI = lastIndex;
                            
                        // send(term, leaderId, prevLogIndex, entries[], leaderCommit)
                    }
                    retry = random;
                }
            
                lab_normal = 2;
         
                assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                old_term = currentTerm;
                old_lab_election = lab_election;
                old_commit = commitIndex;
                old_lab_normal = lab_normal;
                old_LLI = lastIndex;
            
                //  MBOX to be emptied after getting enough msgs (however we assume here that send s have been shifted leftward)
                // Empty mbox
                num_mbox_AE_ack = 0;
                
                retry = random;
                while (retry && num_mbox_AE_ack < (num/2)) {
                    if (m_AE_ack.term > currentTerm) {
                        state = FOLLOWER;
                        currentTerm = m_AE_ack.term;
                        
                        // Just to make sure
                        assert(currentTerm > old_term);
            
                        // when entering a different machine
                        // old_lab_election = 0;
                        
                        break;
                    }
            
                    if (m_AE_ack.success == 0 && m_AE_ack.term == currentTerm) {
                        // set next index one by one
                    }
            
                    if (filter_AE_ack(&m_AE_ack, currentTerm)) {
                        // mbox_AE_ack[num_mbox_AE_ack] = m_AE_ack;
                        num_mbox_AE_ack = num_mbox_AE_ack + 1;
                    }
            
                    retry = random;
                }
        
                // Leader can't timeout
            
                if (num_mbox_AE_ack >= num/2) {
                    commitIndex = commitIndex + 1;
                }
                else {
                    //currentTerm = currentTerm + 1;
                    state = CANDIDATE;

                    // when entering a different machine
                    old_term = 0;
                    old_lab_election = 0;
                    old_commit = 0;
                    old_lab_normal = 0;
                    old_LLI = 0;

                    break;
                }  
            }
    
            if (state == FOLLOWER) {
                retry = random;
                while (retry) {
                    lab_normal = 1;
                    assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                    old_term = currentTerm;
                    old_lab_election = lab_election;
                    old_commit = commitIndex;
                    old_lab_normal = lab_normal;
                    old_LLI = lastIndex;
                    
                    // Empty mbox
                    num_mbox_AE = 0;
            
                    // receive
                    retry = random;
                    while (retry && num_mbox_AE < 1) {
                        if (m_AE_ack.term > currentTerm) {
                            state = FOLLOWER;
                            currentTerm = m_AE_ack.term;
            
                            // Just to make sure
                            assert(currentTerm > old_term);
            
                            // when entering a different machine
                            //old_lab_election = 0;
            
                            break;
                        }
            
                        if (filter_AE(&m_AE, currentTerm, lastIndex, lastTerm)) {
                            // mbox_AE[num_mbox_AE] = m_AE;
                            num_mbox_AE = num_mbox_AE + 1;
                        }
            
                        retry = random;
                    }
            
                    timeout = random;
                    if (timeout) {
                        state = CANDIDATE;
            
                        break;
                    }
        
                    if (num_mbox_AE >= 1 && state == FOLLOWER) {
                        if (leaderCommit > commitIndex) {
                            commitIndex = leaderCommit;
                            assert(commitIndex > old_commit);
                            // Commit all terms till min(commitIndex, lastIndex)
                        }
        
                        lastIndex = lastIndex + 1;
                    }
                    else {
                        state = CANDIDATE;
                        break;
                    }
                }
        
                if (state == FOLLOWER) {
                    lab_normal = 2;
                    
                    assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                    old_term = currentTerm;
                    old_lab_election = lab_election;
                    old_commit = commitIndex;
                    old_lab_normal = lab_normal;
                    old_LLI = lastIndex;
            
                    // send(term, success) to leader
            
                    commitIndex = commitIndex + 1;
                }
                else {
                    // when entering a different machine
                    old_term = 0;
                    old_lab_election = 0;
                    old_commit = 0;
                    old_lab_normal = 0;
                    old_LLI = 0;
                }
            }
        }
    }
}

void NormalOp(int pid, int num, int *voting, int* state, int* currentTerm, int* lab_election, int* commitIndex, int* lab_normal, int* lastIndex, int* old_term, int* old_lab_election, int* old_commit, int* old_lab_normal, int* old_LLI) {
    //int log[1000];

    int lastTerm = 0;

    int cmd;

    int lastApplied = 0;

    int leaderCommit;

    //int nextIndex[100];
    //int matchIndex[100];

    //msg_AE mbox_AE[200];
    int num_mbox_AE = 0;
    msg_AE m_AE;

    //msg_AE_ack mbox_AE_ack[200];
    int num_mbox_AE_ack = 0;
    msg_AE_ack m_AE_ack;

    int retry;
    int timeout;
    int election;

    volatile int random;

    *old_commit = 0;
    *old_lab_normal = 0;
    *old_LLI = 0;

    while (*state != CANDIDATE) {
        if (*state == LEADER) {
            if(*voting != 2){
                if(*voting != 1) {
                    retry = random;
                    
                    
                    while (retry) {
                        // receive command from client
                        cmd = random;
                        if (cmd == 0) {   // Empty command (HeartBeat)
                            *lab_normal = 1;
                
                            assert ((*currentTerm > *old_term) || ((*currentTerm == *old_term) && (*lab_election > *old_lab_election)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex > *old_commit)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex == *old_commit) && (*lab_normal > *old_lab_normal)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex == *old_commit) && (*lab_normal == *old_lab_normal) && (*lastIndex >= *old_LLI)));            
                            *old_term = *currentTerm;
                            *old_lab_election = *lab_election;
                            *old_commit = *commitIndex;
                            *old_lab_normal = *lab_normal;
                            *old_LLI = *lastIndex;
            
                            // assert((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex == old_LLI));
                
                            // send(term, leaderId, prevLogIndex, entries[], leaderCommit) with empty entries
                        }
                        else {
                            *lastIndex = *lastIndex + 1;
                
                            *lab_normal = 1;
                            
                            assert ((*currentTerm > *old_term) || ((*currentTerm == *old_term) && (*lab_election > *old_lab_election)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex > *old_commit)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex == *old_commit) && (*lab_normal > *old_lab_normal)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex == *old_commit) && (*lab_normal == *old_lab_normal) && (*lastIndex >= *old_LLI)));            
                            *old_term = *currentTerm;
                            *old_lab_election = *lab_election;
                            *old_commit = *commitIndex;
                            *old_lab_normal = *lab_normal;
                            *old_LLI = *lastIndex;
                                
                            // send(term, leaderId, prevLogIndex, entries[], leaderCommit)
                        }
                        retry = random;
                    }
        
                    // Receive 
        
                    // Empty mbox
                    num_mbox_reqVote = 0;
                    num_mbox_AE = 0;
                }
                
                retry = random;
                while(retry) {
                    // m = receive()
                    if (m.type == 1) {   // reqVote
                        if(filter_reqVote(&m_reqVote, *currentTerm)) {
                            if(m_reqVote.term > *currentTerm) {
                                *currentTerm = m_reqVote.term;
                                *state = FOLLOWER;
                                *voting = 1;
                                return;
                            }
                            num_mbox_reqVote = num_mbox_reqVote + 1;
                        }
        
                        if(num_mbox_reqVote >= 1) {
                            break;
                        }
                    }
    
                    // Leader does not respond to Append Entries RPC
    
                    retry = random;
                }
            
                *lab_normal = 2;
                
                assert ((*currentTerm > *old_term) || ((*currentTerm == *old_term) && (*lab_election > *old_lab_election)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex > *old_commit)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex == *old_commit) && (*lab_normal > *old_lab_normal)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex == *old_commit) && (*lab_normal == *old_lab_normal) && (*lastIndex >= *old_LLI)));            
                *old_term = *currentTerm;
                *old_lab_election = *lab_election;
                *old_commit = *commitIndex;
                *old_lab_normal = *lab_normal;
                *old_LLI = *lastIndex;
            
                //  MBOX to be emptied after getting enough msgs (however we assume here that send s have been shifted leftward)
                // Empty mbox
                num_mbox_AE_ack = 0;
                num_mbox_reqVote = 0;
                num_mbox_AE = 0;
            }
            
            retry = random;
            while (retry && num_mbox_AE_ack < ((num+1)/2)) {
                if(m.type == 4) {    // Ack for Append Entries
                    if (m_AE_ack.term > *currentTerm) {
                        *state = FOLLOWER;
                        *currentTerm = m_AE_ack.term;
            
                        // when entering a different machine
                        // old_lab_election = 0;
                
                        break;
                    }
            
                    if (m_AE_ack.success == 0 && m_AE_ack.term == *currentTerm) {
                        // set next index one by one
                    }
            
                    if (filter_AE_ack(&m_AE_ack, *currentTerm)) {
                        // mbox_AE_ack[num_mbox_AE_ack] = m_AE_ack;
                        num_mbox_AE_ack = num_mbox_AE_ack + 1;
                    }
    
                    if(num_mbox_AE_ack < ((num+1)/2)) {
                        break;
                    }
                }

                if (m.type == 1) {   // reqVote
                    if(filter_reqVote(&m_reqVote, *currentTerm)) {
                        if(m_reqVote.term > *currentTerm) {
                            *currentTerm = m_reqVote.term;
                            *state = FOLLOWER;
                            *voting = 2;
                            return;
                        }
                        num_mbox_reqVote = num_mbox_reqVote + 1;
                    }
    
                    if(num_mbox_reqVote >= 1) {
                        break;
                    }
                }

                // Leader does not respond to Append Entries RPC           
        
                retry = random;
            }
    
            // Leader can't timeout
        
            if(*state == LEADER) {
                if (num_mbox_AE_ack >= (num+1)/2) {
                    *commitIndex = *commitIndex + 1;
    
                    continue;
                }
                else {
                    //currentTerm = currentTerm + 1;
                    *state = CANDIDATE;
    
                    /*
                    // when entering a different machine
                    old_term = 0;
                    old_lab_election = 0;
                    old_commit = 0;
                    old_lab_normal = 0;
                    old_LLI = 0;
                    */
                    return;
                }  
            }
        }

        if (*state == FOLLOWER) {
            if (*voting != 2) {
                retry = random;
                while (retry) {
                    *lab_normal = 1;
                    assert ((*currentTerm > *old_term) || ((*currentTerm == *old_term) && (*lab_election > *old_lab_election)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex > *old_commit)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex == *old_commit) && (*lab_normal > *old_lab_normal)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex == *old_commit) && (*lab_normal == *old_lab_normal) && (*lastIndex >= *old_LLI)));            
                    *old_term = *currentTerm;
                    *old_lab_election = *lab_election;
                    *old_commit = *commitIndex;
                    *old_lab_normal = *lab_normal;
                    *old_LLI = *lastIndex;
                    
                    // Empty mbox
                    num_mbox_AE = 0;
                    num_mbox_reqVote = 0;
            
                    // receive
                    retry = random;
                    while (retry) {
                        if (m.type == 3) {
                            if (m_AE_ack.term > *currentTerm) {
                                *state = FOLLOWER;
                                *currentTerm = m_AE_ack.term;
                
                                // Just to make sure
                                assert(*currentTerm > *old_term);
                
                                // when entering a different machine
                                //old_lab_election = 0;
                
                            }
                
                            if (filter_AE(&m_AE, *currentTerm, *lastIndex, lastTerm)) {
                                // mbox_AE[num_mbox_AE] = m_AE;
                                num_mbox_AE = num_mbox_AE + 1;
                            }
        
                            if(num_mbox_AE < 1) {
                                break;
                            }
                
                            retry = random;
                        }
    
                        if (m.type == 1) {   // reqVote
                            if(filter_reqVote(&m_reqVote, *currentTerm)) {
                                if(m_reqVote.term > *currentTerm) {
                                    *currentTerm = m_reqVote.term;
                                    *state = FOLLOWER;
                                    *voting = 1;
                                    return;
                                }
                                num_mbox_reqVote = num_mbox_reqVote + 1;
                            }
            
                            if(num_mbox_reqVote >= 1) {
                                break;
                            }
                        }
                    }
            
                    timeout = random;
                    if (timeout) {
                        *state = CANDIDATE;
            
                        return;
                    }
        
                    if (num_mbox_AE >= 1) {
                        // We use leaderCommit since array are not allowed (yet)
                        if (leaderCommit > *commitIndex) {
                            *commitIndex = leaderCommit;
                            assert(*commitIndex > *old_commit);
                            // Commit all terms till min(commitIndex, lastIndex)
                        }
        
                        *lastIndex = *lastIndex + 1;
                    }
                    else {
                        *state = CANDIDATE;
                        return;
                    }
                }
        
                *lab_normal = 2;
                
                assert ((*currentTerm > *old_term) || ((*currentTerm == *old_term) && (*lab_election > *old_lab_election)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex > *old_commit)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex == *old_commit) && (*lab_normal > *old_lab_normal)) || ((*currentTerm == *old_term) && (*lab_election == *old_lab_election) && (*commitIndex == *old_commit) && (*lab_normal == *old_lab_normal) && (*lastIndex >= *old_LLI)));            
                *old_term = *currentTerm;
                *old_lab_election = *lab_election;
                *old_commit = *commitIndex;
                *old_lab_normal = *lab_normal;
                *old_LLI = *lastIndex;
        
                // send(term, success) to leader
        
                *commitIndex = *commitIndex + 1;
    
                // Empty mbox
                num_mbox_AE = 0;
                num_mbox_reqVote = 0;
            }
    
            // receive
            retry = random;
            while (retry) {
                if (m.type == 3) {
                    if (m_AE_ack.term > *currentTerm) {
                        *state = FOLLOWER;
                        *currentTerm = m_AE_ack.term;
        
                        // when entering a different machine
                        //old_lab_election = 0;
        
                    }
        
                    if (filter_AE(&m_AE, *currentTerm, *lastIndex, lastTerm)) {
                        // mbox_AE[num_mbox_AE] = m_AE;
                        num_mbox_AE = num_mbox_AE + 1;
                    }

                    if(num_mbox_AE < 1) {
                        break;
                    }
        
                    retry = random;
                }

                if (m.type == 1) {   // reqVote
                    if(filter_reqVote(&m_reqVote, *currentTerm)) {
                        if(m_reqVote.term > *currentTerm) {
                            *currentTerm = m_reqVote.term;
                            *state = FOLLOWER;
                            *voting = 2;
                            return;
                        }
                        num_mbox_reqVote = num_mbox_reqVote + 1;
                    }
    
                    if(num_mbox_reqVote >= 1) {
                        break;
                    }
                }
            }
        }
    }
}

void election(int pid, int num) {
    int state = CANDIDATE;
    int currentTerm = 0;
    int votedFor = pid;
    //int log[1000];

    int lastIndex = 0;
    int lastTerm = 0;

    int cmd;

    int commitIndex = 0;
    int lastApplied = 0;

    int leaderCommit;

    //int nextIndex[100];
    //int matchIndex[100];

    //msg_reqVote mbox_reqVote[200];
    int num_mbox_reqVote = 0;
    msg_reqVote m_reqVote;

   // msg_vote mbox_vote[200];
    int num_mbox_vote = 0;
    msg_vote m_vote;

    int lab_election = 0;
    int lab_normal = 0;

    int retry;
    int timeout;
    int election;

    volatile int random;

    int old_term = 0;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0;

    while (1) {
        if (state == CANDIDATE) {
            // Since candidate votes for itself instantaneously, we don't need to consider others asking for its vote
            currentTerm = currentTerm + 1;

            votedFor = pid; // vote for itself

            // labels should be immidiately before sends/receives
            lab_election = 1;

            //assert(old_term == 0 || old_term == currentTerm - 1);
            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;

            // Ask for votes
            // send(term, c_id, lastLogIndex, lastLogTerm)

            // Receive 

            // Empty mbox
            num_mbox_vote = 0;
            num_mbox_AE = 0;
            
            retry = random;
            while(retry) {
                // m = receive()
                if (m.type == 1) {   // reqVote
                    if(filter_reqVote(&m_reqVote, currentTerm)) {
                        if(m_reqVote.term > currentTerm) {
                            currentTerm = m_reqVote.term;
                            state = FOLLOWER;
                            break;
                        }
                        num_mbox_reqVote = num_mbox_reqVote + 1;
                    }
    
                    if(num_mbox_reqVote >= 1) {
                        break;
                    }
                }

                if(m.type == 3) {   // appendEntries
                
                    if (filter_AE(&m_AE, currentTerm, lastIndex, lastTerm)) {
                        if (m_AE_ack.term > currentTerm) {
                            state = FOLLOWER;
                            currentTerm = m_AE_ack.term;
                            break;
                        }
                        // mbox_AE[num_mbox_AE] = m_AE;
                        num_mbox_AE = num_mbox_AE + 1;
                    }

                    if(num_mbox_AE >= 1) {
                        break;
                    }
                }

                retry = random;
            }

            if(state == CANDIDATE) {
                lab_election = 2;
                
                assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                old_term = currentTerm;
                old_lab_election = lab_election;
                old_commit = commitIndex;
                old_lab_normal = lab_normal;
                old_LLI = lastIndex;
    
                // Empty mbox
                num_mbox_vote = 0;
    
                // receive votes
                retry = random;
                while(retry) {
                    if(m.type = 2) {  // Vote
                        if (m_vote.term > currentTerm) {
                            state = FOLLOWER;
                            currentTerm = m_vote.term;
                            assert(currentTerm > old_term);
        
                            break;
                        }
        
                        if(filter_vote(&m_vote, currentTerm)) {
                            // mbox_vote[num_mbox_vote] = m_vote;
                            num_mbox_vote = num_mbox_ + 1;
                        }
        
                        if(num_mbox_vote >= (num+1)/2) {
                            break;
                        }
                    }

                    if (m.type == 1) {   // reqVote
                        if(filter_reqVote(&m_reqVote, currentTerm)) {
                            if(m_reqVote.term > currentTerm) {
                                currentTerm = m_reqVote.term;
                                state = FOLLOWER;
                                break;
                            }
                            num_mbox_reqVote = num_mbox_reqVote + 1;
                        }
        
                        if(num_mbox_reqVote >= 1) {
                            break;
                        }
                    }
    
                    if(m.type == 3) {   // appendEntries
                    
                        if (filter_AE(&m_AE, currentTerm, lastIndex, lastTerm)) {
                            if (m_AE_ack.term > currentTerm) {
                                state = FOLLOWER;
                                currentTerm = m_AE_ack.term;
                                break;
                            }
                            // mbox_AE[num_mbox_AE] = m_AE;
                            num_mbox_AE = num_mbox_AE + 1;
                        }
    
                        if(num_mbox_AE >= 1) {
                            break;
                        }
                    }    
      
                    retry = random;
                }
    
                timeout = random;
                if (timeout) {
                    state = CANDIDATE;
                }
    
                if(num_mbox_vote >= (num+1)/2) {
                    state = LEADER;
    
                    /*
                    // when entering a different machine
                    old_term = 0;
                    old_lab_election = 0;
                    old_commit = 0;
                    old_lab_normal = 0;
                    old_LLI = 0;
                    */
                }
                else {
                    // Not required?
                }
            }
        }

        if (state == FOLLOWER) {
            lab_election = 1;

            assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;

            if(voting != 0) { // Case where leader rcvs reqVote in round 1 
                // Empty mbox
                num_mbox_reqVote = 0;
                num_mbox_AE = 0;
    
                // receive
                retry = random;
                while (retry && num_mbox_reqVote < 1) {
                    if(m.type == 1) {
                        if (m_vote.term > currentTerm) {
                            state = FOLLOWER;
                            currentTerm = m_vote.term;
                            break;
                        }
        
                        if (filter_reqVote(&m_reqVote,currentTerm)) {
                            // mbox_reqVote[num_mbox_reqVote] = m_reqVote;
                            num_mbox_reqVote = num_mbox_reqVote + 1;
                        }
        
                        if (num_mbox_reqVote >= 1) {
                            break;
                        }
                    }
    
                    if(m.type == 3) {   // appendEntries
                        
                        if (filter_AE(&m_AE, currentTerm, lastIndex, lastTerm)) {
                            if (m_AE_ack.term > currentTerm) {
                                state = FOLLOWER;
                                currentTerm = m_AE_ack.term;
                                break;
                            }
                            // mbox_AE[num_mbox_AE] = m_AE;
                            num_mbox_AE = num_mbox_AE + 1;
                        }
    
                        if(num_mbox_AE >= 1) {
                            break;
                        }
                    }
    
                    retry = random;
                }
            }

            timeout = random;
            if (timeout) {
                state = CANDIDATE;
            }

            if (num_mbox_reqVote >= 1 && state == FOLLOWER) {
                lab_election = 2;

                assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
                old_term = currentTerm;
                old_lab_election = lab_election;
                old_commit = commitIndex;
                old_lab_normal = lab_normal;
                old_LLI = lastIndex;

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
                            voting = 0;
                        }
                        else
                            send vote with success = 0;
                    }
                }
                */

                /*
                // when entering a different machine
                old_term = 0;
                old_lab_election = 0;
                old_commit = 0;
                old_lab_normal = 0;
                old_LLI = 0;
                */

                // Empty mbox
                num_mbox_reqVote = 0;
                num_mbox_AE = 0;
    
                // receive
                retry = random;
                while (retry && num_mbox_reqVote < 1) {
                    if(m.type == 1) {
                        if (m_vote.term > currentTerm) {
                            state = FOLLOWER;
                            currentTerm = m_vote.term;
                            break;
                        }
        
                        if (filter_reqVote(&m_reqVote,currentTerm)) {
                            // mbox_reqVote[num_mbox_reqVote] = m_reqVote;
                            num_mbox_reqVote = num_mbox_reqVote + 1;
                        }
        
                        if (num_mbox_reqVote >= 1) {
                            break;
                        }
                    }
    
                    if(m.type == 3) {   // appendEntries
                        
                        if (filter_AE(&m_AE, currentTerm, lastIndex, lastTerm)) {
                            if (m_AE_ack.term > currentTerm) {
                                state = FOLLOWER;
                                currentTerm = m_AE_ack.term;
                                break;
                            }
                            // mbox_AE[num_mbox_AE] = m_AE;
                            num_mbox_AE = num_mbox_AE + 1;
                        }
    
                        if(num_mbox_AE >= 1) {
                            break;
                        }
                    }
    
                    retry = random;
                }
            }
            else {
                state = CANDIDATE;

                /*
                // when entering a different machine
                old_term = 0;
                old_lab_election = 0;
                old_commit = 0;
                old_lab_normal = 0;
                old_LLI = 0;
                */
            }
        } 

        if (state != CANDIDATE) {
            NormalOp(pid, num, &state, &currentTerm, &lab_election, &commitIndex, &lab_normal, &lastIndex, &old_term, &old_lab_election, &old_commit, &old_lab_normal, &old_LLI);
            // assert(state == CANDIDATE || state == FOLLOWER);
            // assert((state == CANDIDATE) || ((state == FOLLOWER) && (currentTerm > old_term)));
        }
    }

}

int main() {
    //Raft(0,5);
    //follower_normal(0,5);
    int state = LEADER;
    int old_term = 0;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0;

    int currentTerm = 0;
    int lab_election = 0;
    int commitIndex = 0;
    int lab_normal = 0;
    int lastIndex = 0;

    //NormalOp(0,5, &state, &currentTerm, &lab_election, &commitIndex, &lab_normal, &lastIndex, &old_term, &old_lab_election, &old_commit, &old_lab_normal, &old_LLI);
    //assert(state == CANDIDATE);
    //assert((state == CANDIDATE) || ((state == FOLLOWER) && (currentTerm > old_term)));
    election(0,5);
    return 0;
}