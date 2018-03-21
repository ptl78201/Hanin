#include "hanin.h"


short	MatchRuleA (PBYTE rule, short count, short len);
short	MatchRuleC(PBYTE st, PBYTE ss);
short	MatchRuleB(PBYTE rule, short count, short len);

unsigned char	rule_A1[][6] = {

	"筆Y\"枝",

	"花Y\"枝",

	"錶Y\"支",

	"箭Y\"支",

	"傘Y\"支",

	"籤Y\"支",

        "煙Y\"支",

	"手Y\"隻",

	"腿Y\"隻",

	"狗Y\"隻",

	"貓Y\"隻",

	"鳥Y\"隻",

	"獅Y\"隻",

	"虎Y\"隻",

	"兔Y\"隻",

	"猴Y\"隻",

	"牛Y\"隻",

	"鼠Y\"隻",

	"手Y\"隻",

	"腳Y\"隻",

	"腿Y\"隻",

//	"羊Y\"隻",

	"雞Y\"隻",

	"鹿Y\"隻",

	"豹\Y\"隻",

	"豬Y\"隻",

	"船Y\"隻",

	"馬&\"匹",

	"狼&\"匹",

	"腳i)砸",

	"獎!X頒",

	"信N%寄",

	"信V<寫",

	"茶%O泡",

	"車k1坐",

	"車]k乘",

	"樹E4棵",

	"草E4棵",

	"糖E4顆",

	"星E4顆",

	"彈E4顆",

	"牙E4顆",

	"珠E4顆",

	"雪N\"積",

	"卡N%寄",

	"沒iN澡",

	"給N%寄",

	"給sm送",

	"車R#騎",

	"馬R#騎",

	"罪-[犯",

	"病K[患",

	"耳aj聲",

	"事![辦",

	"課5e堂",

	"罵u@挨",

	"說K+話",

	"講K+話" };

//#define RULE_A1_COUNT sizeof(rule_A1)/6

short RULE_A1_COUNT = sizeof(rule_A1)/6;



unsigned char	rule_A2[][8] = {

	"自首Vg向",

	"低頭Vg向",

	"解釋Vg向",

	"說明Vg向",

	"報告Vg向",

	"認罪Vg向",

	"一樣Vg像",

	"這樣Vg像",

	"這麼Vg像",

	"那樣Vg像",

	"那麼Vg像",

	"一般Vg像",

	"同樣Vg像",

	"軍隊Y\"支",

	"隊伍Y\"支",

	"舞曲Y\"支",

	"熱狗Y\"支",

	"蠟燭Y\"枝",

//	"鋼筆Y\"枝",

//	"鉛筆Y\"枝",

//	"毛筆Y\"枝",

//	"子筆Y\"枝",

	"蝴蝶Y\"隻",

	"烏鴉Y\"隻",

	"種子E4顆",

	"責任Na盡",

	"義務Na盡",

	"道義Na盡",

	"能力Na盡",

	"力量Na盡",

	"孝道Na盡",

	"本分Na盡",

	"房間Na進",

	"權利Vf享",

	"協定RX簽",

	"契約RX簽",

	"條約RX簽",

	"公車!X班",

	"火車!X班",

	"飛機!X班",

	"學生!X班",

	"步兵!X班",

	"總統ea任",

	"主席ea任",

	"代表ea任",

	"帽子1C戴",

	"遊戲wY玩",

	"門鈴u[按",

/*	"動會N=屆", */

/*	"委員N=屆", */

/*	"總統N=屆", */

/*	"主席N=屆", */

/*	"代表N=屆", */

	"便宜Y[占",

	"贓款_\"出",

	"吃飯wI餵",

	"吃奶wI餵",

	"使用Cj供" };

//#define RULE_A2_COUNT sizeof(rule_A2)/8

short RULE_A2_COUNT = sizeof(rule_A2)/8;



unsigned char	rule_B[][6] = {

	"天>g亮",

        "靠u[岸",

	"挺Xj胸",

	"碗-[飯",

	"首a\"詩",

	"枝\"$筆",

	"流V<血",

//	"流I[汗",

	"生1[蛋",

	"孵1[蛋",

	"盞1j燈",

	"打N+架",

	"打RS球",

	"洗iN澡",

	"跳w$舞",

	"算Yg帳",

	"打Yg仗",

	"說K+話",

	"說aj聲",

	"叫aj聲",

	"發aj聲",

	"收VO效",

	"遇a%事",

	"畫K+畫",

	"睡NO覺",

	"瓶NT酒",

	"壺NT酒",

	"杯NT酒",

	"壺])茶",

	"座RM橋",

	"條x#魚",

	"尾x#魚",

	"到wY玩",

	"來wY玩",

	"去wY玩",

	"買RY錢",

	"賣RY錢",	

	"賺RY錢",

	"值RY錢",

	"欠RY錢",

	"筆RY錢",

	"湊RY錢",

	"張Y$紙",

	"講K+話",

	"句K+話" };

//#define RULE_B_COUNT sizeof(rule_B)/6

short RULE_B_COUNT = sizeof(rule_B)/6;



unsigned char	title[][4]={

"經理",   // Postion sensitive

"程師",   // Postison sesitive

"先生",

"小姐",

"太太",

"夫人",

"媽媽",

"爸爸",

"姐姐",

"哥哥",

"伯父",

"伯伯",

"伯母",

"叔叔",

"爺爺",

"奶奶",

"阿姨",

"襄理",

"協理",

"主任",

"課長",

"股長",

"科長",

"次長",

"科員",

"局長",

"處長",

"廠長",

"議員",

"委員",

"立委",

"師父",

"醫師",

"老師",

"教授",

"同學",

"博士",

"老闆",

"老板",

"秘書",

"會計",

"司機",

"顧問",

"董事",

"監事",

"理事",

"主席",

"幹事",

"總統",

"部長",

"縣長",

"市長",

"鎮長",

"鄉長",

"警員",

"導演",

"導播",

"班長",

"上校",

"中校",

"少校",

"上尉",

"中尉",

"少尉",

"連長",

"營長",

"排長",

"上將",

"中將",

"少將",

"團長",

"旅長",

"師長",

"軍長",

"管家",

"專員",

"站長" };

//#define RULE_C1_COUNT sizeof(title)/4

short RULE_C1_COUNT = sizeof(title)/4;



unsigned char	surname[][4]={

"!(巴",

"#$卜",

"%X潘",

"%k彭",

")C麥",

")G梅",

")M毛",

")m孟",

"*M苗",

"-I費",

"-[范",

"-d方",

"1C戴",

"1m鄧",

"2j丁",

"3%杜",

"3l董",

"5M陶",

"5Y譚",

"5e唐",

"7k童",

"=C賴",

"=M勞",

"=Y藍",

">#黎",

">$李",

">O廖",

">S劉",

">Y連",

">_林",

"?#盧",

"?$魯",

"?%陸",

"?/羅",

"@$呂",

"A6葛",

"AL高",

"AX甘",

"C$古",

"C%顧",

"C.郭",

"CX官",

"Cj宮",

"E4柯",

"Gl孔",

"I5何",

"I7賀",

"IS侯",

"IY韓",

"K#胡",

"Ke黃",

"Kk洪",

"N%季",

"N*賈",

"NZ簡",

"N^金",

"Nd江",

"Nf蔣",

"RR邱",

"RY錢",

"R_秦",

"V#席",

"V+夏",

"V=謝",

"VL蕭",

"X#徐",

"X$\263\134",

"Xk熊",

"YO趙",

"YR周",

"YX詹",

"Yd張",

"Ym鄭",

"[%祝",

"[\"朱",

"[d莊",

"[j鍾",

"]_陳",

"]k程",

"a\"施",

"aO邵",

"a^申",

"a`沈",

"am盛",

"gI芮",

"gZ阮",

"ij曾",

"mC蔡",

"mM曹",

"oF崔",

"s\"蘇",

"s^孫",

"sm宋",

"uC艾",

"v=葉",

"vM姚",

"vS尤",

"vS游",

"vY顏",

"v^殷",

"ve楊",

"w\"巫",

"w#吳",

"wI魏",

"w^溫",

"wd汪",

"we王",

"wj翁",

"x#余",

//"x#于",

"x%郁",

"xY袁",

/**************/

"V^辛",

"/%傅",

"IN郝"};



//#define RULE_C2_COUNT sizeof(surname)/4

short RULE_C2_COUNT = sizeof(surname)/4;





short	SpecialRuleMatch()

{
    return(Spatch() ||

  	   MatchRuleA((PBYTE) rule_A2, RULE_A2_COUNT, 4) ||

	   MatchRuleA((PBYTE) rule_A1, RULE_A1_COUNT, 2) ||

	   MatchRuleB((PBYTE) rule_B, RULE_B_COUNT, 2)   ||

	   MatchRuleC((PBYTE) title,(PBYTE) surname));

}

short	MatchRuleA (PBYTE rule, short count, short len)
{
    short 	start, i, j, k;

    if ((start=f_want-len) <= 0) 

    	return(0);

    for (i=0; i<count; i++, rule+=len+4) 

    {

		for (j=0; j<len && youwant[start+j] == *(rule+j); j++);

		if (j == len) 

		{

	    	for (k=start-2; k>=0; k-=2) 

	    	{

	    		if (cmpkigo[k] == '~' )

	    			return(0);

				if (cmpkigo[k] == *(rule+j) && cmpkigo[k+1] == *(rule+j+1))

				{

		    		if ( /*len == 2 &&*/ ((cmpkigo[k] == 'Y' && cmpkigo[k+1] == '\"')||(cmpkigo[k]=='E' && cmpkigo[k+1]=='4')

		        		|| (cmpkigo[k] == '&' && cmpkigo[k+1]== '\"')) && chk_unit((short) (k-2)) == -1) 

		    			return(0);

		    		//if ( (cmpkigo[k] == 'Y' && cmpkigo[k+1] == '\"')||(cmpkigo[k]=='&' && cmpkigo[k+1]=='\"') || (Core[k>>1].flag & WRD_FG+CHG_FG) == 0 ) 

		    		if ((Core[k>>1].flag & CHG_FG) == 0 && 

		    			((Core[k>>1].flag & WRD_FG) == 0 || 

		    			((cmpkigo[k] == 'Y' && cmpkigo[k+1] == '\"') || (cmpkigo[k]=='&' && cmpkigo[k+1]=='\"'))))

		    		{

		    			youwant[k] = *(rule+j+2);

		    			youwant[k+1] = *(rule+j+3);

		    			words_dis((short) (W_START+k), youwant+k, 2, NOR);

		    			return(1);

		    		}

				}

	    	}

		}

    }

    return(0);

}

short MatchRuleB(PBYTE rule, short count, short len)
{
    short	start, i, j, k;

    if ((start=f_want-len) <= 0 ||

	(Core[(f_want-2)>>1].flag & 0x0f) > 1) return(0);

    for (i=0; i<count; i++, rule+=len+4) {

	for (j=0; j<len && cmpkigo[start+j] == *(rule+j+2); j++);

	if (j == len) {

	    for (j=0, k=start-2; k>=0; k-=2) {

	    	if ( cmpkigo[k] == '~' )

	    		return(0);

		if (youwant[k] == *(rule+j) && youwant[k+1] == *(rule+j+1)) {

		    youwant[start] = *(rule+j+4);

		    youwant[start+1] = *(rule+j+5);

		    words_dis((short) (W_START+start), youwant+start, 2, NOR);

		    return(1);

		} 	    } 	}     }     return(0);

}

short	MatchRuleC(PBYTE st, PBYTE ss)
{
    short 	start, i, j, k;

    if ((start = f_want-4) <= 0) 

    	return(0);
   	

    k = start-2; 

    for (i = 0; i < RULE_C1_COUNT; i++, st += 4) 
    {
		if (youwant[start] == *(st) && youwant[start+1] == *(st+1)

	 		&& youwant[start+2] == *(st+2) && youwant[start+3] == *(st+3))
	 	{
	    	if ( k-2>=0 )
	 	    {
	 		    if ( (i==0 && youwant[k]==0xc1 && youwant[k+1]==0x60) ||    // SHIA "總經理，工程師" 
	 	   	         (i==1 && youwant[k]==0xa4 && youwant[k+1]==0x75) )
	 		          k -= 2;
	 	    }
	    	for (j = 0; j < RULE_C2_COUNT-3; j++, ss += 4)         /* 避免 傅郝辛 */
	    	{
				if (cmpkigo[k] == *(ss) && cmpkigo[k + 1] == *(ss + 1) &&
		    		(Core[k >> 1].flag & WRD_FG + CHG_FG) == 0) 
		    	{
	    			if ( f_want >= 8 && (cmpkigo[k]=='w' && cmpkigo[k+1]=='I') && (k==0 || chk_unit((short) (k-2)) != -1) )
    					return(0);        // SHIA for "一魏同學,etc"

			    	youwant[k] = *(ss+2);
		    		youwant[k+1] = *(ss+3); 
		    		words_dis((short) (W_START+k), youwant+k, 2, NOR);
		    		return(1);
		 		}
	  		}
      	}
    }
    return(0);

}



/* 

short		name_search (st,)

short		st;

{

    short		lo, hi, mi;

    char	c0, c1;

    char	cmp0, cmp1;

    extern unsigned char liang[];



    	

    lo= 0;

    hi = RULE_CONUT_C2;

    c0 = cmpkigo[st];

    c1 = cmpkigo[st+1];

    

    while (1) 

    {

	if (hi < lo) 

		return(-1);

			

	mi = (lo+hi) >> 1;

	cmp0 = fre[mi].cmp0;

	cmp1 = fre[mi].cmp1;

	if (c0 < cmp0 || (c0 == cmp0 && c1 < cmp1))

	    hi = mi - 1;

	else if (c0 > cmp0 || (c0 == cmp0 && c1 > cmp1))

		lo = mi + 1;

	else 

	{

		liang[0] = fre[mi].code >> 8;

	    	liang[1] = fre[mi].code & 0xff;

	       

	    	return(mi);

	}

    }

}

*/ 

