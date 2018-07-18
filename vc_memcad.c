typedef struct _msg_startVC {
    int v;
    int pid;
} msg_startVC;

int filter_startVC (msg_startVC* m, int v) {
    if (m->v == v) { 
        return 1;
    }
    return 0; 
}

typedef struct _msg_doVC {
    int v;
    int pid;
    int log_k;
    int log_top;
} msg_doVC;

int filter_doVC (msg_doVC* m, int v) {
    if (m->v == v)  {
        return 1;
    }
    return 0; 
}

typedef struct _msg_startView {
    int v;
    int pid;
    int log_k;
    int log_top;
} msg_startView;

int filter_startView (msg_startView* m, int v) {
    if (m->v == v) {
        return 1;
    }
    return 0; 
}

typedef struct _msg_prep {
    int v;
    int n;
    int k;
} msg_prep;

int filter_prep (msg_prep* m, int n, int v) {
    if (m->v == v && m->n == n+1) { 
        return 1;
    }
    return 0; 
}

typedef struct _msg_prepOK {
    int v;
    int n;
    int k;
} msg_prepOK;

int filter_prepOK (msg_prepOK* m, int v, int k) {
    if (m->n == k+1 && m->v == v) {
        return 1;
    }
    return 0; 
}

/*
int vc_msg (msg m, int v) {
    if ((m.lab_vc == 1 || m.lab_vc == 2) && m.v > v)  
        return 1;
    return 0; 
}
*/

// The case where messages have higher view number is ignored for now (recovery)
int NormalOp(int pid, int num, int leader, int* v, int* lab_vc, int* k, int* lab, int* n, int* old_v, int* old_lab_vc, int* old_k, int* old_lab, int* old_n)
{
    int retry;
    int timeout;
    int mbox;
    
    int cmd; // Declaration for incoming command
    
    int vc_msg;

    // msg_prep mbox_prep[2*num];
    int num_mbox_prep = 0;
    msg_prep m_prep;

    // msg_prepOK mbox_prepOK[2*num];
    int num_mbox_prepOK = 0;
    msg_prepOK m_prepOK;

    volatile int random;

    int leaderCommit;

    // new machine entrance
    *old_k = 0;
    *old_lab = 0;
    *old_n = 0;

    
    while (1) {
        if (pid == leader) {
            retry = random;
            while (retry) {
                
                // cmd = in()
                cmd = random;

                // Command is empty
                if (cmd == 0)
                {
                    *lab = 1; // Prepare

                    assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
                    *old_v = *v;
                    *old_lab_vc = *lab_vc;
                    *old_k = *k;
                    *old_lab = *lab;
                    *old_n = *n;

                    // send empty prepare with commit number
                }
                else
                {
                    *n = *n + 1;
                    //log[n] = cmd;

                    *lab = 1; // Prepare
                    
                    assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
                    *old_v = *v;
                    *old_lab_vc = *lab_vc;
                    *old_k = *k;
                    *old_lab = *lab;
                    *old_n = *n;

                    // send prep
                }
            }


            *lab = 2;  // PrepareOK

            // assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
            *old_v = *v;
            *old_lab_vc = *lab_vc;
            *old_k = *k;
            *old_lab = *lab;
            *old_n = *n;

            // Empty mbox
            // memset(mbox_prepOK,0,sizeof(mbox_prepOK));
            num_mbox_prepOK = 0;

            retry = random;
            while (retry && num_mbox_prepOK < (num/2)) {
            
                timeout = random;
                if (timeout) {
                    return 0;
                }

                // receive Transaction
                if(filter_prepOK(&m_prepOK, v, k)) {
                    // mbox_prepOK[num_mbox_prepOK] = m_prepOK;
                    num_mbox_prepOK = num_mbox_prepOK + 1;
                }

                vc_msg = random;
                if (vc_msg) {             // if msg is startVC or doVC
                    return 0; // to VC
                }

                retry = random;
            }

            if (num_mbox_prepOK >= num/2) { 
                // out()
                *k = *k + 1;
                    
            }
            else {
                // failure = 1;
                return 0;
            }
        }/*
       else {
            retry = random;
            while (retry) {
                *lab = 1; // Prepare
                
                assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
                *old_v = *v;
                *old_lab_vc = *lab_vc;
                *old_k = *k;
                *old_lab = *lab;
                *old_n = *n;

                // Empty mbox
                // memset(mbox_prep,0,sizeof(mbox_prep));
                num_mbox_prep = 0;  

                // receive Transaction
                retry = random;
                while(retry && num_mbox_prep < 1){
                    if(filter_prep(&m_prep, v, n)) {
                        // mbox_prep[num_mbox_prep] = m_prep;
                        num_mbox_prep = num_mbox_prep + 1;
                    } 

                    if(num_mbox_prep >= 1) {
                        break; 
                    }

                    retry = random;
    
                    vc_msg = random;
                    if (vc_msg) {              // if msg is startVC or doVC
                        return 0; // to VC
                    }

                    retry = random;
                }

                timeout = random;
                if (timeout)  {
                    return 0;
                }

                if (num_mbox_prep >= 1) {
                    // We use leaderCommit since array are not allowed (yet)
                    if (leaderCommit > *k) {
                        *k = leaderCommit;
                        assert(*k > *old_k);
                        // Commit all terms till min(commitIndex, lastIndex)
                    }
    
                    *n = *n + 1; 
                }
                else {
                    return 0;
                }

                retry = random;
            }
            
            lab = 2;
            
            assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
            *old_v = *v;
            *old_lab_vc = *lab_vc;
            *old_k = *k;
            *old_lab = *lab;
            *old_n = *n;

            // send Ack

            *k = *k + 1;
        }
    
    
    */}    
}

// Tag (v, lab_vc)
int VC(int pid, int num)
{ 
    int v = 0;
    int lab_vc = 0;
    int old_v = v - 1; 
    int old_lab_vc = 0;
    
    int n;
    int *log;
    int k;

    int retry;

    // msg_startVC mbox_startVC[2*num];
    int num_mbox_startVC = 0;
    msg_startVC m_startVC;

    // msg_doVC mbox_doVC[2*num];
    int num_mbox_doVC = 0;
    msg_doVC m_doVC;

    // msg_startView mbox_startView[2*num];
    int num_mbox_startView = 0;
    msg_startView m_startView;
    
    volatile int random;
    
    while (1) { 
    // loop invariant v > old_v
        
        int leader = v % num; // num = no of processes
        
        lab_vc = 1; // startVC
    
        // (v, lab_vc) > (old_v, old_lab_vc)
        assert((v > old_v) || ((v == old_v) && (lab_vc > old_lab_vc)));
        old_v = v;
        old_lab_vc = lab_vc;
    
        // send <startVC v, i> to all
    
        // Empty mbox
        // memset(mbox_startVC,0,sizeof(mbox_startVC));
        num_mbox_startVC = 0;   

        // receive Transaction
        retry = random;
        while(retry && num_mbox_startVC < (num/2)){
            if(filter_startVC(&m_startVC, v)) {
                // mbox_startVC[num_mbox_startVC] = m_startVC;
                num_mbox_startVC = num_mbox_startVC + 1;
            } 

            if(num_mbox_startVC >= num/2) {
                break; 
            }

            retry = random;
        }

        if (num_mbox_startVC >= num/2) {             
            lab_vc = 2; // doVC
    
            // (v, lab_vc) > (old_v, old_lab_vc)
            assert((v > old_v) || ((v == old_v) && (lab_vc > old_lab_vc)));            
            old_v = v;
            old_lab_vc = lab_vc;
    
            if (pid == leader) {
                // empty mbox
                //memset(mbox_doVC,0,sizeof(mbox_doVC));
                num_mbox_doVC = 0;

                // receive doVC messages
                retry = random;
                while(retry && num_mbox_doVC < (num/2)){
                    if(filter_doVC(&m_doVC, v)) {
                        // mbox_doVC[num_mbox_doVC] = m_doVC;
                        num_mbox_doVC = num_mbox_doVC + 1;
                    } 

                    if(num_mbox_doVC >= num/2) { 
                        break; 
                    }

                    retry = random;
                }

                if (num_mbox_doVC >= num/2) { // check m.msg = doVC, m.v = v and cardinality > num/2
                    // log = largest_log(mbox);
                    // n = mbox_doVC[0].log_top;
                    // k = mbox_doVC[0].log_k;

                    lab_vc = 3;  // startView

                    // (v, lab_vc) > (old_v, old_lab_vc)
                    assert((v > old_v) || ((v == old_v) && (lab_vc > old_lab_vc)));                                
                    old_v = v;
                    old_lab_vc = lab_vc;

                    // send <startView v, l, n, k> to all
                    // NormalOp(pid,leader,v,n,k,num,log);
                    v = v + 1;                    
                }
                else { // if leader does not receive enough doVC msgs
                    v = v + 1;
                }
            }
            else {
                //send <doVC v,l,v',n,k,i> to leader

                lab_vc = 3;  // startView
                
                // (v, lab_vc) > (old_v, old_lab_vc)
                assert((v > old_v) || ((v == old_v) && (lab_vc > old_lab_vc)));                            
                old_v = v;
                old_lab_vc = lab_vc;

                // Empty mbox
                // memset(mbox_startView,0,sizeof(mbox_startView));
                num_mbox_startView = 0;   

                // receive Transaction
                retry = random;
                while(retry) {
                    if(filter_startView(&m_startView, v)) {
                        // mbox_startView[num_mbox_startView] = m_startView;
                        num_mbox_startView = num_mbox_startView + 1;
                    } 

                    if(num_mbox_startView >= 1) {
                        break; 
                    }

                    retry = random;
                }

                if (num_mbox_startView >= 1) { // m.msg = startView, m.v > v
                    // log = m.log;
                    // n = mbox_startView[0].log_top;
                    // k = mbox_startView[0].log_k;
                    // v = mbox_startView[0].v;

                    // NormalOp(pid,leader,v,n,k,num,log);
                    v = v + 1;
                }
                else {
                    v = v + 1;
                }    
            }
        }
        else { // if not enough startVC msgs
            v = v + 1;
        }
    }
}

int main(){
    int v = 0;
    int lab_vc = 0;
    int k = 0;
    int lab = 0;
    int n = 0;

    int old_v = 0;
    int old_lab_vc = 0;
    int old_k = 0;
    int old_lab = 0;
    int old_n = 0;

    // VC(0,5);
    NormalOp(0,5,0, &v, &lab_vc, &k, &lab, &n, &old_v, &old_lab_vc, &old_k, &old_lab, &old_n);
}
