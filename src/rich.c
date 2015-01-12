/*  linux终端下C语言编程练习 大富翁游戏 可参考rich_jpeg.jpg中的数据结构 
 *  在src目录下执行make, 执行./rich 运行游戏  网络版运行方式:打开两个终端，在终端1
 *  下的src目录下执行./rich -s ,在终端2下的src目录下执行./rich -c,终端1首先运行,然后
 *  终端2运行。
 *
 *
 *  V0.1 完成一位玩家的行走，在land中用symbol保存临时的显示符号 打印地图时首先判断
 *       有无临时显示的元素。2012.11.22
 *  V0.2 修改update_map函数，完成4人切换显示 在地图上显示玩家符号颜色，用于每次刷
 *       新land中的显示符号，程序框架基本确定 2012.11.22
 *  V0.3 添加查询玩家信息函数player_info, 添加game中id及player_num相关函数，修改    
 *  		update_map中的玩家数量的bug, 添加buy_land函数。2012.11.23
 *  V0.4 添加单一玩家退出函数player_quit(),修正打印玩家信息时需要的id为玩家自身id，
 *  		修改了deinit_game()中i的范围bug。
 *  V0.5 在land中添加土地所有者的颜色owner_color,修改init_land(),print_game_map()
 *		 修正玩家退出后仍显示符号的bug,修改print_game_map()中land显示bug。添加主函
 *		 数文件rich.c。修改print_game_map()，使其在终端的中间打印地图。2012.11.25
 *  V0.6 重新在util中写了print_bar代替了welcome(). 删除main中多余代码。
 *  V0.7 添加roll函数
 *  V0.8 重写cmd_go中的if-else为switch，暂时没想到更好的方式处理，静态表或树结构�
 *  		添加upgrade_house函数。简化main函数，好像没啥好处。添加print_p_info 可查看
 *       玩家拥有的土地数量。修改退出时的显示
 *  V0.9 添加tool相关功能，为玩家添加tool字段，购买道具，修改print_p_info的bug，重写
 *		 init_land();  2012.11.30
 *  V1.0 添加道具使用相关结构，和cmd的执行类似
 *  V1.1 添加路障Block的使用和遇到Block的操作2012.12.04
 *  V1.2 修改print_color函数，见tuil.h。添加道具使用的空函数，防止段错误
 *  V1.3 修改 window_col()函数中获取标准输出的终端宽度，以防止输入重定向后不能得到标准
 *       输入的宽度的bug，测试用的input.txt文件中，若单行只有一个命令，后面要跟个空格，
 *       否则读的命令不正确。测试方式 ./rich.out < input.txt  重定向输出会错误 同样是
 *       window_col()引起的，可#undef LINUX_CONSOLE进行输出重定向，使用diff对测试结果比
 *       较。bug:Block不能在有玩家的地块上使用。
 *  V1.4 重写six_rand()函数。      
 *  V1.5 添加网络相关函数，在/net目录下，./a.out是服务器，首先运行，然后分别在另外两个
 *		 终端下执行/src目录下的./rich.out -n，实现两位玩家的互动 ,实现客户端主要修改的
 *		 函数有:get_cmd_line();get_num();next_player(发送字符x);init();is_yes();
 *		 添加全局变量sockfd, game结构体中添加turn标志位代表是否轮到自己。2012.12.14
 *	V1.6 重写makefile，以同时make程序和服务器，写法还是不标准。启动服务器的方式:在init
 *		 函数中使用fork和信号。ParseCmdLine函数确定程序的启动方式:
 *		 	-server 服务器 -clinet 客户端 默认本地运行  
 *		 在init中#define PLAYER_NUM 2设置玩家数量，同时修改rich_server.c中的wait_client(2);
 *		 运行时终端数量也要对应增加		2012.12.21
 *	V1.7 修改4人时退出的bug，在rich_server.c中为write函数添加pipe信号捕获函数write_err
 *		 ,防止在write错误后程序直接终止。
 *  V1.8 添加一些注释
 * 	V1.9 将fork中的tell_parent挪至execl启动后的进程中，去掉延时。加入选择玩家数量功能。
 *  V2.0 修复行走负步数跨原点S时越界，和block道具使用中的类似bug。2013.1.24
 * 		 open_gg@qq.com
 */

#include"console.h"


int main(int argc, char *argv[])
{

	if(init(ParseCmdLine(argc,argv))<0) return -1;

	print_bar("Welcome to Richman!",' ',Green);
	cmd_help(0,NULL);
	while(1)
	{
		playing();
	}

}



