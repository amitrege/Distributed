int p = 0;
int lab = 0;
int i = 0;
int labr = 0;
int leader = 0;

typedef struct _msg {
    int p;
    int lab;
    int i;
    int labr;
    int val;
    // history not included (as it is not relevant to prove increasing tags)
} msg;

int filter_propose (msg* m, int p, int i) {
    if (m->p >= p && m->i >= i)
        return 1;
    return 0;
}

int filter_ack_p (msg* m, int p, int lab, int i, int labr) {
    if (m->p >= p && m->lab == lab && m->i >= i && m->labr == labr)
        return 1;
    return 0;
}

int filter_cmt (msg* m, int p, int i, int labr) {
    if (m->p >= p && m->i >= i && m->labr == labr)
        return 1;
    return 0;
}

int filter_curr_e (msg* m, int p, int lab) {
    if (m->p >= p && m->lab >= lab)
        return 1;
    return 0;
}

int filter_new_e (msg* m, int p, int lab) {
    if (m->p >= p && m->lab == lab)
        return 1;
    return 0;
}

int filter_ack_e (msg* m, int p, int lab) {
    if (m->p >= p && m->lab == lab)
        return 1;
    return 0;
}

int filter_new_l (msg* m, int p, int lab) {
    if (m->p >= p && m->lab == lab)
        return 1;
    return 0;
}

int filter_ack_l (msg* m, int p, int lab) {
    if (m->p == p && m->lab == lab)
        return 1;
    return 0;
}

int filter_com (msg* m, int p, int lab) {
    if (m->p >= p && m->lab >= lab)
        return 1;
    return 0;
}

int sendingThread (int p, int lab, int i, int labr){

    i = 1;
    labr = 1;

    int old_p = p;
    int old_lab = lab;
    int old_i = i;
    int old_labr = labr;
    
    while (1 && i < 10000){
        // Send v-in
        i = i + 1;

        assert ((p > old_p) || ((p == old_p) && lab > old_lab) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));
        old_p = p;
        old_lab = lab;
        old_i = i;
        old_labr = labr;
    }
}

int Broadcast (int num, int pid, int leader, int *p, int *lab, int *i, int *labr, int *old_p, int *old_lab, int *old_i, int *old_labr) {

    int num_mbox = 0;
    msg m_propose;

    //int num_mbox_ack_p = 0;
    msg m_ack_p;

    //int num_mbox_cmt = 0;
    msg m_cmt;

    int retry;

    volatile int random;

    // To clear the first assert on loop entry
    *old_i = 0;
    *old_labr = 0;

    *old_p = *p;
    *old_lab = *lab;

    *i = 1;

    while(1) {
        *labr = 1; // Prp

        assert((*p > *old_p) || ((*p == *old_p) && (*lab > *old_lab)) || ((*p == *old_p) && (*lab == *old_lab) && (*i > *old_i)) || ((*p == *old_p) && (*lab == *old_lab) && (*i == *old_i) && (*labr >= *old_labr)));
        *old_p = *p;
        *old_lab = *lab;
        *old_i = *i;
        *old_labr = *labr;

        if (pid != leader) {
            // Empty Mbox
            num_mbox = 0;

            // receive propose
            retry = random;
            while(1) {
                // m = receive()
                if(m_propose.labr == 1) {
                    if(filter_propose(&m_propose, *p, *i)) {
                        if(m_propose.p > *p) {
                            // Skip forward
                            *p = m_propose.p;

                            // Empty Mbox
                            num_mbox = 0;
                        }
                        //mbox_propose[num_mbox_propose] = &m_propose;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= 1) {
                        break;
                    }
                }   
    
                retry = random;
            }

            // Update mbox
            // Update history
            
            *labr = 2; // ack_p
            
            assert((*p > *old_p) || ((*p == *old_p) && (*lab > *old_lab)) || ((*p == *old_p) && (*lab == *old_lab) && (*i > *old_i)) || ((*p == *old_p) && (*lab == *old_lab) && (*i == *old_i) && (*labr >= *old_labr)));                
            *old_p = *p;
            *old_lab = *lab;
            *old_i = *i;
            *old_labr = *labr;

            // send(p, lab, i, labr, val) to leader

            *labr = 3; // cmt

            assert((*p > *old_p) || ((*p == *old_p) && (*lab > *old_lab)) || ((*p == *old_p) && (*lab == *old_lab) && (*i > *old_i)) || ((*p == *old_p) && (*lab == *old_lab) && (*i == *old_i) && (*labr >= *old_labr)));                
            *old_p = *p;
            *old_lab = *lab;
            *old_i = *i;
            *old_labr = *labr;

            // Empty Mbox
            num_mbox = 0;
            
            // receive commit
            retry = random;
            while(1) {
                // m = receive()
                if(m_propose.labr == 3) {
                    if(filter_cmt(&m_cmt, *p, *i, *labr)) {
                        if(m_cmt.p > *p) {
                            // Skip forward
                            *p = m_cmt.p;

                            // Empty Mbox
                            num_mbox = 0;
                        }
                        //mbox_propose[num_mbox_propose] = &m_propose;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= 1) {
                        break;
                    }
                }   
    
                retry = random;
            }

            // Commit
            // out()
            
            (*i) = (*i) + 1;
        }
        else {
            // send propose

            *labr = 2; // ack_p
            
            assert((*p > *old_p) || ((*p == *old_p) && (*lab > *old_lab)) || ((*p == *old_p) && (*lab == *old_lab) && (*i > *old_i)) || ((*p == *old_p) && (*lab == *old_lab) && (*i == *old_i) && (*labr >= *old_labr)));            
            *old_p = *p;
            *old_lab = *lab;
            *old_i = *i;
            *old_labr = *labr;

            // Empty Mbox
            num_mbox = 0;
            
            // receive commit
            retry = random;
            while(1) {
                // m = receive()
                if(m_ack_p.labr == 2) {
                    if(filter_ack_p(&m_ack_p, *p, *lab, *i, *labr)) {
                        if(m_ack_p.p > *p) {
                            // Skip forward
                            *p = m_ack_p.p;

                            // Empty Mbox
                            num_mbox = 0;
                        }
                        //mbox_propose[num_mbox_propose] = &m_propose;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= (num+1)/2) {
                        break;
                    }
                }   
    
                retry = random;
            }

            *labr = 3; // Commit
            
            assert((*p > *old_p) || ((*p == *old_p) && (*lab > *old_lab)) || ((*p == *old_p) && (*lab == *old_lab) && (*i > *old_i)) || ((*p == *old_p) && (*lab == *old_lab) && (*i == *old_i) && (*labr >= *old_labr)));                
            *old_p = *p;
            *old_lab = *lab;
            *old_i = *i;
            *old_labr = *labr;

            // send commit (labr, a, i) to all

            (*i) = (*i) + 1;
        }
    }
}
/*
int main_thread(int pid, int num){
    int lab = 0;
    int p = 0;
    int i = 0;
    int labr = 0;

    int old_p = p - 1;
    int old_lab = 0;
    int old_i = 0;
    int old_labr = 0;

    int leader;

    // msg_curr_e* mbox_curr_e[200];
    int num_mbox_curr_e = 0;
    msg_curr_e m_curr_e;

    // msg_new_e* mbox_new_e[200];
    int num_mbox_new_e = 0;
    msg_new_e m_new_e;

    // msg_ack_e* mbox_ack_e[200];
    int num_mbox_ack_e = 0;
    msg_ack_e m_ack_e;

    // msg_new_l* mbox_new_l[200];
    int num_mbox_new_l = 0;
    msg_new_l m_new_l;

    // msg_ack_l* mbox_ack_l[200];
    int num_mbox_ack_l = 0;
    msg_ack_l m_ack_l;

    // msg_com* mbox_com[200];
    int num_mbox_com = 0;
    msg_com m_com;

    int retry;
    volatile int random;

    while (p < 1000) {
        //leader = rand() % num;
        //_memcad("assume(leader >= 0)");
        //_memcad("assume(leader <= num)");
        leader = p % num;

        lab = 1; // Curr_E

        assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
        // assert((p > old_p));
        old_p = p;
        old_lab = lab;
        old_i = i;
        old_labr = labr;
        
        // send (p,lab) to leader
    
        if (leader == pid){
            // receive curr_e

            // Empty mbox
            num_mbox_curr_e = 0;

            // retry = rand() % 2;
            // rand(&retry);
            retry = random;
            while(retry && (num_mbox_curr_e < ((num+1)/2))) {
                if(filter_curr_e(&m_curr_e, p, lab)) {
                    //mbox_curr_e[num_mbox_curr_e] = &m_curr_e;
                    num_mbox_curr_e = num_mbox_curr_e + 1;
                }
    
                if (num_mbox_curr_e >= (num+1)/2) {
                    break;
                }
    
                retry = random;
            }
            
            if (num_mbox_curr_e >= (num+1)/2) {
                // Update set Q which is the set of pids in mbox
                // get max p in mbox and add 1

                // rand(&retry);
                retry = random;
                if(retry) {  // Actually, p is the max value of all p's received by the leader
                    p = p + 1;
                }
                else {
                    p = p + 2;
                } 
                p = p + 1;

                lab = 2; // new_e

                assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
                // assert(lab > old_lab);
                old_p = p;
                old_lab = lab;
                old_i = i;
                old_labr = labr;

                //send p to all (in set q)

                lab = 3; // ack_e

                assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
                // assert(lab > old_lab);
                old_p = p;
                old_lab = lab;
                old_i = i;
                old_labr = labr;

                // receive ack_e

                // Empty mbox
                num_mbox_ack_e = 0;

                // retry = rand() % 2;
                // rand(&retry);
                retry = random;
                while(retry && (num_mbox_ack_e < ((num+1)/2))) {
                    if(filter_ack_e(&m_ack_e, p, lab)) {
                        //mbox_ack_e[num_mbox_ack_e] = &m_ack_e;
                        num_mbox_ack_e = num_mbox_ack_e + 1;
                    }
        
                    if (num_mbox_ack_e >= (num+1)/2) {
                        break;
                    }
        
                    // retry = rand() % 2;
                    //rand(&retry);
                    retry = random;
                }

                if(num_mbox_ack_e >= (num+1)/2) {
                    // Update history

                    lab = 4; // new_l

                    assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
                    // assert(lab > old_lab);
                    old_p = p;
                    old_lab = lab;
                    old_i = i;
                    old_labr = labr;

                    // send (p, lab, h) to Q

                    lab = 5; // ack_l

                    assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
                    // assert(lab > old_lab);
                    old_p = p;
                    old_lab = lab;
                    old_i = i;
                    old_labr = labr;

                    // receive ack_l

                    // Empty mbox
                    num_mbox_ack_l = 0;

                    // retry = rand() % 2;
                    // rand(&retry);
                    retry = random;
                    while(retry && (num_mbox_ack_l < ((num+1)/2))) {
                        if(filter_ack_l(&m_ack_l, p, lab)) {
                            //mbox_ack_l[num_mbox_ack_l] = &m_ack_l;
                            num_mbox_ack_l = num_mbox_ack_l + 1;
                        }
            
                        if (num_mbox_ack_l >= (num+1)/2) {
                            break;
                        }
            
                        // retry = rand() % 2;
                        // rand(&retry);
                        retry = random;
                    }
                    
                    if (num_mbox_ack_l >= (num+1)/2) {
                        lab = 6; // cmt

                        assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
                        // assert(lab > old_lab);
                        old_p = p;
                        old_lab = lab;
                        old_i = i;
                        old_labr = labr;

                        // send (p, lab, h) to Q

                        // Start Broadcast
                        //Broadcast(num, pid, leader, &p, &lab, &i, &labr, &old_p, &old_lab, &old_i, &old_labr);
                        
                        // Simulating Brooadcast
                        retry = random;
                        if(retry) {
                            p = p + 1;
                        }
                        else {
                            i = i + 1;
                            p = p + 1;
                        }
                    }
                    else {
                        p = p + 1;
                    } 
                }
                else {
                    p = p + 1;
                } 
            }
            else {
                p = p + 1;
            }
        }
        else {
            lab = 2; // New_E

            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
            // assert(lab > old_lab);
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;

            // receive new_e

            // Empty mbox
            num_mbox_new_e = 0;

            // retry = rand() % 2;
            // rand(&retry);
            retry = random;
            while(retry && num_mbox_new_e < 1) {
                if(filter_new_e(&m_new_e, p, lab)) {
                    if(m_new_e.p > p) {
                        // Skip forward
                        p = m_new_e.p;
                    }
                    //mbox_new_e[num_mbox_new_e] = &m_new_e;
                    num_mbox_new_e = num_mbox_new_e + 1;
                }
    
                if (num_mbox_new_e >= 1) {
                    break;
                }
    
                // retry = rand() % 2;
                // rand(&retry);
                retry = random;
            }
            
            if (num_mbox_new_e >= 1) {
                // rand(&retry);
                retry = random;
                if(retry) {  // Actually, p is the max value of all p's received by the leader
                    p = p + 1;
                }
                else {
                    p = p + 2;
                } 
                p = p + 1;

                lab = 3; // ack_e

                assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
                // assert(lab > old_lab);
                old_p = p;
                old_lab = lab;
                old_i = i;
                old_labr = labr;

                // send (p, lab, a, h) to leader

                lab = 4; // new_l

                assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
                // assert(lab > old_lab);
                old_p = p;
                old_lab = lab;
                old_i = i;
                old_labr = labr;

                // receive new_l

                // Empty mbox
                num_mbox_new_l = 0;

                // retry = rand() % 2;
                // rand(&retry);
                retry = random;
                while(retry && num_mbox_new_l < 1) {
                    if(filter_new_l(&m_new_l, p, lab)) {
                        if(m_new_l.p > p) {
                            // Skip forward
                            p = m_new_l.p;
                        }
                        //mbox_new_l[num_mbox_new_l] = &m_new_l;
                        num_mbox_new_l = num_mbox_new_l + 1;
                    }
        
                    if (num_mbox_new_l >= 1) {
                        break;
                    }
        
                    // retry = rand() % 2;
                    // rand(&retry);
                    retry = random;
                }

                if(num_mbox_new_l >= 1) {
                    // Update a
                    // Update history

                    lab = 5; // ack_l

                    assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
                    // assert(lab > old_lab);
                    old_p = p;
                    old_lab = lab;
                    old_i = i;
                    old_labr = labr;

                    // send (lab, a, h) to leader

                    lab = 6; // cmt

                    assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
                    // assert(lab > old_lab);
                    old_p = p;
                    old_lab = lab;
                    old_i = i;
                    old_labr = labr;
                    
                    // receive commit

                    // Empty mbox
                    num_mbox_com = 0;

                    // retry = rand() % 2;
                    // rand(&retry);
                    retry = random;
                    while(retry && num_mbox_com < 1) {
                        if(filter_com(&m_com, p, lab)) {
                            if(m_new_l.p > p) {
                                // Skip forward
                                p = m_new_l.p;
                            }
                            //mbox_com[num_mbox_com] = &m_com;
                            num_mbox_com = num_mbox_com + 1;
                        }
            
                        if (num_mbox_com >= 1) {
                            break;
                        }
            
                        // retry = rand() % 2;
                        // rand(&retry);
                        retry = random;
                    }

                    if (num_mbox_com >= 1) {
                        // Update zvid

                        // Start Broadcast
                        //Broadcast(num, pid, leader, &p, &lab, &i, &labr, &old_p, &old_lab, &old_i, &old_labr);
                        
                        // Simulating Brooadcast
                        retry = random;
                        if(retry) {
                            p = p + 1;
                        }
                        else {
                            i = i + 1;
                            p = p + 1;
                        }
                    }
                    else {
                        p = p + 1;
                        continue;
                    }
                }
                else {
                    p = p + 1;
                    continue;
                }
            }
            else {
                // Special Case (state transfer?) 
                p = p + 1;
                continue;
            }
        }
    }
}

int leadership (int num) {
    int lab = 0;
    int p = 0;
    int i = 0;
    int labr = 0;

    int old_p = p - 1;
    int old_lab = 0;
    int old_i = 0;
    int old_labr = 0;

    int leader;

    msg_curr_e* mbox_curr_e[200];
    int num_mbox_curr_e = 0;
    msg_curr_e m_curr_e;

    msg_new_e* mbox_new_e[200];
    int num_mbox_new_e = 0;
    msg_new_e m_new_e;

    msg_ack_e* mbox_ack_e[200];
    int num_mbox_ack_e = 0;
    msg_ack_e m_ack_e;

    msg_new_l* mbox_new_l[200];
    int num_mbox_new_l = 0;
    msg_new_l m_new_l;

    msg_ack_l* mbox_ack_l[200];
    int num_mbox_ack_l = 0;
    msg_ack_l m_ack_l;

    msg_com* mbox_com[200];
    int num_mbox_com = 0;
    msg_com m_com;

    int retry;

    volatile int random;
    
    while (1) {
        lab = 1; // Curr_E
        
        //assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
        assert(p > old_p);
        old_p = p;
        old_lab = lab;
        old_i = i;
        old_labr = labr;
        
        // send (p,lab) to leader
    
        // receive curr_e
        retry = random;
        while(retry && (num_mbox_curr_e < ((num+1)/2))) {
            if(filter_curr_e(&m_curr_e, p, lab)) {
                // mbox_curr_e[num_mbox_curr_e] = &m_curr_e;
                num_mbox_curr_e = num_mbox_curr_e + 1;
            }
    
            if (num_mbox_curr_e > ((num+1)/2)) {
                break;
            }
    
            retry = random;
        }
        
        if (num_mbox_curr_e >= ((num+1)/2)) {
            // Update set Q which is the set of pids in mbox
            // get max p in mbox and add 1
    
            retry = random;
            if(retry) {  // Actually, p is the max value of all p's received by the leader
                p = p + 1;
            }
            else {
                p = p + 2;
            } 
            p = p + 1;
    
            lab = 2; // new_e
    
            //assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                        
            assert(lab > old_lab);
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;
    
            //send p to all (in set q)
    
            lab = 3; // ack_e
    
            //assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                        
            assert(lab > old_lab);
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;
    
            // receive ack_e
            retry = random;
            while(retry ) {
                if(filter_ack_e(&m_ack_e, p, lab)) {
                    // mbox_ack_e[num_mbox_ack_e] = &m_ack_e;
                    num_mbox_ack_e = num_mbox_ack_e + 1;
                }
    
                if (num_mbox_ack_e >= ((num+1)/2)) {
                    break;
                }
    
                retry = random;
            }
    
            if(num_mbox_ack_e >= (num+1)/2) {
                // Update history
    
                lab = 4; // new_l
    
                //assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                            
                assert(lab > old_lab);
                old_p = p;
                old_lab = lab;
                old_i = i;
                old_labr = labr;
    
                // send (p, lab, h) to Q
    
                lab = 5; // ack_l
    
                //assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                            
                assert(lab > old_lab);
                old_p = p;
                old_lab = lab;
                old_i = i;
                old_labr = labr;
    
                // receive ack_l
                retry = random;
                while(retry) {
                    if(filter_ack_l(&m_ack_l, p, lab)) {
                        mbox_ack_l[num_mbox_ack_l] = &m_ack_l;
                        num_mbox_ack_l = num_mbox_ack_l + 1;
                    }
        
                    if (num_mbox_ack_l >= ((num+1)/2)) {
                        break;
                    }
        
                    retry = random;
                }
                
                if (num_mbox_ack_l >= ((num+1)/2)) {
                    lab = 6; // cmt
    
                    //assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                                
                    assert(lab > old_lab);
                    old_p = p;
                    old_lab = lab;
                    old_i = i;
                    old_labr = labr;
    
                    // send (p, lab, h) to Q
    
                    // Start Broadcast
                    //Broadcast(num, pid, leader, &p, &lab, &i, &labr, &old_p, &old_lab, &old_i, &old_labr);
                    
                    // Simulating Broadcast
                    retry = random;
                    if(retry) {
                        p = p + 1;
                    }
                    else {
                        i = i + 1;
                        p = p + 1;
                    }
                }
                else {
                    p = p + 1;
                } 
            }
            else {
                p = p + 1;
            } 
        }
        else {
            p = p + 1;
        }
    lab=1;
    }
}

int notLeader(int num) {
    int lab = 0;
    int p = 0;
    int i = 0;
    int labr = 0;

    int old_p = p - 1;
    int old_lab = 0;
    int old_i = 0;
    int old_labr = 0;

    int leader;

    msg_curr_e* mbox_curr_e[200];
    int num_mbox_curr_e = 0;
    msg_curr_e m_curr_e;

    msg_new_e* mbox_new_e[200];
    int num_mbox_new_e = 0;
    msg_new_e m_new_e;

    msg_ack_e* mbox_ack_e[200];
    int num_mbox_ack_e = 0;
    msg_ack_e m_ack_e;

    msg_new_l* mbox_new_l[200];
    int num_mbox_new_l = 0;
    msg_new_l m_new_l;

    msg_ack_l* mbox_ack_l[200];
    int num_mbox_ack_l = 0;
    msg_ack_l m_ack_l;

    msg_com* mbox_com[200];
    int num_mbox_com = 0;
    msg_com m_com;

    int retry;
    volatile int random;
    
    while(1) {
        lab = 1; // Curr_E
        
        assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
        old_p = p;
        old_lab = lab;
        old_i = i;
        old_labr = labr;
        
        // send (p,lab) to leader
    
        lab = 2; // New_E
        
        assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                    
        old_p = p;
        old_lab = lab;
        old_i = i;
        old_labr = labr;
    
        // receive new_e
        // retry = rand() % 2;
        retry = random;
        while(retry && num_mbox_new_e < 1) {
            if(filter_new_e(&m_new_e, p, lab)) {
                // mbox_new_e[num_mbox_new_e] = &m_new_e;
                num_mbox_new_e = num_mbox_new_e + 1;
            }
    
            if (num_mbox_new_e >= 1) {
                break;
            }
    
            // retry = rand() % 2;
            retry = random;
        }
        
        if (num_mbox_new_e >= 1) {
            retry = random;
            if(retry) {  // Actually, p is the max value of all p's received by the leader
                p = p + 1;
            }
            else {
                p = p + 2;
            } 
            p = p + 1;
    
            lab = 3; // ack_e
    
            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                                    
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;
    
            // send (p, lab, a, h) to leader
    
            lab = 4; // new_l
    
            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                                    
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;
    
            // receive new_l
            // retry = rand() % 2;
            retry = random;
            while(retry && num_mbox_new_l < 1) {
                if(filter_new_l(&m_new_l, p, lab)) {
                    // mbox_new_l[num_mbox_new_l] = &m_new_l;
                    num_mbox_new_l = num_mbox_new_l + 1;
                }
    
                if (num_mbox_new_l >= 1) {
                    break;
                }
    
                // retry = rand() % 2;
                retry = random;
            }
    
            if(num_mbox_new_l >= 1) {
                // Update a
                // Update history
    
                lab = 5; // ack_l
    
                assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                                        
                old_p = p;
                old_lab = lab;
                old_i = i;
                old_labr = labr;
    
                // send (lab, a, h) to leader
    
                lab = 6; // cmt
                
                assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                                        
                old_p = p;
                old_lab = lab;
                old_i = i;
                old_labr = labr;
                
                // receive commit
                // retry = rand() % 2;
                retry = random;
                while(retry && num_mbox_com < 1) {
                    if(filter_com(&m_com, p, lab)) {
                        //mbox_com[num_mbox_com] = &m_com;
                        num_mbox_com = num_mbox_com + 1;
                    }
        
                    if (num_mbox_com >= 1) {
                        break;
                    }
        
                    // retry = rand() % 2;
                    retry = random;
                }
    
                if (num_mbox_com >= 1) {
                    // Update zvid
    
                    // Start Broadcast
                    //Broadcast(num, pid, leader, &p, &lab, &i, &labr, &old_p, &old_lab, &old_i, &old_labr);
                    
                    // Simulating Brooadcast
                    retry = random;
                    if(retry) {
                        p = p + 1;
                    }
                    else {
                        i = i + 1;
                        p = p + 1;
                    }
                }
                else {
                    p = p + 1;
                }
            }
            else {
                p = p + 1;
            }
        }
        else {
            // Special Case (state transfer?) 
            p = p + 1;
        }
    }
            
}

int test(int num) {
    int lab = 0;
    int p = 0;
    int i = 0;
    int labr = 0;

    int old_p = p - 1;
    int old_lab = 0;
    int old_i = 0;
    int old_labr = 0;

    int leader;

    msg_curr_e* mbox_curr_e[200];
    int num_mbox_curr_e = 0;
    msg_curr_e m_curr_e;

    msg_new_e* mbox_new_e[200];
    int num_mbox_new_e = 0;
    msg_new_e m_new_e;

    msg_ack_e* mbox_ack_e[200];
    int num_mbox_ack_e = 0;
    msg_ack_e m_ack_e;

    msg_new_l* mbox_new_l[200];
    int num_mbox_new_l = 0;
    msg_new_l m_new_l;

    msg_ack_l* mbox_ack_l[200];
    int num_mbox_ack_l = 0;
    msg_ack_l m_ack_l;

    msg_com* mbox_com[200];
    int num_mbox_com = 0;
    msg_com m_com;

    int retry;
    int retry1;
    int retry2;

    // send (p,lab) to leader

    //_memcad("assume(num_mbox_new_e > 1)");
    //assert(num_mbox_new_e > 1);
    num_mbox_new_e = 2;

    if (num_mbox_new_e >= 1) {
        //rand(&retry);
        if(retry) {  // Actually, p is the max value of all p's received by the leader
            p = p + 1;
        }
        else {
            p = p + 2;
        } 
        p = p + 1;

        lab = 3; // ack_e

        assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                                    
        old_p = p;
        old_lab = lab;
        old_i = i;
        old_labr = labr;

        // send (p, lab, a, h) to leader

        lab = 4; // new_l

        assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                                    
        old_p = p;
        old_lab = lab;
        old_i = i;
        old_labr = labr;

        // receive new_l
        // retry = rand() % 2;
        //rand(&retry);
        while(retry1 && num_mbox_new_l < 1) {
            if(filter_new_l(&m_new_l, p, lab)) {
                //mbox_new_l[num_mbox_new_l] = &m_new_l;
                num_mbox_new_l = num_mbox_new_l + 1;
            }

            if (num_mbox_new_l >= 1) {
                break;
            }

            // retry = rand() % 2;
            //rand(&retry);
        }

        if(num_mbox_new_l >= 1) {
            // Update a
            // Update history

            lab = 5; // ack_l

            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                                        
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;

            // send (lab, a, h) to leader

            lab = 6; // cmt
            
            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));                                        
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;
            
            // receive commit
            // retry = rand() % 2;
            // rand(&retry);
            while(retry2 && num_mbox_com < 1) {
                if(filter_com(&m_com, p, lab)) {
                    //mbox_com[num_mbox_com] = &m_com;
                    num_mbox_com = num_mbox_com + 1;
                }
    
                if (num_mbox_com >= 1) {
                    break;
                }
    
                // retry = rand() % 2;
                //rand(&retry);
            }

            if (num_mbox_com >= 1) {
                assert(num_mbox_com >= 1);
                // Update zvid

                // Start Broadcast
                //Broadcast(num, pid, leader, &p, &lab, &i, &labr, &old_p, &old_lab, &old_i, &old_labr);
                p = p + 1;
            }
            else {
                p = p + 1;
            }
        }
        else {
            p = p + 1;
        }
    }
    else { 
        p = p + 1;
    }
}
*/

int main() {
    int p = 0;
    int lab = 0;
    int i = 0;
    int labr = 0;
    int old_p = 0;
    int old_lab = 0;
    int old_i = 0;
    int old_labr = 0;
    //sendingThread(0, 0, 0, 0);
    // main_thread(0, 5); // pid, num
    // leadership(5);
    Broadcast(5,0,0,&p,&lab,&i,&labr,&old_p,&old_lab,&old_i,&old_labr);
}
