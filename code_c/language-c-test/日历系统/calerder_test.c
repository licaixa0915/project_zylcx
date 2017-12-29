#include <stdio.h>

/************************************************************
 *函数名称：farsight_isLeap
 *函数描述：判断当前输入的年是不是润年
 *函数入参：int iYear 输入的年
 *函数出参：无
 *函数返回值：1表示是润年   0表示不是润年
 *作者：licaixia
 *创建时间：2017年11月11日
************************************************************/
int farsight_isLeap(int iYear)
{
    if(((iYear % 4 == 0) && (iYear % 100 != 0)) || (iYear % 400 == 0))  
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/***********************************************************
 *函数名称：farsight_getDayByYearMonth
 *函数描述：判断当前输入的年月日是属于这一年的第多少天
 *函数入参：int iYear 输入的年
 *          int iMonth 输入的月
 *          int iDay 输入的日
 *函数出参：无
 *函数返回值：int 是这一年的第多少天
 *作者：licaixia
 *创建时间：2017年11月11日
 ***********************************************************/
int farsight_getDayByYearMonth(int iYear, int iMonth)
{
    int aiMaxDay[12] = {31,28,31,30,31,40,31,31,30,31,30,31};

    /* 如果是润年，二月分为29天 */
    if (1 == farsight_isLeap(iYear))
    {
        aiMaxDay[1] = 29;
    }

    return aiMaxDay[iMonth - 1];
}

/***********************************************************
 *函数名称：farsight_calcTotalDay
 *函数描述：判断当前输入的年月日是属于这一年的第多少天
 *函数入参：int iYear 输入的年
 *          int iMonth 输入的月
 *          int iDay 输入的日
 *函数出参：无
 *函数返回值：int 是这一年的第多少天
 *作者：licaixia
 *创建时间：2017年11月11日
 ***********************************************************/
int farsight_calcTotalDay(int iYear, int iMonth, int iDay)
{
    int iTotal = 0;
    int i;

    for (i = 1; i <= iMonth - 1; i++)
    {
        iTotal += farsight_getDayByYearMonth(iYear, i);
    }

    iTotal += iDay;
    return iTotal;
}

/***********************************************************
 *函数名称：farsight_dispTotalDay
 *函数描述：判断当前输入的年月日是属于这一年的第多少天
 *函数入参：int iYear 输入的年
 *          int iMonth 输入的月
 *          int iDay 输入的日
 *函数出参：无
 *函数返回值：void
 *作者：licaixia
 *创建时间：2017年11月11日
 ***********************************************************/
void farsight_dispTotalDay(int iYear, int iMonth, int iDay)
{
    int iTotalDay = 0;

    iTotalDay = farsight_calcTotalDay(iYear, iMonth, iDay);
    printf("%d-%d-%d is %d day of the year\n",iYear, iMonth, iDay, iTotalDay);
    printf("\n");
    return;
}

/************************************************************
 *函数名称：farsight_calcWeek
 *函数描述： 根据输入的年月日判断是星期几
 *函数入参：int iYear 输入的年
 *          int iMonth 输入的月
 *          int iDay 输入的日
 *函数出参：无
 *函数返回值：int 是星期几 0～6，分别表示星期日～星期六
 *作者：licaixia
 *创建时间：2017年11月11日
 ***********************************************************/
int farsight_calcWeek(int iYear, int iMonth, int iDay)
{
    int iWeek = 0;

    if (iMonth <= 2)
    {
        iMonth += 12;
        iYear -= 1;
    }

    iWeek = (iDay + 2 * iMonth + 3 * (iMonth + 1) / 5 + iYear + iYear / 4 - iYear / 100 + iYear / 400 + 1) % 7;
    return iWeek;
}

/************************************************************
 *函数名称：farsight_displayWeekInfo
 *函数描述：根据输入的年月日将星期的信息打印出来
 *函数入参：int iYear 输入的年
 *          int iMonth 输入的月
 *          int iDay 输入的日
 *函数出参：无
 *函数返回值：void
 *作者：licaixia
 *创建时间：2017年11月11日
 ***********************************************************/
void farsight_displayWeekInfo(int iYear, int iMonth, int iDay)
{
    char *apWeekInfo[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    int iWeekInfo;

    iWeekInfo = farsight_calcWeek(iYear, iMonth, iDay);
    if (iWeekInfo < 0 || iWeekInfo > 6)
    {
        printf("farsight_displayWeek call farsight_calcWeek error, iWeekInfo = %d.", iWeekInfo);
        return;
    }

    printf("%d-%d-%d is %s", iYear, iMonth, iDay, apWeekInfo[iWeekInfo]);
    printf("\n");
    return;
}

/***********************************************************
 *函数名称：farsight_dispMonthInfo
 *函数描述：判断当前输入的年月日是属于这一年的第多少天
 *函数入参：int iYear 输入的年
 *          int iMonth 输入的月
 *          int iDay 输入的日
 *函数出参：无
 *函数返回值：void
 *作者：licaixia
 *创建时间：2017年11月11日
 ***********************************************************/
void farsight_dispMonthInfo(int iYear, int iMonth, int iDay)
{
    int iWeekInfo;
    int i;
    int iDays = 0;

    /*获得每个月1号是星期几*/
    iWeekInfo = farsight_calcWeek(iYear, iMonth, 1);
    
    printf("\t\t%d year %d month\n", iYear, iMonth);
    printf("SUN\tMON\tTUE\tWED\tTHU\tFRI\tSAT\n");
    
    /*打印日历前面的空格*/
    for (i = 0; i < iWeekInfo; i++)
    {
        printf("\t");   
    }

    /* 获取当月的天数 */
    iDays = farsight_getDayByYearMonth(iYear, iMonth);
    for (i = 1; i <= iDays; i++)
    {
        if (i == iDay)
        {
            printf("\033[44;37m");
            printf("%d", i);
            printf("\033[0m");
            printf("\t");
        }
        else
        {
            printf("%d\t", i);   
        }

        if((iWeekInfo + i) % 7 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    return;
}

int main(int argc, const char *argv[])
{
    int iYear, iMonth, iDay;

    printf("please input [year month day] : ");
    scanf("%d%d%d",&iYear,&iMonth,&iDay);

    /* 若是输入非法，则退出*/
    if((iMonth < 1) || (iMonth > 12) || (iDay < 1) || (iDay > 31))
    {
        printf("Input date is error!\n");       
        return -1;
    }

    farsight_displayWeekInfo(iYear, iMonth, iDay);
    farsight_dispTotalDay(iYear, iMonth, iDay);
    farsight_dispMonthInfo(iYear, iMonth, iDay);
    return 0;
}
