#include"funtion.h"

void user_register(USER *usermsg)
{
	sqlite3 *db;
	int ret;
	char sql[100];
	sqlite3_stmt *stmt;
	const char *tail;
	
	/*打开数据库*/
	ret = sqlite3_open(DB_FILE, &db);
	if(ret != SQLITE_OK)
	{
		printf("unable open database.\n");
		usermsg->result = 0;
		sprintf(usermsg->MsgBuff,"%s","服务器数据库出错");
		return ;
	}
	/*检查要注册的用户名是否已存在*/
	memset(sql , 0 , sizeof(sql));
	sprintf(sql , "select * from User where username='%s';",(usermsg->From_UseName));
	ret = sqlite3_prepare(db , sql , strlen(sql) , &stmt , &tail);	
	if(ret != SQLITE_OK)
	{
		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		//printf("database select fail!\n");
		usermsg->result = 0;
		printf("服务器数据库出错\n");
		sprintf(usermsg->MsgBuff,"%s","服务器数据库出错");
		return ;
	}
	/*执行*/
	ret = sqlite3_step(stmt);
	//如果有数据则返回SQLITE_ROW，当到达末尾返回SQLITE_DONE
	if(ret == SQLITE_ROW)
	{
		//ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		
		usermsg->result = 0;
		printf("用户名已存在\n");
		sprintf(usermsg->MsgBuff,"%s","用户名已存在");
		return ;
	}
	/*销毁句柄*/
	sqlite3_finalize(stmt);
	
	/*执行插入操作*/
	memset(sql , 0 , sizeof(sql));
	sprintf(sql , "insert into User(username, password, quesion, answer) \
	               values(%s,%s,%s,%s);",usermsg->From_UseName, usermsg->password, "", "");

	ret = sqlite3_prepare(db , sql , strlen(sql) , &stmt , &tail);	
	if(ret != SQLITE_OK)
	{
		//这里出错
		perror("sqlite3_prepare error");
		
		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		
		usermsg->result = 0;
		printf("注册失败\n");
		sprintf(usermsg->MsgBuff,"%s","注册失败");
		return ;
	}
	
	/*注册成功*/
	printf("注册成功\n");
	ret = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	usermsg->result = 1;
	sprintf(usermsg->MsgBuff,"%s","注册成功");
	
}

void user_login(USER *usermsg)
{
	sqlite3 *db;
	int ret;
	char sql[100];
	sqlite3_stmt *stmt;
	const char *tail;
	
	/*打开数据库*/
	ret = sqlite3_open(DB_FILE, &db);
	if(ret != SQLITE_OK)
	{
		printf("unable open database.\n");
		usermsg->result = 0;
		sprintf(usermsg->MsgBuff,"%s","服务器数据库出错");
		return ;
	}
	/*检查用户名和密码*/
	memset(sql , 0 , sizeof(sql));
	sprintf(sql , "select * from User where username='%s' and password='%s';",usermsg->From_UseName , usermsg->password);
	ret = sqlite3_prepare(db , sql , strlen(sql) , &stmt , &tail);	
	if(ret != SQLITE_OK)
	{
		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		//printf("database select fail!\n");
		usermsg->result = 0;
		sprintf(usermsg->MsgBuff,"%s","服务器数据库出错");
		return ;
	}
	/*执行*/
	ret = sqlite3_step(stmt);
	//如果有数据则返回SQLITE_ROW，当到达末尾返回SQLITE_DONE
	if(ret != SQLITE_ROW)
	{
		//ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		
		usermsg->result = 0;
		sprintf(usermsg->MsgBuff,"%s","用户名或密码不正确");
		return ;
	}
	
	/*登陆成功*/
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	usermsg->result = 1;
	sprintf(usermsg->MsgBuff,"%s","登陆成功");
	
}
