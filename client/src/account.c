/*File for account creation and log-in*/
// src/account.c

#include "../include/account.h"

static void get_username(char* username, size_t length, LOGIN_TYPE type);
static void get_passwd(char* passwd, size_t length, LOGIN_TYPE type);
static void login_account(Account *account);
static void register_account(Account *account);
static void to_str(Account *account, char* str);

Account new_Account() {
    /*
    * @return: a new Account object
    * @description: create a new Account object
    */
    Account account;
    memset(account.username, 0, MAX_USERNAME_LENGTH);
    memset(account.passwd, 0, MAX_PASSWD_LENGTH);
    memset(account.passwd_hash, 0, SHA256_BLOCK_SIZE);
    
    account.login_account = login_account;
    account.register_account = register_account;
    account.to_str = to_str;
    return account;
}


static void get_username(char* username, size_t length, LOGIN_TYPE type) {
    /*
    * @param username: the buffer to store the username
    * @param length: the length of the buffer
    * @description: get the username from console
    */
    while (1) {
        printf("Please input your username (maximum %llu characters): ", length - 1);
        fgets(username, length, stdin);
        if (username[strlen(username) - 1] == '\n') {
            username[strlen(username) - 1] = '\0';
        }
        if (strlen(username) == 0) {
            printf("Username cannot be empty!\n");
            continue;
        }
        break;
    }
}

static void get_passwd(char* passwd, size_t length, LOGIN_TYPE type) {
    /*
    * @param passwd: the buffer to store the password
    * @param length: the length of the buffer
    * @description: get the password from console
    */
    while (1) {
        printf("Please input your password (maximum %llu characters): ", length - 1);
        if (length == 0) {
            printf("Password too long!\n");
            exit(1);
        }
        char ch;
        int i = 0;
        while ((ch = getch()) != '\r') {
            if (ch == '\b') {
                if (i == 0) continue;
                printf("\b \b");
                passwd[--i] = '\0';
            }
            else if (i == length - 1) {
                printf("\nPassword too long!\n");
                exit(1);
            }
            else {
                printf("*");
                passwd[i++] = ch;
            }
        }
        putchar('\n');
        passwd[i] = '\0';
        if (type == LOGIN) break;

        // if the type is REGISTER, get the password again to confirm
        if (type == REGISTER) {
            char* passwd_confirm = (char*)malloc(length * sizeof(char));
            char ch;
            int i = 0;
            printf("Please confirm your password: ");
            while ((ch = getch()) != '\r') {
                if (ch == '\b') {
                    if (i == 0) continue;
                    printf("\b \b");
                    passwd_confirm[--i] = '\0';
                }
                else if (i == length - 1) {
                    printf("\nPassword too long!\n");
                    exit(1);
                }
                else {
                    printf("*");
                    passwd_confirm[i++] = ch;
                }
            }
            putchar('\n');
            passwd_confirm[i] = '\0';
            if (strcmp(passwd, passwd_confirm) == 0) {
                free(passwd_confirm);
                break;
            }
            printf("Password not match, please set again.\n");
        }
    }
}

static void login_account(Account *account) {
    /*
    * @param account: the account to store the username and password
    * @description: login an account
    */
    get_username(account->username, MAX_USERNAME_LENGTH, LOGIN);
    get_passwd(account->passwd, MAX_USERNAME_LENGTH, LOGIN);
    hash_with_salt(account->passwd, strlen(account->passwd), account->passwd_hash);
}

static void register_account(Account *account) {
    /*
    * @param account: the account to store the username and password
    * @description: register an account
    */
    get_username(account->username, MAX_USERNAME_LENGTH, REGISTER);
    get_passwd(account->passwd, MAX_USERNAME_LENGTH, REGISTER);
    hash_with_salt(account->passwd, strlen(account->passwd), account->passwd_hash);
}

void to_str(Account *account, char* str) {
    /*
    * @param account: the account to be converted
    * @param str: the buffer to store the converted char*
    * @description: convert an account to char*
    */
    for (int i = 0; i < MAX_USERNAME_LENGTH; i++) {
        str[i] = account->username[i];
        str[i + MAX_USERNAME_LENGTH] = account->passwd[i];
    }
}