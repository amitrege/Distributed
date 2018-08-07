typedef struct _msg {
    int p;
    int lab;
    int i;
    int labr;
    int val;
    // history not included (as it is not relevant to prove increasing tags)
} msg;

int filter_propose (msg* m, int p, int i, int labr) {
    if (m->p == p && m->i == i && m->labr == labr) {
        return 1;
    }
    return 0;
}

int filter_ack_p (msg* m, int p, int i, int labr) {
    if (m->p == p && m->i == i && m->labr == labr) {
        return 1;
    }
    return 0;
}

int filter_cmt (msg* m, int p, int i, int labr) {
    if (m->p == p && m->i == i && m->labr == labr) {
        return 1;
    }
    return 0;
}

int filter_curr_e (msg* m, int p, int lab) {
    if (m->p == p && m->lab == lab) {
        return 1;
    }
    return 0;
}

int filter_new_e (msg* m, int p, int lab) {
    if (m->p >= p && m->lab == lab) {
        return 1;
    }
    return 0;
}

int filter_ack_e (msg* m, int p, int lab) {
    if (m->p == p && m->lab == lab) {
        return 1;
    }
    return 0;
}

int filter_new_l (msg* m, int p, int lab) {
    if (m->p >= p && m->lab == lab) {
        return 1;
    }
    return 0;
}

int filter_ack_l (msg* m, int p, int lab) {
    if (m->p == p && m->lab == lab) {
        return 1;
    }
    return 0;
}

int filter_com (msg* m, int p, int lab) {
    if (m->p == p && m->lab == lab) {
        return 1;
    }
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

void Broadcast (int num, int pid, int leader, int *p, int *lab, int *i, int *labr, int *old_p, int *old_lab, int *old_i, int *old_labr) {

    // Value received from client
    int val;

    int num_mbox = 0;
    msg m_propose;

    //int num_mbox_ack_p = 0;
    msg m_ack_p;

    //int num_mbox_cmt = 0;
    msg m_cmt;

    int retry;

    int timeout;

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
                    if(filter_propose(&m_propose, *p, *i, *labr)) {
                        //mbox_propose[num_mbox_propose] = &m_propose;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= 1) {
                        break;
                    }
                } 
                
                timeout = random;
                if(timeout) {
                    *p = *p + 1;
                    return;
                }
    
                retry = random;
            }

            // Update mbox
            // Update history
            
            *labr = 2; // ack_p

            assert((*i == *old_i) && (*labr == *old_labr + 1));            
            assert((*p > *old_p) || ((*p == *old_p) && (*lab > *old_lab)) || ((*p == *old_p) && (*lab == *old_lab) && (*i > *old_i)) || ((*p == *old_p) && (*lab == *old_lab) && (*i == *old_i) && (*labr >= *old_labr)));                
            *old_p = *p;
            *old_lab = *lab;
            *old_i = *i;
            *old_labr = *labr;

            msg m_ack;
            m_ack.p = *p;
            m_ack.i = *i;
            m_ack.labr = *labr;
            m_ack.val = val;
            // send(p, i, labr, val) to leader
            assert((m_ack.p == *p) && (m_ack.i == *i) && (m_ack.labr == *labr));

            *labr = 3; // cmt

            assert((*i == *old_i) && (*labr == *old_labr + 1));            
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
                        //mbox_propose[num_mbox_propose] = &m_propose;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= 1) {
                        break;
                    }
                }
                
                timeout = random;
                if(timeout) {
                    *p = *p + 1;
                    return;
                }
    
                retry = random;
            }

            // Commit
            // out()
            
            (*i) = (*i) + 1;
        }
        else {
            msg m_propose;
            m_propose.p = *p;
            m_propose.i = *i;
            m_propose.labr = *labr;
            m_propose.val = val;
            // send propose
            assert((m_propose.p == *p) && (m_propose.i == *i) && (m_propose.labr == *labr));

            *labr = 2; // ack_p

            assert((*i == *old_i) && (*labr == *old_labr + 1));            
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
                    if(filter_ack_p(&m_ack_p, *p, *i, *labr)) {
                        //mbox_propose[num_mbox_propose] = &m_propose;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= (num+1)/2) {
                        break;
                    }
                }
                
                timeout = random;
                if(timeout) {
                    *p = *p + 1;
                    return;
                }
    
                retry = random;
            }

            *labr = 3; // Commit
            
            assert((*i == *old_i) && (*labr == *old_labr + 1));            
            assert((*p > *old_p) || ((*p == *old_p) && (*lab > *old_lab)) || ((*p == *old_p) && (*lab == *old_lab) && (*i > *old_i)) || ((*p == *old_p) && (*lab == *old_lab) && (*i == *old_i) && (*labr >= *old_labr)));                
            *old_p = *p;
            *old_lab = *lab;
            *old_i = *i;
            *old_labr = *labr;

            msg m_commit;
            m_commit.p = *p;
            m_commit.i = *i;
            m_commit.labr = *labr;
            // send commit (labr, a, i) to all
            assert((m_commit.p == *p) && (m_commit.i == *i) && (m_commit.labr == *labr));
            
            (*i) = (*i) + 1;
        }
    }
}

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

    int num_mbox = 0;

    // msg_curr_e* mbox_curr_e[200];
    // int num_mbox_curr_e = 0;
    msg m_curr_e;

    // msg_new_e* mbox_new_e[200];
    // int num_mbox_new_e = 0;
    msg m_new_e;

    // msg_ack_e* mbox_ack_e[200];
    // int num_mbox_ack_e = 0;
    msg m_ack_e;

    // msg_new_l* mbox_new_l[200];
    // int num_mbox_new_l = 0;
    msg m_new_l;

    // msg_ack_l* mbox_ack_l[200];
    // int num_mbox_ack_l = 0;
    msg m_ack_l;

    // msg_com* mbox_com[200];
    // int num_mbox_com = 0;
    msg m_com;

    int retry;
    volatile int random;

    while (1) {
        // Actually, leader is chosen by an oracle
        leader = p % num;

        lab = 1; // Curr_E

        assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
        assert((p > old_p));
        
        old_p = p;
        old_lab = lab;
        old_i = i;
        old_labr = labr;
        
        msg m_curr_e;
        m_curr_e.p = p;
        m_curr_e.lab = lab;
        // send (p,lab) to leader
        assert((m_curr_e.p == p) && (m_curr_e.lab == lab));
    
        if (leader == pid){
            // receive curr_e

            // Empty mbox
            num_mbox = 0;

            retry = random;
            while(1) {
                // m = receive()
                if(m_curr_e.lab == 1) {
                    if(filter_curr_e(&m_curr_e, p, lab)) {
                        //mbox_curr_e[num_mbox_curr_e] = &m_curr_e;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= (num+1)/2) {
                        break;
                    }
                }
    
                retry = random;
            }
            
            // Update set Q which is the set of pids in mbox
            // get max p in mbox and add 1

            // Simulating picking the max p
            // Since lists do not work yet, we make do with randomly increasing the value of p
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
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;
            
            msg m_new_e;
            m_new_e.p = p;
            m_new_e.lab = lab;
            //send p to all (in set q)
            assert((m_new_e.p == p) && (m_new_e.lab == lab));            

            lab = 3; // ack_e
            
            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
            // assert(lab > old_lab);
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;

            // receive ack_e

            // Empty mbox
            num_mbox = 0;
            
            while(1) {
                // m = receive()
                if(m_ack_e.lab == 3) {
                    if(filter_ack_e(&m_ack_e, p, lab)) {
                        //mbox_curr_e[num_mbox_curr_e] = &m_curr_e;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= (num+1)/2) {
                        break;
                    }
                }
    
                retry = random;
            }

            // Pick the most up to date history from the followers
            // Update history and store to disk

            // Synchronization Phase starts here

            lab = 4; // new_l
            
            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
            // assert(lab > old_lab);
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;

            msg m_new_l;
            m_new_l.p = p;
            m_new_l.lab = lab;
            // send (p, lab, h) to Q
            assert((m_new_l.p == p) && (m_new_l.lab == lab));                        

            lab = 5; // ack_l

            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
            // assert(lab > old_lab);
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;

            // receive ack_l

            // Empty mbox
            num_mbox = 0;
            
            retry = random;
            while(1) {
                // m = receive()
                if(m_ack_l.lab == 5) {
                    if(filter_ack_l(&m_ack_l, p, lab)) {
                        //mbox_curr_e[num_mbox_curr_e] = &m_curr_e;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= (num+1)/2) {
                        break;
                    }
                }
    
                retry = random;
            }

            lab = 6; // cmt
            
            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
            // assert(lab > old_lab);
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;

            msg m_cmt;
            m_cmt.p = p;
            m_cmt.lab = lab;
            // send (p, lab, h) to Q
            assert((m_cmt.p == p) && (m_cmt.lab == lab));                          

            // Start Broadcast
            Broadcast(num, pid, leader, &p, &lab, &i, &labr, &old_p, &old_lab, &old_i, &old_labr);
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
            num_mbox = 0;
            
            retry = random;
            while(1) {
                // m = receive()
                if(m_new_e.lab == 2) {
                    if(filter_new_e(&m_new_e, p, lab)) {
                        if(m_new_e.p > p) {
                            p = m_new_e.p;
                        }
                        //mbox_curr_e[num_mbox_curr_e] = &m_curr_e;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= 1) {
                        break;
                    }
                }
    
                retry = random;
            }
            
            // Simulating the p-max follower gets from the leader 
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

            msg m_ack_e;
            m_ack_e.p = p;
            m_ack_e.lab = lab;
            // send (p, lab, a, h) to leader
            assert((m_ack_e.p == p) && (m_ack_e.lab == lab));                                      

            // Synchronization Phase starts here

            lab = 4; // new_l

            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
            // assert(lab > old_lab);
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;

            // receive new_l

            // Empty mbox
            num_mbox = 0;
            
            retry = random;
            while(1) {
                // m = receive()
                if(m_new_l.lab == 4) {
                    if(filter_new_l(&m_new_l, p, lab)) {
                        if(m_new_l.p == p) {
                            //mbox_curr_e[num_mbox_curr_e] = &m_curr_e;
                            num_mbox = num_mbox + 1;
                        }
                        else {
                            // New election 
                            p = p + 1;
                            break;
                        }
                    }
        
                    if (num_mbox >= 1) {
                        break;
                    }
                }
    
                retry = random;
            }

            if(num_mbox == 0) {
                continue;
            }
 
            // Update a
            // Update history (using history of the leader)

            lab = 5; // ack_l
            
            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
            // assert(lab > old_lab);
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;

            msg m_ack_l;
            m_ack_l.p = p;
            m_ack_l.lab = lab;
            // send (lab, a, h) to leader
            assert((m_ack_l.p == p) && (m_ack_l.lab == lab));                                                  

            lab = 6; // cmt

            assert((p > old_p) || ((p == old_p) && (lab > old_lab)) || ((p == old_p) && (lab == old_lab) && (i > old_i)) || ((p == old_p) && (lab == old_lab) && (i == old_i) && (labr >= old_labr)));        
            // assert(lab > old_lab);
            old_p = p;
            old_lab = lab;
            old_i = i;
            old_labr = labr;

            // receive commit

            // Empty mbox
            num_mbox = 0;
            
            retry = random;
            while(1) {
                // m = receive()
                if(m_com.lab == 6) {
                    if(filter_com(&m_com, p, lab)) {
                        //mbox_curr_e[num_mbox_curr_e] = &m_curr_e;
                        num_mbox = num_mbox + 1;
                    }
        
                    if (num_mbox >= 1) {
                        break;
                    }
                }
    
                retry = random;
            }

            // Update zvid

            // Start Broadcast
            Broadcast(num, pid, leader, &p, &lab, &i, &labr, &old_p, &old_lab, &old_i, &old_labr);
        }
    }
}

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
    main_thread(0, 5); // pid, num
    // leadership(5);
    //Broadcast(5,1,0,&p,&lab,&i,&labr,&old_p,&old_lab,&old_i,&old_labr);
}
