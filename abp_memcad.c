// Assumption - network delay is bounded

typedef struct _msg {
    int count;
    int req;
} msg;

int filter_msg_0(msg* m, int count) {
    if (m->req == 0 && m->count == count) {
        return 1;
    }
    return 0;
}

int filter_msg_1(msg* m, int count) {
    if (m->count == count && m->req == 1) {
        return 1;
    }
    return 0;
}

typedef struct _ack {
    int count;
    int req;
} ack;

int filter_ack_0 (ack* m, int count) {
    if (m->count == count && m->req == 0) {
        return 1;
    }
    return 0;
}

int filter_ack_1 (ack* m, int count) {
    if (m->count == count && m->req == 1) {
        return 1;
    }
    return 0;
}

void rand(int* retry) {
    int p = *retry;   // Workaround to solve error 
    _memcad("assume(p >= 0)");
    _memcad("assume(p <= 1)");
}

int AlternatingBit(int id) {
    int lab = 0;
    int count = 0;

    int old_lab = 0;
    int old_count = count - 1;  // needed for first assertion
    
    msg* mbox_msg[2];
    msg m;
    int num_mbox_msg;

    ack* mbox_ack[2];
    ack m_ack;
    int num_mbox_ack;

    int retry;
    int retry1;

    while (count < 10000) {
        lab = 1;

        //assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
	    old_count = count;
        old_lab = lab;

        if (id == 1) { // Process is A

	        //rand(&retry);
            while (retry) {
                // send 0

                //retry = rand() % 2;
                //rand(&retry);
            }

            lab = 2;

            // assert count > oldcount || count == oldcount ==> lab > oldlab
            //assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
	        old_count = count;
            old_lab = lab;

            // Recieve ack for 0
            // Empty mbox
            num_mbox_ack = 0;

            //retry = rand() % 2;
            //rand(&retry);
            //assert(retry == 0);
            //_memcad("assume(retry >= 0)");
            //assert(retry == 0);

            while (retry1 && num_mbox_ack < 1) {
                //assert(retry1 != 0);
                if (filter_ack_0(&m_ack, count)) {
                    mbox_ack[num_mbox_ack] = &m_ack;
                    num_mbox_ack = num_mbox_ack + 1;
                }

                if (num_mbox_ack >= 1) {
                    assert(num_mbox_ack >= 1);
                    break;
                }

                //retry = rand() % 2;
                //rand(&retry);
            }

            if (num_mbox_ack >= 1) {
                lab = 3;

                //assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                old_count = count;
                old_lab = lab;

                //retry = rand() % 2;
                //rand(&retry);

                while (retry) {
                    // send 1 to B

                    //retry = rand() % 2;
                    rand(&retry);
                }

                lab = 4;

                //assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                old_count = count;
                old_lab = lab;

                // Recieve ack for 0
                // Empty mbox
                num_mbox_ack = 0;

                //retry = rand() % 2;
                //rand(&retry);

                while (retry && num_mbox_ack < 1) {
                    if (filter_ack_1(&m_ack, count)) {
                        mbox_ack[num_mbox_ack] = &m_ack;
                        num_mbox_ack = num_mbox_ack + 1;
                    }
    
                    if (num_mbox_ack >= 1) {
                        break;
                    }
    
                    //retry = rand() % 2;
                    //rand(&retry);
                }
    
                if (num_mbox_ack >= 1) {
                    count = count + 1;
                }
                else {
                    count = count + 1;
                }
            } 
            else {
                count = count + 1;
            }
        }
        else {  // Process is B
            // Recieve ack for 0
            // Empty mbox
            num_mbox_msg = 0;

            //retry = rand() % 2;
            //rand(&retry);

            while (retry && num_mbox_msg < 1) {
                if (filter_msg_0(&m, count)) {
                    mbox_msg[num_mbox_msg] = &m;
                    num_mbox_msg = num_mbox_msg + 1;
                }

                if (num_mbox_msg >= 1) {
                    break;
                }

                //retry = rand() % 2;
                //rand(&retry);
            }

            if (num_mbox_msg >= 1) {
                lab = 2;

                //assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                old_count = count;
                old_lab = lab;

                //retry = rand() % 2;
                rand(&retry);
                
                while (retry) {
                    // send ack to A
                    
                    //retry = rand() % 2;
                    //rand(&retry);
                }

                lab = 3;

                //assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                old_count = count;
                old_lab = lab;

                // Recieve ack for 0
                // Empty mbox
                num_mbox_msg = 0;

                //retry = rand() % 2;
                //rand(&retry);
                
                while (retry && num_mbox_msg < 1) {
                    if (filter_msg_1(&m, count)) {
                        mbox_msg[num_mbox_msg] = &m;
                        num_mbox_msg = num_mbox_msg + 1;
                    }
    
                    if (num_mbox_msg >= 1) {
                        break;
                    }
    
                    //retry = rand() % 2;
                    //rand(&retry);
                }

                if (num_mbox_msg >= 1) {
                    lab = 4;

                    //assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                    old_count = count;
                    old_lab = lab;

                    //retry = rand() % 2;
                    //rand(&retry);
                    while (retry) {
                        // send ack to A

                        //retry = rand() % 2;
                        //rand(&retry);
                    }
                    
                    count = count + 1;
                }
                else {
                    count = count + 1;
                }
            }
            else {
                count = count + 1;
            }
        }
    }
}

int main() {
    AlternatingBit(1);
    return 0;
} 

/*
Code for Interproc

proc AlternatingBit(id:int) returns (count : int)
var lab:int, retry:int, m:int; 
begin
    lab = 0;
    retry = 1;
    count = 0;

    while true and count < 1000 
    do
        lab = 1;
        if id == 1 then
            while retry == 1
            do
                retry = random;
                assume retry>=0 and retry <= 1;
            done;
            
            lab = 2;
            m = random;

            if m == 2 then 
                lab = 3;
                
                while retry == 1
                do
                    retry = random;
                    assume retry>=0 and retry <= 1;
                done;

                lab = 4;

                m = random;
                if m == 4 then
                    lab = 1;
                endif;
            endif;
        else
            m = random;

            if m == 1 then
                lab = 2; 
                while retry == 1
                do
                    retry = random;
                    assume retry>=0 and retry <= 1;
                done;
                lab = 3;

                m = random;
                if m==3 then
                    lab = 4;
                    while retry == 1
                    do
                        retry = random;
                        assume retry>=0 and retry <= 1;
                    done;
                endif;
            endif;
        endif;

        count = count + 1;
    done;
end

var pid:int;
begin
  pid = 0;
  pid = AlternatingBit(pid);
end
*/
