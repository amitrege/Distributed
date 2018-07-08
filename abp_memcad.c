// Assumption - network delay is bounded

typedef struct _msg {
    int count;
    int req;
} msg;

/*
msg init_msg () {
    msg m;
    m.count = rand();
    m.req = rand() % 2;
}
*/

int filter_msg_0(msg* m, int count) {
    if (m->req == 0 && m->count == count)
        return 1;
    return 0;
}

int filter_msg_1(msg* m, int count) {
    if (m->count == count && m->req == 1)
        return 1;
    return 0;
}

typedef struct _ack {
    int count;
    int req;
} ack;

/*
ack init_ack () {
    ack m;
    m.count = rand();
    m.req = rand() % 2;
}
*/

int filter_ack_0 (ack* m, int count) {
    if (m->count == count && m->req == 0)
        return 1;
    return 0;
}

int filter_ack_1 (ack* m, int count) {
    if (m->count == count && m->req == 1)
        return 1;
    return 0;
}

int AlternatingBit(int id) {
    int lab = 0;
    int count = 0;
    
    msg mbox_msg[2];
    msg* m;
    int num_mbox_msg;

    ack mbox_ack[2];
    ack* m_ack;
    int num_mbox_ack;

    int retry;
    int failure = 0;

    while (count < 10000) {
        lab = 1;

        // assert count > oldcount || count == oldcount ==> lab > oldlab || failure
        // oldcount = count;
        // oldlab = lab;
        // failure = 0;

        if (id == 1) { // Process is A

            _memcad("assume(retry >= 0)");
            _memcad("assume(retry <= 1)");

            while (retry) {
                // send 0
                //retry = rand() % 2;
                _memcad("assume(retry >= 0)");
                _memcad("assume(retry <= 1)");
            }

            lab = 2;

            // assert count > oldcount || count == oldcount ==> lab > oldlab
            // oldcount = count;
            // oldlab = lab;

            /*
            // Empty mbox_ack
            memset(mbox_ack,0,sizeof(mbox_ack));
            num_mbox_ack = 0;
            */

            // Recieve ack for 0
            //retry = rand() % 2;
            _memcad("assume(retry >= 0)");
            _memcad("assume(retry <= 1)");

            while (retry) {
                //m_ack = init_ack();
                if (filter_ack_0(m_ack, count)) {
                    mbox_ack[num_mbox_ack] = *m_ack;
                    num_mbox_ack++;
                }

                if (num_mbox_ack >= 1) {
                    break;
                }

                //retry = rand() % 2;
                _memcad("assume(retry >= 0)");
                _memcad("assume(retry <= 1)");
            }

            if (num_mbox_ack >= 1) {
                lab = 3;

                // assert count > oldcount || count == oldcount ==> lab > oldlab
                // oldcount = count;
                // oldlab = lab;

                //retry = rand() % 2;
                _memcad("assume(retry >= 0)");
                _memcad("assume(retry <= 1)");

                while (retry) {
                    // send 1 to B
                    //retry = rand() % 2;
                    _memcad("assume(retry >= 0)");
                    _memcad("assume(retry <= 1)");
                }

                lab = 4;

                // assert count > oldcount || count == oldcount ==> lab > oldlab
                // oldcount = count;
                // oldlab = lab;

                /*
                // Empty mbox_ack
                memset(mbox_ack,0,sizeof(mbox_ack));
                num_mbox_ack = 0;
                */

                // Recieve ack for 0
                //retry = rand() % 2;
                _memcad("assume(retry >= 0)");
                _memcad("assume(retry <= 1)");

                while (retry) {
                    //m_ack = init_ack();
                    if (filter_ack_1(m_ack, count)) {
                        mbox_ack[num_mbox_ack] = *m_ack;
                        num_mbox_ack++;
                    }
    
                    if (num_mbox_ack >= 1) {
                        break;
                    }
    
                    //retry = rand() % 2;
                    _memcad("assume(retry >= 0)");
                    _memcad("assume(retry <= 1)");
                }
    
                if (num_mbox_ack >= 1) {
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
        else {  // Process is B
            /*
            // Empty mbox_msg
            memset(mbox_msg,0,sizeof(mbox_msg));
            num_mbox_msg = 0;
            */

            // Recieve ack for 0
            //retry = rand() % 2;
            _memcad("assume(retry >= 0)");
            _memcad("assume(retry <= 1)");

            while (retry) {
                //m = init_msg();
                if (filter_msg_0(m, count)) {
                    mbox_msg[num_mbox_msg] = *m;
                    num_mbox_msg++;
                }

                if (num_mbox_msg >= 1) {
                    break;
                }

                //retry = rand() % 2;
                _memcad("assume(retry >= 0)");
                _memcad("assume(retry <= 1)");
            }

            if (num_mbox_msg >= 1) {
                lab = 2;

                // assert count > oldcount || count == oldcount ==> lab > oldlab
                // oldcount = count;
                // oldlab = lab;

                //retry = rand() % 2;
                _memcad("assume(retry >= 0)");
                _memcad("assume(retry <= 1)");
                while (retry) {
                    // send ack to A
                    //retry = rand() % 2;
                    _memcad("assume(retry >= 0)");
                    _memcad("assume(retry <= 1)");
                }

                lab = 3;

                // assert count > oldcount || count == oldcount ==> lab > oldlab
                // oldcount = count;
                // oldlab = lab;

                /*
                // Empty mbox_msg
                memset(mbox_msg,0,sizeof(mbox_msg));
                num_mbox_msg = 0;
                */

                // Recieve ack for 0
                //retry = rand() % 2;
                _memcad("assume(retry >= 0)");
                _memcad("assume(retry <= 1)");
                while (retry) {
                    //m = init_msg();
                    if (filter_msg_1(m, count)) {
                        mbox_msg[num_mbox_msg] = *m;
                        num_mbox_msg++;
                    }
    
                    if (num_mbox_msg >= 1) {
                        break;
                    }
    
                    //retry = rand() % 2;
                    _memcad("assume(retry >= 0)");
                    _memcad("assume(retry <= 1)");
                }

                if (num_mbox_msg >= 1) {
                    lab = 4;

                    // assert count > oldcount || count == oldcount ==> lab > oldlab
                    // oldcount = count;
                    // oldlab = lab;

                    //retry = rand() % 2;
                    _memcad("assume(retry >= 0)");
                    _memcad("assume(retry <= 1)");
                    while (retry) {
                        // send ack to A
                        //retry = rand() % 2;
                        _memcad("assume(retry >= 0)");
                        _memcad("assume(retry <= 1)");
                    }
                    
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

int main() {
    AlternatingBit(0);
    return 0;
} 

/*
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