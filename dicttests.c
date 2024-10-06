/* Tests functions for implementing a dictionary.
 * CSC 357, Assignment 3
 * Given tests, Winter '24 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dict.h"
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#define ARGSCOUNT 2

typedef struct {
    char* key;
    int frequency;
} Tuple;

int tupleComparator(const void *a, const void *b) {
    const Tuple *ta = *(const Tuple **) a;
    const Tuple *tb = *(const Tuple **) b;

    if ((ta->frequency) != (tb->frequency)) {
        return tb->frequency - ta->frequency;
    }
    return strcmp(ta->key, tb->key);
}


 void printFrequencies(Dict *dct){
    char** key;
    
    int j = 0;
    int i = 0;
    int val;
    Tuple** tuple ;
    if (dct == NULL || dct->size == 0) {
        printf("NOTHING TO PRINT\n");
        return;
    }

    key = dctkeys(dct);

    tuple = malloc(dct->size * sizeof(Tuple*));
    for (j = 0; j < dct->size; j++) {
        tuple[j] = malloc(sizeof(Tuple));
    }

    for (j = 0; j < dct->size; j++){
        val = (long)dctget(dct, key[j]);
        tuple[j]->key = (char *)key[j];
        tuple[j]->frequency = val;
    }
    /*
    printf("Keys before sorting: ");
    for (i = 0; i < dct->size; i++) {
        printf("\"%s\", ", key[i]);
    }
    printf("\n");
    */


    qsort(tuple, dct->size, sizeof(char*), tupleComparator);

    /*
    printf("Keys after sorting: ");
    for (i = 0; i < dct->size; i++) {
        printf("\"%s\", ", key[i]);
    }
    printf("\n");
    */ 

    for (i = 0; i < dct->size; i++){
        if (i>= 10){break;}
        val = tuple[i]->frequency;
        printf("%s (%d)\n", tuple[i]->key, val);
    }
    for (i = 0; i < dct->size; i++) {
        free(tuple[i]);
    }
    free(tuple);
    free(key);
}

/* test01: Tests creating dictionaries. and empty dictionary */
void test01() {
    Dict *dct = dctcreate();

    assert(dct != NULL);
    assert(dct->cap >= 1);
    assert(dct->arr != NULL);
    assert(dct->size == 0);

    assert(dctkeys(dct) == NULL);
    assert(dctget(dct, "a") == NULL);
    assert(dctremove(dct, "a") == NULL);

    dctdestroy(dct);
}

/* test02: Tests inserting into dictionaries. */
void test02() {
    Dict *dct = dctcreate();

    dctinsert(dct, "a", (void *)1);
    dctinsert(dct, "b", (void *)3);
    dctinsert(dct, "f", (void *)2);

    assert(dct->size == 3);
    assert(dctget(dct, "a") == (void *)1);
    assert(dctget(dct, "b") == (void *)3);
    assert(dctget(dct, "f") == (void *)2);
    assert(dct->size == 3);

    assert(dctremove(dct, "a") == (void *)1);
    assert(dctremove(dct, "b") == (void *)3);
    assert(dctremove(dct, "f") == (void *)2);
    assert(dct->size == 0);

    dctinsert(dct, "x", (void *)1);
    dctinsert(dct, "y", (void *)3);
    dctinsert(dct, "z", (void *)2);

    assert(dctget(dct, "x") == (void *)1);
    assert(dctget(dct, "y") == (void *)3);
    assert(dctget(dct, "z") == (void *)2);
    assert(dct->size == 3);

    assert(dctremove(dct, "x") == (void *)1);
    assert(dctremove(dct, "y") == (void *)3);
    assert(dctremove(dct, "z") == (void *)2);
    assert(dct->size == 0);


    dctdestroy(dct);
}

/* test03: Tests removing from dictionaries. */
void test03() {
    Dict *dct = dctcreate();

    dctinsert(dct, "a", (void *)1);
    dctinsert(dct, "b", (void *)3);
    dctinsert(dct, "f", (void *)2);
    dctremove(dct, "b");
    assert(dct->size == 2);
    dctinsert(dct, "c", (void *)2);

    assert(dct->size == 3);
    assert(dctget(dct, "a") == (void *)1);
    assert(dctget(dct, "c") == (void *)2);
    assert(dctget(dct, "f") == (void *)2);
    assert(dctget(dct, "b") == NULL);

    dctdestroy(dct);
}

/* test04: Tests enumerating keys in dictionaries. */
void test04() {
    char **keys;
    Dict *dct = dctcreate();

    dctinsert(dct, "a", (void *)1);
    dctinsert(dct, "b", (void *)3);
    dctinsert(dct, "f", (void *)2);
    dctremove(dct, "b");
    dctinsert(dct, "c", (void *)2);

    /* NOTE: Dictionaries are unordered, and my tests will not assume any
     *       particular arrangement of keys within the enumerating array. */
    keys = dctkeys(dct);
    assert(
     (!strcmp(keys[0], "a") && !strcmp(keys[1], "c") && !strcmp(keys[2], "f"))||
     (!strcmp(keys[0], "a") && !strcmp(keys[1], "f") && !strcmp(keys[2], "c"))||
     (!strcmp(keys[0], "c") && !strcmp(keys[1], "a") && !strcmp(keys[2], "f"))||
     (!strcmp(keys[0], "c") && !strcmp(keys[1], "f") && !strcmp(keys[2], "a"))||
     (!strcmp(keys[0], "f") && !strcmp(keys[1], "a") && !strcmp(keys[2], "c"))||
     (!strcmp(keys[0], "f") && !strcmp(keys[1], "c") && !strcmp(keys[2], "a")));
    free(keys);

    dctdestroy(dct);
}

/* test05: Tests dictionary rehashing */
void test05() {
    int i;
    char key[10];
    Dict *dct = dctcreate();
    
    for (i = 0; i < 32; i++) {
        sprintf(key, "key%d", i);
        dctinsert(dct, key, (void *)(long)i); 
    }
    
    assert(dct->cap == 63);

    for (i = 0; i < 32; i++) {
        sprintf(key, "key%d", i);
        assert(dctget(dct, key) == (void *)(long)i);
    }

    dctdestroy(dct);
}

/* test06: Tests inserting duplicate keys. */
void test06() {
    Dict *dct = dctcreate();

    dctinsert(dct, "a", (void *)1);
    dctinsert(dct, "a", (void *)2);

    assert(dctget(dct, "a") == (void *)2);
    assert(dct->size == 1); 

    dctdestroy(dct);
}

/* test07: Tests handling null inputs. */
void test07() {
    Dict *dct = dctcreate();

    dctinsert(dct, NULL, (void *)1); 
    assert(dctget(dct, NULL) == NULL);
    assert(dctremove(dct, NULL) == NULL);

    dctdestroy(dct);
}
/* test08: Tests dictionary rehashing */
void test08() {
    int i;
    char key[10];
    Dict *dct = dctcreate();
    
    for (i = 0; i < 32; i++) {
        sprintf(key, "key%d", i);
        dctinsert(dct, key, (void *)(long)i); 
    }
    
    /*hash and make sure that it got to the right new size*/

    assert(dct->cap == 63);

    for (i = 0; i < 32; i++) {
        sprintf(key, "key%d", i);
        assert(dctget(dct, key) == (void *)(long)i);
    }

    /*hash again*/

    for (i = 0; i < 64; i++) {
        sprintf(key, "key%d", i);
        dctinsert(dct, key, (void *)(long)i); 
    }

    assert(dct->cap == 127);

    for (i = 0; i < 64; i++) {
        sprintf(key, "key%d", i);
        assert(dctget(dct, key) == (void *)(long)i);
    }

    dctdestroy(dct);
}


/* test09: check that it got rehashed to the right spot*/
void test09() {
    Dict *dct = dctcreate();

    dctinsert(dct, "it", (void *)1);
    dctinsert(dct, "is", (void *)1);
    dctinsert(dct, "a", (void *)1);
    dctinsert(dct, "period", (void *)1);
    dctinsert(dct, "of", (void *)1);
    dctinsert(dct, "civil", (void *)1);
    dctinsert(dct, "war", (void *)1);

    assert(dct->size == 7);

    assert(dctget(dct, "it") == (void *)1);
    assert(dctget(dct, "is") == (void *)1);
    assert(dctget(dct, "a") == (void *)1);
    assert(dctget(dct, "period") == (void *)1);
    assert(dctget(dct, "of") == (void *)1);
    assert(dctget(dct, "civil") == (void *)1);
    assert(dctget(dct, "war") == (void *)1);

    assert(dct->size == 7);

    dctdestroy(dct);
}

/* test10: check that it got rehashed to the right spot*/
void test10() {
    
    int i=0;
    char key[10];
    
    int index_a=0;
    int index_b=0;
    int index_c=0;
    Dict *dct = dctcreate();

    /*put the things in*/
    dctinsert(dct, "a", (void *)1);
    dctinsert(dct, "b", (void *)1);
    dctinsert(dct, "c", (void *)1);

    /*calcualte the indeces it goes in*/
    assert(index_a = getIndex(dct, "a")==4);
    assert(index_b = getIndex(dct, "b")==5);
    assert(index_c = getIndex(dct, "c")==6);

    assert(dct->size == 3);

    /*make sure the values are still what I wanted*/
    assert(dctget(dct, "a") == (void *)1);
    assert(dctget(dct, "b") == (void *)1);
    assert(dctget(dct, "c") == (void *)1);

    assert(dct->size == 3);
        /*fill with random stuff until it has to rehash*/
    for (i = 4; i < 33; i++) {
        sprintf(key, "key%d", i);
        dctinsert(dct, key, (void *)(long)i); 
    }
    /*make sure it made it to the right size*/
    assert(dct->cap == 63);


    /*make sure it still got put at the right index*/
    assert(dctget(dct, "a") == (void *)1);
    assert(dctget(dct, "b") == (void *)1);
    assert(dctget(dct, "c") == (void *)1);
    /*make sure the values didnt get changed*/
    assert(dct->size == 32);


    dctdestroy(dct);
}




int makeUsableWord(char *string){
    int stringLength = strlen(string);
    int temp = 0;
    int i = 0;
    for (i = 0; i < stringLength; i++){
        if(isalpha(string[i])){
            string[temp] = tolower(string[i]);
            temp++;
        }
    }
    string[temp] = '\0';

    if (strlen(string) == 0) {
        return -1; 
    }
    return 0;
}

int main(int argc, char *argv[]) {

    Dict *dct = dctcreate();
    char buffer[2048];
    FILE *file;
    char *delimiters = " \t\n\v\f\r";
    
    
    test01(); 
    test02(); 
    test03();
    test04();
    test05();
    test06();
    test07();
    test08();
    test09();
    test10();


    if (argc < ARGSCOUNT) {
        printf("./freq: Too few arguments\n");
        return 1;
    }
    if (argc > ARGSCOUNT) {
        printf("./freq: Too many arguments\n");
        return 1; 
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("./freq: No such file or directory\n");
        exit(1);
    }

     
    while (fgets(buffer, sizeof(buffer), file)) { /* as long as I can read lines from the file */
        char *word; /* creating my word */
        void *value;
        int validWordFlag;
        word = strtok(buffer, delimiters); /* my first word will be what's before the first delimiter */
        while (word != NULL) { /* as long as my word isn't NULL */
            validWordFlag = makeUsableWord(word); /* make the word usable */
            if (validWordFlag == -1) { /* should just continue here */
                word = strtok(NULL, delimiters);
                continue;
            }
            
            value = dctget(dct, word);
            if (value != NULL) {
                dctinsert(dct, word, (void *)((long)value + 1));
            } else {
                dctinsert(dct, word, (void *)(long)1);
            }

            word = strtok(NULL, delimiters); /* look at the next word */
        }
    }

    printFrequencies(dct);

    fclose(file);
    dctdestroy(dct);
    return 0;
}
