typedef struct _msg {
    int lab;
    int pid;
    int round;
    int estimate;
    int timestamp;
    int ack;
} msg;

int filter_est(msg* m, int round) {
    if (m->round == round) {
        return 1;
    }
    return 0;
}

int filter_propose(msg* m, int round) {
    if (m->round == round) {
        return 1;
    }
    return 0;
}

int filter_ack (msg* m, int round) {
    if (m->round == round) {
        return 1;
    }
    return 0;
}

// If the message is of commit type, we are done
int filter_commit(msg* m) {
    return 1;
}

// The non deterministic R deliver is not a sequential round so it has only one aseert unlike others
// which have two - one for showing increasing tags and one for showing the sequence of rounds
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

    volatile int random;

    while(state != 1) {    // while state is undecided
        leader = (round % num) + 1;

        lab = 1;

        assert(round == (old_round + 1));
        assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
        old_round = round;
        old_lab = lab;

        if (pid == leader) {
            // Empty mbox
            num_mbox = 0;
            num_mbox_commit = 0;

            while(1){ 
                // m = receive()
                if(m_1.lab == 1) {  // Estimate sent by followers to leader
                    if(filter_est(&m_1, round)) {
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
                
                retry = random;
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_1.estimate;
                state = 1;
                break;
            }

            // pick message with highest timestamp
            // pick estimate

            lab = lab + 1; // lab = 2
            
            assert((round == old_round) && (lab == (old_lab + 1)));
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;

            msg m_propose;
            m_propose.lab = lab;
            m_propose.estimate = estimate;
            m_propose.round = round;
            m_propose.pid = pid;
            // send (p, round, estimate) to all
            assert((m_propose.round == round) && (m_propose.lab = lab));

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
                timeout = random;
                if(timeout) {
                    break;
                }
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_2.estimate;
                state = 1;
                break;
            }

            lab = lab + 1; // lab = 3
            
            assert((round == old_round) && (lab == (old_lab + 1)));
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;

            // Empty mbox
            num_mbox = 0;
            num_mbox_commit = 0;

            while(1){ 
                // m = receive()
                if(m_3.lab == 3) {  // Estimate sent by followers to leader
                    if(filter_ack(&m_3, round)) {
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
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_3.estimate;
                state = 1;
                break;
            }

            lab = 4; // lab = 4
            
            assert((round == old_round) && (lab == (old_lab + 1)));
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;

            msg m_commit;
            m_commit.lab = lab;
            m_commit.estimate = estimate;
            m_commit.round = round;
            m_commit.pid = pid;
            // send commit (p, round, estimate, decide)
            assert((m_commit.round == round) && (m_commit.lab = lab));
            
            
            // State is decided
            state = 1;
            break;
        }
        else {
            msg m_est;
            m_est.lab = lab;
            m_est.estimate = estimate;
            m_est.round = round;
            m_est.pid = pid;
            m_est.timestamp = timestamp;
            // send (p, round, est, ts) to leader
            assert((m_est.round == round) && (m_est.lab = lab));
            
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
                timeout = random;
                if(timeout) {
                    break;
                }
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_4.estimate;
                state = 1;
                break;
            }

            lab = lab + 1; // lab = 2

            assert((round == old_round) && (lab == (old_lab + 1)));
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;

            // Empty mbox
            num_mbox = 0;
            num_mbox_commit = 0;

            while(1){ 
                // m = receive()
                if(m_5.lab == 1) {  // Estimate sent by followers to leader
                    if(filter_propose(&m_5, round)) {
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
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                estimate = m_5.estimate;
                state = 1;
                break;
            }

            // Update estimate and timestamp

            lab = lab + 1; // lab = 3
            
            assert((round == old_round) && (lab == (old_lab + 1)));
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;

            // Send an ack or nack
            int ack;
            msg m_ack;
            m_ack.lab = lab;
            m_ack.round = round;
            m_ack.pid = pid;
            m_ack.ack = ack;
            // send (p, round, ack) to leader
            assert((m_ack.round == round) && (m_ack.lab = lab));            

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
                timeout = random;
                if(timeout) {
                    break;
                }
            }

            if(num_mbox_commit >= 1) {
                lab = 4;
                
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