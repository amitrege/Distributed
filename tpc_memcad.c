typedef struct _msg {
    int count;
    int lab;
    int response; // Only for Commit Request 2 (0 -> abort, 1 -> commit)
} msg;

int filter_cr1 (msg* m, int count) {
    if (m->count == count) {
        return 1;
    }
    return 0;
}

int filter_cr2 (msg* m, int count) {
    if (m->count == count) {
        return 1;
    }
    return 0;
}

int filter_c1 (msg* m, int count) {
    if (m->count == count) {
        return 1;
    }
    return 0;
}

int filter_c2 (msg* m, int count) {
    if (m->count == count) {
        return 1;
    }
    return 0;
}

msg init_msg(int count, int lab) {
    msg m;
    m.count = count;
    m.lab = lab;
    m.response = 0;
    return m;
}

msg init_cr2(int count, int lab, int response) {
    msg m;
    m.count = count;
    m.lab = lab;
    m.response = response;
    return m;
}

void TwoPhaseCommit(int pid, int leader, int num) {
    int commit = 0;
    
    // Tags
    int count = 1;
    int lab = 1;

    int num_mbox = 0;
    int num_acks = 0;
    
    volatile int random;
    
    int retry;

    int old_count = count - 1;
    int old_lab = 0;

    msg m;
    // msg* mbox[200];  // memcad doesn't handle variable size

    while (1) {
        // New Transaction

        lab = 1; // Commit Request 1
        
        assert(count == (old_count + 1));        
        assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
        old_count = count;
        old_lab = lab;

        if (pid == leader) {
            msg cr1;
            cr1.count = count;
            cr1.lab = lab;
            // send Transaction to all
            assert((cr1.count == count) && (cr1.lab == lab));

            lab = lab + 1; // lab = 2 -> Commit Request 2

            assert((count == old_count) && (lab == (old_lab + 1)));        
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            // receive msgs in mbox
            // Empty mbox
            num_mbox = 0;
            num_acks = 0;

            while(1){
                // m = receive()
                if(m.lab == 2) {
                    if(filter_cr2(&m, count)) {
                        if(m.response == 1) {
                            num_acks = num_acks + 1;
                        }
                        // mbox[num_mbox] = &m;
                        num_mbox = num_mbox + 1;
                    }
                    
                    if(num_mbox == num) {
                        break;
                    }
                }                
            }

            if (num_acks == num) {
                commit = 1;  // Move to Commit Phase
                lab = lab + 1; // lab = 3 -> Commit Phase 1

                assert((count == old_count) && (lab == (old_lab + 1)));        
                assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                old_count = count;
                old_lab = lab;

                msg c1_commit;
                c1_commit.count = count;
                c1_commit.lab = lab; 
                // send Commit to all
                assert((c1_commit.count == count) && (c1_commit.lab == lab));
            }
            else {
                commit = 0; // Perform rollback
                lab = lab + 1; // lab = 3 -> Commit Phase 1
                
                assert((count == old_count) && (lab == (old_lab + 1)));        
                assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                old_count = count;
                old_lab = lab;
        
                msg c1_rollback;
                c1_rollback.count = count;
                c1_rollback.lab = lab;
                // send RollBack to all
                assert((c1_rollback.count == count) && (c1_rollback.lab == lab));
            }

            lab = lab + 1; // lab = 4 -> Commit Phase 2

            assert((count == old_count) && (lab == (old_lab + 1)));        
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            // receive
            // Empty mbox
            num_mbox = 0;           

            while(1){
                // m = receive()
                if(m.lab == 4) {
                    if(filter_c2(&m, count)) {
                        // mbox[num_mbox] = &m;
                        num_mbox = num_mbox + 1;
                    }
                    
                    if(num_mbox == num) {
                        break;
                    }
                }       
            } 
            
            count = count + 1;
        }
        else {
            // receive Transaction
            // Empty mbox
            num_mbox = 0;
             
            while(1) { 
                // m = receive()
                if(m.lab == 1) {
                    if(filter_cr1(&m, count)) {
                        // mbox[num_mbox] = &m;
                        num_mbox = num_mbox + 1;
                    }
                    
                    if(num_mbox == 1) {
                        break;
                    }
                }    
            }

            lab = lab + 1; // lab = 2 -> Commit Request 2
            
            assert((count == old_count) && (lab == (old_lab + 1)));        
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;
        
            int r; // This is the response (can be 0 or 1)
            msg cr2;
            cr2.count = count;
            cr2.lab = lab;
            cr2.response = r;
            // send Respomse to leader
            assert((cr2.count == count) && (cr2.lab == lab));

            lab = lab + 1; // lab = 3 -> Commit Phase 1 
            
            assert((count == old_count) && (lab == (old_lab + 1)));        
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            // receive
            // Empty mbox
            num_mbox = 0; 

            while(1){
                // m = receive()
                if(m.lab == 3) {
                    if(filter_c1(&m, count)) {
                        // mbox[num_mbox] = &m;
                        num_mbox = num_mbox + 1;
                    }
                    
                    if(num_mbox == 1) {
                        break;
                    }
                }    
            }

            // Do something depending on whether the msg is commit or rollback

            // Complete Transaction
            lab = lab + 1; // lab = 4 -> Commit Phase 2
            
            assert((count == old_count) && (lab == (old_lab + 1)));        
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            msg c2;
            c2.count = count;
            c2.lab = lab;
            // send Ack to leaderr
            assert((c2.count == count) && (c2.lab == lab));

            count = count + 1;
        } 
    }
}

int main () {
    TwoPhaseCommit(1,0,3);
    return 0;
}