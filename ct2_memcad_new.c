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

    msg m;
    msg* mbox[200];
    int num_mbox = 0;
    int num_mbox_commit = 0;

    // Non deterministic variables
    int retry;
    int timeout;

    volatile int random;

    while(state != 1) {    // while state is undecided
        leader = (round % num) + 1;

        lab = 1;

        assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
        old_round = round;
        old_lab = lab;

        if (pid == leader) {
            // Empty mbox
            num_mbox = 0;
            num_mbox_commit = 0;

            retry = random;

            while(1){ 
                // m = receive()
                if(m.lab == 1) {  // Estimate sent by followers to leader
                    if(filter_est(&m, round)) {
                        if(m.round > round) {
                            round = m.round;

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
                
                if(m.lab == 4) {   // Commit Received
                    if(filter_commit(&m)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }
                
                retry = random;
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m.estimate;
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
                if(m.lab == 4) {   // Commit Received
                    assert(m.lab == 4);
                    if(filter_commit(&m)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }

                // Timeout is required because it is possible that commit doesnt arrive in this round
                timeout = random;
                if(timeout) {
                    break;
                }
                
                retry = random;
            }
            assert(num_mbox_commit == 0);

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m.estimate;
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

            retry = random;

            while(1){ 
                // m = receive()
                if(m.lab == 3) {  // Estimate sent by followers to leader
                    if(filter_ack(&m, round)) {
                        if(m.round > round) {
                            round = m.round;

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
                
                if(m.lab == 4) {   // Commit Received
                    if(filter_commit(&m)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }
                
                retry = random;
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m.estimate;
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
                if(m.lab == 4) {   // Commit Received
                    if(filter_commit(&m)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }

                // Timeout is required because it is possible that commit doesnt arrive in this round
                timeout = random;
                if(timeout) {
                    break;
                }
                
                retry = random;
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m.estimate;
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

            retry = random;

            while(1){ 
                // m = receive()
                if(m.lab == 1) {  // Estimate sent by followers to leader
                    if(filter_propose(&m, round)) {
                        if(m.round > round) {
                            round = m.round;

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
                
                if(m.lab == 4) {   // Commit Received
                    if(filter_commit(&m)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }
                
                retry = random;
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m.estimate;
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
                if(m.lab == 4) {   // Commit Received
                    if(filter_commit(&m)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }

                // Timeout is required because it is possible that commit doesnt arrive in this round
                timeout = random;
                if(timeout) {
                    break;
                }
                
                retry = random;
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m.estimate;
                state = 1;
                break;
            }

            round = round + 1;
        }
    }
}

int main() {
    propose(1, 5, 10); // pid, num, estimate
}