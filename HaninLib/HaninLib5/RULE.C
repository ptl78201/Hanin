#include "hanin.h"


short	MatchRuleA (PBYTE rule, short count, short len);
short	MatchRuleC(PBYTE st, PBYTE ss);
short	MatchRuleB(PBYTE rule, short count, short len);

unsigned char	rule_A1[][6] = {

	"��Y\"�K",

	"��Y\"�K",

	"��Y\"��",

	"�bY\"��",

	"��Y\"��",

	"��Y\"��",

        "��Y\"��",

	"��Y\"��",

	"�LY\"��",

	"��Y\"��",

	"��Y\"��",

	"��Y\"��",

	"��Y\"��",

	"��Y\"��",

	"��Y\"��",

	"�UY\"��",

	"��Y\"��",

	"��Y\"��",

	"��Y\"��",

	"�}Y\"��",

	"�LY\"��",

//	"��Y\"��",

	"��Y\"��",

	"��Y\"��",

	"�\\Y\"��",

	"��Y\"��",

	"��Y\"��",

	"��&\"��",

	"�T&\"��",

	"�}i)�{",

	"��!X�{",

	"�HN%�H",

	"�HV<�g",

	"��%O�w",

	"��k1��",

	"��]k��",

	"��E4��",

	"��E4��",

	"�}E4��",

	"�PE4��",

	"�uE4��",

	"��E4��",

	"�]E4��",

	"��N\"�n",

	"�dN%�H",

	"�SiN��",

	"��N%�H",

	"��sm�e",

	"��R#�M",

	"��R#�M",

	"�o-[��",

	"�fK[�w",

	"��aj�n",

	"��![��",

	"��5e��",

	"�|u@��",

	"��K+��",

	"��K+��" };

//#define RULE_A1_COUNT sizeof(rule_A1)/6

short RULE_A1_COUNT = sizeof(rule_A1)/6;



unsigned char	rule_A2[][8] = {

	"�ۭ�Vg�V",

	"�C�YVg�V",

	"����Vg�V",

	"����Vg�V",

	"���iVg�V",

	"�{�oVg�V",

	"�@��Vg��",

	"�o��Vg��",

	"�o��Vg��",

	"����Vg��",

	"����Vg��",

	"�@��Vg��",

	"�P��Vg��",

	"�x��Y\"��",

	"����Y\"��",

	"�R��Y\"��",

	"����Y\"��",

	"����Y\"�K",

//	"����Y\"�K",

//	"�]��Y\"�K",

//	"��Y\"�K",

//	"�l��Y\"�K",

	"����Y\"��",

	"�Q�~Y\"��",

	"�ؤlE4��",

	"�d��Na��",

	"�q��Na��",

	"�D�qNa��",

	"��ONa��",

	"�O�qNa��",

	"���DNa��",

	"����Na��",

	"�ж�Na�i",

	"�v�QVf��",

	"��wRXñ",

	"����RXñ",

	"����RXñ",

	"����!X�Z",

	"����!X�Z",

	"����!X�Z",

	"�ǥ�!X�Z",

	"�B�L!X�Z",

	"�`��ea��",

	"�D�uea��",

	"�N��ea��",

	"�U�l1C��",

	"�C��wY��",

	"���au[��",

/*	"�ʷ|N=��", */

/*	"�e��N=��", */

/*	"�`��N=��", */

/*	"�D�uN=��", */

/*	"�N��N=��", */

	"�K�yY[�e",

	"�B��_\"�X",

	"�Y��wI��",

	"�Y��wI��",

	"�ϥ�Cj��" };

//#define RULE_A2_COUNT sizeof(rule_A2)/8

short RULE_A2_COUNT = sizeof(rule_A2)/8;



unsigned char	rule_B[][6] = {

	"��>g�G",

        "�au[��",

	"��Xj��",

	"�J-[��",

	"��a\"��",

	"�K\"$��",

	"�yV<��",

//	"�yI[��",

	"��1[�J",

	"��1[�J",

	"��1j�O",

	"��N+�[",

	"��RS�y",

	"�~iN��",

	"��w$�R",

	"��Yg�b",

	"��Yg�M",

	"��K+��",

	"��aj�n",

	"�saj�n",

	"�oaj�n",

	"��VO��",

	"�Ja%��",

	"�eK+�e",

	"��NOı",

	"�~NT�s",

	"��NT�s",

	"�MNT�s",

	"��])��",

	"�yRM��",

	"��x#��",

	"��x#��",

	"��wY��",

	"��wY��",

	"�hwY��",

	"�RRY��",

	"��RY��",	

	"��RY��",

	"��RY��",

	"��RY��",

	"��RY��",

	"��RY��",

	"�iY$��",

	"��K+��",

	"�yK+��" };

//#define RULE_B_COUNT sizeof(rule_B)/6

short RULE_B_COUNT = sizeof(rule_B)/6;



unsigned char	title[][4]={

"�g�z",   // Postion sensitive

"�{�v",   // Postison sesitive

"����",

"�p�j",

"�Ӥ�",

"�ҤH",

"����",

"����",

"�j�j",

"����",

"�B��",

"�B�B",

"�B��",

"����",

"�ݷ�",

"����",

"����",

"���z",

"��z",

"�D��",

"�Ҫ�",

"�Ѫ�",

"���",

"����",

"���",

"����",

"�B��",

"�t��",

"ĳ��",

"�e��",

"�ߩe",

"�v��",

"��v",

"�Ѯv",

"�б�",

"�P��",

"�դh",

"����",

"�ѪO",

"����",

"�|�p",

"�q��",

"�U��",

"����",

"�ʨ�",

"�z��",

"�D�u",

"�F��",

"�`��",

"����",

"����",

"����",

"����",

"�m��",

"ĵ��",

"�ɺt",

"�ɼ�",

"�Z��",

"�W��",

"����",

"�֮�",

"�W�L",

"���L",

"�ֱL",

"�s��",

"���",

"�ƪ�",

"�W�N",

"���N",

"�ֱN",

"�Ϊ�",

"�Ȫ�",

"�v��",

"�x��",

"�ޮa",

"�M��",

"����" };

//#define RULE_C1_COUNT sizeof(title)/4

short RULE_C1_COUNT = sizeof(title)/4;



unsigned char	surname[][4]={

"!(��",

"#$�R",

"%X��",

"%k�^",

")C��",

")G��",

")M��",

")m�s",

"*M�]",

"-I�O",

"-[�S",

"-d��",

"1C��",

"1m�H",

"2j�B",

"3%��",

"3l��",

"5M��",

"5Y��",

"5e��",

"7k��",

"=C��",

"=M��",

"=Y��",

">#��",

">$��",

">O��",

">S�B",

">Y�s",

">_�L",

"?#�c",

"?$�|",

"?%��",

"?/ù",

"@$�f",

"A6��",

"AL��",

"AX��",

"C$�j",

"C%�U",

"C.��",

"CX�x",

"Cj�c",

"E4�_",

"Gl��",

"I5��",

"I7�P",

"IS�J",

"IY��",

"K#�J",

"Ke��",

"Kk�x",

"N%�u",

"N*��",

"NZ²",

"N^��",

"Nd��",

"Nf��",

"RR��",

"RY��",

"R_��",

"V#�u",

"V+�L",

"V=��",

"VL��",

"X#�}",

"X$\263\134",

"Xk��",

"YO��",

"YR�P",

"YX��",

"Yd�i",

"Ym�G",

"[%��",

"[\"��",

"[d��",

"[j��",

"]_��",

"]k�{",

"a\"�I",

"aO��",

"a^��",

"a`�H",

"am��",

"gIͺ",

"gZ��",

"ij��",

"mC��",

"mM��",

"oF�Z",

"s\"Ĭ",

"s^�]",

"sm��",

"uC��",

"v=��",

"vM��",

"vS��",

"vS��",

"vY�C",

"v^��",

"ve��",

"w\"��",

"w#�d",

"wI�Q",

"w^��",

"wd�L",

"we��",

"wj��",

"x#�E",

//"x#�_",

"x%��",

"xY�K",

/**************/

"V^��",

"/%��",

"IN�q"};



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
	 		    if ( (i==0 && youwant[k]==0xc1 && youwant[k+1]==0x60) ||    // SHIA "�`�g�z�A�u�{�v" 
	 	   	         (i==1 && youwant[k]==0xa4 && youwant[k+1]==0x75) )
	 		          k -= 2;
	 	    }
	    	for (j = 0; j < RULE_C2_COUNT-3; j++, ss += 4)         /* �קK �Űq�� */
	    	{
				if (cmpkigo[k] == *(ss) && cmpkigo[k + 1] == *(ss + 1) &&
		    		(Core[k >> 1].flag & WRD_FG + CHG_FG) == 0) 
		    	{
	    			if ( f_want >= 8 && (cmpkigo[k]=='w' && cmpkigo[k+1]=='I') && (k==0 || chk_unit((short) (k-2)) != -1) )
    					return(0);        // SHIA for "�@�Q�P��,etc"

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
