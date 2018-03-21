#include "hanin.h"

#define b_code	    0
#define p_code	    1
#define m_code	    2
#define f_code	    3
#define d_code	    4
#define t_code	    5
#define n_code	    6
#define l_code	    7
#define g_code	    8
#define k_code	    9
#define h_code	    10
#define j_code	    11
#define q_code	    12
#define x_code	    13
#define zh_code     14
#define ch_code     15
#define sh_code     16
#define r_code	    17
#define z_code	    18
#define c_code	    19
#define s_code	    20
#define i_code	    21
#define u_code	    22
#define v_code	    23
#define a_code	    24
#define o_code	    25
#define e_code	    26
#define ee_code     27
#define ai_code     28
#define ei_code     29
#define ao_code     30
#define ou_code     31
#define an_code     32
#define en_code     33
#define ang_code    34
#define eng_code    35
#define er_code     36
#define t1_code     37
#define t2_code     38
#define t3_code     39
#define t4_code     40
#define t5_code     41

#define a_char	    0
#define b_char	    1
#define c_char	    2
#define d_char	    3
#define e_char	    4
#define f_char	    5
#define g_char	    6
#define h_char	    7
#define i_char	    8
#define j_char	    9
#define k_char	    10
#define l_char	    11
#define m_char	    12
#define n_char	    13
#define o_char	    14
#define p_char	    15
#define q_char	    16
#define r_char	    17
#define s_char	    18
#define t_char	    19
#define u_char	    20
#define v_char	    21
#define w_char	    22
#define x_char	    23
#define y_char	    24
#define z_char	    25
#define cap_char    26
#define any_char    31

#define CHUIN_MASK  1<<5
#define ROMAN_MASK  2<<5
#define PININ_MASK  3<<5

char last_c = ' ';

char index_pin[] = {
    /* a */  0,/* b */	1,/* c */  2,/* d */  4,
    /* e */  5,/* f */	9,/* g */ 10,/* h */ 13,
    /* i */ 17,/* j */ 33,/* k */ 36,/* l */ 37,
    /* m */ 38,/* n */ 39,/* o */ 47,/* p */ 49,
    /* q */ 50,/* r */ 51,/* s */ 56,/* t */ 58,
    /* u */ 59,/* v */ 69,/* w */ 71,/* x */ 72,
    /* y */ 73,/* z */ 77,/* ^ */ 81,/* { */ 83
};

char pinin[][4] = { /* not unsigned */
/* 0 */  PININ_MASK|any_char,	-24,	    -1, 	a_code,     /* a */
/* 1 */  PININ_MASK|any_char,	-1,	    -1, 	b_code,     /* b */
/* 2 */  PININ_MASK|any_char,	-1,	    -1, 	c_code,     /* c */
/* 3 */  PININ_MASK|c_char,	c_code,     ch_code,	-1,
/* 4 */  PININ_MASK|any_char,	-1,	    -1, 	d_code,     /* d */
/* 5 */  PININ_MASK|any_char,	-21,	    -1, 	e_code,     /* e */
/* 6 */  PININ_MASK|any_char,	i_code,     -1, 	ee_code,
/* 7 */  PININ_MASK|any_char,	u_code,     -1, 	ee_code,
/* 8 */  PININ_MASK|any_char,	v_code,     -1, 	ee_code,
/* 9 */  PININ_MASK|any_char,	-1,	    -1, 	f_code,     /* f */
/* 10 */ PININ_MASK|any_char,	-1,	    -1, 	g_code,     /* g */
/* 11 */ PININ_MASK|any_char,	an_code,    ang_code,	-1,
/* 12 */ PININ_MASK|any_char,	en_code,    eng_code,	-1,
/* 13 */ PININ_MASK|any_char,	-1,	    -1, 	h_code,     /* h */
/* 14 */ PININ_MASK|z_char,	z_code,     zh_code,	-1,
/* 15 */ PININ_MASK|c_char,	c_code,     ch_code,	-1,
/* 16 */ PININ_MASK|s_char,	s_code,     sh_code,	-1,
/* 17 */ CHUIN_MASK|j_char,	zh_code,    j_code,	i_code,     /* i */
/* 18 */ CHUIN_MASK|h_char,	ch_code,    q_code,	i_code,
/* 19 */ CHUIN_MASK|h_char,	sh_code,    x_code,	i_code,
/* 20 */ ROMAN_MASK|h_char,	zh_code,    zh_code,	-1,
/* 21 */ ROMAN_MASK|h_char,	ch_code,    ch_code,	 -1,
/* 22 */ ROMAN_MASK|h_char,	sh_code,    sh_code,	 -1,
/* 23 */ ROMAN_MASK|r_char,	r_code,     r_code,	 -1,
/* 24 */ ROMAN_MASK|z_char,	z_code,     z_code,	 -1,
/* 25 */ ROMAN_MASK|c_char,	c_code,     c_code,	 -1,
/* 26 */ ROMAN_MASK|s_char,	s_code,     s_code,	 -1,
/* 27 */ PININ_MASK|any_char,	-21,	    -1, 	i_code,
/* 28 */ PININ_MASK|y_char,	i_code,     i_code,	 -1,
/* 29 */ PININ_MASK|any_char,	u_code,     -1, 	ei_code,
/* 30 */ PININ_MASK|a_char,	a_code,     ai_code,	-1,
/* 31 */ PININ_MASK|e_char,	e_code,     ei_code,	-1,
/* 32 */ PININ_MASK|e_char,	ee_code,    ei_code,	-1,
/* 33 */ ROMAN_MASK|any_char,	-1,	    -1, 	j_code,     /* j */
/* 34 */ CHUIN_MASK|any_char,	-1,	    -1, 	zh_code,
/* 35 */ PININ_MASK|j_char,	j_code,     zh_code,	-1,
/* 36 */ PININ_MASK|any_char,	-1,	    -1, 	k_code,     /* k */
/* 37 */ PININ_MASK|any_char,	-1,	    -1, 	l_code,     /* l */
/* 38 */ PININ_MASK|any_char,	-1,	    -1, 	m_code,     /* m */
/* 39 */ PININ_MASK|any_char,	-1,	    -1, 	n_code,     /* n */
/* 40 */ PININ_MASK|any_char,	i_code,     -1, 	en_code,
/* 41 */ PININ_MASK|any_char,	u_code,     -1, 	en_code,
/* 42 */ PININ_MASK|any_char,	v_code,     -1, 	en_code,
/* 43 */ PININ_MASK|a_char,	a_code,     an_code,	-1,
/* 44 */ PININ_MASK|o_char,	o_code,     u_code,	en_code,    /* special case: on,ion */
/* 45 */ PININ_MASK|e_char,	e_code,     en_code,	-1,
/* 46 */ PININ_MASK|e_char,	ee_code,    en_code,	-1,
/* 47 */ PININ_MASK|any_char,	-24,	    -1, 	o_code,     /* o */
/* 48 */ PININ_MASK|a_char,	a_code,     ao_code,	-1,
/* 49 */ PININ_MASK|any_char,	-1,	    -1, 	p_code,     /* p */
/* 50 */ PININ_MASK|any_char,	-1,	    -1, 	q_code,     /* q */
/* 51 */ PININ_MASK|any_char,	-1,	    -1, 	r_code,     /* r */
/* 52 */ CHUIN_MASK|j_char,	zh_code,    zh_code,	 -1,
/* 53 */ CHUIN_MASK|h_char,	ch_code,    ch_code,	 -1,
/* 54 */ CHUIN_MASK|h_char,	sh_code,    sh_code,	 -1,
/* 55 */ PININ_MASK|e_char,	e_code,     er_code,	 -1,
/* 56 */ PININ_MASK|any_char,	-1,	    -1, 	s_code,     /* s */
/* 57 */ PININ_MASK|t_char,	t_code,     c_code,	-1,
/* 58 */ PININ_MASK|any_char,	-1,	    -1, 	t_code,     /* t */
/* 59 */ PININ_MASK|any_char,	j_code,     -1, 	v_code,     /* u */
/* 60 */ PININ_MASK|any_char,	q_code,     -1, 	v_code,
/* 61 */ PININ_MASK|any_char,	x_code,     -1, 	v_code,
/* 62 */ PININ_MASK|any_char,	-21,	    -1, 	u_code,
/* 63 */ CHUIN_MASK|any_char,	i_code,     v_code,	-1,
/* 64 */ ROMAN_MASK|i_char,	i_code,     -1, 	ou_code,
/* 65 */ ROMAN_MASK|y_char,	i_code,     v_code,	-1,
/* 66 */ PININ_MASK|w_char,	u_code,     u_code,	    -1,
/* 67 */ PININ_MASK|a_char,	a_code,     ao_code,	-1,
/* 68 */ PININ_MASK|o_char,	o_code,     ou_code,	-1,
/* 69 */ PININ_MASK|any_char,	-21,	    -1, 	v_code,     /* v */
/* 70 */ PININ_MASK|any_char,	i_code,     v_code,	-1,
/* 71 */ PININ_MASK|any_char,	-21,	    -1, 	u_code,     /* w */
/* 72 */ PININ_MASK|any_char,	-1,	    -1, 	x_code,     /* x */
/* 73 */ CHUIN_MASK|j_char,	zh_code,    j_code,	i_code,     /* y */
/* 74 */ CHUIN_MASK|h_char,	ch_code,    q_code,	i_code,
/* 75 */ CHUIN_MASK|h_char,	sh_code,    x_code,	i_code,
/* 76 */ PININ_MASK|any_char,	-21,	    -1, 	i_code,
/* 77 */ PININ_MASK|any_char,	-1,	    -1, 	z_code,     /* z */
/* 78 */ PININ_MASK|t_char,	t_code,     z_code,	-1,
/* 79 */ CHUIN_MASK|s_char,	c_code,     c_code,	-1,
/* 80 */ CHUIN_MASK|s_char,	s_code,     s_code,	-1,
/* 81 */ PININ_MASK|e_char,	e_code,     ee_code,	-1,
/* 82 */ PININ_MASK|e_char,	ee_code,    ee_code,	-1
};

short	PininMatch()
{
    short   i, iStart, iEnd;
    char  code;
    char  prevChar;

    if ((inkey >= 'a' && inkey <= 'z') || inkey == '^') {
	i = (inkey=='^') ? cap_char : inkey-'a';
	iStart = index_pin[i];
	iEnd = index_pin[i+1];
	for (i=iStart; i<iEnd; i++) {
	    if ((pinin[i][0]>>5 & PininMode) == 0) continue;
	    prevChar = (pinin[i][0] & any_char);
	    if (prevChar != any_char &&
		(PininPos == -1 || nowkey_buf[PininPos] != (prevChar+'a')))
		continue;
	    if ((ownoff == 0 && pinin[i][1] < 0) ||
		(ownoff != 0 && (pinin[i][1] == distmp[ownoff-1] ||
		 pinin[i][1] < -1 && -pinin[i][1] > distmp[ownoff-1]))) {
		if (i != 44 || ownoff <= 1 || distmp[ownoff-2] < i_code) {
		    rule_buf[++PininPos] = (BYTE) i;
		    nowkey_buf[PininPos] = inkey;
		    if ((code=pinin[i][2]) != -1)      /* replace */
			show_pinin((short) (SP_INSERT|SP_REPLACE|ownoff), code);
		    if ((code=pinin[i][3]) != -1) {    /* append */
			pos_buf[ownoff++] = PininPos;
			show_pinin((short) (SP_INSERT|ownoff), code);
		    }
		    return(1);
		}
		else if (distmp[ownoff-2] == 21) {     /* ion */
		    rule_buf[++PininPos] = 99;
		    nowkey_buf[PininPos] = inkey;
		    show_pinin((short) (SP_INSERT|SP_REPLACE|ownoff-1), v_code);	/* more replace */
		    show_pinin((short) (SP_INSERT|SP_REPLACE|ownoff), en_code);	/* replace */
		    return(1);
		}
		else
		    return(5);
	    }
	}
	return(5);
    }
    else if (ownoff != 0) {
	if (inkey == ' ' || inkey == '1' || inkey == '6')
		code = 32+TONE_COUNT;
	else if (inkey	== '2' || inkey == '7')
		code = 33+TONE_COUNT;
	else if (inkey	== '3' || inkey == '8')
		code = 34+TONE_COUNT;
	else if (inkey	== '4' || inkey == '9')
		code = 35+TONE_COUNT;
	else if (inkey	== '5' || inkey == '0')
		code = 36+TONE_COUNT;
	else
		return(5);

	nowkey_buf[++PininPos] = inkey;
	ownoff++;
	show_pinin((short) (SP_INSERT|ownoff), code);
	return(4);
    }
    else
	return(5);
}

/********************************************************************
Function : PininDelete
Purpose  : back and delete a pinin symbol
********************************************************************/
void	PininDelete()
{
	BYTE	r;
	short	i;

	i = (DisplayMode != DISPLAY_ALPHA) ? PininPos - pos_buf[ownoff-1]+1 : 1;
	for ( ; i>0; i--) {
	    if ((r = rule_buf[PininPos--]) != 99) {    /* is not special rule? */
		if (pinin[r][3] != -1) {   /* append */
		    ownoff--;
		    show_pinin((short) (SP_DELETE|ownoff), 0);
		}
		if (pinin[r][2] != -1)	   /* replace */
		    show_pinin((short) (SP_DELETE|SP_REPLACE|ownoff), pinin[r][1]);
	    }
	    else {
		show_pinin((short) (SP_DELETE|SP_REPLACE|ownoff-1), i_code);   /* more replace */
		show_pinin((short) (SP_DELETE|SP_REPLACE|ownoff), o_code);   /* replace */
	    }
	}
}

short	Code2Level(char code)
{
    if (code < i_code)
	return(1);
    else if (code < a_code)
	return(2);
    else if (code < t1_code)
	return(3);
    else
	return(4);
}
