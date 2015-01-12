/*  linuxÖÕ¶ËÏÂCÓïÑÔ±à³ÌÁ·Ï° ´ó¸»ÎÌÓÎÏ· ¿É²Î¿¼rich_jpeg.jpgÖÐµÄÊý¾Ý½á¹¹ 
 *  ÔÚsrcÄ¿Â¼ÏÂÖ´ÐÐmake, Ö´ÐÐ./rich ÔËÐÐÓÎÏ·  ÍøÂç°æÔËÐÐ·½Ê½:´ò¿ªÁ½¸öÖÕ¶Ë£¬ÔÚÖÕ¶Ë1
 *  ÏÂµÄsrcÄ¿Â¼ÏÂÖ´ÐÐ./rich -s ,ÔÚÖÕ¶Ë2ÏÂµÄsrcÄ¿Â¼ÏÂÖ´ÐÐ./rich -c,ÖÕ¶Ë1Ê×ÏÈÔËÐÐ,È»ºó
 *  ÖÕ¶Ë2ÔËÐÐ¡£
 *
 *
 *  V0.1 Íê³ÉÒ»Î»Íæ¼ÒµÄÐÐ×ß£¬ÔÚlandÖÐÓÃsymbol±£´æÁÙÊ±µÄÏÔÊ¾·ûºÅ ´òÓ¡µØÍ¼Ê±Ê×ÏÈÅÐ¶Ï
 *       ÓÐÎÞÁÙÊ±ÏÔÊ¾µÄÔªËØ¡£2012.11.22
 *  V0.2 ÐÞ¸Äupdate_mapº¯Êý£¬Íê³É4ÈËÇÐ»»ÏÔÊ¾ ÔÚµØÍ¼ÉÏÏÔÊ¾Íæ¼Ò·ûºÅÑÕÉ«£¬ÓÃÓÚÃ¿´ÎË¢
 *       ÐÂlandÖÐµÄÏÔÊ¾·ûºÅ£¬³ÌÐò¿ò¼Ü»ù±¾È·¶¨ 2012.11.22
 *  V0.3 Ìí¼Ó²éÑ¯Íæ¼ÒÐÅÏ¢º¯Êýplayer_info, Ìí¼ÓgameÖÐid¼°player_numÏà¹Øº¯Êý£¬ÐÞ¸Ä    
 *  		update_mapÖÐµÄÍæ¼ÒÊýÁ¿µÄbug, Ìí¼Óbuy_landº¯Êý¡£2012.11.23
 *  V0.4 Ìí¼Óµ¥Ò»Íæ¼ÒÍË³öº¯Êýplayer_quit(),ÐÞÕý´òÓ¡Íæ¼ÒÐÅÏ¢Ê±ÐèÒªµÄidÎªÍæ¼Ò×ÔÉíid£¬
 *  		ÐÞ¸ÄÁËdeinit_game()ÖÐiµÄ·¶Î§bug¡£
 *  V0.5 ÔÚlandÖÐÌí¼ÓÍÁµØËùÓÐÕßµÄÑÕÉ«owner_color,ÐÞ¸Äinit_land(),print_game_map()
 *		 ÐÞÕýÍæ¼ÒÍË³öºóÈÔÏÔÊ¾·ûºÅµÄbug,ÐÞ¸Äprint_game_map()ÖÐlandÏÔÊ¾bug¡£Ìí¼ÓÖ÷º¯
 *		 ÊýÎÄ¼þrich.c¡£ÐÞ¸Äprint_game_map()£¬Ê¹ÆäÔÚÖÕ¶ËµÄÖÐ¼ä´òÓ¡µØÍ¼¡£2012.11.25
 *  V0.6 ÖØÐÂÔÚutilÖÐÐ´ÁËprint_bar´úÌæÁËwelcome(). É¾³ýmainÖÐ¶àÓà´úÂë¡£
 *  V0.7 Ìí¼Órollº¯Êý
 *  V0.8 ÖØÐ´cmd_goÖÐµÄif-elseÎªswitch£¬ÔÝÊ±Ã»Ïëµ½¸üºÃµÄ·½Ê½´¦Àí£¬¾²Ì¬±í»òÊ÷½á¹¹¿
 *  		Ìí¼Óupgrade_houseº¯Êý¡£¼ò»¯mainº¯Êý£¬ºÃÏñÃ»É¶ºÃ´¦¡£Ìí¼Óprint_p_info ¿É²é¿´
 *       Íæ¼ÒÓµÓÐµÄÍÁµØÊýÁ¿¡£ÐÞ¸ÄÍË³öÊ±µÄÏÔÊ¾
 *  V0.9 Ìí¼ÓtoolÏà¹Ø¹¦ÄÜ£¬ÎªÍæ¼ÒÌí¼Ótool×Ö¶Î£¬¹ºÂòµÀ¾ß£¬ÐÞ¸Äprint_p_infoµÄbug£¬ÖØÐ´
 *		 init_land();  2012.11.30
 *  V1.0 Ìí¼ÓµÀ¾ßÊ¹ÓÃÏà¹Ø½á¹¹£¬ºÍcmdµÄÖ´ÐÐÀàËÆ
 *  V1.1 Ìí¼ÓÂ·ÕÏBlockµÄÊ¹ÓÃºÍÓöµ½BlockµÄ²Ù×÷2012.12.04
 *  V1.2 ÐÞ¸Äprint_colorº¯Êý£¬¼ûtuil.h¡£Ìí¼ÓµÀ¾ßÊ¹ÓÃµÄ¿Õº¯Êý£¬·ÀÖ¹¶Î´íÎó
 *  V1.3 ÐÞ¸Ä window_col()º¯ÊýÖÐ»ñÈ¡±ê×¼Êä³öµÄÖÕ¶Ë¿í¶È£¬ÒÔ·ÀÖ¹ÊäÈëÖØ¶¨Ïòºó²»ÄÜµÃµ½±ê×¼
 *       ÊäÈëµÄ¿í¶ÈµÄbug£¬²âÊÔÓÃµÄinput.txtÎÄ¼þÖÐ£¬Èôµ¥ÐÐÖ»ÓÐÒ»¸öÃüÁî£¬ºóÃæÒª¸ú¸ö¿Õ¸ñ£¬
 *       ·ñÔò¶ÁµÄÃüÁî²»ÕýÈ·¡£²âÊÔ·½Ê½ ./rich.out < input.txt  ÖØ¶¨ÏòÊä³ö»á´íÎó Í¬ÑùÊÇ
 *       window_col()ÒýÆðµÄ£¬¿É#undef LINUX_CONSOLE½øÐÐÊä³öÖØ¶¨Ïò£¬Ê¹ÓÃdiff¶Ô²âÊÔ½á¹û±È
 *       ½Ï¡£bug:Block²»ÄÜÔÚÓÐÍæ¼ÒµÄµØ¿éÉÏÊ¹ÓÃ¡£
 *  V1.4 ÖØÐ´six_rand()º¯Êý¡£      
 *  V1.5 Ìí¼ÓÍøÂçÏà¹Øº¯Êý£¬ÔÚ/netÄ¿Â¼ÏÂ£¬./a.outÊÇ·þÎñÆ÷£¬Ê×ÏÈÔËÐÐ£¬È»ºó·Ö±ðÔÚÁíÍâÁ½¸ö
 *		 ÖÕ¶ËÏÂÖ´ÐÐ/srcÄ¿Â¼ÏÂµÄ./rich.out -n£¬ÊµÏÖÁ½Î»Íæ¼ÒµÄ»¥¶¯ ,ÊµÏÖ¿Í»§¶ËÖ÷ÒªÐÞ¸ÄµÄ
 *		 º¯ÊýÓÐ:get_cmd_line();get_num();next_player(·¢ËÍ×Ö·ûx);init();is_yes();
 *		 Ìí¼ÓÈ«¾Ö±äÁ¿sockfd, game½á¹¹ÌåÖÐÌí¼Óturn±êÖ¾Î»´ú±íÊÇ·ñÂÖµ½×Ô¼º¡£2012.12.14
 *	V1.6 ÖØÐ´makefile£¬ÒÔÍ¬Ê±make³ÌÐòºÍ·þÎñÆ÷£¬Ð´·¨»¹ÊÇ²»±ê×¼¡£Æô¶¯·þÎñÆ÷µÄ·½Ê½:ÔÚinit
 *		 º¯ÊýÖÐÊ¹ÓÃforkºÍÐÅºÅ¡£ParseCmdLineº¯ÊýÈ·¶¨³ÌÐòµÄÆô¶¯·½Ê½:
 *		 	-server ·þÎñÆ÷ -clinet ¿Í»§¶Ë Ä¬ÈÏ±¾µØÔËÐÐ  
 *		 ÔÚinitÖÐ#define PLAYER_NUM 2ÉèÖÃÍæ¼ÒÊýÁ¿£¬Í¬Ê±ÐÞ¸Ärich_server.cÖÐµÄwait_client(2);
 *		 ÔËÐÐÊ±ÖÕ¶ËÊýÁ¿Ò²Òª¶ÔÓ¦Ôö¼Ó		2012.12.21
 *	V1.7 ÐÞ¸Ä4ÈËÊ±ÍË³öµÄbug£¬ÔÚrich_server.cÖÐÎªwriteº¯ÊýÌí¼ÓpipeÐÅºÅ²¶»ñº¯Êýwrite_err
 *		 ,·ÀÖ¹ÔÚwrite´íÎóºó³ÌÐòÖ±½ÓÖÕÖ¹¡£
 *  V1.8 Ìí¼ÓÒ»Ð©×¢ÊÍ
 * 	V1.9 ½«forkÖÐµÄtell_parentÅ²ÖÁexeclÆô¶¯ºóµÄ½ø³ÌÖÐ£¬È¥µôÑÓÊ±¡£¼ÓÈëÑ¡ÔñÍæ¼ÒÊýÁ¿¹¦ÄÜ¡£
 *  V2.0 ÐÞ¸´ÐÐ×ß¸º²½Êý¿çÔ­µãSÊ±Ô½½ç£¬ºÍblockµÀ¾ßÊ¹ÓÃÖÐµÄÀàËÆbug¡£2013.1.24
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



