#include "hanin.h"

struct spec_key spec_key[] = {
	'!',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'"',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT |
		(ET_BIT << 4),
	'#',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'$',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'%',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'&',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'\'',   STD_BIT | IBM_BIT | MITEC_BIT,
	'(',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	')',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'*',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'+',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT |
		(ET_BIT << 4) | (MITEC_BIT << 4),
	',',    IBM_BIT | MITEC_BIT,
	'-',    0, /* ET_BIT, */
	'.',    IBM_BIT | MITEC_BIT,
	'/',    IBM_BIT | MITEC_BIT, /* STD_BIT | IBM_BIT | MITEC_BIT, */
	'0',    0,
	'1',    0,
	'2',    0,
	'3',    0,
	'4',    0,
	'5',    0,
	'6',    0,
	'7',    0,
	'8',    0,
	'9',    0,
	':',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT |
		(STD_BIT<<4) | (IBM_BIT<<4) | (ET_BIT<<4) | (MITEC_BIT<<4),
	';',    0,
	'<',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT |
		(STD_BIT << 4) | (ET_BIT << 4),
	'=',    STD_BIT | IBM_BIT,
	'>',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT |
		(STD_BIT << 4) | (ET_BIT << 4),
	'?',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT |
		(STD_BIT << 4) | (ET_BIT << 4),
	'@',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'[',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	']',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'^',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'_',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT |
		(STD_BIT<<4) | (IBM_BIT<<4) | (ET_BIT<<4) | (MITEC_BIT<<4),
	'`',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'{',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT |
		(MITEC_BIT << 4),
	'|',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT |
		(STD_BIT<<4) | (IBM_BIT<<4) | (ET_BIT<<4) | (MITEC_BIT<<4),
	'}',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
	'~',    STD_BIT | IBM_BIT | ET_BIT | MITEC_BIT,
};

struct frestr {
	char	cmp0;
	char	cmp1;
	unsigned short code;
	char	freq;
}	fre[] = {
	'!','I',0xB351, 0x30, /* "被", */
	'\"','$',0xA4F1,0x32, /*22 "比", */
	'\"','[',0xAB4B,0x30, /* "便", */
	'\"','m',0xA8C3,0x33, /*33 "並", */
	'#','#',0xA4A3, 0x30, /* "不", */
	'#','%',0xA4A3, 0x60, /* "不", */
	'&','k',0xBECC, 0x43, /*33 "憑", */
	//')','H',0xA843, 0x20, /* "每" */
	'1','+',0xA46A, 0x20, /* "大", */
	'1','8',0xAABA, 0x30, /* "的", */
	'1','O',0xA8EC, 0x50, /* "到", */  //
	'1','[',0xA6FD, 0x30, /* "但", */
	'1','l',0xB5A5, 0x32, /* 22"等", */
	'9','+',0xA8BA, 0x32, /* 22"那", */
	'=','A',0xA8D3, 0x20, /* "來", */
	'=','N',0xA6D1, 0x20, /* "老", */
	'>','$',0xB8CC, 0x30, /* "裡", */
	'E','6',0xA569, 0x20, /* "可", */
	'I','A',0xC1D9, 0x20, /* "還", */
	'I','U',0xABE1, 0x43, /* 33"後", */
	'I','[',0xA94D, 0x30, /* "和", */
	'I','`',0xABDC, 0x50, /* "很", */
	'K','1',0xA9CE, 0x30, /* "或", */
	'K','I',0xB77C, 0x40, /* "會", */
	'N','#',0XA4CE, 0x32, /* 22"及"  */
	'N','O',0xB8FB, 0x32, /*22 "較"  */
	'N','U',0xB44E, 0x30, /* "就", */
	'N','d',0xB14E, 0x43, /* 33"將"  */
	'R','#',0xA8E4, 0x30, /* 33"其", */ //
	'R','Y',0xAB65, 0x40, /* 33"前", */ //
	'R','l',0xBDD0, 0x60, /* "請", */
	'T','%',0xA568, 0x30, /* "去", */
	'V','+',0xA455, 0x30, /* "下", */
	'V','^',0xB773, 0x20, /* "新", */
//	'V','g',0xA656, 0x20, /* "向", */
	'Y','"',0xA4A7, 0x30, /* "之", */
//	'Y','%',0xA6DC, 0x10, /* "至", */   
//	'Y','7',0xB36F, 0x20, /* "這"  */
	'[','j',0xA4A4, 0x32, /* 22"中", */
	'_','"',0xA558, 0x30, /*12 "出", */
//	'a','#',0xA451, 0x30, /*22 "十", */ 
	'a','$',0xA8CF, 0x20, /* "使", */
	'a','%',0xAC4F, 0x53, /*53 "是", */
	'a','g',0xA457, 0x32, /*22 "上", */
	'e','_',0xA448, 0x32, /*22 "人", */
	'e','g',0xC5FD, 0x70, /* "讓", */
	'g','#',0xA670, 0x30, /*22 "如", */
	'i','5',0xAB68, 0x30, /*22 "則", */
	'i','C',0xA662, 0x50, /*42 "在", */     
	'k','1',0xB0B5, 0x20, /* "做","作"A740, */
	'k','I',0xB3CC, 0x50, /* "最", */
	'm','A',0xA47E, 0x20, /* "才", */
	'o','k',0xB171, 0x50, /* "從", */
	's','0',0xA9D2, 0x40, /* 33"所", */
	'u','q',0xA6D3, 0x30, /* "而", */
	'v','#',0xA440, 0x10, /* "一", */
	'v','$',0xA548, 0x30, /* "以", */
	'v','<',0xA45D, 0x30, /* 22"也", */
	'v','O',0xAD6E, 0x40, /* 33"要", */
	'v','S',0xA5D1, 0x30, /*22 "由", */
	'v','T',0xA6B3, 0x50, /* "有", */
	'v','U',0xA453, 0x30, /* "又", */
	'v','^',0xA65D, 0x30, /* "因", */
	'w','#',0xB54C, 0x20, /* "無", */
	'w','G',0xACB0, 0x30, /* 22"為", */
	'x','#',0xA9F3, 0x40, /*32 "於", */
	'x','$',0xBB50, 0x50, /* 42"與", */
	'x','%',0xB1FD, 0x30, /*22 "欲", */
	'x','m',0xA5CE, 0x30  /*22 "用", */
};

#define FRE_LO	 0
//#define FRE_HI	 sizeof(fre) / sizeof(struct frestr);
short FRE_HI	 =  sizeof(fre) / sizeof(struct frestr);

struct	Unit {
	char	fc;
	char	lc;
	short	charval;
} fst_unit[] = {
		')', 'H', 0xA843, /* "每" */
		'>', 'm', 0xA574, /* "另" */
		'2', '%', 0xB2C4, /* "第" */
		'9', '+', 0xA8BA, /* "那" */ 
//		'V', 'g', 0xB9B3, /* "像" */
//		'N', '(', 0xa55b, // 加
		'Y', '7', 0xB36F, /* "這" */ 
		'm', '%', 0xA6B8, /* "次" */
		'v', 'T', 0xA6B3, /* "有", */
		'!', '`', 0xA5BB, /* "本" */
		'm', '$', 0xA6B9  /* "此" */
};
//#define FST_UNIT_HI	sizeof(fst_unit)/sizeof(struct Unit)
short FST_UNIT_HI	=  sizeof(fst_unit)/ sizeof(struct Unit);

#define NUMSPEC_A 1
#define NUMSPEC_B 2
#define NUMSPEC_C 16
struct	Unit fst_num_unit[] = {
		'0', '0', 0x0000, /* half code,  no use now! */
		'v', '#', 0xA440, /* "一2"  NUMSPEC_A = 1 */
		'v', '%', 0xA440, /* "一4"  NUMSPEC_B = 2 */
//		'v', '"', 0xA440, /* "一1" */	// 2-21-1995 Liu
		'N', '$', 0xb458, // 幾
		'!', '[', 0xa562, // 半
		'3', '.', 0xa668, // 多
		'c', '%', 0xbcc6, // 數
		'~', '0' ,0xA2AF, /* "０" */
		'~', '1', 0xA2B0, /* "１" */
		'~', '2', 0xA2B1, /* "２" */
		'~', '3', 0xA2B2, /* "３" */
		'~', '4', 0xA2B3, /* "４" */
		'~', '5', 0xA2B4, /* "５" */
		'~', '6', 0xA2B5, /* "６" */
		'~', '7', 0xA2B6, /* "７" */
		'~', '8', 0xA2B7, /* "８" */
		'~', '9', 0xA2B8, /* "９"   NUMSPEC_C = 16 */
	//-----------------------------------^-^ special coding
		'v', '\"', 0xA440, /* "一1" */
		'!', '(', 0xa44b, /* "八" */
		'>', 'U', 0xA4BB, /* "六" */
		'>', 'f', 0xA8E2, /* "兩" */
		'N', 'T', 0xA445, /* "九" */
		'R', '\"', 0xA443, /* "七" */
		'a', '#', 0xA451, /* "十" */
		'q', '%', 0xA57C, /* "四" */
		'q', 'X', 0xA454, /* "三" */
		'u', 's', 0xA447, /* "二" */
		'w', '$', 0xA4AD, /* "五" */ 
	//	'x', '=', 0xA4EB, /* "月" */
	//	'1', 'O', 0xA8EC, /* "到" */
	//	'Y', '%', 0xA6DC  /* "至" */	
};
//#define FST_NUM_HI	sizeof(fst_num_unit)/sizeof(struct Unit)
short FST_NUM_HI	=  sizeof(fst_num_unit)/ sizeof(struct Unit);

struct	Unit snd_num_unit[] = {
		'!', 'B', 0xA6CA, /* "百" */
		'R', 'X', 0xA464, /* "千" */
		'Y', 'O', 0xa5fc, // 兆
		'w', '[', 0xB855, /* "萬" */
		'v', '%', 0xBBF5, /* "億" 1992,9,15 */
		'x', '#', 0xbe6c // 餘
};
//#define SND_NUM_HI	sizeof(snd_num_unit)/sizeof(struct Unit)
short SND_NUM_HI	=  sizeof(snd_num_unit)/  sizeof(struct Unit);

struct	Unit adr_snd_unit[] = {
	'3', '[', 0xac71, /* 段 */
	';', 'm', 0xa7cb, /* 弄 */
	'=', 'S', 0xbcd3, /* 樓 */
	'>', '_', 0xbe46, /* "鄰" */
	'?', '%', 0xb8f4, /* 路 */
	'I', 'O', 0xb8b9, /* "號" */
	'V', 'g', 0xabd1, /* 巷 , 項 */
	'Y', '\"', 0xa4a7, /* 之  */
	'a', '%', 0xabc7  /* 室 */
};
//#define ADR_SND_UNIT_HI sizeof(adr_snd_unit) / sizeof(struct Unit)
short ADR_SND_UNIT_HI =  sizeof(adr_snd_unit) /  sizeof(struct Unit);

struct	Unit sys_snd_unit[] = {
		'!', '*', 0xA7E2, /* "把" */
		'!', 'F', 0xAA4D, /* "杯" */
		'!', 'I', 0xADBF, /* "倍" */
		'!', 'L', 0xA55D, /* "包" */
		'!', 'X', 0xAF5A, /* "班" */	
		'!', '`', 0xA5BB, /* "本" */
		'\"', '$', 0xB5A7, /* "筆" */ 
		'\"', '[', 0xB94D, /* "遍" */
		'#', '%', 0xB3A1, /* "部" 步*/
		'%', 'A', 0xB1C6, /* "排" */
		'%', 'Y', 0xBD4C, /* "盤" */
		'&', '\"', 0xA7E5, /* "批" */
		'&', '\"', 0xA4C7,/* 匹 */
		'&', '$', 0xA5D4, /* 疋 */
		'&', 'X', 0xBD67, /* "篇" */
		'&', '[', 0xA4F9, /* "片" */
		'&', 'k', 0xB27E, /* "瓶" */
		')', 'G', 0xAA54, /* "枚" */
//		'*', 'N', 0xACED, /* "秒" */
		'*', '[', 0xADB1, /* "面" */
		'*', 'k', 0xA657, /* "名" */
		'+', '%', 0xB9F5, /* "幕" */
//		'-', '^', 0xA4C0, /* "分" */
		'-', 'd', 0xA4E8, /* "方" */
		'-', 'j', 0xABCA, /* "封" */
		'/', '#', 0xB454, /* "幅" */
//這時代->這十代	'1', 'C', 0xA54E, 0,/* "代" */
		'1', 'O', 0xB944, /* "道" */
		'2', ';', 0xC57C, /* "疊" */
		'2', 'Z', 0xC249, /* "點" */
		'2', 'l', 0xB3BB, /* "頂" */
		'3', '0', 0xA6B7, /* "朵" */
		'3', 'F', 0xB0EF, /* "堆" */
		'3', '[', 0xAC71, /* 段 */
		'3', 'a', 0xB979, /* "頓" */
		'3', 'a', 0xBEB7, /* 噸 */
		'3', 'm', 0xAC7D, /* 洞 */
		'5', 'A', 0xA578, /* 台 */
		'5', 'S', 0xC059, /* "頭" */
		'6', 'M', 0xB1F8, /* "條" */
		'6', 'X', 0xA4D1, /* "天" */
		'7', 'Y', 0xB9CE, /* "團" */
		'7', 'l', 0xB1ED, /* "桶" */
		':', 'Y', 0xA67E, /* "年" */
		'=', 'I', 0xC3FE, /* "類" */
		'=', 'S', 0xBCD3, /* "樓" */
		'=', 'Y', 0xC478, /* "籃" */
		'>', '$', 0xA8BD, /* "里" */
		'>', '%', 0xB2C9, /* "粒" */
		'>', 'g', 0xBDF8, /* "輛" */
		'A', '7', 0xADD3, /* "個" */
		'A', '8', 0xADD3, /* "個" */
		'A', 'Z', 0xB1EC, /* "桿" */
		'A', '^', 0xAEDA, /* "根" */
//		'C', 'j', 0xA4BD, /* "公" */
		'C', '[', 0xC5F8, /* "罐" */
		'E', '4', 0xB4CA, /* "棵" */
//		'E', '4', 0xC1FB, /* 顆 */
		'E', '7', 0xBDD2, /* "課" */
		'E', 'T', 0xA466, /* "口" */
		'G', 'C', 0xB6F4, /* "塊" */
		'I', 'e', 0xA6E6, /* "行" */
		'K', '#', 0xB3FD, /* "壺" */
		'K', 'G', 0xA65E, /* "回" */
		'N', '#', 0xAFC5, /* "級" */
		'N', '(', 0xAE61, /* "家" */
		'N', '+', 0xAC5B, /* "架" */
		'N', ';', 0xB860, /* "節" */
		'N', '=', 0xA9A1, /* "屆" */
		'N', '[', 0xA5F3, /* "件" */
		'N', '^', 0xA4E7, /* "斤" */
		'N', 'f', 0xBCFA, /* "獎" */
		'P', '%', 0xA8E3, /* "具" */
		'R', '#', 0xB4C1, /* "期" */
		'V', 'g', 0xB6B5, /* "項" */
		'Y', '\"', 0xB0A6, /* "隻" !! 枝 肢 */
//		'Y', '\"', 0xA4E4, /* "支" */
		'Y', '$', 0xA575, /* "只" */
		'Y', 'R', 0xB667, /* "週" */
		'Y', 'Z', 0xB7F8, /* "盞" */
//		'Y', 'd', 0xB169, /* "張" 章*/
		'[', '\"', 0xAEE8, /* "株" */
/*		'[', 'd', 0xA74E,  "樁" */
		'[', 'l', 0xBAD8, /* "種" */ 
		']', '$', 0xA4D8, /* "尺" */
		']', 'f', 0xB3F5, /* "場" */
//		'_', '\"', 0xC4D5, /* "齣" */
		'_', '%', 0xB342, /* "處" */
		'_', 'l', 0xA7C9, /* "床" 幢*/
		'a', '%', 0xA540, /* "世" */
		'a', 'T', 0xADBA, /* "首" */
		'c', '%', 0xA7F4, /* "束" */
		'e', '%', 0xA4E9, /* "日" */
		'e', '_', 0xA448, /* "人" */
		'a', '[', 0xAEB0, /* "扇" */
		'k', '$', 0xB2D5, /* "組" */
		'k', '1', 0xAE79, /* "座" */
		'k', '^', 0xB44C, /* "尊" */
		'm', '7', 0xA555, /* "冊" */
		'm', '%', 0xA6B8, /* "次" */
		'm', 'k', 0xBC68, /* "層" */
		'o', 'k', 0xC24F, /* "叢" */
		'q', 'L', 0xBFB4, /* "艘" */
/* 面試時所用的 */
/*		's', '0', 0xA9D2,  "所" */
		's', 'I', 0xB7B3, /* 歲 1992, 9, 14 */
		'v', '=', 0xADB6, /* "頁" */
		'w', 'I', 0xA6EC, /* "位" */
//		'w', 'Z', 0xB84A, /* "碗" */
		'x', '=', 0xA4EB, /* "月" */
		'x', 'Y', 0xA4B8/* "元" 員*/
};

//#define SYS_SND_UNIT_HI sizeof(sys_snd_unit) / sizeof(struct Unit)
short SYS_SND_UNIT_HI =  sizeof(sys_snd_unit) /  sizeof(struct Unit);

void	ReDisplay(short st,unsigned char *ss,short j);
short	fdan(char *buf, struct Unit *unit_ptr, short hi);
short	do_rep();

short	UnitSearchWord(short st,short len,short mode)
{
    short     i,j,k,l;
    short     snd_unit_hi;
    short     a_ind, b_ind, ind=0, stage=0;
    char    c;
    unsigned char	toutbuf[512];
    struct  Unit *snd_unit_pt;

    oldchlen = chlen;
    i = j = k = l = -1;
    if ((i=fdan((char *)cmpkigo+st+ind, &fst_unit[0], FST_UNIT_HI)) >= 0) 
    {
		toutbuf[ind] = (fst_unit[i].charval >> 8) ^ HANIN_CRYPT;
		toutbuf[ind+1] = (fst_unit[i].charval & 0xff) ^ HANIN_CRYPT;
		ind += 2;
		stage = 0;
    }

	// note: stage == 2 no use, on 11/23/1993
    while (ind < len) {
   		if(ind == 2 && len == 4)
    			break;
		if((stage == 0 || (stage == 1 && j > 2) || stage == 2) &&
		(j=fdan((char *)cmpkigo+st+ind, &fst_num_unit[0], FST_NUM_HI)) >= 0)  {
	
		if (j == NUMSPEC_A || j == NUMSPEC_B) 
		{
	    	if (stage == 1) 
	    		break;
	    		
	    	if (ind <= len-4) 
	    	{
				c = cmpkigo[st+ind+3] - '!';
				c %= TONE_COUNT;
				if ((j == 1 && c < TONE_COUNT-2) || (j == 2 && c > TONE_COUNT-3)) 
		    		break;
	    	}
		}
		// Modified on 11/23/1993, for 746 ok, but prevent 8half
		else if (j <= NUMSPEC_C)
		{ // means && > NUMSPEC_B
	    	if (stage == 1)
	    	{
		 		break;
	    	}
		}

		if (j != 0) 
		{
	    	toutbuf[ind] = (fst_num_unit[j].charval >> 8) ^ HANIN_CRYPT;
	    	toutbuf[ind+1] = (fst_num_unit[j].charval & 0xff) ^ HANIN_CRYPT;
		}
		//	else {	 /* half code */
		//	}
		ind += 2;
		stage = 1;
	
		if (j <=NUMSPEC_C && j > NUMSPEC_B) // for prevent half8
			break;
	}
	else
		break;
    }

    while (ind < len &&
		(stage == 1 || (stage == 2 && j > 1)) &&
		(k=fdan((char *)cmpkigo+st+ind, &snd_num_unit[0], SND_NUM_HI)) >= 0) 
	{
		toutbuf[ind] = (snd_num_unit[k].charval >> 8) ^ HANIN_CRYPT;
		toutbuf[ind+1] = (snd_num_unit[k].charval & 0xff) ^ HANIN_CRYPT;
		ind += 2;
		stage = 2;
    }

    if (ind < len && ((stage == 0 && i >= 0) || stage == 1 || stage == 2)) 
    {
		if (AddrState )
		{
		    snd_unit_pt = (struct Unit *)&adr_snd_unit;
	    	snd_unit_hi = ADR_SND_UNIT_HI;
		}
		else
		{
		    snd_unit_pt = (struct Unit *)&sys_snd_unit;
	    	snd_unit_hi = SYS_SND_UNIT_HI;
		}
		a_ind = ind;
		b_ind = 0;
		ind -= 2;
		if (a_ind < len) 
		{    /* condition added by chen */
		    for (l=0; l<snd_unit_hi; l++) 
		    {
				if (cmpkigo[st+a_ind] == snd_unit_pt[l].fc &&
		    		cmpkigo[st+a_ind+1] == snd_unit_pt[l].lc) 
		    	{
		    		ind += 2;
		    		toutbuf[ind] = (snd_unit_pt[l].charval >> 8) ^ HANIN_CRYPT;
		    		toutbuf[ind+1] = (snd_unit_pt[l].charval & 0xff) ^ HANIN_CRYPT;
		    		ind += 2;
		    		if (len != 4 || mode != SEARCH_ALL) 
		    			break;
		    		strncpy((char *)toutbuf+ind, (char *)toutbuf, 4);
				}
	    	}
	    	toutbuf[ind] = '\0';
		}
    }

    freq_fg = 0x30;
    if (ind < len) 
    {
		ind = 0;
		freq_fg = 0;
    }
    else 
    {
		toutbuf[ind] = '\0';
		AppendToBuffer(toutbuf, ind, st, len, mode);
    }
  //  freq_fg = FRQ_FG;  
    word_fg = 0;   /* for special rule matching */
    
    return(chlen-oldchlen);
}


short fdan(char buf[], struct Unit *unit_ptr, short hi)
{
	char	c0, c1;
	short	i;

	c0 = buf[0]; c1 = buf[1];
	for (i = 0; i < hi; i++)
		if ((c0 == (unit_ptr+i)->fc) && (c1 == (unit_ptr+i)->lc))
			return(i);
	return(-1);
}

short fndx(short st)
{
    char  c0, c1;
    short   lo, hi, mi;
    char  cmp0, cmp1;

    if (AddrState || st < 0) return(-1);
    lo= FRE_LO;
    hi = FRE_HI;
    c0 = cmpkigo[st];
    c1 = cmpkigo[st+1];
    while (1) {
	if (hi < lo) return(-1);
	mi = (lo+hi) >> 1;
	cmp0 = fre[mi].cmp0;
	cmp1 = fre[mi].cmp1;
	if (c0 < cmp0 || (c0 == cmp0 && c1 < cmp1))
	    hi = mi - 1;
	else if (c0 > cmp0 || (c0 == cmp0 && c1 > cmp1))
	    lo = mi + 1;
	else {
	    /* set recover code to junction code */
	    liang[0] = fre[mi].code >> 8;
	    liang[1] = fre[mi].code & 0xff;
	    liang[2] = fre[mi].freq;
	    return(mi);
	}
    }
}

short chk_unit(short k)
{
	short	status;
	
    if ((status=fdan((char *)cmpkigo+k, &fst_num_unit[0], FST_NUM_HI)) != -1 )
    	return(status);
    else if ( (status=fdan((char *)cmpkigo+k, &fst_unit[0], FST_UNIT_HI)) != -1 )
    	return(status);
    else
    	return(-1);
           
}
 
 unsigned char	spatch_char[][4] = {
 		"N#即",
 		"a7設",
 		"i\"茲",
 		"u[按",
 		"N=藉",
 		"i%自",
 		"TY全"

 	//	"a#十"
};
 
short	SPATCH_CCHAR =  sizeof(spatch_char) / 4;
 
unsigned char  srep_word[][4] = {
	"xY圓",
	"Vd香",
	">g亮",
	"aU瘦",
	">%麗",
	"[l腫",
	"6Y甜",
	"sj鬆",
	"E6渴",
	"g1弱",
	"V$喜",
	"Nj驚",
	"]e長",
	"u[暗",
	"V%細",
	"Ga睏",
	"=I累",
	"N\"飢",
	"[g壯",
	"aX搧",
	"IU厚",
	"cC帥",
	"u7餓",
	"K)滑",
	"NO叫",
	"[m重", 
	"V[陷",
	"a^深",
	"-F飛",
        "iL糟",
        "1T陡",
	"3#讀"
};
//#define	SREP_WCOUNT sizeof(srep_word) / 4
short	SREP_WCOUNT =  sizeof(srep_word) / 4;


unsigned char  spatch_data[][8] = {
		"!I![倍半",
		"!FNT杯酒",
		"&kNT瓶酒",
		"6M_Y條船",
		"6MI5條河",
		"6MV\"條溪",
		"6M_k條蟲",
		"6Mx#條魚",
		"1+Vg大項",
		"2Z[j點鐘",
		"2Z![點半",
		"N[a%件事",
		"3[K+段話",
		"P%K+句話",
		"YdY$張紙",
		":Y![年半",
		":YN#年級",
		":YRY年前",
		":Y=A年來",
		"!`c\"本書",
		"e%xY日圓",
		"e%\"%日幣",
		"5A\"%台幣",
		"5eE7堂課",
		"N;E7節課",
		">=a%烈士",
		"-^[j分鐘",
		"*N[j秒鐘",
		"CjE7公克",
		"Cj>$公里",
		"Cj]$公尺",
		"Cj>f公兩",
		"CjN^公斤",
		"5AN^台斤",
		"Cjaj公升",
		"Cj3a公噸",
		"CjRl公頃",
		"Cj+$公畝",
		"oa]e寸長",
		"]$]e尺長",
		"Yg]e丈長",
		"GCRY塊錢", 
		"A8i%個字",
		"A8x=個月",
		"A8e_個人",
		"A81j個燈",
		"wZ-[碗飯",
		"wZ*[碗麵",
		"E4c%棵樹",
		"E45e顆糖",
		"E4V^顆心",
		"E4Vj顆星",
		"KGI5回合",
		"[l\"m種病",
		"Y\"_Y隻船",
		"a%N%世紀",
		"k1RM座橋",
		"m7c\"冊書"
};

//#define	SPATCH_COUNT sizeof(spatch_data) / 8
short	SPATCH_COUNT =  sizeof(spatch_data) / 8;

unsigned char  spatch_de[][4] = {
	"K+話",
	"V^心",
	"e_人",
	"NO叫",
	"1j燈",
	"RM橋",
	"RY錢",
//	"a%事",
	"\"m病"
};

//#define	SPATCH_DE_COUNT sizeof(spatch_de) / 4
short	SPATCH_DE_COUNT =  sizeof(spatch_de) / 4;

short Spatch()
{
    short st, i, j, k, sp;
    unsigned char *ss;
    unsigned char *sc,*sd;

  if ( ((Core[(f_want>>1)-1].flag & CHG_FG) != 0) || ((Core[(f_want>>1)-1].flag & WRD_FG) != 0) ) return(0);
    
    if ( f_want == 2 || (f_want > 2 && cmpkigo[f_want-4] == '~') )
    { 
     	sc= &spatch_char[0][0];
     	sp=f_want-2;

    	for(i=0;i<SPATCH_CCHAR;i++, sc += 4 )
    	{
    		if ( cmpkigo[sp]== *sc && cmpkigo[sp+1] == *(sc+1) )
    		{
    			youwant[sp]= *(sc+2);
    			youwant[sp+1]= *(sc+3);
    			words_dis((short) (W_START+sp), youwant+sp, 2, NOR);
    			return(1);
    		}
    	}
    }
    
     /*   
      處理  又＊又＊
      處理  越＊越＊
    */   
    if(do_rep()) return(1);   

    st=f_want-4;
    if (st < 2 ) return(0);
    
    /* 處理 "的心"  "的話"etc. */    

    sd= &spatch_de[0][0];
    if ( cmpkigo[st] == '1' && cmpkigo[st+1]== '8')
    { 
    	for(i=0;i<SPATCH_DE_COUNT;i++, sd += 4)
    	{
	    	if(cmpkigo[st+2] == *(sd)  && cmpkigo[st+3] == *(sd+1) )
	    	{
	  			youwant[st+2]= *(sd+2);
	  			youwant[st+3]= *(sd+3); 
				words_dis((short) (W_START+st+2), youwant+st+2, 2, NOR);
	  			return(1);  		  	
	    	}
		}
    }
    
    ss= &spatch_data[0][0];
    for (i=0; i<SPATCH_COUNT; i++, ss += 8) 
    {
     for(j=0;j<4 && cmpkigo[st+j] == *(ss+j) ;j++) ;
     	if ( j == 4 )
     	{
	   if((k=fdan((char *)cmpkigo+st-2, &fst_num_unit[0], FST_NUM_HI)) >= 0) 
	   {
	    	youwant[st-2] = (char) (fst_num_unit[k].charval >> 8) ;
	    	youwant[st-1] = (char) (fst_num_unit[k].charval & 0xff) ;
	    	ReDisplay(st,ss,j);
	    	if ( st-4 >= 0 )
	    	{
    		  if (cmpkigo[st-4] == ')' && cmpkigo[st-3] == 'H')
    		  {
    		  	youwant[st-4]=0xa8;
    		  	youwant[st-3]=0x43;  /* 每 */ 
				words_dis((short) (W_START+st-4),youwant+st-4, 2, NOR);
		      }
	    	}
		return(1);
	   }
	   else if((k=fdan((char *)cmpkigo+st-2,&fst_unit[0],FST_UNIT_HI)) >= 0 )
	   {
	        youwant[st-2] = (char) (fst_unit[k].charval >> 8) ;
	    	youwant[st-1] = (char) (fst_unit[k].charval & 0xff) ;
	    	ReDisplay(st,ss,j);
	    	return(1);
	   }
	   else if((k=fdan((char *)cmpkigo+st-2, &snd_num_unit[0], SND_NUM_HI)) >= 0) 
	   {
	    	youwant[st-2] = (char) (snd_num_unit[k].charval >> 8) ;
	    	youwant[st-1] = (char) (snd_num_unit[k].charval & 0xff) ;
	    	ReDisplay(st,ss,j);
		    return(1);
	    }
	   else
	   {
    		  if (cmpkigo[st-2] == ')' && cmpkigo[st-1] == 'H')
    		  {
    		  	youwant[st-2]=0xa8;
    		  	youwant[st-1]=0x43;  /* 每 */ 
		    	ReDisplay(st,ss,j);
			    return(1);
		     }
	   }
	 }
     }
    return(0);
}

void ReDisplay(short st,unsigned char *ss,short j)
{
    youwant[st]=  *(ss+j);
    youwant[st+1]=  *(ss+j+1);
    youwant[st+2]=  *(ss+j+2);
    youwant[st+3]=  *(ss+j+3);
	words_dis((short) (W_START+st-2), youwant+st-2,6, NOR); 
	Core[(st+2)>>1].flag = (0x33 | WRD_FG);
}


short do_rep()
{
unsigned char	*sr1,*sr2;
short	i,j,k,count;
 
    sr1=sr2=&srep_word[0][0];
    
    count=SREP_WCOUNT;
  //   code1=0xa4; code2=0x53; 
  //   code1=0xb6; code2=0x56;
     
    if ((f_want>=8) && (Core[(f_want>>1)-3].flag & CHG_FG) == 0 ) 
       { 
         	k=f_want-4;
         	if ( (youwant[k]==0xa4 && youwant[k-4]==0xa4) ||
         	     (youwant[k]==0xb6 && youwant[k-4]==0xb6) )
         	{
         		if ( (youwant[k]==0xa4 &&youwant[k+1]==0x53 && youwant[k-3]==0x53)||
         		     (youwant[k]==0xb6 &&youwant[k+1]==0x56 && youwant[k-3]==0x56) )
         		{
         		   for(i=0;i<count;i++,sr1 += 4)
         		   {
         		   		if ( cmpkigo[k+2] == *(sr1) && cmpkigo[k+3] == *(sr1+1))
         		   		{
         		   			youwant[k+2] = *(sr1+2);
         		   			youwant[k+3] = *(sr1+3);
         		   			words_dis((short) (W_START+k+2),  youwant+k+2, 2, NOR);
         		   			break;
         		   		}
         		   }
         		   for(j=0;j<count;j++,sr2 += 4)
         		   {
         		   		if ( cmpkigo[k-2] == *(sr2) && cmpkigo[k-1] == *(sr2+1))
         		   		{
         		   			youwant[k-2] = *(sr2+2);
         		   			youwant[k-1] = *(sr2+3);
         		   			words_dis((short) (W_START+k-2), youwant+k-2, 2, NOR);
         		   			break;
         		   		}
         		   }
         		  return(1);	
         		}
         	} 
         }
         return(0);
}

short	AddrCharMatch (short st)
{
	int		i;
	BYTE	szBuf[3];

	for (i = 0; i < ADR_SND_UNIT_HI; i++)
	{
		if (cmpkigo[st] == adr_snd_unit[i].fc && 
			cmpkigo[st+1] == adr_snd_unit[i].lc)
		{
			szBuf[0] = (adr_snd_unit[i].charval >> 8) ^ HANIN_CRYPT;
			szBuf[1] = (adr_snd_unit[i].charval & 0xff) ^ HANIN_CRYPT;
			szBuf[2] = 0;

			AppendToBuffer(szBuf, 2, st, 2, SEARCH_ONE);

			return 2;
		}
	}
	return 0;
}
