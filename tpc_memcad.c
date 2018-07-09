int filter_cr1 (int m) {
    if (m == 1)
        return 1;
    return 0;
}

int filter_cr2 (int m) {
    if (m == 2)
        return 1;
    return 0;
}

int filter_c1 (int m) {
    if (m == 3)
        return 1;
    return 0;
}

int filter_c2 (int m) {
    if (m == 4)
        return 1;
    return 0;
}

/*@
    predicate gt(int count, int lab, int old_count, int old_lab) =
    (count > old_count) || ((count == old_count) ==> lab >= old_lab);
*/

void TwoPhaseCommit(int pid, int leader, int num) {
    int commit = 0;
    int count = 1;
    int num_mbox = 0;
    int retry;

    int lab = 1;

    int old_count = count - 1;
    int old_lab = 0;

    int failure = 0;

    int m;
    int mbox[2*num];

    while (1) {
        // New Transaction

        lab = 1; // Commit Request 1
        
        // assert (count > old_count) || ((count == old_count) ==> lab >= old_lab) || failure;
        old_count = count;
        old_lab = lab;

        if (pid == leader) {
            // send Transaction to all

            lab = 2; // Commit Request 2

            // assert (count > old_count) || ((count == old_count) ==> lab >= old_lab)
            assert(count == old_count);
            assert(lab > old_lab);
            old_count = count;
            old_lab = lab;

            // Empty mbox
            for (int i=0; i < 2*num; i++)
                mbox[i] = 0;
            num_mbox = 0;
            
            // receive msgs in mbox
            retry = rand() % 2;
            while(retry){
                m = (rand() % 4) + 1;
                if(filter_cr2(m)) {
                    mbox[num_mbox] = m;
                    num_mbox++;
                }
                if(num_mbox == num) 
                    break;  
                retry = rand() % 2;
            }

            if (num_mbox == num) {
                commit = 1;  // Move to Commit Phase
                lab = 3; // Commit Phase 1

                // assert (count > old_count) || ((count == old_count) ==> lab >= old_lab)
                assert(count == old_count);
                assert(lab > old_lab);
                old_count = count;
                old_lab = lab;

                // send commit message
            }
            else {
                commit = 0; // Perform rollback
                lab = 3;

                // assert (count > old_count) || ((count == old_count) ==> lab >= old_lab)
                assert(count == old_count);
                assert(lab > old_lab);
                old_count = count;
                old_lab = lab;
        
                // send rollback message
            }

            lab = 4;

            // assert (count > old_count) || ((count == old_count) ==> lab >= old_lab)
            assert(count == old_count);
            assert(lab > old_lab);
            old_count = count;
            old_lab = lab;

            // Empty mbox
            for (int i=0; i < 2*num; i++)
                mbox[i] = 0;
            num_mbox = 0;
    
            // receive
            retry = rand() % 2;
            while(retry){
                m = (rand() % 4) + 1;
                if(filter_c2(m)) {
                    mbox[num_mbox] = m;
                    num_mbox++;
                }  
                if(num_mbox == num) 
                    break;
                retry = rand() % 2;
            } 
            
            if (num_mbox == num) {
                // Complete Transaction
                count++;
            }
            else {
                failure = 1;
            }
        }
        else {
            // Empty mbox
            for (int i=0; i < 2*num; i++)
            mbox[i] = 0;
            num_mbox = 0;   
            
            // receive Transaction
            retry = rand() % 2;
            while(retry) {
                m = (rand() % 4) + 1;
                
                if(filter_cr1(m)) {
                    mbox[num_mbox] = m;
                    num_mbox++;
                } 

                if(num_mbox >= 1) 
                    break; 
                
                retry = rand() % 2;
            }

            if (num_mbox >= 1) {
                lab = 2; // Commit Request 2
    
                // assert (count > old_count) || ((count == old_count) ==> lab >= old_lab)
                // old_count = count;
                // old_lab = lab;
            
                // send agreement/ abort to leader

                lab = 3;
                
                // assert (count > old_count) || ((count == old_count) ==> lab >= old_lab)
                // old_count = count;
                // old_lab = lab;

                // Empty mbox
                for (int i=0; i < 2*num; i++)
                    mbox[i] = 0;
                num_mbox = 0;
        
                // receive
                retry = rand() % 2;
                while(retry){
                    m = (rand() % 4) + 1;
                    if(filter_c1(m)) {
                        mbox[num_mbox] = m;
                        num_mbox++;
                    }  
                    if(num_mbox >= 1) 
                        break;
                    retry = rand() % 2;
                }

                if (num_mbox >= 1) {
                    // Complete Transaction
                    lab = 4; // Commit Phase 2
                
                    // assert (count > old_count) || ((count == old_count) ==> lab >= old_lab)
                    // old_count = count;
                    // old_lab = lab;

                    // send ack

                    count++;
                }
                else {
                    failure = 1;
                }
            }
            else {
                failure = 1;
            }
        } 
    }
}

int main () {
    TwoPhaseCommit(0,0,3);
    return 0;
}