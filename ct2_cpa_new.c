#include "assert.h"
#include<stdlib.h>

typedef struct _msg {
    int lab;
    int pid;
    int round;
    int estimate;
    int timestamp;
    int ack;
} msg;

int filter_est(msg* m, int round) {
    if (m->round >= round) {
        return 1;
    }
    return 0;
}

int filter_propose(msg* m, int round) {
    if (m->round >= round) {
        return 1;
    }
    return 0;
}

int filter_ack (msg* m, int round) {
    if (m->round >= round) {
        return 1;
    }
    return 0;
}

// If the message is of commit type, we are done
int filter_commit(msg* m) {
    return 1;
}

int propose(int pid, int num, int estimate) {
    int state = 0; // 0 -> undecided, 1 -> decided
    int round = 0;
    int timestamp = 0;
    int leader = 0;
    int lab = 0;
    
    int old_lab = 0;
    int old_round = round - 1; // for first assertion on first iteration

    // Memcad requires new variables to be used for each reception loop
    // For leader branch
    msg m_1;
    msg m_2;
    msg m_3;

    // For follower branch
    msg m_4;
    msg m_5;
    msg m_6;

    msg* mbox[200];
    int num_mbox = 0;
    int num_mbox_commit = 0;

    // Non deterministic variables
    int retry;
    int timeout;

    while(state != 1 && round < 100) {    // while state is undecided
        leader = (round % num) + 1;

        lab = 1;

        assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
        old_round = round;
        old_lab = lab;

        if (pid == leader) {
            // Empty mbox
            num_mbox = 0;
            num_mbox_commit = 0;

            retry = rand();

            while(1){ 
                // m = receive()
                if(m_1.lab == 1) {  // Estimate sent by followers to leader
                    if(filter_est(&m_1, round)) {
                        if(m_1.round > round) {
                            round = m_1.round;

                            // Empty Mbox
                            num_mbox = 0;
                        }
                        //mbox_est[num_mbox_est] = &m_est;
                        num_mbox = num_mbox + 1;
                    }
                
                    if (num_mbox >= (num + 1)/2) {
                        break;
                    }
                }
                
                if(m_1.lab == 4) {   // Commit Received
                    if(filter_commit(&m_1)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }
                
                retry = rand();
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_1.estimate;
                state = 1;
                break;
            }

            // pick message with highest timestamp
            // pick estimate

            lab = 2;
            
            // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;

            // send (p, round, estimate) to all

            // Empty mbox
            num_mbox_commit = 0;

            while(1){
                // m = receive()
                if(m_2.lab == 4) {   // Commit Received
                    if(filter_commit(&m_2)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }

                // Timeout is required because it is possible that commit doesnt arrive in this round
                timeout = rand();
                if(timeout) {
                    break;
                }
                
                retry = rand();
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_2.estimate;
                state = 1;
                break;
            }

            lab = 3;
            
            // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;

            // Empty mbox
            num_mbox = 0;
            num_mbox_commit = 0;

            retry = rand();

            while(1){ 
                // m = receive()
                if(m_3.lab == 3) {  // Estimate sent by followers to leader
                    if(filter_ack(&m_3, round)) {
                        if(m_3.round > round) {
                            round = m_3.round;

                            // Empty Mbox
                            num_mbox = 0;
                        }
                        //mbox_est[num_mbox_est] = &m_est;
                        num_mbox = num_mbox + 1;
                    }
                
                    if (num_mbox >= (num + 1)/2) {
                        break;
                    }
                }
                
                if(m_3.lab == 4) {   // Commit Received
                    if(filter_commit(&m_3)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }
                
                retry = rand();
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_3.estimate;
                state = 1;
                break;
            }

            lab = 4;
            
            // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;

            // send commit (p, round, estimate, decide)
            
            // State is decided
            state = 1;
            break;
        }
        else {
            // send (p, round, est, ts) to leader

            // Empty mbox
            num_mbox_commit = 0;
            
            while(1){
                // m = receive()
                if(m_4.lab == 4) {   // Commit Received
                    if(filter_commit(&m_4)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }

                // Timeout is required because it is possible that commit doesnt arrive in this round
                timeout = rand();
                if(timeout) {
                    break;
                }
                
                retry = rand();
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_4.estimate;
                state = 1;
                break;
            }

            lab = 2;

            // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;

            // Empty mbox
            num_mbox = 0;
            num_mbox_commit = 0;

            retry = rand();

            while(1){ 
                // m = receive()
                if(m_5.lab == 1) {  // Estimate sent by followers to leader
                    if(filter_propose(&m_5, round)) {
                        if(m_5.round > round) {
                            round = m_5.round;

                            // Empty Mbox
                            num_mbox = 0;
                        }
                        //mbox_est[num_mbox_est] = &m_est;
                        num_mbox = num_mbox + 1;
                    }
                
                    if (num_mbox >= (num + 1)/2) {
                        break;
                    }
                }
                
                if(m_5.lab == 4) {   // Commit Received
                    if(filter_commit(&m_5)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }
                
                retry = rand();
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_5.estimate;
                state = 1;
                break;
            }

            // Update estimate and timestamp

            lab = 3;
            
            // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;

            // send (p, round, ack) to leader

            // Empty mbox
            num_mbox_commit = 0;
            
            while(1){
                // m = receive()
                if(m_6.lab == 4) {   // Commit Received
                    if(filter_commit(&m_6)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }

                // Timeout is required because it is possible that commit doesnt arrive in this round
                timeout = rand();
                if(timeout) {
                    break;
                }
                
                retry = rand();
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_6.estimate;
                state = 1;
                break;
            }

            round = round + 1;
        }
    }
}

int main() {
    propose(2, 5, 10); // pid, num, estimate
}