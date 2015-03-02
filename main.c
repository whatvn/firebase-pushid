/*
 * Fancy ID generator that creates 20-character string identifiers with the
 * following properties:
 * 
 * 1. They're based on timestamp so that they sort *after* any existing ids.
 * 2. They contain 72-bits of random data after the timestamp so that IDs won't
 * collide with other clients' IDs.
 * 3. They sort *lexicographically* (so the timestamp is converted to characters
 * that will sort properly).
 * 4. They're monotonically increasing. Even if you generate more than one in
 * the same timestamp, the
 * latter ones will sort after the former ones. We do this by using the previous
 * random bits
 * but "incrementing" them by 1 (only in the case of a timestamp collision).
 * author: hungnv@opensource.com.vn 
 * origin: https://gist.github.com/mikelehen/3596a30bd69384624c11#file-generate-pushid-js
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

static char DICT[] = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";

#include <sys/time.h>
#include <malloc.h>
#include <string.h>
#include <zlib.h>


long long last_gen_time = 0L;

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); 
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; 
    return milliseconds;
}

double
randr() {
    return (double) rand() / (double) (RAND_MAX + 1.0);
}

int main() {
    srand(current_timestamp());
    long long mls = current_timestamp();
    int i;
    char last_random_char[72] = "";
    int collison = (mls == last_gen_time) ? 1 : 0;
    char time_stamp_char[8] = "";

    for (i = 7; i >= 0; i--) {
        time_stamp_char[i] = DICT[(int) (mls % 64)];
        printf("time stamp char at %d: %c\n", i, time_stamp_char[i]);
        mls = mls / 64;
    }

    if (mls != 0) {
        printf("mls should be 0");
        return 1;
    }
    if (!collison) {
        for (i = 0; i < 12; i++) {
            double fl = randr() * 64;
            printf("floor: %lf\n", fl);
            //            last_random_char[i] = (char) (fl);
            last_random_char[i] = (char) floor(fl);
        }
    } else {
        int last_value_int = 0;
        for (i = 11; i >= 0 && last_random_char[i] == 63; i--) {
            last_value_int = i;
            last_random_char[i] = 0;
        }

        last_random_char[last_value_int]++;
    }
    time_stamp_char[8] = '\0';
    char id[20] = "";
    strcpy(id, time_stamp_char);


    for (i = 0; i < 12; i++) {
        int g = (int) last_random_char[i];
        //printf("id_: %d\n", g);
        strncat(id, &DICT[g], 1);
    }

    printf("final: %s \n", id);
    printf("id length: %zu\n", strlen(id));
    //printf("current time: %lld\n", mls);
    return 0;
}