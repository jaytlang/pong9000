/* Billiards edition 1 - not well done edition*/

int isidle;
int incr;
int timer;
int periodloop;  /* there is only signed 32 bit ints in the c compiler */
int primary_timer;
int posting_timer;
char *body;
int body_len;
char *delim;

char *saved_pointer;
char *charstarscore;
int break_this_bitch;


int RESPONSE_TIMEOUT;
int IN_BUFFER_SIZE;
int OUT_BUFFER_SIZE;
char *request_buffer;
char *response_buffer;

char *var1;
char *var2;
char *var3;  /* USE THE HEAP */
char *var4;
char *var5;
char *var6;
char *var7;

char *names;
char *host;
int player;

int done;

int DT;
int SCALER;
int BUTTON_PIN;

int dttimer;
int force_timer;

int *x_pos_array;
int *y_pos_array;
int *x_vel_array;
int *y_vel_array;
int *x_accel_array;
int *y_accel_array;
int *x_force_array;
int *y_force_array;
int *color_array;
int *removed_array;

int total_score;
int opponents_score;
int score;
int ball_shot;
int opponents_turn;
int pressed;
int stop_game;

int *hole_x_pos_array;
int *hole_y_pos_array;
int HOLE_RADIUS;

int angle;
int old_angle;

int MASS;
int RADIUS;
int FRICTION;
int STOPPING_SPEED;
int K_SPRING;

int LEFT_LIMIT;
int RIGHT_LIMIT;
int TOP_LIMIT;
int BOTTOM_LIMIT;

int state;
int steps;
int older_g_x, old_g_x;
int older_acc_mag, old_acc_mag;
char *output;

int a_x, a_y, a_z;
int g_x, g_y, g_z;
int peak_acc;
int push_detected;
int ACC_BASELINE;
int GYRO_BASELINE;
int THRESHOLD;

int old_x_pos;
int old_y_pos;

int push_acc;
int scale_speed;

void moveBall(int i)
{
    int diffx;
    int diffy;
    diffx = 1 * DT * x_vel_array[i];
    diffy = 1 * DT * y_vel_array[i];
    if(x_pos_array[i] + diffx > RIGHT_LIMIT) {
        x_pos_array[i] = RIGHT_LIMIT - K_SPRING * (diffx - (RIGHT_LIMIT - x_pos_array[i]));
        x_vel_array[i] = x_vel_array[i] * -K_SPRING;
    }
    else if(x_pos_array[i] + diffx < LEFT_LIMIT) {
        x_pos_array[i] = LEFT_LIMIT - K_SPRING * (diffx - (LEFT_LIMIT - x_pos_array[i]));
        x_vel_array[i] = x_vel_array[i] * -K_SPRING;
    }
    else{
        x_pos_array[i] = x_pos_array[i] + 1 * DT * x_vel_array[i];
    }
    if(y_pos_array[i] + diffy > BOTTOM_LIMIT) {
        y_pos_array[i] = BOTTOM_LIMIT - K_SPRING * (diffy - (BOTTOM_LIMIT - y_pos_array[i]));
        y_vel_array[i] = y_vel_array[i] * -K_SPRING;
    }
    else if(y_pos_array[i] + diffy < TOP_LIMIT) {
        y_pos_array[i] = TOP_LIMIT - K_SPRING * (diffy - (TOP_LIMIT - y_pos_array[i]));
        y_vel_array[i] = y_vel_array[i] * -K_SPRING;
    }
    else{
        y_pos_array[i] = y_pos_array[i] + 1 * DT * y_vel_array[i];
    }
}


void step(int x_force, int y_force, int i)
{
    x_accel_array[i] = x_force_array[i] / MASS;
    y_accel_array[i] = y_force_array[i] / MASS;
    x_vel_array[i] = x_vel_array[i] + 1 * DT * x_accel_array[i] - x_vel_array[i] / FRICTION;
    y_vel_array[i] = y_vel_array[i] + 1 * DT * y_accel_array[i] - y_vel_array[i] / FRICTION;
    if(absvalue(x_vel_array[i]) < STOPPING_SPEED && absvalue(y_vel_array[i]) < STOPPING_SPEED){
        x_vel_array[i] = 0;
        y_vel_array[i] = 0;
    }
    moveBall(i);
}

int circleIntersect(int x_1, int y_1, int x_2, int y_2, int r_1, int r_2)
{
    int distance_squared;
    int radius_sum_squared;
    distance_squared = (x_1 / 1000 / 1000 - x_2 / 1000 / 1000) * (x_1 / 1000 / 1000 - x_2 / 1000 / 1000) + (y_1 / 1000 / 1000 - y_2 / 1000 / 1000) * (y_1 / 1000 / 1000 - y_2 / 1000 / 1000);
    radius_sum_squared = (2 * r_1 / 1000 / 1000) * (2 * r_2 / 1000 / 1000);
    if(distance_squared > radius_sum_squared){
        return 0;
    }
    else{
        return 1;
    }
}

int ballsNotMoving()
{
    int i;
    i = 0;
    while(i < 11){
        if(!removed_array[i]){
            if(!x_vel_array[i] == 0 || !y_vel_array[i] == 0){
                return 0;
            }
        }
        i = i + 1;
    }
    return 1;
}

void updateOnCollision()
{
    int i;
    int removed;
    int j;
    int gotemall;
    total_score = 0;
    gotemall = 1;
    i = 0;
    while(i < 7){
        removed = 0;
        j = 0;
        while(j < 6){
            if(circleIntersect(x_pos_array[i], y_pos_array[i], hole_x_pos_array[j], hole_y_pos_array[j], RADIUS, HOLE_RADIUS)){
                removed_array[i] = 1;
                if(i == 0){
                    done = !player;
                    gotemall = 0;
                    return;
                }
                tftdrawcircle(x_pos_array[i] / 1000 / 1000, y_pos_array[i] / 1000 / 1000, RADIUS / 1000 / 1000, 0x2FE0);
                removed = 1;
                total_score = total_score + 1;
                j = 99999;
            }
            else{
                if(i != 0) gotemall = 0;
            }
            j = j + 1;
        }
        if(!removed){
            j = 0;
            while(j < 7){
                if(i != j && !removed_array[j] && circleIntersect(x_pos_array[i], y_pos_array[i], x_pos_array[j], y_pos_array[j], RADIUS, RADIUS)){
                    x_vel_array[i] = x_vel_array[i] + x_vel_array[j] / 2;
                    x_vel_array[j] = x_vel_array[j] - x_vel_array[j] / 2;
                    y_vel_array[i] = y_vel_array[i] + y_vel_array[j] / 2;
                    y_vel_array[j] = y_vel_array[j] - y_vel_array[j] / 2;
                }
                j = j + 1;
            }
        }
        i = i + 1;
    }
    score = total_score - opponents_score;
    if(gotemall) done = player;
}

int wonGame()
{
    int i;
    i = 1;
    while(i < 7){
        if(!removed_array[i]){
            return 0;
        }
        i++;
    }
    if(score >= opponents_score){
        return 1;
    }
}

void undrawAngle()
{
    int ANGLE_LENGTH;
    int angle_endpt_x;
    int angle_endpt_y;
    ANGLE_LENGTH = 15;
    angle_endpt_x = old_x_pos / 1000 / 1000 + ANGLE_LENGTH * (cosine(old_angle));
    angle_endpt_y = old_y_pos / 1000 / 1000 + ANGLE_LENGTH * (sine(old_angle));
    tftdrawline(old_x_pos / 1000 / 1000, old_y_pos / 1000 / 1000, angle_endpt_x, angle_endpt_y, 0x2FE0);
    /*Serial.println("LAST ANGLE");
    Serial.println(old_angle);*/
}

void displayAngle()
{
    int angle_endpt_x, angle_endpt_y;
    int ANGLE_LENGTH;
    ANGLE_LENGTH = 15;
    angle_endpt_x = x_pos_array[0] / 1000 / 1000 + ANGLE_LENGTH * (cosine(angle));
    angle_endpt_y = y_pos_array[0] / 1000 / 1000 + ANGLE_LENGTH * (sine(angle));
    undrawAngle();
    tftdrawline(x_pos_array[0] / 1000 / 1000, y_pos_array[0] / 1000 / 1000, angle_endpt_x, angle_endpt_y, 0xF800);
    old_x_pos = x_pos_array[0];
    old_y_pos = y_pos_array[0];
    old_angle = angle;
}



/* BEGINNING OF IMU CODE */

void calibrate()
{
    int i;
    int total_acc;
    int total_g_x;
    int acc_mag;
    int *buf;

    buf = (int *)heapalloc(3 * sizeof(int));

    i = 0;
    total_acc = 0;
    total_g_x = 0;

    while(i < 10) {
        imureadaccel(buf);

        a_x = buf[0];
        a_y = buf[1];
        a_z = buf[2];

        acc_mag = root(a_x * a_x + a_y * a_y + a_z * a_z);
        total_acc = total_acc + acc_mag;

        i++;
    }
    ACC_BASELINE = total_acc / 10;

}

void detect_push(int acc)
{
    if(state == isidle){
        push_detected = 0;
        if(acc > (ACC_BASELINE + THRESHOLD)) {
            state = incr;
        }
    }
    if(state == incr){
        timer = getmillis();

        if(old_acc_mag > acc) {
            push_detected = 1;
            peak_acc = old_acc_mag;
            state = isidle;
        }
    }
}

void postPositions()
{
    printtxt("Starting posty", "SERIAL", 0, 0);
    printnum(x_pos_array[0], var1, 30, 1);
    printchr(',', var1, 30, 2);
    printnum(y_pos_array[0], var1, 30, 2);
    printnum(x_pos_array[1], var2, 30, 1);
    printchr(',', var2, 30, 2);
    printnum(y_pos_array[1], var2, 30, 2);
    printnum(x_pos_array[2], var3, 30, 1);
    printchr(',', var3, 30, 2);
    printnum(y_pos_array[2], var3, 30, 2);
    printnum(x_pos_array[3], var4, 30, 1);
    printchr(',', var4, 30, 2);
    printnum(y_pos_array[3], var4, 30, 2);
    printnum(x_pos_array[4], var5, 30, 1);
    printchr(',', var5, 30, 2);
    printnum(y_pos_array[4], var5, 30, 2);
    printnum(x_pos_array[5], var6, 30, 1);
    printchr(',', var6, 30, 2);
    printnum(y_pos_array[5], var6, 30, 2);
    printnum(x_pos_array[6], var7, 30, 1);
    printchr(',', var7, 30, 2);
    printnum(y_pos_array[6], var7, 30, 2);
    printtxt("Posty 2", "SERIAL", 0, 0);

    printtxt("variable_1=", body, body_len, 1);
    printtxt(var1, body, body_len, 2);
    printtxt("&variable_2=", body, body_len, 2);
    printtxt(var2, body, body_len, 2);
    printtxt("&variable_3=", body, body_len, 2);
    printtxt(var3, body, body_len, 2);
    printtxt("&variable_4=", body, body_len, 2);
    printtxt(var4, body, body_len, 2);
    printtxt("&variable_5=", body, body_len, 2);
    printtxt(var5, body, body_len, 2);
    printtxt("&variable_6=", body, body_len, 2);
    printtxt(var6, body, body_len, 2);
    printtxt("&variable_7=", body, body_len, 2);
    printtxt(var7, body, body_len, 2);
    printtxt("&name=", body, body_len, 2);
    printtxt(names, body, body_len, 2);
    printtxt("&host=", body, body_len, 2);
    printtxt(host, body, body_len, 2);
    printtxt("&player=", body, body_len, 2);
    printnum(player, body, body_len, 2);
    printtxt("&score=", body, body_len, 2);
    printnum(score, body, body_len, 2);
    printtxt("&done=", body, body_len, 2);
    printnum(done, body, body_len, 2);
    printtxt("k ima actually post now", "SERIAL", 0, 0);


    httppost(body, "/sandbox/sc/team070/request_handler/request_handler.py", response_buffer, 1000);
    printtxt("START RESPONSE BUFFER(POST:", "SERIAL", 0, 0);
    printtxt(response_buffer, "SERIAL", 0, 0);
    printtxt("END RESPONSE BUFFER(POST):", "SERIAL", 0, 0);
    printtxt("START RESPONSE BUFFER(POST:", "SERIAL", 0, 0);
    printtxt(response_buffer, "SERIAL", 0, 0);
    printtxt("END RESPONSE BUFFER(POST):", "SERIAL", 0, 0);

}

char *fuckingstirtalk(char *pointer, char *delimiters)
{
    int break_loop;
    int break_first_loop;
    int count;
    int i;
    int fuck;
    int no_delims_present;
    if(break_this_bitch){
        return -1;
    }
    pointer = saved_pointer;
    break_loop = 0;
    break_first_loop = 0;
    count = 0;
    i = 0;

    while(!break_first_loop){
        no_delims_present = 1;
        fuck = 0;
        i = 0;
        while(i < 4 && fuck == 0){
            if(*pointer == *(delimiters + i)){
                if(*pointer == 0){
                    break_this_bitch = 1;
                    return -1;
                }
                pointer = pointer + 1;
                no_delims_present = 0;
                fuck = 1;
            }
            i = i + 1;
        }

        if(no_delims_present){
            break_first_loop = 1;
        }
    }

    while(!break_loop){
        i = 0;
        while(i < 4){
            if(*(pointer + count) == *(delimiters + i)){
                if(*(pointer + count) == 0){
                    break_this_bitch = 1;
                }
                *(pointer + count) = 0;
                saved_pointer = pointer + count + 1;
                return pointer;
            }
            i = i + 1;
        }
        count = count + 1;
    }
}


int data_extractor(char *data_array)
{
    char *char_data;
    int counter;
    printtxt("Entered the DATA EXTRAKTOR", "SERIAL", 0, 0);
    counter = 0;
    delim[0] = ';';
    delim[1] = '=';
    delim[2] = ',';
    delim[3] = 0;
    saved_pointer = data_array;
    break_this_bitch = 0;
    char_data = fuckingstirtalk(data_array, delim);
    printtxt("Evaluated garbage fuckingstirtalk", "SERIAL", 0, 0);
    if((*data_array - '0') == 0){
        printtxt("i am not stupid", "SERIAL", 0, 0);
        return 0;
    }
    while(char_data > 0){
        printtxt("I AM VALID", "SERIAL", 0, 0);
        printtxt(char_data, "SERIAL", 0, 0);
        printnum(counter, "SERIAL", 0, 0);
        if(counter == 25){
            done = *char_data - '0';
        }
        else if(counter == 2){
            x_pos_array[0] = ifroma(char_data);
        }
        else if(counter == 3){
            y_pos_array[0] = ifroma(char_data);
        }
        else if(counter == 5){
            x_pos_array[1] = ifroma(char_data);
        }
        else if(counter == 6){
            y_pos_array[1] = ifroma(char_data);
        }
        else if(counter == 8){
            x_pos_array[2] = ifroma(char_data);
        }
        else if(counter == 9){
            y_pos_array[2] = ifroma(char_data);
        }
        else if(counter == 11){
            x_pos_array[3] = ifroma(char_data);
        }
        else if(counter == 12){
            y_pos_array[3] = ifroma(char_data);
        }
        else if(counter == 14){
            x_pos_array[4] = ifroma(char_data);
        }
        else if(counter == 15){
            y_pos_array[4] = ifroma(char_data);
        }
        else if(counter == 17){
            x_pos_array[5] = ifroma(char_data);
        }
        else if(counter == 18){
            y_pos_array[5] = ifroma(char_data);
        }
        else if(counter == 20){
            x_pos_array[6] = ifroma(char_data);
        }
        else if(counter == 21){
            y_pos_array[6] = ifroma(char_data);
        }
        else if(counter == 23){
            opponents_score = ifroma(char_data);
        }
        counter = counter + 1;

        char_data = fuckingstirtalk(data_array, delim);
    }
    tftfill(0x2FE0);
    return 1;
}


int getPositions()
{
    printtxt("hello", "SERIAL", 0, 0);
    printtxt("name=", request_buffer, 200, 1);
    printtxt(names, request_buffer, 200, 2);
    printtxt("&host=", request_buffer, 200, 2);
    printtxt(host, request_buffer, 200, 2);
    printtxt("&player=", request_buffer, 200, 2);
    printnum(player, request_buffer, 200, 2);
    httpget(request_buffer, "/sandbox/sc/team070/request_handler/request_handler.py", response_buffer, 1000);
    printtxt("START RESPONSE BUFFER(get):", "SERIAL", 0, 0);
    printtxt(response_buffer, "SERIAL", 0, 0);
    printtxt("END RESPONSE BUFFER(get):", "SERIAL", 0, 0);
    return data_extractor(response_buffer);
}


int main()
{
    int i;
    int acc_mag;
    int avg_acc_mag;
    int didsetplayer;
    int *buf;

    periodloop = 50;
    primary_timer = 0;
    posting_timer = 0;
    RESPONSE_TIMEOUT = 6000;
    IN_BUFFER_SIZE = 1000;
    OUT_BUFFER_SIZE = 1000;

    buf = (int *)heapalloc(3 * sizeof(int));

    request_buffer = (char *)heapalloc(200 * sizeof(char));
    response_buffer = (char *)heapalloc(1000 * sizeof(char));
    body_len = 300;
    body = (char *)heapalloc(body_len * sizeof(char));
    var1 = (char *)heapalloc(30 * sizeof(char));
    var2 = (char *)heapalloc(30 * sizeof(char));
    var3 = (char *)heapalloc(30 * sizeof(char));
    var4 = (char *)heapalloc(30 * sizeof(char));
    var5 = (char *)heapalloc(30 * sizeof(char));
    var6 = (char *)heapalloc(30 * sizeof(char));
    var7 = (char *)heapalloc(30 * sizeof(char));
    names = "Billiards";
    host = (char *)heapalloc(50 * sizeof(char));
    gethostname(host, 50 * sizeof(char));
    printtxt("Set player nums", "SERIAL", 0, 0);
    done = 2;
    DT = 40;
    SCALER = 1;
    BUTTON_PIN = 16;
    x_pos_array = (int *)heapalloc(7 * sizeof(int));
    printtxt("Allocated xpos", "SERIAL", 0, 0);
    /*x_pos_array = {64,64,58,70,52,64,76,46,58,70,82};*/
    x_pos_array[0] = 64;
    x_pos_array[1] = 64;
    x_pos_array[2] = 58;
    x_pos_array[3] = 70;
    x_pos_array[4] = 52;
    x_pos_array[5] = 64;
    x_pos_array[6] = 76;
    y_pos_array = (int *)heapalloc(7 * sizeof(int));
    printtxt("Allocated ypos", "SERIAL", 0, 0);
    /*y_pos_array = {20,90,101,101,112,112,112,123,123,123,123};*/
    y_pos_array[0] = 20;
    y_pos_array[1] = 90;
    y_pos_array[2] = 101;
    y_pos_array[3] = 101;
    y_pos_array[4] = 112;
    y_pos_array[5] = 112;
    y_pos_array[6] = 112;
    x_vel_array = (int *)heapalloc(7 * sizeof(int));
    y_vel_array = (int *)heapalloc(7 * sizeof(int));
    x_accel_array = (int *)heapalloc(7 * sizeof(int));
    y_accel_array = (int *)heapalloc(7 * sizeof(int));
    x_force_array = (int *)heapalloc(7 * sizeof(int));
    y_force_array = (int *)heapalloc(7 * sizeof(int));
    color_array = (int *)heapalloc(7 * sizeof(int));
    printtxt("Allocated color", "SERIAL", 0, 0);
    color_array[0] = 0xFFFF;
    color_array[1] = 0xFCE0;
    color_array[2] = 0x001F;
    color_array[3] = 0xF819;
    color_array[4] = 0xA01F;
    color_array[5] = 0x0000;
    color_array[6] = 0xF800;
    removed_array = (int *)heapalloc(7 * sizeof(int));
    printtxt("Got removed", "SERIAL", 0, 0);
    total_score = 0;
    opponents_score = 0;
    score = 1;
    ball_shot = 0;
    pressed = 0;
    stop_game = 0;
    HOLE_RADIUS = 10 * 1000 * 1000;
    angle = 0;
    old_angle = 0;
    MASS = 1;
    RADIUS = 5 * 1000 * 1000;
    FRICTION = 100;
    STOPPING_SPEED = 5000;
    K_SPRING = 1;
    LEFT_LIMIT = RADIUS;
    RIGHT_LIMIT = 127 * 1000 * 1000 - RADIUS;
    TOP_LIMIT = RADIUS;
    BOTTOM_LIMIT = 159 * 1000 * 1000 - RADIUS;
    output = (char *)heapalloc(80 * sizeof(char));
    printtxt("Output", "SERIAL", 0, 0);
    isidle = 0;
    incr = 1;
    push_detected = 0;
    THRESHOLD = 10;
    printtxt("Saved pointer done", "SERIAL", 0, 0);
    i = 0;
    delim = (char *)heapalloc(4 * sizeof(char));
    hole_x_pos_array = (int *)heapalloc(6 * sizeof(int));
    hole_x_pos_array[0] = 10;
    hole_x_pos_array[1] = 10;
    hole_x_pos_array[2] = 10;
    hole_x_pos_array[3] = 127;
    hole_x_pos_array[4] = 127;
    hole_x_pos_array[5] = 127;
    hole_y_pos_array = (int *)heapalloc(6 * sizeof(int));
    hole_y_pos_array[0] = 0;
    hole_y_pos_array[1] = 80;
    hole_y_pos_array[2] = 159;
    hole_y_pos_array[3] = 0;
    hole_y_pos_array[4] = 80;
    hole_y_pos_array[5] = 159;
    charstarscore = (char *)heapalloc(10 * sizeof(char));

    didsetplayer = 0;
    tftprint("Please set your player number. A for 0. B for 1.");
    while(didsetplayer == 0){
        if(!buttonread(16)){
            while(!buttonread(16));
            didsetplayer = 1;
            player = 0;
        }
        else if(!buttonread(5)){
            while(!buttonread(5));
            didsetplayer = 1;
            player = 1;
        }
    }
    opponents_turn = player;
    printtxt("Got button read", "SERIAL", 0, 0);
    timer = getmillis();
    tftrotation(2);
    tfttextsize(1);
    tftfill(0x2FE0);
    tfttextcolor(0x0000, 0x2FE0);
    dttimer = getmillis();
    printtxt("Initial config done", "SERIAL", 0, 0);

    i = 0;
    while(i < 7){
        printtxt("Iter", "SERIAL", 0, 0);
        if(i < 6){
            printtxt("Triggered if", "SERIAL", 0, 0);
            hole_x_pos_array[i] = hole_x_pos_array[i] * 1000 * 1000;
            hole_y_pos_array[i] = hole_y_pos_array[i] * 1000 * 1000;
        }
        x_pos_array[i] = x_pos_array[i] * 1000 * 1000;
        y_pos_array[i] = y_pos_array[i] * 1000 * 1000;
        i = i + 1;
    }
    printtxt("Set up arrs", "SERIAL", 0, 0);

    calibrate();

    while(stop_game == 0){
        if(done != 2){
            printtxt("WINNERS CIRCLE", "SERIAL", 0, 0);
            if(done == !player){
                tftsetcursor(0, 30);
                tftprint("YOU LOST WOW UR BAD");
                printtxt("YOU LOST", "SERIAL", 0, 0);
                done = !player;
                stop_game = 1;
                undrawAngle();
                postPositions();
            }

            else if(done == player){
                tftsetcursor(0, 30);
                tftprint("well, you won i guess");
                printtxt("YOU WIN", "SERIAL", 0, 0);
                done = player;
                stop_game = 1;
                postPositions();
            }
        }
        else{
            printtxt("THE GAME CONTINUES", "SERIAL", 0, 0);
            if(ball_shot && ballsNotMoving()){
                ball_shot = 0;
                angle = 0;
                postPositions();
                opponents_turn = 1;
            }

            if(opponents_turn){
                printtxt("get loop", "SERIAL", 0, 0);
                if(getPositions() == 1){
                    printtxt("I SHOULD NOT BE HAPPENINGINGING", "SERIAL", 0, 0);
                    opponents_turn = 0;
                }
                while(getmillis() - timer < 1000){}
                timer = getmillis();
            }
            else if(!opponents_turn){
                i = 0;
                imureadaccel(buf);
                a_x = 6 * buf[0] / 7;
                a_y = 6 * buf[1] / 7;
                a_z = 6 * buf[2] / 7;

                acc_mag = root(a_x * a_x + a_y * a_y + a_z * a_z);
                avg_acc_mag = (acc_mag + old_acc_mag + older_acc_mag) / 3;
                detect_push(acc_mag);

                older_acc_mag = old_acc_mag;
                old_acc_mag = acc_mag;

                if(push_detected == 1) {
                    printtxt("Push detected", "SERIAL", 0, 0);
                    push_acc = peak_acc - ACC_BASELINE;
                    ball_shot = 1;
                    i = 0;
                    while(i < 1){
                        scale_speed = 16;
                        x_force_array[i] = push_acc;
                        y_force_array[i] = push_acc;
                        x_force_array[i] = x_force_array[i] * cosine(angle) / scale_speed;
                        y_force_array[i] = y_force_array[i] * sine(angle) / scale_speed;
                        x_vel_array[i] = 0;
                        y_vel_array[i] = 0;
                        i = i + 1;
                    }
                    push_detected = 0;

                }

                tftsetcursor(28, 0);
                tftprint("YOUR SCORE: ");
                printnum(score, charstarscore, 10, 1);
                tftprint(charstarscore);

                updateOnCollision();

                i = 0;
                printtxt("In the 7 loop", "SERIAL", 0, 0);
                while(i < 7){
                    if(!removed_array[i]){
                        tftfillcircle(x_pos_array[i] / 1000 / 1000, y_pos_array[i] / 1000 / 1000, RADIUS / 1000 / 1000, 0x2FE0);
                        step(x_force_array[i] * SCALER, y_force_array[i] * SCALER, i);
                        x_force_array[i] = 0;
                        y_force_array[i] = 0;
                        tftfillcircle(x_pos_array[i] / 1000 / 1000, y_pos_array[i] / 1000 / 1000, RADIUS / 1000 / 1000, color_array[i]);
                    }
                    i = i + 1;
                }

                i = 0;
                printtxt("In the 6 loop", "SERIAL", 0, 0);
                while(i < 6){
                    tftfillcircle(hole_x_pos_array[i] / 1000 / 1000, hole_y_pos_array[i] / 1000 / 1000, HOLE_RADIUS / 1000 / 1000, 0x0000);
                    i = i + 1;
                }

                if(push_detected == 0 && ballsNotMoving()) {
                    if(buttonread(BUTTON_PIN) == 0 && !pressed) {
                        angle = angle + 30;
                        pressed = 1;
                    }
                    else if(buttonread(BUTTON_PIN) == 1) {
                        pressed = 0;
                        displayAngle();
                    }
                }

                i = 5;
                while(i != 0){
                    i = getmillis() - dttimer;
                    if(i >= 40) i = 0;
                }
                dttimer = getmillis();
            }
        }
    }
    return 0;
}