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
	'!','I',0xB351, 0x30, /* "�Q", */
	'\"','$',0xA4F1,0x32, /*22 "��", */
	'\"','[',0xAB4B,0x30, /* "�K", */
	'\"','m',0xA8C3,0x33, /*33 "��", */
	'#','#',0xA4A3, 0x30, /* "��", */
	'#','%',0xA4A3, 0x60, /* "��", */
	'&','k',0xBECC, 0x43, /*33 "��", */
	//')','H',0xA843, 0x20, /* "�C" */
	'1','+',0xA46A, 0x20, /* "�j", */
	'1','8',0xAABA, 0x30, /* "��", */
	'1','O',0xA8EC, 0x50, /* "��", */  //
	'1','[',0xA6FD, 0x30, /* "��", */
	'1','l',0xB5A5, 0x32, /* 22"��", */
	'9','+',0xA8BA, 0x32, /* 22"��", */
	'=','A',0xA8D3, 0x20, /* "��", */
	'=','N',0xA6D1, 0x20, /* "��", */
	'>','$',0xB8CC, 0x30, /* "��", */
	'E','6',0xA569, 0x20, /* "�i", */
	'I','A',0xC1D9, 0x20, /* "��", */
	'I','U',0xABE1, 0x43, /* 33"��", */
	'I','[',0xA94D, 0x30, /* "�M", */
	'I','`',0xABDC, 0x50, /* "��", */
	'K','1',0xA9CE, 0x30, /* "��", */
	'K','I',0xB77C, 0x40, /* "�|", */
	'N','#',0XA4CE, 0x32, /* 22"��"  */
	'N','O',0xB8FB, 0x32, /*22 "��"  */
	'N','U',0xB44E, 0x30, /* "�N", */
	'N','d',0xB14E, 0x43, /* 33"�N"  */
	'R','#',0xA8E4, 0x30, /* 33"��", */ //
	'R','Y',0xAB65, 0x40, /* 33"�e", */ //
	'R','l',0xBDD0, 0x60, /* "��", */
	'T','%',0xA568, 0x30, /* "�h", */
	'V','+',0xA455, 0x30, /* "�U", */
	'V','^',0xB773, 0x20, /* "�s", */
//	'V','g',0xA656, 0x20, /* "�V", */
	'Y','"',0xA4A7, 0x30, /* "��", */
//	'Y','%',0xA6DC, 0x10, /* "��", */   
//	'Y','7',0xB36F, 0x20, /* "�o"  */
	'[','j',0xA4A4, 0x32, /* 22"��", */
	'_','"',0xA558, 0x30, /*12 "�X", */
//	'a','#',0xA451, 0x30, /*22 "�Q", */ 
	'a','$',0xA8CF, 0x20, /* "��", */
	'a','%',0xAC4F, 0x53, /*53 "�O", */
	'a','g',0xA457, 0x32, /*22 "�W", */
	'e','_',0xA448, 0x32, /*22 "�H", */
	'e','g',0xC5FD, 0x70, /* "��", */
	'g','#',0xA670, 0x30, /*22 "�p", */
	'i','5',0xAB68, 0x30, /*22 "�h", */
	'i','C',0xA662, 0x50, /*42 "�b", */     
	'k','1',0xB0B5, 0x20, /* "��","�@"A740, */
	'k','I',0xB3CC, 0x50, /* "��", */
	'm','A',0xA47E, 0x20, /* "�~", */
	'o','k',0xB171, 0x50, /* "�q", */
	's','0',0xA9D2, 0x40, /* 33"��", */
	'u','q',0xA6D3, 0x30, /* "��", */
	'v','#',0xA440, 0x10, /* "�@", */
	'v','$',0xA548, 0x30, /* "�H", */
	'v','<',0xA45D, 0x30, /* 22"�]", */
	'v','O',0xAD6E, 0x40, /* 33"�n", */
	'v','S',0xA5D1, 0x30, /*22 "��", */
	'v','T',0xA6B3, 0x50, /* "��", */
	'v','U',0xA453, 0x30, /* "�S", */
	'v','^',0xA65D, 0x30, /* "�]", */
	'w','#',0xB54C, 0x20, /* "�L", */
	'w','G',0xACB0, 0x30, /* 22"��", */
	'x','#',0xA9F3, 0x40, /*32 "��", */
	'x','$',0xBB50, 0x50, /* 42"�P", */
	'x','%',0xB1FD, 0x30, /*22 "��", */
	'x','m',0xA5CE, 0x30  /*22 "��", */
};

#define FRE_LO	 0
//#define FRE_HI	 sizeof(fre) / sizeof(struct frestr);
short FRE_HI	 =  sizeof(fre) / sizeof(struct frestr);

struct	Unit {
	char	fc;
	char	lc;
	short	charval;
} fst_unit[] = {
		')', 'H', 0xA843, /* "�C" */
		'>', 'm', 0xA574, /* "�t" */
		'2', '%', 0xB2C4, /* "��" */
		'9', '+', 0xA8BA, /* "��" */ 
//		'V', 'g', 0xB9B3, /* "��" */
//		'N', '(', 0xa55b, // �[
		'Y', '7', 0xB36F, /* "�o" */ 
		'm', '%', 0xA6B8, /* "��" */
		'v', 'T', 0xA6B3, /* "��", */
		'!', '`', 0xA5BB, /* "��" */
		'm', '$', 0xA6B9  /* "��" */
};
//#define FST_UNIT_HI	sizeof(fst_unit)/sizeof(struct Unit)
short FST_UNIT_HI	=  sizeof(fst_unit)/ sizeof(struct Unit);

#define NUMSPEC_A 1
#define NUMSPEC_B 2
#define NUMSPEC_C 16
struct	Unit fst_num_unit[] = {
		'0', '0', 0x0000, /* half code,  no use now! */
		'v', '#', 0xA440, /* "�@2"  NUMSPEC_A = 1 */
		'v', '%', 0xA440, /* "�@4"  NUMSPEC_B = 2 */
//		'v', '"', 0xA440, /* "�@1" */	// 2-21-1995 Liu
		'N', '$', 0xb458, // �X
		'!', '[', 0xa562, // �b
		'3', '.', 0xa668, // �h
		'c', '%', 0xbcc6, // ��
		'~', '0' ,0xA2AF, /* "��" */
		'~', '1', 0xA2B0, /* "��" */
		'~', '2', 0xA2B1, /* "��" */
		'~', '3', 0xA2B2, /* "��" */
		'~', '4', 0xA2B3, /* "��" */
		'~', '5', 0xA2B4, /* "��" */
		'~', '6', 0xA2B5, /* "��" */
		'~', '7', 0xA2B6, /* "��" */
		'~', '8', 0xA2B7, /* "��" */
		'~', '9', 0xA2B8, /* "��"   NUMSPEC_C = 16 */
	//-----------------------------------^-^ special coding
		'v', '\"', 0xA440, /* "�@1" */
		'!', '(', 0xa44b, /* "�K" */
		'>', 'U', 0xA4BB, /* "��" */
		'>', 'f', 0xA8E2, /* "��" */
		'N', 'T', 0xA445, /* "�E" */
		'R', '\"', 0xA443, /* "�C" */
		'a', '#', 0xA451, /* "�Q" */
		'q', '%', 0xA57C, /* "�|" */
		'q', 'X', 0xA454, /* "�T" */
		'u', 's', 0xA447, /* "�G" */
		'w', '$', 0xA4AD, /* "��" */ 
	//	'x', '=', 0xA4EB, /* "��" */
	//	'1', 'O', 0xA8EC, /* "��" */
	//	'Y', '%', 0xA6DC  /* "��" */	
};
//#define FST_NUM_HI	sizeof(fst_num_unit)/sizeof(struct Unit)
short FST_NUM_HI	=  sizeof(fst_num_unit)/ sizeof(struct Unit);

struct	Unit snd_num_unit[] = {
		'!', 'B', 0xA6CA, /* "��" */
		'R', 'X', 0xA464, /* "�d" */
		'Y', 'O', 0xa5fc, // ��
		'w', '[', 0xB855, /* "�U" */
		'v', '%', 0xBBF5, /* "��" 1992,9,15 */
		'x', '#', 0xbe6c // �l
};
//#define SND_NUM_HI	sizeof(snd_num_unit)/sizeof(struct Unit)
short SND_NUM_HI	=  sizeof(snd_num_unit)/  sizeof(struct Unit);

struct	Unit adr_snd_unit[] = {
	'3', '[', 0xac71, /* �q */
	';', 'm', 0xa7cb, /* �� */
	'=', 'S', 0xbcd3, /* �� */
	'>', '_', 0xbe46, /* "�F" */
	'?', '%', 0xb8f4, /* �� */
	'I', 'O', 0xb8b9, /* "��" */
	'V', 'g', 0xabd1, /* �� , �� */
	'Y', '\"', 0xa4a7, /* ��  */
	'a', '%', 0xabc7  /* �� */
};
//#define ADR_SND_UNIT_HI sizeof(adr_snd_unit) / sizeof(struct Unit)
short ADR_SND_UNIT_HI =  sizeof(adr_snd_unit) /  sizeof(struct Unit);

struct	Unit sys_snd_unit[] = {
		'!', '*', 0xA7E2, /* "��" */
		'!', 'F', 0xAA4D, /* "�M" */
		'!', 'I', 0xADBF, /* "��" */
		'!', 'L', 0xA55D, /* "�]" */
		'!', 'X', 0xAF5A, /* "�Z" */	
		'!', '`', 0xA5BB, /* "��" */
		'\"', '$', 0xB5A7, /* "��" */ 
		'\"', '[', 0xB94D, /* "�M" */
		'#', '%', 0xB3A1, /* "��" �B*/
		'%', 'A', 0xB1C6, /* "��" */
		'%', 'Y', 0xBD4C, /* "�L" */
		'&', '\"', 0xA7E5, /* "��" */
		'&', '\"', 0xA4C7,/* �� */
		'&', '$', 0xA5D4, /* �� */
		'&', 'X', 0xBD67, /* "�g" */
		'&', '[', 0xA4F9, /* "��" */
		'&', 'k', 0xB27E, /* "�~" */
		')', 'G', 0xAA54, /* "�T" */
//		'*', 'N', 0xACED, /* "��" */
		'*', '[', 0xADB1, /* "��" */
		'*', 'k', 0xA657, /* "�W" */
		'+', '%', 0xB9F5, /* "��" */
//		'-', '^', 0xA4C0, /* "��" */
		'-', 'd', 0xA4E8, /* "��" */
		'-', 'j', 0xABCA, /* "��" */
		'/', '#', 0xB454, /* "�T" */
//�o�ɥN->�o�Q�N	'1', 'C', 0xA54E, 0,/* "�N" */
		'1', 'O', 0xB944, /* "�D" */
		'2', ';', 0xC57C, /* "�|" */
		'2', 'Z', 0xC249, /* "�I" */
		'2', 'l', 0xB3BB, /* "��" */
		'3', '0', 0xA6B7, /* "��" */
		'3', 'F', 0xB0EF, /* "��" */
		'3', '[', 0xAC71, /* �q */
		'3', 'a', 0xB979, /* "�y" */
		'3', 'a', 0xBEB7, /* �� */
		'3', 'm', 0xAC7D, /* �} */
		'5', 'A', 0xA578, /* �x */
		'5', 'S', 0xC059, /* "�Y" */
		'6', 'M', 0xB1F8, /* "��" */
		'6', 'X', 0xA4D1, /* "��" */
		'7', 'Y', 0xB9CE, /* "��" */
		'7', 'l', 0xB1ED, /* "��" */
		':', 'Y', 0xA67E, /* "�~" */
		'=', 'I', 0xC3FE, /* "��" */
		'=', 'S', 0xBCD3, /* "��" */
		'=', 'Y', 0xC478, /* "�x" */
		'>', '$', 0xA8BD, /* "��" */
		'>', '%', 0xB2C9, /* "��" */
		'>', 'g', 0xBDF8, /* "��" */
		'A', '7', 0xADD3, /* "��" */
		'A', '8', 0xADD3, /* "��" */
		'A', 'Z', 0xB1EC, /* "��" */
		'A', '^', 0xAEDA, /* "��" */
//		'C', 'j', 0xA4BD, /* "��" */
		'C', '[', 0xC5F8, /* "��" */
		'E', '4', 0xB4CA, /* "��" */
//		'E', '4', 0xC1FB, /* �� */
		'E', '7', 0xBDD2, /* "��" */
		'E', 'T', 0xA466, /* "�f" */
		'G', 'C', 0xB6F4, /* "��" */
		'I', 'e', 0xA6E6, /* "��" */
		'K', '#', 0xB3FD, /* "��" */
		'K', 'G', 0xA65E, /* "�^" */
		'N', '#', 0xAFC5, /* "��" */
		'N', '(', 0xAE61, /* "�a" */
		'N', '+', 0xAC5B, /* "�[" */
		'N', ';', 0xB860, /* "�`" */
		'N', '=', 0xA9A1, /* "��" */
		'N', '[', 0xA5F3, /* "��" */
		'N', '^', 0xA4E7, /* "��" */
		'N', 'f', 0xBCFA, /* "��" */
		'P', '%', 0xA8E3, /* "��" */
		'R', '#', 0xB4C1, /* "��" */
		'V', 'g', 0xB6B5, /* "��" */
		'Y', '\"', 0xB0A6, /* "��" !! �K �� */
//		'Y', '\"', 0xA4E4, /* "��" */
		'Y', '$', 0xA575, /* "�u" */
		'Y', 'R', 0xB667, /* "�g" */
		'Y', 'Z', 0xB7F8, /* "��" */
//		'Y', 'd', 0xB169, /* "�i" ��*/
		'[', '\"', 0xAEE8, /* "��" */
/*		'[', 'd', 0xA74E,  "��" */
		'[', 'l', 0xBAD8, /* "��" */ 
		']', '$', 0xA4D8, /* "��" */
		']', 'f', 0xB3F5, /* "��" */
//		'_', '\"', 0xC4D5, /* "��" */
		'_', '%', 0xB342, /* "�B" */
		'_', 'l', 0xA7C9, /* "��" �l*/
		'a', '%', 0xA540, /* "�@" */
		'a', 'T', 0xADBA, /* "��" */
		'c', '%', 0xA7F4, /* "��" */
		'e', '%', 0xA4E9, /* "��" */
		'e', '_', 0xA448, /* "�H" */
		'a', '[', 0xAEB0, /* "��" */
		'k', '$', 0xB2D5, /* "��" */
		'k', '1', 0xAE79, /* "�y" */
		'k', '^', 0xB44C, /* "�L" */
		'm', '7', 0xA555, /* "�U" */
		'm', '%', 0xA6B8, /* "��" */
		'm', 'k', 0xBC68, /* "�h" */
		'o', 'k', 0xC24F, /* "�O" */
		'q', 'L', 0xBFB4, /* "��" */
/* ���ծɩҥΪ� */
/*		's', '0', 0xA9D2,  "��" */
		's', 'I', 0xB7B3, /* �� 1992, 9, 14 */
		'v', '=', 0xADB6, /* "��" */
		'w', 'I', 0xA6EC, /* "��" */
//		'w', 'Z', 0xB84A, /* "�J" */
		'x', '=', 0xA4EB, /* "��" */
		'x', 'Y', 0xA4B8/* "��" ��*/
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
 		"N#�Y",
 		"a7�]",
 		"i\"��",
 		"u[��",
 		"N=��",
 		"i%��",
 		"TY��"

 	//	"a#�Q"
};
 
short	SPATCH_CCHAR =  sizeof(spatch_char) / 4;
 
unsigned char  srep_word[][4] = {
	"xY��",
	"Vd��",
	">g�G",
	"aU�G",
	">%�R",
	"[l�~",
	"6Y��",
	"sj�P",
	"E6��",
	"g1�z",
	"V$��",
	"Nj��",
	"]e��",
	"u[�t",
	"V%��",
	"Ga�t",
	"=I��",
	"N\"��",
	"[g��",
	"aXݵ",
	"IU�p",
	"cC��",
	"u7�j",
	"K)��",
	"NO�s",
	"[m��", 
	"V[��",
	"a^�`",
	"-F��",
        "iL�V",
        "1T�~",
	"3#Ū"
};
//#define	SREP_WCOUNT sizeof(srep_word) / 4
short	SREP_WCOUNT =  sizeof(srep_word) / 4;


unsigned char  spatch_data[][8] = {
		"!I![���b",
		"!FNT�M�s",
		"&kNT�~�s",
		"6M_Y����",
		"6MI5���e",
		"6MV\"����",
		"6M_k����",
		"6Mx#����",
		"1+Vg�j��",
		"2Z[j�I��",
		"2Z![�I�b",
		"N[a%���",
		"3[K+�q��",
		"P%K+�y��",
		"YdY$�i��",
		":Y![�~�b",
		":YN#�~��",
		":YRY�~�e",
		":Y=A�~��",
		"!`c\"����",
		"e%xY���",
		"e%\"%���",
		"5A\"%�x��",
		"5eE7���",
		"N;E7�`��",
		">=a%�P�h",
		"-^[j����",
		"*N[j����",
		"CjE7���J",
		"Cj>$����",
		"Cj]$����",
		"Cj>f����",
		"CjN^����",
		"5AN^�x��",
		"Cjaj����",
		"Cj3a����",
		"CjRl����",
		"Cj+$���a",
		"oa]e�o��",
		"]$]e�ت�",
		"Yg]e�V��",
		"GCRY����", 
		"A8i%�Ӧr",
		"A8x=�Ӥ�",
		"A8e_�ӤH",
		"A81j�ӿO",
		"wZ-[�J��",
		"wZ*[�J��",
		"E4c%�ʾ�",
		"E45e���}",
		"E4V^����",
		"E4Vj���P",
		"KGI5�^�X",
		"[l\"m�دf",
		"Y\"_Y����",
		"a%N%�@��",
		"k1RM�y��",
		"m7c\"�U��"
};

//#define	SPATCH_COUNT sizeof(spatch_data) / 8
short	SPATCH_COUNT =  sizeof(spatch_data) / 8;

unsigned char  spatch_de[][4] = {
	"K+��",
	"V^��",
	"e_�H",
	"NO�s",
	"1j�O",
	"RM��",
	"RY��",
//	"a%��",
	"\"m�f"
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
      �B�z  �S���S��
      �B�z  �V���V��
    */   
    if(do_rep()) return(1);   

    st=f_want-4;
    if (st < 2 ) return(0);
    
    /* �B�z "����"  "����"etc. */    

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
    		  	youwant[st-3]=0x43;  /* �C */ 
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
    		  	youwant[st-1]=0x43;  /* �C */ 
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