#define PORT 8888
#define LISTENQ 256
#define MAXSIZE 2048

#define DB_FILE "../chatroom.db"

typedef struct User{
	int MsgType;                  //消息类型
	/*1:连接; 2:登陆; 3:注册; 4:修改密码; 5:找回密码; 6:私聊; 7:显示在线用户*/
	int result;                   //操作结果：0:失败;  1:成功
	int From_sockfd;              //消息来源套接字
	int To_sockfd;                //发送目标套接字
	char From_UseName[21];        //登陆用户名
	char password[21];            //密码
	char To_UseName[21];          //传输用户名
	char MsgBuff[MAXSIZE-50];     //消息内容
}USER;
