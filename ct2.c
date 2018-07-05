#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

typedef struct _msg_est {
    int pid;
    int round;
    int estimate;
    int timestamp;
} msg_est;

msg_est init_msg_est(int num, int round) {
    msg_est m;
    m.pid = rand() % num;
    m.round = rand();
    m.estimate = rand();
    m.timestamp = rand() % (round);
    return m;
}

int filter_est(msg_est m, int round) {
    if (m.round == round)
        return 1;
    return 0;
}

typedef struct _msg_propose {
    int pid;
    int round;
    int estimate;
} msg_propose;

msg_propose init_msg_propose(int num) {
    msg_propose m;
    m.pid = rand() % num;
    m.round = rand();
    m.estimate = rand();
    return m;
}

int filter_propose(msg_propose m, int round) {
    if (m.round == round)
        return 1;
    return 0;
}

typedef struct _msg_ack {
    int pid;
    int round;
} msg_ack;

msg_ack init_msg_ack(int num) {
    msg_ack m;
    m.pid = rand() % num;
    m.round = rand();
    return m;
}

int filter_ack (msg_ack m, int round) {
    if (m.round == round)
        return 1;
    return 0;
}

typedef struct _msg_commit {
    int pid;
    int round;
    int estimate;
} msg_commit;

msg_commit init_msg_commit(int num) {
    msg_commit m;
    m.pid = rand() % num;
    m.round = rand();
    m.estimate = rand();
    return m;
}

int propose(int pid, int num, int estimate) {
    int state = 0; // 0 -> undecided, 1 -> decided
    int round = 1;
    int timestamp = 0;
    int leader = 0;

    //@ ghost lab = 1;
    //@ ghost old_lab = 0;
    //@ ghost old_round = 0;

    msg_est m_est;
    msg_est mbox_est[2*num];
    int num_mbox_est = 0;

    msg_propose m_propose;
    msg_propose mbox_propose[2*num];
    int num_mbox_propose = 0;

    msg_ack m_ack;
    msg_ack mbox_ack[2*num];
    int num_mbox_ack = 0;

    msg_commit m_commit;
    msg_commit mbox_commit[2*num];
    int num_mbox_commit = 0;

    int retry;
    int commit;

    while(state != 1) {    // while state is undecided
        leader = (round % num) + 1;

        // lab = 1;

        // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
        // old_count = count;
        // old_lab = lab;
        // failure = 0;

        if (pid == leader) {
            
            // Empty mbox_est
            memset(mbox_est,0,sizeof(mbox_est));
            num_mbox_est = 0;

            // receive
            retry = rand() % 2;
            while(retry){
                m_est = init_msg_est(num, round);
            
                if(filter_est(m_est, round)) {
                    mbox_est[num_mbox_est] = m_est;
                    num_mbox_est++;
                }
            
                if (num_mbox_est >= (num + 1)/2)
                    break;  
                
                retry = rand() % 2;
            }

            if (num_mbox_est >= (num + 1)/2) {
                // pick message with highest timestamp
                // pick estimate

                // Non deterministic R-Deliver
                commit = rand() % 2;
                if(commit) {
                    // lab = 4;

                    // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                    // old_count = count;
                    // old_lab = lab;
                
                    // receive
                    m_commit = init_msg_commit(num); // has to be deterministic as lab = 4 is the greatest 
                                                  // round possible and thus should be last
                
                    estimate = m_commit.estimate;
                    state = 1;
                    break;
                }

                //@ ghost lab = 2;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                // old_count = count;
                // old_lab = lab;

                // send (p, round, estimate) to all 

                // Non deterministic R-Deliver
                commit = rand() % 2;
                if(commit) {
                    // lab = 4;

                    // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                    // old_count = count;
                    // old_lab = lab;
                
                    // receive
                    m_commit = init_msg_commit(num); // has to be deterministic as lab = 4 is the greatest 
                                                  // round possible and thus should be last
                
                    estimate = m_commit.estimate;
                    state = 1;
                    break;
                }

                // lab = 3;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                // old_count = count;
                // old_lab = lab;

                // Empty mbox_ack
                memset(mbox_ack,0,sizeof(mbox_ack));
                num_mbox_ack = 0;
        
                // receive
                retry = rand() % 2;
                while(retry) {
                    m_ack = init_msg_ack(num);
                    
                    if(filter_ack(m_ack, round)) {
                        mbox_ack[num_mbox_ack] = m_ack;
                        // We add to the mbox all valid replies but only increment the counter if the reply is an Ack
                        num_mbox_ack = num_mbox_ack + (rand() % 2);
                    }
                    
                    if (num_mbox_ack >= (num + 1)/2)
                        break;  
                    
                    retry = rand() % 2;
                }

                if (num_mbox_ack >= (num + 1)/2) {
                    // Non deterministic R-Deliver
                    commit = rand() % 2;
                    if(commit) {
                        // lab = 4;
                        
                        // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                        // old_count = count;
                        // old_lab = lab;
                    
                        // receive
                        m_commit = init_msg_commit(num); // has to be deterministic as lab = 4 is the greatest 
                                                      // round possible and thus should be last
                    
                        estimate = m_commit.estimate;
                        state = 1;
                        break;
                    }

                    //@ ghost lab = 4;

                    // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                    // old_count = count;
                    // old_lab = lab;
        
                    // send commit (p, round, estimate, decide)
                    
                    // State is decided
                    state = 1;
                    break;
                }
                else {
                    round++;
                }
            }
            else {
                round++;
            }
        }
        else {
            // send (p, round, est, ts) to leader

            // Non deterministic R-Deliver
            commit = rand() % 2;
            if(commit) {
                // lab = 4;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                // old_count = count;
                // old_lab = lab;
            
                // receive
                m_commit = init_msg_commit(num); // has to be deterministic as lab = 4 is the greatest 
                                              // round possible and thus should be last
            
                estimate = m_commit.estimate;
                state = 1;
                break;
            }

            //@ ghost lab = 2;

            // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
            // old_count = count;
            // old_lab = lab;

            // Empty mbox_propose
            memset(mbox_propose,0,sizeof(mbox_propose));
            num_mbox_propose = 0;
    
            // receive
            retry = rand() % 2;
            while(retry){
                m_propose = init_msg_propose(num);
            
                if(filter_propose(m_propose, round)) {
                    mbox_propose[num_mbox_propose] = m_propose;
                    num_mbox_propose++;
                }

                if (num_mbox_propose >= 1)
                    break;  
                
                retry = rand() % 2;
            }
    
            if (num_mbox_propose >= 1) {
                // Update estimate and timestamp

                // Non deterministic R-Deliver
                commit = rand() % 2;
                if(commit) {
                    // lab = 4;
                    
                    // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                    // old_count = count;
                    // old_lab = lab;
                
                    // receive
                    m_commit = init_msg_commit(num); // has to be deterministic as lab = 4 is the greatest 
                                                  // round possible and thus should be last
                
                    estimate = m_commit.estimate;
                    state = 1;
                    break;
                }
    
                //@ ghost lab = 3;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                // old_count = count;
                // old_lab = lab;
    
                // send (p, round, ack) to leader
            }
            else {
                // Non deterministic R-Deliver
                commit = rand() % 2;
                if(commit) {
                    // lab = 4;

                    // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                    // old_count = count;
                    // old_lab = lab;
                
                    // receive
                    m_commit = init_msg_commit(num); // has to be deterministic as lab = 4 is the greatest 
                                                  // round possible and thus should be last
                
                    estimate = m_commit.estimate;
                    state = 1;
                    break;
                }

                //@ ghost lab = 3;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                // old_count = count;
                // old_lab = lab;
    
                // send (p, round, nack) to leader
            }

            // Non deterministic R-Deliver
            commit = rand() % 2;
            if(commit) {
                // lab = 4;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                // old_count = count;
                // old_lab = lab;
            
                // receive
                m_commit = init_msg_commit(num); // has to be deterministic as lab = 4 is the greatest 
                                              // round possible and thus should be last
            
                estimate = m_commit.estimate;
                state = 1;
                break;
            }

            round++;
        }
    }
}