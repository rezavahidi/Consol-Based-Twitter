#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "slre.h"
#include <winsock2.h>
void signup();
void login();
int main();
void menu();
void timeline();
void search();
void tweetprofiles();
void personalarea();
void logout();
void sendtweet();
void ref();
void like();
void comment();
void seecomment();
void setbio();
void changepass();
void mainmenu(void);
int initialize(int);
char *send_data(char *);
struct sockaddr_in server;
int m_index = 0;
char token[100000];
int main() {
    int port = 12345;
    int can_initialize = initialize(port);
    while (can_initialize == 0) {
       menu();
        }
    {
        main();
    }
        return 0;
}
int initialize(int port)
{
    WSADATA wsadata;
    int wsaStartUp = WSAStartup(MAKEWORD(2,2),&wsadata);
    if (wsaStartUp != 0){
        printf("Error");
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    return 0;
}
char *send_data(char *data)
{
   char *buffer = malloc(1000) ;
   memset(buffer,0,1000);
   int client_socket = socket(AF_INET,SOCK_STREAM, 0);
   if (client_socket== INVALID_SOCKET)
   {
       return buffer;
   }
    int can_connect = connect(client_socket, (struct sockaddr *) &server, sizeof(server));
   if (can_connect!=0)
   {
       return buffer;
   }
   send(client_socket, data, strlen(data),0);
   recv(client_socket, buffer, 999, 0);
   closesocket(client_socket);
   return buffer;
}

void menu() {
    printf("welcome!\n");
    while(1){
    int num;

    printf("1. sign up\n");
    printf("2. log in\n");
    printf("3. exit\n");
    printf("enter your selected number:");
    scanf("%d", &num);
    switch (num) {
        case 1:
            signup();
            break;
        case 2:
            login();
            break;
        case 3:
            return;
    }
    if(num!=1 && num!=2 && num!=3)
    {
        printf("wrong number,try again\n");
    }
}
}
void signup(void) {
    //int num_caps = 1;
    //char result[num_caps][100000];
    //char recive[100000];
    char username[100000];
    char password[100000];
    printf("enter username:(your username consists of capital alphabets,small alphabets and digits)\n");
    scanf("%s", &username);
    char sign[100000] = "signup ";
    strcat(sign, username);
    strcat(sign,", ");
    printf("enter password:(your password consists of capital alphabets,small alphabets and digits)\n");
    scanf("%s", &password);
    strcat(sign,password);
    strcat(sign,"\n");
    char *rec = send_data(sign);
    char t[100000];
    strcpy(t,rec);
    //char rejex[100000] = "{\"type\":\"Error\",\"message\":\"([\\w\\W\\S]+)\"}\\n";
    //printf("regex: %s",rejex);
    //slre_match(rejex, t, 100000, result, num_caps, 0);
    char c[100000];
       strcpy(c,"{\"type\":\"Error\",\"message\":\"This username is already taken.\"}\n");
    if(strcmp(t,c) ==0)
    {
        printf("This username is already taken\ntry again!\n");
        return;
    }
    else{
        int a;
    printf("\nsignup done!\nenter 1 to continue\n");

    scanf("%d", &a);
    return;}
}
void login(void)
{
    char username[100000];
    char password[100000];
    printf("enter username:\n");
    scanf("%s", &username);
    char log[100000] = "login ";
    strcat(log, username);
    strcat(log,", ");
    printf("enter password:\n");
    scanf("%s", &password);
    strcat(log,password);
    strcat(log,"\n");
    char *rec = send_data(log);
    char t[100000];
    strcpy(t,rec);
    char c[100000];
    strcpy(c,"{\"type\":\"Error\",\"message\":\"Incorrect password.\"}\n");
    if(strcmp(t,c) ==0)
    {
        printf("Invalid password\ntry again!\n");
        return;
    }
    strcpy(c,"{\"type\":\"Error\",\"message\":\"This username is not valid.\"}\n");
    if(strcmp(t,c) ==0)
    {
        printf("Invalid usernamne\ntry again!\n");
        return;
    }
    char z=t[9];
    if(z=='E')
    {
        printf("ERROR!you have already logged in,plz close the app and run it again\n");
        return;
    }

//        char regex[] = "{\"type\":\"Token\",\"message\":\"([a-zA-Z0-9\\_\\-]+)\"}\n";
        //int num_caps = 1;
        //char caps[num_caps][100000];
        char message[100000];
        strcpy(message,rec);
//        printf("%s%s",message,regex);
//        slre_match(regex, message, 100000, caps, num_caps, 0);
        int i,j=27;
        for(i=0;i<32;i++)
        {
            token[i] = message [j];
            j++;
        }
        //printf("your token is: %s\n",token);
        int a;
        printf("\nlogin done!\nenter 1 to continue\n");
        scanf("%d", &a);
        mainmenu();

}//
void mainmenu()
{
    while(1)
    {
    int num;
    printf("1.Timeline\n2.Search\n3.Tweet Profiles\n4.Personal area\n5.Log out\nEnter your number:\n");
    scanf("%d",&num);
    switch(num) {
        case 1:
            timeline();
            break;
        case 2:
            search();
            break;
        case 4:
            tweetprofiles();
            break;
        case 3:
            personalarea();
            break;
        case 5:
            logout();
            return;}
    }
printf("invalid number!\ntryagain\n");
    mainmenu();
}
void timeline() {

    while (1){
        int num;
    printf("1.send tweet\n2.refresh\n3.main menu\nenter your number:\n");
    scanf("%d", &num);
    switch (num) {
        case 1:
            sendtweet();
            break;
        case 2:
            ref();
            break;
        case 3:
            return;
    }
}
}
void sendtweet()
{
    int num;
    printf("enter 1 and then type your tweet\n");
    scanf("%d\n",&num);
    char Tweet[100000];
    scanf("%[^\n]s",Tweet);
    //fgets(Tweet,100000,stdin);
    char tweet[100000];
    strcpy(tweet,Tweet);
    char sendtweet[100000]; //= "send tweet ";
//    strcat(sendtweet,token);
//    strcat(sendtweet,", ");
//    strcat(sendtweet,tweet);
//    strcat(sendtweet,"\n");
    sprintf(sendtweet,"send tweet %s, %s\n",token,tweet);
    char *response = send_data(sendtweet);
    char m=response[32];
    for(int i=32;m!='"';i++)
    {
        printf("%c",m);
        m = response[i+1];
    }
    printf("\n");
        return;
}
void ref()
{
    char refresh[100000] = "refresh ";
    strcat(refresh,token);
    strcat(refresh,"\n");
    char *recv = send_data(refresh);
    char rec[100000];
    strcpy(rec,recv);
    char ID[100000], author[100000], content[100000], comments[100000] = " ", likes[100000];
    long long i=24,j=0;
    char m = rec[i+2];
    char n = m;
    while (n != ']') {
        i += 8;
        j = 0;
        m = rec[i];
        while (m != ',') {
            ID[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        i += (11);
        j = 0;
        m = rec[i];
        while (m != '"') {
            author[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        printf("%s\t%s\n", author, ID);
        i += (13);
        j = 0;
        m = rec[i];
        while (m != '"') {
            content[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        i += (14);
        printf("%s\n", content);
        j = 0;
        m = rec[i];
        int cm=0,cmm=0;
        while (m != '}') {
            comments[j] = rec[i];
            j++, i++;
            m = rec[i];
            cmm=1;
            if(m==',')
            {
                cm++;
            }
        }
        if(j==0){strcpy(comments," ");}
        i += (10);
        printf("comments :%d\t",cm+cmm);
//        while (m != '}') {
//            comments[j] = rec[i];
//            j++, i++;
//            m = rec[i];
//        }
//        if(j==0){strcpy(comments," ");}
//        i += (10);
//        printf("comments: %s\t", comments);
        j = 0;
        m = rec[i];
        while (m != '}') {
            likes[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        printf("likes: %s\n\n", likes);
        m = rec[i + 1];
        n = m;
        //strcpy(ID," "); strcpy(author," ");strcpy(content," ");strcpy(comments," ");strcpy(likes," ");
        memset(ID,0,100000);memset(author,0,100000);memset(content,0,100000);memset(likes,0,100000);
        memset(comments,0,100000);
    }
    while(1) {
    printf("1. like\n2. comment\n3. see comments\n4.back\nenter your number: \n");
    int num;
        scanf("%d", &num);
        switch (num) {
            case 1:
                like();
                break;
            case 2:
                comment();
                break;
            case 3:
                seecomment();
                break;
            case 4:
                return;
        }
    }
}
void like()
{
    int like;
    char lke[10000];
    printf("enter tweet number:\n");
    scanf("%d",&like);
    sprintf(lke,"like %s, %d\n",token,like);
    send_data(lke);
    printf("tweet was liked!\n");
    return;
}
void comment()
{
    int comment;
    char cm[100000],content[100000];
    printf("enter tweet number and then enter your comment:\n");
    scanf("%d\n",&comment);
    scanf("%[^\n]s",content);
    sprintf(cm,"comment %s, %d, %s\n",token,comment,content);
    send_data(cm);
    printf("comment submited!\n");
    return;
}
void seecomment()
{

}
void search() {
    printf("enter id you want to search:\n");
    char id[100000];
    scanf("%s", &id);
    char Search[100000] = "search ";
    strcat(Search, token);
    strcat(Search, ", ");
    strcat(Search, id);
    strcat(Search, "\n");
    char *recv = send_data(Search);
    char rec[100000];
    strcpy(rec, recv);
    char m, n, username[100000], USERNAME[100000], bio[100000] = " ", followers[100000], followings[100000], statue[100000];
    char ID[100000], author[100000], content[100000], comments[100000] = " ", likes[100000];
    int i = 27, j;
    for (j = 0; j < 27; j++) {
        USERNAME[j] = rec[i];
        i++;
    }
    if (strcmp(USERNAME, "This username is not valid.") == 0) {
        printf("This username is not valid.\n");
    } else {
        i = 41, j = 0;
        m = rec[i];
        while (m != '"') {
            username[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        i += (9);
        printf("username: %s\n", username);
        j = 0;
        m = rec[i];
        while (m != '"') {
            bio[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        i += (22);
        printf("bio: %s\n", bio);
        j = 0;
        m = rec[i];
        while (m != ',') {
            followers[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        i += (22);
        printf("followers: %s\t", followers);
        j = 0;
        m = rec[i];
        while (m != ',') {
            followings[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        i += (17);
        printf("followings: %s\n", followings);
        j = 0;
        m = rec[i];
        while (m != '"') {
            statue[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        printf("%s\n\n\n", statue);
        i += (13);
        j = 0;
        m = rec[i + 2];
        n = m;
        while (n != ']') {
            i += 8;
            j = 0;
            m = rec[i];
            while (m != ',') {
                ID[j] = rec[i];
                j++, i++;
                m = rec[i];
            }
            i += (11);
            j = 0;
            m = rec[i];
            while (m != '"') {
                author[j] = rec[i];
                j++, i++;
                m = rec[i];
            }
            i += (13);
            j = 0;
            m = rec[i];
            while (m != '"') {
                content[j] = rec[i];
                j++, i++;
                m = rec[i];
            }
            i += (14);
            printf("%s\n", content);
            j = 0;
            m = rec[i];
            int cm=0,cmm=0;
            while (m != '}') {
                comments[j] = rec[i];
                j++, i++;
                m = rec[i];
                cmm=1;
                if(m==',')
                {
                    cm++;
                }
            }
            if(j==0){strcpy(comments," ");}
            i += (10);
            //printf("comments: %s\t", comments);
            printf("comments :%d\t",cm+cmm);
            j = 0;
            m = rec[i];
            while (m != '}') {
                likes[j] = rec[i];
                j++, i++;
                m = rec[i];
            }
            printf("likes: %s\n\n", likes);
            m = rec[i + 1];
            n = m;
            //strcpy(ID,""); strcpy(author,"");strcpy(content,"");strcpy(comments," ");strcpy(likes,"");
            memset(ID,0,100000);memset(author,0,100000);memset(content,0,100000);memset(likes,0,100000);
            memset(comments,0,100000);
        }
        int x;
        if (strcmp(statue, "NotFollowed") == 0) {
            printf("Enter 1 to follow or 0 to continue: \n");
            scanf("%d", &x);
            if (x == 1) {
                char follow[10000] = "follow ";
                strcat(follow, token);
                strcat(follow, ", ");
                strcat(follow, username);
                strcat(follow,"\n");
                send_data(follow);
            }

        } else {
            printf("Enter 1 to unfollow or 0 to continue: \n");
            scanf("%d", &x);
            if (x == 1) {
                char follow[10000] = "unfollow ";
                strcat(follow, token);
                strcat(follow, ", ");
                strcat(follow, username);
                strcat(follow,"\n");
                send_data(follow);
            }
        }

    }
    int y;
    printf("1.back to main menu\nenter your selected number: \n");
    scanf("%d",&y);
}
void tweetprofiles()
{
    while(1) {
        int num, a;
        printf("1. set bio\n2. change password\n3. back\nenter your selected number:\n");
        scanf("%d", &num);
        switch (num) {
            case 1:
                printf("enter 1 and then enter your bio:\n");
                scanf("%d\n", &a);
                setbio();
                break;
            case 2:
                changepass();
                break;
            case 3:
                return;
        }
    }
}
void setbio()
{
    char bio[100000],Bio[100000];
    scanf("%[^\n]s ",bio);
    sprintf(Bio,"set bio %s, %s\n",token,bio);
    char *res = send_data(Bio);
    char m = res[9];
    if(m=='E')
    {
        printf("you have set your bio before!\n");
    }
    else
    {
        printf("bio has been set\n");
    }
    //mainmenu();
}
void changepass()
{
    char pass[100000],newpass[100000],chngpass[100000];
    printf("enter your current password:\n");
    scanf("%s",pass);
    printf("enter your new password:\n");
    scanf("%s",newpass);
    sprintf(chngpass,"change password %s, %s, %s\n",token,pass,newpass);
    char *res = send_data(chngpass);
    char m = res[27];
    if(m=='E')
    {
        printf("wrong password!\n");
    }
    else
    {
        printf("password was changed\n");
    }
    //mainmenu();
}
void personalarea() {
    int a;
    printf("enter 1 to continue:\n");
    scanf("%d",&a);
    char profile[100];
    sprintf(profile, "profile %s\n",token);
    char *recv = send_data(profile);
    char rec[100000];
    strcpy(rec, recv);
    char m, n, username[100000], USERNAME[100000], bio[100000] = " ", followers[100000], followings[100000], statue[100000];
    char ID[100000], author[100000], content[100000], comments[100000] = " ", likes[100000];
    int i = 27, j;
    i = 41, j = 0;
    m = rec[i];
    while (m != '"') {
        username[j] = rec[i];
        j++, i++;
        m = rec[i];
    }
    i += (9);
    printf("username: %s\n", username);
    j = 0;
    m = rec[i];
    while (m != '"') {
        bio[j] = rec[i];
        j++, i++;
        m = rec[i];
    }
    i += (22);
    printf("bio: %s\n", bio);
    j = 0;
    m = rec[i];
    while (m != ',') {
        followers[j] = rec[i];
        j++, i++;
        m = rec[i];
    }
    i += (22);
    printf("followers: %s\t", followers);
    j = 0;
    m = rec[i];
    while (m != ',') {
        followings[j] = rec[i];
        j++, i++;
        m = rec[i];
    }
    i += (17);
    printf("followings: %s\n", followings);
    j = 0;
    m = rec[i];
    while (m != '"') {
        statue[j] = rec[i];
        j++, i++;
        m = rec[i];
    }
    printf("%s\n\n\n", statue);
    i += (13);
    j = 0;
    m = rec[i + 2];
    n = m;
    while (n != ']') {
        i += 8;
        j = 0;
        m = rec[i];
        while (m != ',') {
            ID[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        i += (11);
        j = 0;
        m = rec[i];
        while (m != '"') {
            author[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        i += (13);
        j = 0;
        m = rec[i];
        while (m != '"') {
            content[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        i += (14);
        printf("%s\n", content);
        j = 0;
        m = rec[i];
        int cm = 0, cmm = 0;
        while (m != '}') {
            comments[j] = rec[i];
            j++, i++;
            m = rec[i];
            cmm = 1;
            if (m == ',') {
                cm++;
            }
        }
        if (j == 0) { strcpy(comments, " "); }
        i += (10);
        //printf("comments: %s\t", comments);
        printf("comments :%d\t", cm + cmm);
        j = 0;
        m = rec[i];
        while (m != '}') {
            likes[j] = rec[i];
            j++, i++;
            m = rec[i];
        }
        printf("likes: %s\n\n", likes);
        m = rec[i + 1];
        n = m;
        //strcpy(ID,""); strcpy(author,"");strcpy(content,"");strcpy(comments," ");strcpy(likes,"");
        memset(ID, 0, 100000);
        memset(author, 0, 100000);
        memset(content, 0, 100000);
        memset(likes, 0, 100000);
        memset(comments, 0, 100000);
    }
    //mainmenu();
}

void logout()
{
    int a;
    printf("enter 1 to continue:\n");
    scanf("%d",&a);
    char logout[10000];
    sprintf(logout,"logout %s\n",token);
    send_data(logout);
    //menu();
}