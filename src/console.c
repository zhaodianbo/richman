�?include <string.h>
#include "console.h"
#include "rich_client.h"
char cmd_line[LINE_MAX];
char cmd[LINE_MAX];
void get_cmd_line(void);
void get_cmd();
int get_num_para();
int cmd_go(int n, char *c[]);
int cmd_help(int n, char *c[]);
int cmd_exitgame(int n, char *c[]);
int cmd_player_info(int n,char *c[]);
int cmd_player_quit(int n,char * c[]);
int cmd_roll(int n,char * c[]);
cmd_tbl_t *find_cmd_tbl (const char *cmd, cmd_tbl_t *table, int table_len);
int cmd_use_tool(int n, char *c[]);

st_game *g_game;
int sockfd=-1;

//所有的cmd函数在这里注�?
cmd_tbl_t cmd_tb[CMD_MAX]={{"go",cmd_go},
						   {"help",cmd_help},
						   {"exitgame",cmd_exitgame},
						   {"quitgame",cmd_player_quit},
						   {"info",cmd_player_info},
						   {"roll",cmd_roll},
						   {"tool",cmd_use_tool},
				   		  };
//没有用到
#if 0
land_ftbl_t land_ftb[MAX_LAND_FUN]={{Na,buy_land},
									{Owner,buy_land},
									{Other,pay_rent},
									{Toolshop,NULL},
								   }
#endif

int cmd_use_tool(int n, char *c[])
{
	use_tool(g_game);
	return 0;
}

int cmd_roll(int n,char * c [ ])
{
	cmd_go(six_rand(),NULL);
	return 0;
}
int cmd_go(int n, char *c[])
{
	n=explore_block(g_game,n);
	printf("%s go %d steps.\n",get_p_name(g_game),n);
	
	update_map(g_game,n);				//更新地图 改变玩家位置信息
	print_game_map(g_game);
	int step=get_p_step(g_game);		//获取玩家land上的位置
	st_land_info *land=get_game_land(g_game,step);	//得到第pos个地�?
	e_LandOwnerType ownertype=whose_land(g_game,land);
	
	switch(ownertype)
	{
		case Na:
			buy_land(g_game,land); 
			break;
		case Owner:
			upgrade_house(g_game,land);
			break;
		case Other:
			pay_rent(g_game,land);
			break;
		case Toolshop:
			shop(g_game);
			break;
		default:
			printf("something wrong with LandOwnerType\n");
	}

	next_player(g_game);
	return 0;
}
int cmd_help(int n, char *c[])
{
	printf("cmd summary(support shortcuts)\n");
	printf("roll: go random 1-6 steps.\n");
	printf("go N: go N steps.\n");
	printf("tool: display and use tool\n");
	printf("info: query player N's information.\n");
	printf("quitgame: player quit game.\n");
	printf("exitgame: end the game.\n");
	return 0;
}

int cmd_exitgame(int n, char *c[])
{
//做好回收工作
	deinit_game(&g_game);

	exit(0);
}

int cmd_player_quit(int n,char * c [ ])
{
	player_quit(g_game,get_p_id(g_game));
	char buf[MAXLINE];

	if(sockfd!=-1 )
	{
		if(is_myturn(g_game))
		{
			next_player(g_game);
			exit(0);
		}
		is_notice(sockfd);
		download_cmd(sockfd,buf);
	}
	return 0;
}

int cmd_player_info(int n,char *c[])
{
	int i,tmp;
	int player_tb[MAX_PLAYER_NUM];
	st_player *player;
	int player_num=get_game_pnum(g_game);
	printf("please enter the id that you want to query\n");
	//打印玩家名和id�?
	for(i=0 ; i < player_num; i++)
	{
		player=get_playern(g_game,i);
		player_tb[i]=get_player_id(player);
		print_color_str(get_player_name(player),get_player_color(player));
		printf(":id=%d ",player_tb[i]);
	}
	printf("\nall:9\n");
	tmp=get_num();
	if(tmp==9)
		print_p_info(g_game,9);
	else
		for(i=0;i<player_num;i++)		//输入id在表中可找到
			if(player_tb[i] == tmp)		//tmp为玩家id 查看是否有该玩家
				print_p_info(g_game,i);	//传入的是player table中的索引
	return 0;
}

int cmd_land_info(int n,char *c[])
{
	return 0;
}
//获取一行的内容
void get_cmd_line(void)
{
	if(sockfd !=-1)
	{
		int turn=client_run(sockfd,cmd_line);
		set_game_turn(g_game,turn);
	}
	else
	{
		#if 1	
		int n=0;
		memset(cmd_line,0,sizeof(cmd_line));
		while(((cmd_line[n]=getc(stdin))!=EOF) && (cmd_line[n]!='\n') && ((++n) < LINE_MAX) );
		cmd_line[n]='\0';	//去掉换行符号
	}
		#endif
	//while(cmd_line[n]!='\n' && n++ < LINE_MAX);
	//	cmd_line[n]='\0';
	//cmd_line[strlen(cmd_line)-1]='\0';
	//fgets(cmd_line,LINE_MAX,stdin);	//换行符号也会保存下来: str\n\0
#ifdef AUTO_TEST
	printf("%s\n",cmd_line);

#endif
}

//在一行输入中获取cmd命令: 第一个字符串
void get_cmd()
{
	memset(cmd,0,sizeof(cmd));
	char *p=cmd_line;
	while(*p!=' ' && *p!='\0') p++;	
	memcpy(cmd,cmd_line,p-cmd_line);
}

int get_num_para()
{
	char *str=cmd_line;
	int ret=0;
	while(*str!='\0' && !is_digtial(*str)) str++;
	ret=str2int(str);
	if(str!=cmd_line && *(str-1)=='-') 
		return -ret;
	return ret;
}

//根据终端获取到的字符串找到对应的cmd 支持不完全匹�?
/*
* cmd : 字符串首地址
* table : 所查找的table
* table_len : table中cmd的个�?
*/
cmd_tbl_t *find_cmd_tbl (const char *cmd, cmd_tbl_t *table, int table_len)
{	
	cmd_tbl_t *cmdtp;	
	cmd_tbl_t *cmdtp_temp = table;	/*Init value */	
	const char *p;	
	int len;	
	int n_found = 0;	
	/*	 
	* Some commands allow length modifiers (like "cp.b");	 
	* compare command name only until first dot.	 */	
	len = ((p = strchr(cmd, '.')) == NULL) ? strlen (cmd) : (p - cmd);	
	for (cmdtp = table;cmdtp != table + table_len;cmdtp++) 
	{		
		if (cmdtp->name!=NULL && strncmp (cmd, cmdtp->name, len) == 0) 
		{			
			if (len == strlen (cmdtp->name))				
				return cmdtp;	/* full match */			
			cmdtp_temp = cmdtp;	/* abbreviated command ? */			
			n_found++;		
			}	
		}	
	if (n_found == 1) 
	{			/* exactly one match */	
		return cmdtp_temp;	
	}
	
	if(cmd[0]!='\0')
		printf("cmd: '%s' not found, try cmd \"help\"?\n",cmd);			
	
	return NULL;	/* not found or ambiguous command */
}

void playing()
{
start:
	update_map(g_game,0);
	print_game_map(g_game);
	print_p_info(g_game,get_game_id(g_game));
	print_name(g_game);
	print_color('>',get_p_color(g_game));
	get_cmd_line();
	get_cmd();
	cmd_tbl_t *do_cmd=find_cmd_tbl(cmd,cmd_tb,CMD_MAX);
	if(do_cmd==NULL) goto start;
	do_cmd->cmd(get_num_para(),NULL);

}

int ParseCmdLine(  int argc , char *argv[] )
{
	int i;
	for( i=0;i<argc;i++)
	{
		 if( strstr( argv[i] , "-server" ) != NULL ||
			 strstr( argv[i] , "-s" ) != NULL )
			{
		 		
			 	return NET_SERVER;
			}
		 if( strstr( argv[i] , "-client" ) != NULL ||
			 strstr( argv[i] , "-c" ) != NULL )
			{
			 	return NET_CLIENT;
			}
		 if( strstr( argv[i] , "-help" ) != NULL ||
			 strstr( argv[i] , "-h" ) != NULL )
			{
			 	printf("net:\n");
				printf("   -server:run as server\n");
				printf("   -client:run as client\n");
				printf("default:local run\n");
				return LOCAL;
				
			}
			
	}
	return LOCAL;
	
}

int init(int net_flag)
{
#define PLAYER_NUM 2
#define INPUT_PLAYER_NUM
	
	int PlayerNum=PLAYER_NUM;
#ifdef INPUT_PLAYER_NUM
	if(net_flag != NET_CLIENT)
	{
		printf("pls enter the player number(2-4): default 2\n");
		scanf("%d",&PlayerNum);
		while(getchar()!='\n');
		if(PlayerNum>4||PlayerNum<2) PlayerNum=PLAYER_NUM;
	}
#endif
	int getServPlayNum=PlayerNum;

	if(net_flag==LOCAL)	goto initgame;

	 //apue中有TELL_WAIT等函数解决进程间同步问题
	if(net_flag==NET_SERVER)
	{		 
		pid_t pid;
		TELL_WAIT();
		if((pid=fork()) < 0 ) 
			cmd_exitgame(0,NULL);
		else if(pid==0)//child
		{	
			
			//TELL_PARENT(getppid());
			execl("rich_server.out","rich_server.out",&PlayerNum,NULL);
			printf("server fork error\n");
			exit(-1);
		}
		else
		{
			WAIT_CHILD();
		}
		
	}	

	sockfd=init_client(&getServPlayNum);
	if(sockfd<0)
	{
		//kill(pid,SIGKILL);
		return -1;
	}

initgame:	
	if((g_game=init_game(getServPlayNum))==NULL) 
		cmd_exitgame(0,NULL);


	return 0;
	
}


#if 0
int main(void)
{
#define PLAYER_NUM 4
	g_game=init_game(PLAYER_NUM);
	if(g_game==NULL) return 1;
	//(p->p_land[LAND_NUM-5]).color=Red;
	//set_land_color(&(g_game->p_land[LAND_NUM-5]),Red);
	print_game_map(g_game);
	update_map(g_game,0);
	print_game_map(g_game);

	while(1)
	{
		print_name(g_game);
		print_color('>',get_p_color(g_game));
		get_cmd_line();
		get_cmd();
		cmd_tbl_t *do_cmd=find_cmd_tbl(cmd,cmd_tb,CMD_MAX);
		
		if(do_cmd==NULL) 
		{
			printf("cmd: '%s' not found, try cmd \"help\"?\n",cmd);
			continue;
		}
		do_cmd->cmd(get_num_para(),NULL);
		update_map(g_game,0);
		print_game_map(g_game);
		//printf("%s\n",cmd_line);
		//printf("%s\n",cmd);
	}
}

#endif
