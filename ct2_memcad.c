typedef struct _msg_est {
    int pid;
    int round;
    int estimate;
    int timestamp;
} msg_est;

int filter_est(msg_est* m, int round) {
    if (m->round == round) {
        return 1;
    }
    return 0;
}

typedef struct _msg_propose {
    int pid;
    int round;
    int estimate;
} msg_propose;

int filter_propose(msg_propose* m, int round) {
    if (m->round == round) {
        return 1;
    }
    return 0;
}

typedef struct _msg_ack {
    int pid; 
    int round;
    int ack; // 1 -> message is an ack, 0 -> message is a nack
} msg_ack;

int filter_ack (msg_ack* m, int round) {
    if (m->round == round) {
        return 1;
    }
    return 0;
}

typedef struct _msg_commit {
    int pid;
    int round;
    int estimate;
} msg_commit;

// If the message is of commit type, we are done
int filter_commit(msg_commit* m) {
    return 1;
}

void rand(int* i) {
    int p = *i;
    _memcad("assume(p >= 0)");
    _memcad("assume(p <= 1)");
}

int propose(int pid, int num, int estimate) {
    int state = 0; // 0 -> undecided, 1 -> decided
    int round = 0;
    int timestamp = 0;
    int leader = 0;
    int lab = 0;
    
    int old_lab = 0;
    int old_round = round - 1; // for first assertion on first iteration

    msg_est m_est;
    msg_est* mbox_est[200];
    int num_mbox_est = 0;

    msg_propose m_propose;
    msg_propose* mbox_propose[200];
    int num_mbox_propose = 0;

    msg_ack m_ack;
    msg_ack* mbox_ack[200];
    int num_mbox_ack = 0;

    msg_commit m_commit1;
    msg_commit m_commit2;
    msg_commit m_commit3;
    msg_commit m_commit4;
    msg_commit m_commit5;
    msg_commit m_commit6;
    msg_commit m_commit7;
    msg_commit* mbox_commit[200];
    int num_mbox_commit = 0;

    int retry1;
    int retry2;
    int retry3;

    int commit1;
    int commit2;
    int commit3;
    int commit4;
    int commit5;
    int commit6;
    int commit7;

    int nack; // ack or nack

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

            // retry = rand() % 2;
            // rand(&retry);

            while(retry1 && (num_mbox_est < (num + 1)/2) ){            
                if(filter_est(&m_est, round)) {
                    mbox_est[num_mbox_est] = &m_est;
                    num_mbox_est = num_mbox_est + 1;
                }
            
                if (num_mbox_est >= (num + 1)/2) {
                    break;
                }  
                
                //retry = rand() % 2;
                // rand(&retry);
            }

            if (num_mbox_est >= (num + 1)/2) {
                // pick message with highest timestamp
                // pick estimate

                // Non deterministic R-Deliver
                //commit = rand() % 2;
                // rand(&commit);
                if(commit1) {
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
                        if(filter_commit(&m_commit1)) {
                            mbox_commit[num_mbox_commit] = &m_commit1;
                            num_mbox_commit = num_mbox_commit + 1;
                        }

                        if (num_mbox_commit >= 1) {
                            break;
                        }
                    }
                
                    if(num_mbox_commit >= 1) {
                        estimate = m_commit1.estimate;
                        state = 1;
                        break;
                    }
                }

                lab = 2;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                // send (p, round, estimate) to all 

                // Non deterministic R-Deliver
                //commit = rand() % 2;
                // rand(&commit);
                if(commit2) {
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
                        if(filter_commit(&m_commit2)) {
                            mbox_commit[num_mbox_commit] = &m_commit2;
                            num_mbox_commit = num_mbox_commit + 1;
                        }

                        if (num_mbox_commit >= 1) {
                            break;
                        }
                    }
                
                    if(num_mbox_commit >= 1) {
                        estimate = m_commit2.estimate;
                        state = 1;
                        break;
                    }
                }

                lab = 3;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;

                // receive
                // Empty mbox
                num_mbox_ack = 0;
            
                // retry = rand() % 2;
                // rand(&retry);
                while(retry2 && (num_mbox_ack < (num + 1)/2)) {
                    if(filter_ack(&m_ack, round)) {
                        mbox_ack[num_mbox_ack] = &m_ack;
                        
                        // We add to the mbox all valid replies but only increment the counter if the reply is an Ack
                        if(m_ack.ack) {
                            num_mbox_ack = num_mbox_ack + 1;
                        } 
                    }
                    
                    if (num_mbox_ack >= (num + 1)/2) {
                        break; 
                    } 
                    
                    // retry = rand() % 2;
                    // rand(&retry);
                }

                // The case where not all messages in the quorum are acks is handled by retry

                if (num_mbox_ack >= (num + 1)/2) {
                    // Non deterministic R-Deliver
                    // commit = rand() % 2;
                    // rand(&commit);
                    if(commit3) {
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
                            if(filter_commit(&m_commit3)) {
                                mbox_commit[num_mbox_commit] = &m_commit3;
                                num_mbox_commit = num_mbox_commit + 1;
                            }
    
                            if (num_mbox_commit >= 1) {
                                break;
                            }
                        }
                    
                        if(num_mbox_commit >= 1) {
                            estimate = m_commit3.estimate;
                            state = 1;
                            break;
                        }
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
            // commit = rand() % 2;
            // rand(&commit);
            if(commit4) {
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
                    if(filter_commit(&m_commit4)) {
                        mbox_commit[num_mbox_commit] = &m_commit4;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }
            
                if(num_mbox_commit >= 1) {
                    estimate = m_commit4.estimate;
                    state = 1;
                    break;
                }
            }

            lab = 2;

            // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
            assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
            old_round = round;
            old_lab = lab;
    
            // receive
            // Empty mbox
            num_mbox_propose = 0;
            
            // retry = rand() % 2;
            // rand(&retry);
            while(retry3 && num_mbox_propose < 1){
                if(filter_propose(&m_propose, round)) {
                    mbox_propose[num_mbox_propose] = &m_propose;
                    num_mbox_propose = num_mbox_propose + 1;
                }

                if (num_mbox_propose >= 1) {
                    break;
                }  
                
                // retry = rand() % 2;
                // rand(&retry);
            }
    
            if (num_mbox_propose >= 1) {
                // Update estimate and timestamp

                // Non deterministic R-Deliver
                // commit = rand() % 2;
                // rand(&commit);
                if(commit5) {
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
                        if(filter_commit(&m_commit5)) {
                            mbox_commit[num_mbox_commit] = &m_commit5;
                            num_mbox_commit = num_mbox_commit + 1;
                        }
    
                        if (num_mbox_commit >= 1) {
                            break;
                        }
                    }
                
                    if(num_mbox_commit >= 1) {
                        estimate = m_commit5.estimate;
                        state = 1;
                        break;
                    }
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
                //commit = rand() % 2;
                // rand(&commit);
                if(commit6) {
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
                        if(filter_commit(&m_commit6)) {
                            mbox_commit[num_mbox_commit] = &m_commit6;
                            num_mbox_commit = num_mbox_commit + 1;
                        }
    
                        if (num_mbox_commit >= 1) {
                            break;
                        }
                    }
                
                    if(num_mbox_commit >= 1) {
                        estimate = m_commit6.estimate;
                        state = 1;
                        break;
                    }
                }

                lab = 3;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_lab = lab;
    
                // send (p, round, nack) to leader
            }

            // Non deterministic R-Deliver
            //commit = rand() % 2;
            // rand(&commit);
            if(commit7) {
                lab = 4;

                // assert lab == 4 || (round > old_round) || ((round == old_round) ==> lab >= old_lab);
                assert((lab == 4) || (round > old_round) || ((round == old_round) && (lab > old_lab)));
                old_round = round;
                old_round = lab;
            
                // receive 
                // Empty mbox
                num_mbox_commit = 0;
                
                // NO retry because if the process transistions (non deterministically) to
                // lab = 4 it must have received an R-Deliver 
                while(num_mbox_commit < 1) {
                    if(filter_commit(&m_commit7)) {
                        mbox_commit[num_mbox_commit] = &m_commit7;
                        num_mbox_commit = num_mbox_commit + 1;
                    }

                    if (num_mbox_commit >= 1) {
                        break;
                    }
                }
            
                if(num_mbox_commit >= 1) {
                    estimate = m_commit7.estimate;
                    state = 1;
                    break;
                }
            }

            round = round + 1;
        }
    }
}

int main() {
    propose(1, 5, 10); // pid, num, estimate
}