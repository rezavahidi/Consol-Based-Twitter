#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <string.h>
char buffer[1000];
int client_socket;
char *token;
int initialize(int);
void setbio();
void search();
char us[100];
void logout();
char passtrue[100];
void changepass();
void follow();
void unfollow();
char *randstring(int);
void profile();
void start(int);
void login();
void sendtweet();
void signup();
int main() {
    int port=12345;
    int server_socket = initialize(port);
    if (server_socket != INVALID_SOCKET) {
        start(server_socket);
    }
    return 0;
}

int initialize(int port) {
    WSADATA wsadata;
    int wsa_res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (wsa_res != 0)
    {
        printf("Failed to run wsadata");
        return INVALID_SOCKET;
    }
    int server_socket=socket(AF_INET,SOCK_STREAM,0);
    if(server_socket==INVALID_SOCKET)
    {
        printf("Could not creat socket");
        return INVALID_SOCKET;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(port);

    int bind_res=bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(bind_res==SOCKET_ERROR)
    {
        printf("Bind failed");
        return INVALID_SOCKET;
    }
    listen(server_socket, SOMAXCONN);
    return server_socket;

}
void start(int server_socket) {
    int len = sizeof(struct sockaddr_in);
    //int client_socket;
    struct sockaddr_in client_addr;
    while ((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &len)) != INVALID_SOCKET) {
        //printf("Client connected");
        //char buffer[1000];
        memset(buffer, 0, 1000);
        recv(client_socket, buffer, 999, 0);
        int i;
        char recv6[7],recv5[6],recv10[11],recv7[8];
        memset(recv6,0,7);
        memset(recv5,0,6);
        memset(recv10,0,11);
        memset(recv7,0,8);
        for(i=0;i<5;i++)
        {
            recv5[i]=buffer[i];
        }
        for(i=0;i<6;i++)
        {
            recv6[i]=buffer[i];
        }
        for(i=0;i<7;i++)
        {
            recv7[i]=buffer[i];
        }
        for(i=0;i<10;i++)
        {
            recv10[i]=buffer[i];
        }
        if(strcmp(recv7,"set bio")==0)
        {
            setbio();
        }
        if(strcmp(recv7,"unfollo")==0)
        {
            unfollow();
        }
        if(strcmp(recv7,"profile")==0)
        {
            profile();
        }
        if(strcmp(recv6,"signup")==0)
        {
            signup();
        }
        if(strcmp(recv6,"logout")==0)
        {
            logout();
        }
        if(strcmp(recv6,"follow")==0)
        {
            follow();
        }
        if(strcmp(recv6,"search")==0)
        {
            search();
        }
        if(strcmp(recv5,"login")==0)
        {
            login();
        }
        if(strcmp(recv10,"send tweet")==0)
        {
            sendtweet();
        }
        if(strcmp(recv10,"change pas")==0)
        {
            changepass();
        }
        //send(client_socket, response, strlen(response), 0);
        //closesocket(client_socket);
    }
}
void signup() {
    char *response = malloc(sizeof(char)*200);
    char use[100], pass[100];
    use[0] = buffer[7];
    int i = 0, j = 0;
    while (buffer[i + 8] != ',') {
        i++;
        use[i] = buffer[i + 7];
    }
    while (buffer[i + 10] != '\n') {
        pass[j] = buffer[i + 10];
        j++, i++;
    }
    char mkuser[100];
    sprintf(mkuser, "%s.user.txt", use);
    FILE *user;
    if (user = fopen(mkuser, "r")) {
        strcpy(response, "{\"type\":\"Error\",\"message\":\"This username is already taken.\"}\n");
        printf("taken usename\n");
    } else {
        user = fopen(mkuser, "w");
        fprintf(user, "{\"username\":\"%s\",\"password\":\"%s\",\"bio\":\"\",\"followers\":[],\"followings\":[],\"personalTweets\":[]}",use,pass);
        strcpy(response,"{\"type\":\"Successful\",\"message\":\"\"}\n");
        printf("signup done\n");
    }
    fclose(user);
    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
    memset(response,0,strlen(response));
    memset(buffer,0,strlen(buffer));
}
void login()
{
    memset(passtrue,0,100);
    char details[1000];
    char *response = malloc(sizeof(char)*200);
    char pass[100];
    us[0] = buffer[6];
    int i = 0, j = 0;
    while (buffer[i + 7] != ',') {
        i++;
        us[i] = buffer[i + 6];
    }
    while (buffer[i + 9] != '\n') {
        pass[j] = buffer[i + 9];
        j++, i++;
    }
    char useropen[200];
    sprintf(useropen,"%s.user.txt",us);
    FILE *user;
    if(user = fopen(useropen,"r"))
    {
        while(!feof(user))
        {
            fgets(details,1000,user);
        }
        int x=18;
        while(details[x]!=':')
        {
            x++;
        }

        x+=2,i=0;
        while(details[x]!='\"')
        {
            passtrue[i]=details[x];
            i++,x++;
        }
        if(strcmp(pass,passtrue)==0)
        {
            token=randstring(32);
            sprintf(response,"{\"type\":\"Token\",\"message\":\"%s\"}\n",token);
            printf("login done\n");
        }
        else
        {
            strcpy(response,"{\"type\":\"Error\",\"message\":\"Incorrect password.\"}\n");
            printf("Incorrect password\n");
        }
    } else
    {
        strcpy(response,"{\"type\":\"Error\",\"message\":\"This username is not valid.\"}\n");
        printf("invalid username\n");
    }
    fclose(user);
    memset(details,0,strlen(details));
    memset(pass,0,strlen(pass));
    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
    memset(response,0,strlen(response));
    memset(buffer,0,strlen(buffer));
}
void sendtweet()
{
    char *response = malloc(200);
    char details[1000];
    char tk[33];
    memset(tk,0,33);
    int i=11,j;
    for(j=0;j<32;j++)
    {
        tk[j]=buffer[i];
        i++;
    }
    if(strcmp(tk,token)==0)
    {
        char tweet[200];
        i=45,j=0;
        while(buffer[i]!='\n')
        {
            tweet[j]=buffer[i];
            i++,j++;
        }
        i=1;
        char tweetnum[20];
        char num[10];
        itoa(i,num,10);
        sprintf(tweetnum,"%s.tweet.txt",num);
        FILE *user;
        while(user = fopen(tweetnum,"r"))
        {
            i++;
            memset(num,0,10);
            memset(tweetnum,0,20);
            itoa(i,num,10);
            sprintf(tweetnum,"%s.tweet.txt",num);
            fclose(user);
        }
        user = fopen(tweetnum,"w");
        fprintf(user,"{\"id\":%s,\"author\":\"%s\",\"content\":\"%s\",\"comments\":{},\"likes\":0}",num,us,tweet);
        fclose(user);
        char useropen[200];
        sprintf(useropen,"%s.user.txt",us);
        user = fopen(useropen,"r");
        while(!feof(user))
        {
            fgets(details,1000,user);
        }
        fclose(user);
        i=strlen(details);
        details[i-1]=0;details[i-2]=0;
        if(details[i-3]=='[')
        {
            strcat(details,num);
            strcat(details,"]}");
        }
        else
        {
            strcat(details,",");
            strcat(details,num);
            strcat(details,"]}");
        }
        user = fopen(useropen,"w");
        {
            fprintf(user,details);
        }
        fclose(user);
        strcpy(response,"{\"type\":\"Successful\",\"message\":\"Tweet is sent successfully.\"}\n");
        printf("tweet was sent\n");
    }
    else
    {
        strcpy(response,"{\"type\":\"Error\",\"message\":\"Bad request format.\"}\n");
        printf("bad format\n");
    }
    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
    memset(response,0,strlen(response));
    memset(buffer,0,strlen(buffer));
    memset(details,0,1000);

}
void setbio()
{
    char *response = malloc(200);
    char details[1000];
    char bio[200];
    char tk[33];
    memset(tk,0,33);
    int i=8,j;
    for(j=0;j<32;j++)
    {
        tk[j]=buffer[i];
        i++;
    }
    i=42,j=0;
    while(buffer[i]!='\n')
    {
        bio[j]=buffer[i];
        i++,j++;
    }
    if(strcmp(tk,token)==0)
    {
        FILE *user;
        char useropen[30];
        sprintf(useropen,"%s.user.txt",us);
        user = fopen(useropen,"r");
        while(!feof(user))
        {
            fgets(details,1000,user);
        }
        fclose(user);
        i= strlen(us)+strlen(passtrue)+36;
        if(details[i]!='\"')
        {
            //bio has been set before
            strcpy(response,"{\"type\":\"Error\",\"message\":\"Bio is already updated.\"}\n");
            printf("bio has been sent before\n");
        }
        else
        {
            char new[500];
            for(j=0;j<i;j++)
            {
                new[j]=details[j];
            }
            strcat(new,bio);
            j=strlen(new);
            while(details[i])
            {
                new[j]=details[i];
                i++,j++;
            }
            user = fopen(useropen,"w");
            {
                fprintf(user,new);
            }
            fclose(user);
            strcpy(response,"{\"type\":\"Successful\",\"message\":\"Bio is updated successfully.\"}\n");
            printf("bio has been set successfully\n");
        }
    } else
    {
        strcpy(response,"{\"type\":\"Error\",\"message\":\"Bad request format.\"}\n");
        printf("bad format\n");
    }
    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
    memset(response,0,strlen(response));
    memset(buffer,0,strlen(buffer));
}
void changepass() {
    char *response = malloc(200);
    char details[1000];
    char oldpass[200];
    char newpass[200];
    char newdetails[1000];
    memset(newdetails, 0, 1000);
    memset(oldpass, 0, 200);
    memset(newpass, 0, 200);
    memset(response, 0, 200);
    memset(details, 0, 1000);
    char tk[33];
    memset(tk, 0, 33);
    int i = 16, j;
    for (j = 0; j < 32; j++) {
        tk[j] = buffer[i];
        i++;
    }
    i = 50, j = 0;
    while (buffer[i] != ',') {
        oldpass[j] = buffer[i];
        i++, j++;
    }
    i += 2, j = 0;
    while (buffer[i] != '\n') {
        newpass[j] = buffer[i];
        i++, j++;
    }
    if(strcmp(tk,token)==0)
    {
    char useropen[200];
    sprintf(useropen, "%s.user.txt", us);
    FILE *user;
    user = fopen(useropen, "r");
    while (!feof(user)) {
        fgets(details, 1000, user);
    }
    fclose(user);
    int x = 18;
    //printf("%s",details);
    while (details[x] != ':') {
        x++;
    }
    x += 2;
    for (j = 0; j < x; j++) {
        newdetails[j] = details[j];
    }
    if (strcmp(oldpass, passtrue) == 0) {
        //right pass
        strcat(newdetails, newpass);
        x += strlen(passtrue);
        j += strlen(newpass);
        while (details[x]!='\0') {
            newdetails[j] = details[x];
            j++, x++;
        }
        user = fopen(useropen, "w");
        fprintf(user, newdetails);
        fclose(user);
        strcpy(response,"{\"type\":\"Successful\",\"message\":\"Password is changed successfully.\"}\n");
        printf("password changed\n");
        strcpy(passtrue,newpass);
    } else {
        //wrong pass
        strcpy(response, "{\"type\":\"Error\",\"message\":\"Entered current password is wrong.\"}\n");
        printf("wrong password\n");
    }
    }
    else
    {
        strcpy(response,"{\"type\":\"Error\",\"message\":\"Bad request format.\"}\n");
        printf("bad format\n");
    }
    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
    memset(response,0,strlen(response));
    memset(buffer,0,strlen(buffer));
    memset(details,0,1000);
    memset(newdetails,0,strlen(newdetails));
    memset(newpass,0,strlen(newpass));
    memset(oldpass,0,strlen(oldpass));
}
void search()
{
    char response[100000];
    char details[10000];
    char username[200],useropen[200];
    char tk[33];
    memset(response,0,100000);
    memset(details,0,10000);
    memset(username,0,200);
    memset(useropen,0,200);
    memset(tk,0,33);
    int i=7,j;
    for(j=0;j<32;j++)
    {
        tk[j]=buffer[i];
        i++;
    }
    i=41,j=0;
    while(buffer[i]!='\n')
    {
        username[j]=buffer[i];
        i++,j++;
    }
    if(strcmp(tk,token)==0) {
        sprintf(useropen, "%s.user.txt", username);
        FILE *user;
        if (user = fopen(useropen, "r")) {
            while (!feof(user)) {
                fgets(details, 10000, user);
            }
            fclose(user);
            //valid user
            strcpy(response, "{\"type\":\"Profile\",\"message\":");
            int noghte = 0;
            i = 28, j = 0;
            while (noghte < 1) {
                response[i] = details[j];
                if (details[j] == ',') {
                    noghte++;
                }
                j++, i++;
            }
            while (noghte < 2) {
                if (details[j] == ',') {
                    noghte++;
                }
                j++;
            }
            while (noghte < 3) {
                response[i] = details[j];
                if (details[j] == ',') {
                    noghte++;
                }
                j++, i++;
            }
            //akhar bio
            strcat(response, "\"numberOfFollowers\":");
            char str[1000];
            i += 20;
            j += 13;
            int k = 0;
            int followers = 0;
            while (details[j] != ']') {
                followers++;
                while (details[j] != ']' && details[j] != ',') {
                    str[k] = details[j];
                    j++, k++;
                }
                if (details[j] == ',') {
                    j++;
                }
            }
            char follower[10];
            itoa(followers, follower, 10);
            strcat(response, follower);
            strcat(response, ",\"numberOfFollowings\":");
            i += strlen(follower) + 22;
            j += 16;
            int followings = 0;
            while (details[j] != ']') {
                followings++;
                while (details[j] != ']' && details[j] != ',') {
                    j++;
                }
                if (details[j] == ',') {
                    j++;
                }
            }
            char following[10];
            itoa(followings, following, 10);
            strcat(response, following);
            strcat(response, ",\"followStatus\":");
            i += strlen(following) + 16;
            j += 20;
            char *flw = strstr(str, us);
            if (flw) {
                strcat(response, "\"Followed\",\"allTweets\":[");
            } else {
                strcat(response, "\"NotFollowed\",\"allTweets\":[");
            }
            char tweetopen[20], tweet[1000];
            k = 0;
            int x = 0;
            while (details[j] != ']') {
                if (x != 0) {
                    strcat(response, ",");
                }
                while (details[j] != ']' && details[j] != ',') {
                    tweetopen[k] = details[j];
                    k++, j++;
                }
                strcat(tweetopen, ".tweet.txt");
                user = fopen(tweetopen, "r");
                while (!feof(user)) {
                    fgets(tweet, 1000, user);
                }
                fclose(user);
                strcat(response, tweet);
                if (details[j] == ',') {
                    j++;
                }
                memset(tweet, 0, 1000);
                memset(tweetopen, 0, 20);
                x = 1;
                k=0;
            }
            strcat(response, "]}}\n");
            printf("search completed\n");
            //fclose(user);
        } else {
            strcpy(response, "{\"type\":\"Error\",\"message\":\"This username is not valid.\"}\n");
            printf("invalid username\n");
            //invalid user
        }
    } else{
        strcpy(response,"{\"type\":\"Error\",\"message\":\"Bad request format.\"}\n");
        printf("bad format\n");
    }
    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
    memset(response,0,strlen(response));
    memset(buffer,0,strlen(buffer));
    memset(details,0,strlen(details));
}
void profile()
{
    char response[100000];
    char details[10000];
    char username[200],useropen[200];
    char tk[33];
    memset(response,0,100000);
    memset(details,0,10000);
    memset(username,0,200);
    memset(useropen,0,200);
    memset(tk,0,33);
    int i=8,j;
    for(j=0;j<32;j++)
    {
        tk[j]=buffer[i];
        i++;
    }
    strcpy(username,us);
    if(strcmp(tk,token)==0) {
        sprintf(useropen, "%s.user.txt", username);
        FILE *user;
        if (user = fopen(useropen, "r")) {
            while (!feof(user)) {
                fgets(details, 10000, user);
            }
            fclose(user);
            //valid user
            strcpy(response, "{\"type\":\"Profile\",\"message\":");
            int noghte = 0;
            i = 28, j = 0;
            while (noghte < 1) {
                response[i] = details[j];
                if (details[j] == ',') {
                    noghte++;
                }
                j++, i++;
            }
            while (noghte < 2) {
                if (details[j] == ',') {
                    noghte++;
                }
                j++;
            }
            while (noghte < 3) {
                response[i] = details[j];
                if (details[j] == ',') {
                    noghte++;
                }
                j++, i++;
            }
            //akhar bio
            strcat(response, "\"numberOfFollowers\":");
            char str[1000];
            memset(str,0,1000);
            i += 20;
            j += 13;
            int k = 0;
            int followers = 0;
            while (details[j] != ']') {
                followers++;
                while (details[j] != ']' && details[j] != ',') {
                    str[k] = details[j];
                    j++, k++;
                }
                if (details[j] == ',') {
                    j++;
                }
            }
            char follower[10];
            memset(follower,0,10);
            itoa(followers, follower, 10);
            strcat(response, follower);
            strcat(response, ",\"numberOfFollowings\":");
            i += strlen(follower) + 22;
            j += 16;
            int followings = 0;
            while (details[j] != ']') {
                followings++;
                while (details[j] != ']' && details[j] != ',') {
                    j++;
                }
                if (details[j] == ',') {
                    j++;
                }
            }
            char following[10];
            memset(following,0,10);
            itoa(followings, following, 10);
            strcat(response, following);
            strcat(response, ",\"followStatus\":");
            i += strlen(following) + 16;
            j += 20;
            strcat(response, "\"Yourself\",\"allTweets\":[");
            char tweetopen[20], tweet[1000];
            k = 0;
            int x = 0;
            while (details[j] != ']') {
                if (x != 0) {
                    strcat(response, ",");
                }
                while (details[j] != ']' && details[j] != ',') {
                    tweetopen[k] = details[j];
                    k++, j++;
                }
                strcat(tweetopen, ".tweet.txt");
                user = fopen(tweetopen, "r");
                while (!feof(user)) {
                    fgets(tweet, 1000, user);
                }
                fclose(user);
                strcat(response, tweet);
                if (details[j] == ',') {
                    j++;
                }
                memset(tweet, 0, 1000);
                memset(tweetopen, 0, 20);
                x = 1;
                k=0;
            }
            strcat(response, "]}}\n");
            printf("tweet profile completed\n");
            //fclose(user);
        } else {
            strcpy(response, "{\"type\":\"Error\",\"message\":\"This username is not valid.\"}\n");
            printf("invalid username\n");
            //invalid user
        }
    } else{
        strcpy(response,"{\"type\":\"Error\",\"message\":\"Bad request format.\"}\n");
        printf("bad format\n");
    }
    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
    memset(response,0,strlen(response));
    memset(buffer,0,strlen(buffer));
    memset(details,0,strlen(details));
}
void follow()
{
    char response[100000];
    char detailsfollower[10000];
    char detailsfollowing[10000];
    char newdetails[10000];
    char username[200],useropen[200];
    char tk[33];
    memset(response,0,100000);
    memset(detailsfollower,0,10000);
    memset(detailsfollowing,0,10000);
    memset(username,0,200);
    memset(useropen,0,200);
    memset(tk,0,33);
    int i=7,j;
    for(j=0;j<32;j++)
    {
        tk[j]=buffer[i];
        i++;
    }
    i=41,j=0;
    while(buffer[i]!='\n')
    {
        username[j]=buffer[i];
        i++,j++;
    }
    if(strcmp(tk,token)==0) {
        sprintf(useropen, "%s.user.txt", username);
        FILE *user;
        if (user = fopen(useropen, "r")) {
            while (!feof(user)) {
                fgets(detailsfollower, 10000, user);
            }
            fclose(user);

            i = 0, j = 0;
            while (detailsfollower[i] != '[') {
                newdetails[i] = detailsfollower[i];
                i++;
            }
            newdetails[i] = detailsfollower[i];
            i++;
            j = i;
            if (detailsfollower[i] == ']') {
                strcat(newdetails, "\"");
                strcat(newdetails, us);
                strcat(newdetails, "\"");
                i += strlen(us) + 2;
            } else {
                strcat(newdetails, "\"");
                strcat(newdetails, us);
                strcat(newdetails, "\",");
                i += strlen(us) + 3;
            }
            while (detailsfollower[j] != '\0') {
                newdetails[i] = detailsfollower[j];
                i++, j++;
            }
            user = fopen(useropen, "w");
            fprintf(user, newdetails);
            fclose(user);
            memset(useropen, 0, 200);
            memset(newdetails, 0, 10000);
            sprintf(useropen, "%s.user.txt", us);
            user = fopen(useropen, "r");
            while (!feof(user)) {
                fgets(detailsfollowing, 10000, user);
            }
            fclose(user);
            int i = 0, j = 0, k = 0;
            while (k < 2) {
                newdetails[i] = detailsfollowing[i];
                if (detailsfollowing[i] == '[') {
                    k++;
                }
                i++;
            }
            j = i;
            if (detailsfollowing[i] == ']') {
                strcat(newdetails, "\"");
                strcat(newdetails, username);
                strcat(newdetails, "\"");
                i += strlen(username) + 2;
            } else {
                strcat(newdetails, "\"");
                strcat(newdetails, username);
                strcat(newdetails, "\",");
                i += strlen(username) + 3;
            }
            while (detailsfollowing[j] != '\0') {
                newdetails[i] = detailsfollowing[j];
                i++, j++;
            }
            user = fopen(useropen, "w");
            fprintf(user, newdetails);
            fclose(user);
            strcpy(response, "{\"type\":\"Successful\",\"message\":\"User is followed successfully.\"}\n");
            printf("follow done\n");
        } else
        {
            strcpy(response,"{\"type\":\"Error\",\"message\":\"This username is not valid.\"}\n");
            printf("invalid username\n");
        }
    }
    else
    {
        strcpy(response,"{\"type\":\"Error\",\"message\":\"Bad request format.\"}\n" );
        printf("bad format\n");
    }
    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
    memset(response,0,strlen(response));
    memset(buffer,0,strlen(buffer));
}
void unfollow()
{
    char response[10000];
    char tk[50];
    memset(tk, 0, 49);
    int i;
    int j = 0;
    for (i = 9; i <= 40; i++)
    {
        tk[j] = buffer[i];
        j++;
    }
    tk[32] = 0;
    if(strcmp(token,tk)==0) {
        char username[50];
        memset(username, 0, 49);
        i = 43;
        j = 0;
        char a;
        a = buffer[i];
        while (a != '\n') {
            username[j] = buffer[i];
            i++;
            j++;
            a = buffer[i];
        }
        char Username[50];
        memset(Username, 0, 49);
        strcpy(Username, username);
        strcat(username, ".txt");
        FILE *fusername;
        char file[10000];
        memset(file, 0, 9999);
        char useropen[1000];
        sprintf(useropen, "%s.user.txt", Username);
        if (fusername = fopen(useropen, "r")) {
            fgets(file, 10000, fusername);
            fclose(fusername);
            i = 0;
            while (file[i] != '[') {
                i++;
            }
            i++;
            int z = 0;
            while (file[i] != ']') {
                if (file[i] == ',') {
                    z = 1;
                }
                i++;
            }
            if (z == 0) {
                char array1[5000];
                memset(array1, 0, 4999);
                i = 0;
                j = 0;
                while (file[i] != '[') {
                    array1[j] = file[i];
                    i++;
                    j++;
                }
                strcat(array1, "[");
                //printf("%s",array1);
                char array2[5000];
                memset(array2, 0, 4999);
                j = 0;
                while (file[i] != ']') {
                    i++;
                }
                while (file[i] != '}') {
                    array2[j] = file[i];
                    i++;
                    j++;
                }
                strcat(array2, "}");
                strcat(array1, array2);
                //printf("%s",array1);
                fusername = fopen(useropen, "w");
                fprintf(fusername, "%s", array1);
                fclose(fusername);

            } else if (z == 1) {
                char array1[5000];
                memset(array1, 0, 4999);
                i = 0;
                while (file[i] != '[') {
                    i++;
                }
                i++;
                char follower[50];
                memset(follower, 0, 49);
                char USER[100];
                memset(USER, 0, 99);
                sprintf(USER, "\"%s\"", us);
                int x = 0;
                char array2[5000];
                memset(array2, 0, 4999);
                char array3[5000];
                memset(array3, 0, 4999);
                int number = 0;
                while (file[i] != ']') {
                    j = 0;
                    while (file[i] != ',' && file[i] != ']') {
                        follower[j] = file[i];
                        i++;
                        j++;
                    }
                    if (file[i] == ',') {
                        i++;
                    }
                    if (strcmp(follower, USER) == 0) {
                        if (x == 0) {
                            int p = 0;
                            int l = 0;
                            while (file[p] != '[') {
                                array2[l] = file[p];
                                l++;
                                p++;
                            }
                            l = 0;
                            strcat(array2, "[");
                            while (file[i] != '}') {
                                array3[l] = file[i];
                                i++;
                                l++;
                            }
                            strcat(array3, "}");
                            strcat(array2, array3);
                            //printf("%s",array2);
                            fusername = fopen(useropen, "w");
                            fprintf(fusername, "%s", array2);
                            fclose(fusername);
                            number = 1;
                        } else if (x == 1) {
                            int n = i;
                            int l = 0, p = 0;
                            while (file[i] != '}') {
                                array3[l] = file[i];
                                i++;
                                l++;
                            }
                            strcat(array3, "}");
                            //printf("%s\n",array3);
                            int g = n - strlen(USER) - 2;
                            l = 0;
                            while (l <= g) {
                                array2[p] = file[l];
                                p++;
                                l++;
                            }
                            //printf("%s",array2);
                            strcat(array2, array3);
                            //printf("%s",array2);
                            fusername = fopen(useropen, "w");
                            fprintf(fusername, "%s", array2);
                            fclose(fusername);
                            number = 1;
                        }
                    }
                    if (number == 1) {
                        break;
                    }
                    x = 1;
                    memset(follower, 0, 49);
                }
            }
            char name[50];
            strcpy(name, us);
            char File[10000];
            char useropen1[1000];
            sprintf(useropen1, "%s.user.txt", us);
            memset(File, 0, 9999);
            strcat(name, ".txt");
            fusername = fopen(useropen1, "r");
            fgets(File, 10000, fusername);
            fclose(fusername);
            i = 0;
            while (File[i] != ']') {
                i++;
            }
            i = i + 15;
            int y = i;
            int o = y;
            int h = y + 1;
            int m = 0;
            while (File[i] != ']') {
                if (File[i] == ',') {
                    m = 1;
                }
                i++;
            }
            if (m == 0) {
                char arr[5000];
                memset(arr, 0, 4999);
                char arr1[5000];
                memset(arr1, 0, 4999);
                int v = 0;
                int k = 0;
                while (v <= y) {
                    arr[v] = File[k];
                    v++;
                    k++;
                }
                //printf("%s\n",arr);
                while (File[y] != ']') {
                    y++;
                }
                v = 0;
                while (File[y] != '}') {
                    arr1[v] = File[y];
                    v++;
                    y++;
                }
                strcat(arr1, "}");
                //printf("%s",arr1);
                strcat(arr, arr1);
                //printf("%s", arr);
                fusername = fopen(useropen1, "w");
                fprintf(fusername, "%s", arr);
                fclose(fusername);
            } else if (m == 1) {
                char following[100];
                memset(following, 0, 99);
                char USERNAME[100];
                memset(USERNAME, 0, 99);
                sprintf(USERNAME, "\"%s\"", Username);
                char arr[5000];
                memset(arr, 0, 4999);
                char arr1[5000];
                memset(arr1, 0, 4999);
                int u = 0;
                int num = 0;
                while (File[h] != ']') {
                    j = 0;
                    while (File[h] != ']' && File[h] != ',') {
                        following[j] = File[h];
                        h++;
                        j++;
                    }
                    if (File[h] == ',') {
                        h++;
                    }
                    if (strcmp(following, USERNAME) == 0) {
                        if (u == 0) {
                            int q = 0;
                            int c = 0;
                            while (q <= o) {
                                arr[c] = File[q];
                                c++;
                                q++;
                            }
                            q = 0;
                            while (File[h] != '}') {
                                arr1[q] = File[h];
                                h++;
                                q++;
                            }
                            strcat(arr1, "}");
                            strcat(arr, arr1);
                            //printf("%s",arr);
                            fusername = fopen(useropen1, "w");
                            fprintf(fusername, "%s", arr);
                            fclose(fusername);
                            num = 1;
                        } else if (u == 1) {
                            int re = h;
                            int q = 0;
                            while (File[h] != '}') {
                                arr[q] = File[h];
                                h++;
                                q++;
                            }
                            strcat(arr, "}");
                            int c = re - strlen(USERNAME) - 2;
                            q = 0;
                            int b = 0;
                            while (q <= c) {
                                arr1[b] = File[q];
                                b++;
                                q++;
                            }
                            strcat(arr1, arr);
                            //printf("%s",arr1);
                            num = 1;
                            fusername = fopen(useropen1, "w");
                            fprintf(fusername, "%s", arr1);
                            fclose(fusername);
                        }
                    }
                    if (num == 1) {
                        break;
                    }
                    u = 1;
                    memset(following, 0, 99);

                }
            }
            strcpy(response, "{\"type\":\"Successful\",\"message\":\"User is unfollowed successfully.\"}\n");
            send(client_socket, response, strlen(response), 0);
            printf("unfollow done\n");
        }
            else
            {
                strcpy(response,"{\"type\":\"Error\",\"message\":\"This username is not valid.\"}\n");
                printf("invalid username\n");
                send(client_socket, response, strlen(response), 0);
            }

    }
    else
    {
        strcpy(response, "{\"type\":\"Error\",\"message\":\"Bad request format.\"}\n");
        send(client_socket, response, strlen(response), 0);
        printf("bad format\n");
    }
}
void logout()
{
    char response[1000];
    char tk[33];
    memset(tk,0,33);
    memset(response,0,1000);
    int i=7,j;
    for(j=0;j<32;j++)
    {
        tk[j]=buffer[i];
        i++;
    }
    if(strcmp(tk,token)==0)
    {
        memset(token,0,strlen(token));
        memset(us,0,strlen(us));
        memset(passtrue,0,strlen(passtrue));
        strcpy(response,"{\"type\":\"Successful\",\"message\":\"\"}");
        printf("logout done\n");
    }
    else
    {
        strcpy(response,"{\"type\":\"Error\",\"message\":\"Bad request format.\"}\n" );
        printf("bad format\n");
    }
    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
    memset(response,0,strlen(response));
    memset(buffer,0,strlen(buffer));
}
char *randstring(int length)
{
    static int mySeed = 25011984;
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-#'?!";
    size_t stringLen = strlen(string);
    char *randomString = NULL;

    srand(time(NULL) * length + ++mySeed);

    if (length < 1) {
        length = 1;
    }

    randomString = malloc(sizeof(char) * (length +1));

    if (randomString) {
        short key = 0;

        for (int n = 0;n < length;n++) {
            key = rand() % stringLen;
            randomString[n] = string[key];
        }

        randomString[length] = '\0';

        return randomString;
    }
    else {
        printf("No memory");
        exit(1);
    }
}