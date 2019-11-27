#include <stdlib.h>

char* encrypt(char* msg, int msg_len, int col_size){
    
    char* encrypted_msg = (char *)malloc(sizeof(char)*(msg_len+1));

    int N = 2*(col_size) - 2;
    int t=0;

    for(int i=0;i<msg_len;i+=N)
        encrypted_msg[t++] = msg[i];

    
    for(int i=1;i<col_size-1;i++){
        for(int j=0;N*j+i<msg_len;j++){
            encrypted_msg[t++] = msg[N*j+i];
            if(N*j+2*(col_size-i-1)+i < msg_len)
                encrypted_msg[t++] = msg[N*j+2*(col_size-i-1)+i];
        }
    }

    for(int i=col_size-1;i<msg_len;i+=N)
        encrypted_msg[t++] = msg[i];
    encrypted_msg[t] = '\0';
    return encrypted_msg;
}

char* decrypt(char* enc, int msg_len, int col_size){

    char* decrypted_msg = (char*)malloc(sizeof(char)*(msg_len+1));
    int N = 2*(col_size) - 2;
    int t=0;

    for(int i=0;i<msg_len;i+=N)
        decrypted_msg[i] = enc[t++];
    
    for(int i=1;i<col_size-1;i++){
        for(int j=0;N*j+i<msg_len;j++){
            decrypted_msg[N*j+i] = enc[t++];
            if(N*j+2*(col_size-i-1)+i < msg_len)
                decrypted_msg[N*j+2*(col_size-i-1)+i] = enc[t++];
        }
    }

    for(int i=col_size-1;i<msg_len;i+=N)
        decrypted_msg[i] = enc[t++];
    decrypted_msg[msg_len] = '\0';
    return decrypted_msg;
}