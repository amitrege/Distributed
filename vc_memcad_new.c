typedef struct _msg {
    int type;   // 0 -> NormalOp message, 1 -> VC message
    int v;
    int lab_vc;
    int pid;
    int k;
    int lab;
    int n;
    int log_k;
    int log_top;
} msg;

int filter_startVC (msg* m, int v) {
    if (m->v >= v) { 
        return 1;
    }
    return 0; 
}

int filter_doVC (msg* m, int v) {
    if (m->v >= v)  {
        return 1;
    }
    return 0; 
}

int filter_startView (msg* m, int v) {
    if (m->v >= v) {
        return 1;
    }
    return 0; 
}

int filter_prep (msg* m, int n, int v) {
    if (m->v == v && m->n == n+1) { 
        return 1;
    }
    return 0; 
}

int filter_prepOK (msg* m, int v, int k) {
    if (m->n == k+1 && m->v == v) {
        return 1;
    }
    return 0; 
}

void test(int pid, int num, int leader,int* num_mbox_startVC, int* num_mbox_doVC, int* v, int* lab_vc, int* k, int* lab, int* n, int* old_v, int* old_lab_vc, int* old_k, int* old_lab, int* old_n){
    int retry;
    int timeout;
    int normal;
    
    int cmd; // Declaration for incoming command
    
    int vc_msg;

    // msg_prep mbox_prep[2*num];
    // int num_mbox_prep = 0;

    // msg_prepOK mbox_prepOK[2*num];
    // int num_mbox_prepOK = 0;

    int num_mbox = 0;

    volatile int random;

    int leaderCommit;

    // For leader branch
    msg m_1;
    msg m_2;

    // For follower branch
    msg m_3;
    msg m_4;

    // new machine entrance
    *old_k = 0;
    *old_lab = 0;
    *old_n = 0;

    while (1) {
        if (pid == leader) {
            while (1) {
                
                // Take input
                // cmd = in()
                cmd = random;

                // Command is empty
                if (cmd == 0)
                {
                    // When Command is empty (or it is too long between two commands), we send an empty heartbeat
                    *lab = 1; // Prepare

                    assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
                    *old_v = *v;
                    *old_lab_vc = *lab_vc;
                    *old_k = *k;
                    *old_lab = *lab;
                    *old_n = *n;

                    // send empty prepare (Heartbeat) with commit number
                }
                else
                {
                    *n = *n + 1;

                    // Add command to Log
                    //log[n] = cmd;

                    *lab = 1; // Prepare
                    
                    assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
                    *old_v = *v;
                    *old_lab_vc = *lab_vc;
                    *old_k = *k;
                    *old_lab = *lab;
                    *old_n = *n;

                    // send prepare
                }

                retry = random;
                if(retry) {
                    break;
                }
            }
        }
    } 
    
}

// The case where messages have higher view number is ignored for now (recovery)
void NormalOp(int pid, int num, int leader,int* num_mbox_startVC, int* num_mbox_doVC, int* v, int* lab_vc, int* k, int* lab, int* n, int* old_v, int* old_lab_vc, int* old_k, int* old_lab, int* old_n)
{
    int retry;
    int timeout;
    int normal;
    
    int cmd; // Declaration for incoming command
    
    int vc_msg;

    // msg_prep mbox_prep[2*num];
    // int num_mbox_prep = 0;

    // msg_prepOK mbox_prepOK[2*num];
    // int num_mbox_prepOK = 0;

    int num_mbox = 0;

    volatile int random;

    int leaderCommit;

    // For leader branch
    msg m_1;
    msg m_2;

    // For follower branch
    msg m_3;
    msg m_4;

    // new machine entrance
    *old_k = 0;
    *old_lab = 0;
    *old_n = 0;

    
    while (1) {
        if (pid == leader) {
            while (1) {
                
                // Take input
                // cmd = in()
                cmd = random;

                // Command is empty
                if (cmd == 0)
                {
                    // When Command is empty (or it is too long between two commands), we send an empty heartbeat
                    *lab = 1; // Prepare

                    assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
                    *old_v = *v;
                    *old_lab_vc = *lab_vc;
                    *old_k = *k;
                    *old_lab = *lab;
                    *old_n = *n;

                    // send empty prepare (Heartbeat) with commit number
                }
                else
                {
                    *n = *n + 1;

                    // Add command to Log
                    //log[n] = cmd;

                    *lab = 1; // Prepare
                    
                    assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
                    *old_v = *v;
                    *old_lab_vc = *lab_vc;
                    *old_k = *k;
                    *old_lab = *lab;
                    *old_n = *n;

                    // send prepare
                }

                retry = random;
                if(retry) {
                    break;
                }
            }

            // Receive startVC or doVC
            *num_mbox_doVC = 0;
            *num_mbox_startVC = 0;

            while (1) {
                // m = receive()
                
                timeout = random;
                if (timeout) {
                    return;
                }

                if(m_1.lab_vc == 1 && m_1.type == 1) { // startVC
                    if(filter_startVC(&m_1, *v)) {
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        *num_mbox_startVC = *num_mbox_startVC + 1;
                    } 
        
                    if(*num_mbox_startVC >= 1) {
                        return; 
                    }
                }

                if(m_1.lab_vc == 2 && m_1.type == 1) { // doVC
                    if(filter_doVC(&m_1, *v)) {
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        *num_mbox_doVC = *num_mbox_doVC + 1;
                    } 
        
                    if(*num_mbox_doVC >= 1) {
                        return; 
                    }
                }

                // The case where nothing is received
                normal = random;
                if(normal) {
                    break;
                }
            }


            *lab = 2;  // PrepareOK

            assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
            *old_v = *v;
            *old_lab_vc = *lab_vc;
            *old_k = *k;
            *old_lab = *lab;
            *old_n = *n;

            // Empty mbox
            // memset(mbox_prepOK,0,sizeof(mbox_prepOK));
            num_mbox = 0;
            *num_mbox_doVC = 0;
            *num_mbox_startVC = 0;

            retry = random;
            while (1) {
                // m = receive()
                
                timeout = random;
                if (timeout) {
                    return;
                }

                if(m_2.lab_vc == 1 && m_2.type == 1) { // startVC
                    if(filter_startVC(&m_2, *v)) {
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        *num_mbox_startVC = *num_mbox_startVC + 1;
                    } 
        
                    if(*num_mbox_startVC >= 1) {
                        return; 
                    }
                }

                if(m_2.lab_vc == 2 && m_2.type == 1) { // doVC
                    if(filter_doVC(&m_2, *v)) {
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        *num_mbox_doVC = *num_mbox_doVC + 1;
                    } 
        
                    if(*num_mbox_doVC >= 1) {
                        return; 
                    }
                }

                if(m_2.lab == 2 && m_2.type == 0) { // prepOK
                    if(filter_prepOK(&m_2, *v, *k)) {
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        num_mbox = num_mbox + 1;
                    } 
        
                    if(num_mbox >= (num+1)/2) {
                        break; 
                    }
                }

                retry = random;
                if(retry) {
                    break;
                }
            }

            // out()
            *k = *k + 1;
        }
        else {  // Process is a Replica
            lab = 1;
            
            assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
            *old_v = *v;
            *old_lab_vc = *lab_vc;
            *old_k = *k;
            *old_lab = *lab;
            *old_n = *n;

            num_mbox = 0;
            *num_mbox_doVC = 0;
            *num_mbox_startVC = 0;

            while (1) {
                // m = receive()
                
                timeout = random;
                if (timeout) {
                    return;
                }

                if(m_3.lab_vc == 1 && m_3.type == 1) { // startVC
                    if(filter_startVC(&m_3, *v)) {
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        *num_mbox_startVC = *num_mbox_startVC + 1;
                    } 
        
                    if(*num_mbox_startVC >= 1) {
                        return; 
                    }
                }

                if(m_3.lab_vc == 2 && m_3.type == 1) { // doVC
                    if(filter_doVC(&m_3, *v)) {
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        *num_mbox_doVC = *num_mbox_doVC + 1;
                    } 
        
                    if(*num_mbox_doVC >= 1) {
                        return; 
                    }
                }

                if(m_3.lab == 1 && m_3.type == 0) { // prep
                    if(filter_prep(&m_3, *n, *v)) {
                        // Update Commit no
                        if(m_3.k > *k) {
                            *k = m_3.k;
                        }
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        num_mbox = num_mbox + 1;
                    } 
        
                    if(num_mbox >= 1) {
                        break; 
                    }
                }
            }

            *n = *n + 1;
            
            lab = 2;
            
            assert((*v > *old_v) || ((*v == *old_v) && (*lab_vc > *old_lab_vc)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k > *old_k)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab > *old_lab)) || ((*v == *old_v) && (*lab_vc == *old_lab_vc) && (*k == *old_k) && (*lab == *old_lab) && (*n >= *old_n)));
            *old_v = *v;
            *old_lab_vc = *lab_vc;
            *old_k = *k;
            *old_lab = *lab;
            *old_n = *n;

            // send Ack

            *k = *k + 1;

            // Receive startVC or doVC
            *num_mbox_doVC = 0;
            *num_mbox_startVC = 0;

            while (1) {
                // m = receive()
                
                timeout = random;
                if (timeout) {
                    return;
                }

                if(m_4.lab_vc == 1 && m_4.type == 1) { // startVC
                    if(filter_startVC(&m_4, *v)) {
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        *num_mbox_startVC = *num_mbox_startVC + 1;
                    } 
        
                    if(*num_mbox_startVC >= 1) {
                        return; 
                    }
                }

                if(m_4.lab_vc == 2 && m_4.type == 1) { // doVC
                    if(filter_doVC(&m_4, *v)) {
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        *num_mbox_doVC = *num_mbox_doVC + 1;
                    } 
        
                    if(*num_mbox_doVC >= 1) {
                        return; 
                    }
                }

                // The case where nothing is received
                normal = random;
                if(normal) {
                    break;
                }
            }
        }
    }    
}


// Tag (v, lab_vc)
int VC(int pid, int num)
{ 
    int v = 0;
    int lab_vc = 0;
    int k = 0;
    int lab = 0;
    int n = 0;
    int *log;

    int old_v = v - 1; 
    int old_lab_vc = 0;
    int old_k = 0;
    int old_lab = 0;
    int old_n = 0;

    int retry;

    // msg_startVC mbox_startVC[2*num];
    int num_mbox_startVC = 0;
    msg m_startVC;

    // msg_doVC mbox_doVC[2*num];
    int num_mbox_doVC = 0;
    msg m_doVC;

    // msg_startView mbox_startView[2*num];
    int num_mbox_startView = 0;
    msg m_startView;
    
    volatile int random;
    
    while (1) {         
        int leader = v % num; // num = no of processes
        
        lab_vc = 1; // startVC
    
        // (v, lab_vc) > (old_v, old_lab_vc)
        assert((v > old_v) || ((v == old_v) && (lab_vc > old_lab_vc)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k > old_k)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k == old_k) && (lab > old_lab)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k == old_k) && (lab == old_lab) && (n >= old_n)));
        old_v = v;
        old_lab_vc = lab_vc;
        old_k = k;
        old_lab = lab;
        old_n = n;
    
        // send <startVC v, i> to all
    
        if(num_mbox_doVC == 1 || num_mbox_startVC == 1) {
            // Empty mbox        
            // memset(mbox_startVC,0,sizeof(mbox_startVC));
            num_mbox_startVC = 0;
        }   

        // receive Transaction
        retry = random;
        while(1){
            // m = receive()
            if(m_startVC.lab_vc == 1 && m_startVC.type == 1) { // startVC
                if(filter_startVC(&m_startVC, v)) {
                    if(m_startVC.v > v) {
                        v = m_startVC.v;

                        // Empty Mbox
                        num_mbox_startVC = 0;
                    }
                    // mbox_startVC[num_mbox_startVC] = m_startVC;
                    num_mbox_startVC = num_mbox_startVC + 1;
                } 
    
                if(num_mbox_startVC >= (num+1)/2) {
                    break; 
                }
            }

            retry = random;
        }

        lab_vc = 2; // doVC
        
        // (v, lab_vc) > (old_v, old_lab_vc)
        assert((v > old_v) || ((v == old_v) && (lab_vc > old_lab_vc)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k > old_k)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k == old_k) && (lab > old_lab)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k == old_k) && (lab == old_lab) && (n >= old_n)));
        old_v = v;
        old_lab_vc = lab_vc;
        old_k = k;
        old_lab = lab;
        old_n = n;

        if (pid == leader) {
            // empty mbox
            //memset(mbox_doVC,0,sizeof(mbox_doVC));
            num_mbox_doVC = 0;

            // receive doVC messages
            retry = random;
            while(1){
                if(m_doVC.lab_vc == 2 && m_doVC.type == 1) { // doVC
                    if(filter_doVC(&m_doVC, v)) {
                        if(m_doVC.v > v) {
                            v = m_doVC.v;

                            // Empty Mbox
                            num_mbox_doVC = 0;
                        }
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        num_mbox_doVC = num_mbox_doVC + 1;
                    } 
        
                    if(num_mbox_doVC >= (num+1)/2) {
                        break; 
                    }
                }

                retry = random;
            }

            // log = largest_log(mbox);
            // n = mbox_doVC[0].log_top;
            // k = mbox_doVC[0].log_k;

            lab_vc = 3;  // startView
            
            // (v, lab_vc) > (old_v, old_lab_vc)
            assert((v > old_v) || ((v == old_v) && (lab_vc > old_lab_vc)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k > old_k)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k == old_k) && (lab > old_lab)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k == old_k) && (lab == old_lab) && (n >= old_n)));
            old_v = v;
            old_lab_vc = lab_vc;
            old_k = k;
            old_lab = lab;
            old_n = n;

            // send <startView v, l, n, k> to all
            NormalOp(pid, num, leader, &num_mbox_startVC, &num_mbox_doVC, &v, &lab_vc, &k, &lab, &n, &old_v, &old_lab_vc, &old_k, &old_lab, &old_n);
            v = v + 1;
        }
        else {
            //send <doVC v,l,v',n,k,i> to leader

            lab_vc = 3;  // startView
            
            // (v, lab_vc) > (old_v, old_lab_vc)
            assert((v > old_v) || ((v == old_v) && (lab_vc > old_lab_vc)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k > old_k)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k == old_k) && (lab > old_lab)) || ((v == old_v) && (lab_vc == old_lab_vc) && (k == old_k) && (lab == old_lab) && (n >= old_n)));
            old_v = v;
            old_lab_vc = lab_vc;
            old_k = k;
            old_lab = lab;
            old_n = n;

            // Empty mbox
            // memset(mbox_startView,0,sizeof(mbox_startView));
            num_mbox_startView = 0;   

            // receive Transaction
            retry = random;
            while(retry) {
                if(m_startView.lab_vc == 3 && m_startView.type == 1) { // doVC
                    if(filter_startView(&m_startView, v)) {
                        if(m_startView.v > v) {
                            v = m_startView.v;

                            // Empty Mbox
                            num_mbox_startView = 0;
                        }
                        // mbox_startVC[num_mbox_startVC] = m_startVC;
                        num_mbox_startView = num_mbox_startView + 1;
                    } 
        
                    if(num_mbox_doVC >= 1) {
                        break; 
                    }
                }

                retry = random;
            }

            // log = m.log;
            // n = mbox_startView[0].log_top;
            // k = mbox_startView[0].log_k;
            // v = mbox_startView[0].v;

            NormalOp(pid, num, leader, &num_mbox_startVC, &num_mbox_doVC, &v, &lab_vc, &k, &lab, &n, &old_v, &old_lab_vc, &old_k, &old_lab, &old_n);
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

    int num_mbox_doVC = 0;
    int num_mbox_startVC = 0;

    // VC(0,5);
    // test(0,5,0, &num_mbox_startVC, &num_mbox_doVC, &v, &lab_vc, &k, &lab, &n, &old_v, &old_lab_vc, &old_k, &old_lab, &old_n);    
    NormalOp(0,5,0, &num_mbox_startVC, &num_mbox_doVC, &v, &lab_vc, &k, &lab, &n, &old_v, &old_lab_vc, &old_k, &old_lab, &old_n);
}
