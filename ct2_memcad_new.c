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

    // Non deterministic variables
    int retry;
    int commit;

    volatile int random;

    while(state != 1 && round < 10000) {    // while state is undecided
        leader = (round % num) + 1;

        lab = 1;

        assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
        old_round = round;
        old_lab = lab;

        if (pid == leader) {
            // receive
            // Empty mbox
            num_mbox_est = 0;

            retry = random;

            while(retry && (num_mbox_est < (num + 1)/2) ){            
                if(filter_est(&m_est, round)) {
                    //mbox_est[num_mbox_est] = &m_est;
                    num_mbox_est = num_mbox_est + 1;
                }
            
                if (num_mbox_est >= (num + 1)/2) {
                    break;
                }  
                
                retry = random;
            }

            if (num_mbox_est >= (num + 1)/2) {
                // pick message with highest timestamp
                // pick estimate

                // Non deterministic R-Deliver
                commit = random;
                if(commit) {
                    lab = 4;

                    // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                    assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                    old_round = round;
                    old_lab = lab;

                    
                    // receive 
                    // Empty mbox
                    num_mbox_commit = 0;

                    // NO retry because if the process transistions (non deterministically) to
                    // lab = 4 it must have received an R-Deliver 
                    while(num_mbox_commit < 1) {
                        if(filter_commit(&m_commit)) {
                            //mbox_commit[num_mbox_commit] = &m_commit;
                            num_mbox_commit = num_mbox_commit + 1;
                        }

                        if (num_mbox_commit >= 1) {
                            break;
                        }
                    }
                
                    if(num_mbox_commit >= 1) {
                        estimate = m_commit.estimate;
                        state = 1;
                        break;
                    }
                    
/*
                    estimate = m_commit.estimate;
                    state = 1;
                    break;
*/
                }

                lab = 2;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                // send (p, round, estimate) to all 

                // Non deterministic R-Deliver
                commit = random;
                if(commit) {
                    lab = 4;

                    // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                    assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                    old_round = round;
                    old_lab = lab;

                    // receive 
                    // Empty mbox
                    num_mbox_commit = 0;
                    
                    // NO retry because if the process transistions (non deterministically) to
                    // lab = 4 it must have received an R-Deliver 
                    while(num_mbox_commit < 1) {
                        if(filter_commit(&m_commit)) {
                            //mbox_commit[num_mbox_commit] = &m_commit;
                            num_mbox_commit = num_mbox_commit + 1;
                        }

                        if (num_mbox_commit >= 1) {
                            break;
                        }
                    }
                
                    if(num_mbox_commit >= 1) {
                        estimate = m_commit.estimate;
                        state = 1;
                        break;
                    }
                /*
                    estimate = m_commit.estimate;
                    state = 1;
                    break;*/
                }

                lab = 3;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                // receive
                // Empty mbox
                num_mbox_ack = 0;
            
                retry = random;
                while(retry && (num_mbox_ack < (num + 1)/2)) {
                    if(filter_ack(&m_ack, round)) {
                        //mbox_ack[num_mbox_ack] = &m_ack;
                        
                        // We add to the mbox all valid replies but only increment the counter if the reply is an Ack
                        if(m_ack.ack) {
                            num_mbox_ack = num_mbox_ack + 1;
                        } 
                    }
                    
                    if (num_mbox_ack >= (num + 1)/2) {
                        break; 
                    } 
                    
                    retry = random;
                }

                // The case where not all messages in the quorum are acks is handled by retry

                if (num_mbox_ack >= (num + 1)/2) {
                    // Non deterministic R-Deliver
                    commit = random;
                    if(commit) {
                        lab = 4;
                        
                        // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                        assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                        old_round = round;
                        old_lab = lab;

                        // receive 
                        // Empty mbox
                        num_mbox_commit = 0;
                        
                        // NO retry because if the process transistions (non deterministically) to
                        // lab = 4 it must have received an R-Deliver 
                        while(num_mbox_commit < 1) {
                            if(filter_commit(&m_commit)) {
                                //mbox_commit[num_mbox_commit] = &m_commit;
                                num_mbox_commit = num_mbox_commit + 1;
                            }
    
                            if (num_mbox_commit >= 1) {
                                break;
                            }
                        }
                    
                        if(num_mbox_commit >= 1) {
                            estimate = m_commit.estimate;
                            state = 1;
                            break;
                        }
/*
                        estimate = m_commit.estimate;
                        state = 1;
                        break;*/
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
                    round = round + 1;
                }
            }
            else {
                round = round + 1;
            }
        }
        else {
            // send (p, round, est, ts) to leader

            // Non deterministic R-Deliver
            commit = random;
            if(commit) {
                lab = 4;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                num_mbox_commit = 0;
                
                // NO retry because if the process transistions (non deterministically) to
                // lab = 4 it must have received an R-Deliver 
                while(num_mbox_commit < 1) {
                    if(filter_commit(&m_commit)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }
            
                if(num_mbox_commit >= 1) {
                    estimate = m_commit.estimate;
                    state = 1;
                    break;
                }
/*
                estimate = m_commit.estimate;
                state = 1;
                break;*/
            }

            lab = 2;

            // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;
    
            // receive
            // Empty mbox
            num_mbox_propose = 0;
            
            retry = random;
            while(retry && num_mbox_propose < 1){
                if(filter_propose(&m_propose, round)) {
                    //mbox_propose[num_mbox_propose] = &m_propose;
                    num_mbox_propose = num_mbox_propose + 1;
                }

                if (num_mbox_propose >= 1) {
                    break;
                }  
                
                retry = random;
            }
    
            if (num_mbox_propose >= 1) {
                // Update estimate and timestamp

                // Non deterministic R-Deliver
                commit = random;
                if(commit) {
                    lab = 4;
                    
                    // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                    assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                    old_round = round;
                    old_lab = lab;

                    num_mbox_commit = 0;
                    
                    // NO retry because if the process transistions (non deterministically) to
                    // lab = 4 it must have received an R-Deliver 
                    while(num_mbox_commit < 1) {
                        if(filter_commit(&m_commit)) {
                            //mbox_commit[num_mbox_commit] = &m_commit;
                            num_mbox_commit = num_mbox_commit + 1;
                        }

                        if (num_mbox_commit >= 1) {
                            break;
                        }
                    }
                
                    if(num_mbox_commit >= 1) {
                        estimate = m_commit.estimate;
                        state = 1;
                        break;
                    }
                /*
                    estimate = m_commit.estimate;
                    state = 1;
                    break;*/
                }
    
                lab = 3;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;
    
                // send (p, round, ack) to leader
            }
            else {
                // Non deterministic R-Deliver
                commit = random;
                if(commit) {
                    lab = 4;

                    // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                    assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                    old_round = round;
                    old_lab = lab;

                    num_mbox_commit = 0;
                    
                    // NO retry because if the process transistions (non deterministically) to
                    // lab = 4 it must have received an R-Deliver 
                    while(num_mbox_commit < 1) {
                        if(filter_commit(&m_commit)) {
                            //mbox_commit[num_mbox_commit] = &m_commit;
                            num_mbox_commit = num_mbox_commit + 1;
                        }

                        if (num_mbox_commit >= 1) {
                            break;
                        }
                    }
                
                    if(num_mbox_commit >= 1) {
                        estimate = m_commit.estimate;
                        state = 1;
                        break;
                    }
                /*
                    estimate = m_commit.estimate;
                    state = 1;
                    break;*/
                }

                lab = 3;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;
    
                // send (p, round, nack) to leader
            }

            // Non deterministic R-Deliver
            commit = random;
            if(commit) {
                lab = 4;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_round = lab;

                num_mbox_commit = 0;
                
                // NO retry because if the process transistions (non deterministically) to
                // lab = 4 it must have received an R-Deliver 
                while(num_mbox_commit < 1) {
                    if(filter_commit(&m_commit)) {
                        //mbox_commit[num_mbox_commit] = &m_commit;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }
            
                if(num_mbox_commit >= 1) {
                    estimate = m_commit.estimate;
                    state = 1;
                    break;
                }
            /*
                estimate = m_commit.estimate;
                state = 1;
                break;*/
            }

            round = round + 1;
        }
    }
}

int main() {
    propose(1, 5, 10); // pid, num, estimate
}