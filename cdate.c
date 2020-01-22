/*
 * show Chinese lunisolar calender date.
 * (c) 2011 vinoca <supoingo@163.com>
 * Licensed under the BSD
 */

#define VERSION	"1.0.5t"
#define LEN	64
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "chinese_calender.h"
#define DIM(x)	((int)(sizeof(x) / sizeof(*x)))

typedef struct
{
	int date;
	char name[LEN];
} datepoint_t;

/* 四柱、節氣、節日名稱 {{{ */
static const char *gan[] = {"甲", "乙", "丙", "丁", "戊",
	 			"己", "庚", "辛", "壬", "癸"};

static const char *zhi[] = {"子", "丑", "寅", "卯", "辰", "巳",
				 "午", "未", "申", "酉", "戌", "亥"};

static const char *czodiac[] = {"鼠", "牛", "虎", "兔", "龍", "蛇",
					 "馬", "羊", "猴", "雞", "狗", "豬"};

static const char *jieqi_name[] = {
	 		"小寒", "大寒", "立春", "雨水", "驚蟄", "春分",
			"清明", "穀雨", "立夏", "小滿", "芒種", "夏至",
			"小暑", "大暑", "立秋", "處暑", "白露", "秋分",
			"寒露", "霜降", "立冬", "小雪", "大雪", "冬至"};

static const char *date_name1[] = {"初", "十", "廿", "三", "大", "小",
	 					"閏",""};

static const char *date_name2[] = {"十", "一", "二", "三", "四", "五",
	 		"六", "七", "八", "九", "十", "十一", "臘", "正"};

static const char *weekday[] = {"一","二","三","四","五","六","日"};

/* 農曆節日 */
static const datepoint_t lunar_festival[] =
{
	{101,	"春節"},
	{115,	"元宵"},
	{202,	"龍擡頭"},
	{408,	"浴佛節"},
	{505,	"端午"},
	{707,	"七夕"},
	{715,	"中元"},
	{815,	"中秋"},
	{909,	"重陽"},
	{1208,	"臘八"},
	{1223,	"小年"},
	{100,	"除夕"}
};

/* 公曆節日 */

static const datepoint_t festival[] =
{
	{101,	"元旦"},
	{228,	"和平紀念日"},
	{308,	"婦女節"},
	{312,	"國父逝世紀念日"},
	{314,	"反侵略日"},
	{329,	"革命先烈紀念日/青年節"},
	{404,	"兒童節/清明節1"},
	{405,	"清明節2"},
	{407,	"言論自由日"},
	{501,	"勞動節"},
	{715,	"解嚴紀念日"},
	{903,	"軍人節"},
	{928,	"教師節"},
	{1010,	"中華民國國慶日"},
	{1024,	"聯合國日"},
	{1025,	"臺灣光復節"},
	{1031,	"萬聖節"},
	{1112,	"國父誕辰紀念日/中華文化復興節"},
	{1225,	"行憲紀念日/聖誕節"}
};

static void show_help(void)
{
	printf(
	"  cdate，顯示中國傳統農曆\n"
	"  用法：\n"
	"  cdate [-a | -g | -z | -n | -d date | date ]\n"
	"  選項：\n"
	"  -a	顯示所有項\n"
	"  -g	顯示公曆年月日\n"
	"  -z	顯示四柱\n"
	"  -n	不顯示農曆日曆\n"
	"  -d	指定公曆年月日時，格式 YYYYMMDDHH，\n"
	"    	年份在 1900 與 2050 之間取值，如 cdate -d 2050010100 \n\n"
	"  -h	顯示此幫助\n"
	"  版本："VERSION"\t回饋及提問： www.vinoca.org\n\n"
);
	exit(1);
}

int main(int argc, char* argv[])
{
	struct c_calender d;
	struct tm *lt;
	time_t tp;

	int i;
	int year,month,day,hour;
	int show_all = 0;
	int show_g_date = 0;
	int not_show_c_cal = 0;
	int show_sizhu = 0;
	int x = -1;

	for (i=1; i<argc && argv[i][0]=='-'; i++)
		switch(argv[i][1]) {
		case 'a': show_all = 1; break;
		case 'd':
			if (++i < argc && isdigit(*argv[i]))
				x = atoi(argv[i]);
			else show_help();
			break;
		case 'g': show_g_date = 1; break;
		case 'h': show_help(); break;
		case 'n': not_show_c_cal = 1; break;
		case 'z': show_sizhu = 1; break;
		default: show_help(); break;
		 }

	if (argc == 2 && argv[1][0] != '-') {
		if (isdigit(*argv[1]))
			x = atoi(argv[1]);
		else
			show_help();
	}

	if (x >= 0) {
		year = x / 1000000;
		month = (x % 1000000) / 10000;
		day = (x % 10000) / 100;
		hour = x % 100;
	} else {
		time(&tp);
		lt = localtime(&tp);
		year = 1900 + lt->tm_year;
		month = 1 + lt->tm_mon;
		day = lt->tm_mday;
		hour = lt->tm_hour;
	}

	if(chinese_calender(year, month, day, hour, &d)) show_help();

	 /* 顯示公曆 */
	 if (show_all || show_g_date) {
		x = month;
		i = year;

		if (month == 1 || month == 2) {
			x = month + 12;
			i = year - 1;
		}
		
		printf("%d年%d月%d日 星期%s\n",
			year,month,day, weekday[(day + 2 * x + 3 * 
			(x + 1) / 5 + i + i / 4 - i / 100 + i / 400) % 7]);
	 }
	
	 /* 顯示農曆年月日 */
	if (show_all || !not_show_c_cal) {

		printf("%s%s",
			gan[(d.sizhu >> 24) % 10],zhi[(d.sizhu >> 24) % 12]);

		printf("%s年%s",
			czodiac[(d.sizhu >> 24) % 12],
			date_name1[(d.month_stat & 1)?6:7]);

		printf("%s月%s",
			date_name2[(d.month == 1)?d.month + 12:d.month],
			date_name1[(d.month_stat >> 1 & 1)?4:5]);

		printf("%s%s\t",
			date_name1[(d.day == 10)?0:d.day / 10],
			date_name2[d.day % 10]);

		/* 判斷“除夕” */
		if (d.month == 12) {
			if ((d.month_stat >> 1 & 1) == 1 && d.day == 30)
				x = 100;
			else if ((d.month_stat & 1) == 0 && d.day == 29)
				x = 100;
			else
				x = d.month * 100 + d.day;
		} else
			x = d.month * 100 + d.day;

		/* 顯示節氣、節日 */
		if ((d.month_stat >> 8) < 24)
			printf("%s ",jieqi_name[d.month_stat >> 8]);

		for (i=0; i < DIM(lunar_festival); i++)
			if (x == lunar_festival[i].date) printf("%s ", lunar_festival[i].name);

		for (i=0; i < DIM(festival); i++) {
			if ((month * 100 + day) == festival[i].date)
				printf("%s ",festival[i].name);
		}
		printf("\n");
	}

	/* 顯示四柱 */
	if (show_all || show_sizhu) {
		printf("%s%s年",
			gan[(d.sizhu >> 24) % 10],
			zhi[(d.sizhu >> 24) % 12]);

		printf("%s%s月",
			gan[((d.sizhu >> 16) & 255) % 10],
			zhi[((d.sizhu >> 16) & 255) % 12]);

		printf("%s%s日",
			gan[((d.sizhu >> 8) & 255) % 10],
			zhi[((d.sizhu >> 8) & 255) % 12]);

		printf("%s%s時\n",
			gan[(d.sizhu & 255) % 10],
			zhi[(d.sizhu & 255) % 12]);
	}

	exit(EXIT_SUCCESS);
}

