#include <stdio.h>

/************************************************************
 *�������ƣ�farsight_isLeap
 *�����������жϵ�ǰ��������ǲ�������
 *������Σ�int iYear �������
 *�������Σ���
 *��������ֵ��1��ʾ������   0��ʾ��������
 *���ߣ�licaixia
 *����ʱ�䣺2017��11��11��
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
 *�������ƣ�farsight_getDayByYearMonth
 *�����������жϵ�ǰ�������������������һ��ĵڶ�����
 *������Σ�int iYear �������
 *          int iMonth �������
 *          int iDay �������
 *�������Σ���
 *��������ֵ��int ����һ��ĵڶ�����
 *���ߣ�licaixia
 *����ʱ�䣺2017��11��11��
 ***********************************************************/
int farsight_getDayByYearMonth(int iYear, int iMonth)
{
    int aiMaxDay[12] = {31,28,31,30,31,40,31,31,30,31,30,31};

    /* ��������꣬���·�Ϊ29�� */
    if (1 == farsight_isLeap(iYear))
    {
        aiMaxDay[1] = 29;
    }

    return aiMaxDay[iMonth - 1];
}

/***********************************************************
 *�������ƣ�farsight_calcTotalDay
 *�����������жϵ�ǰ�������������������һ��ĵڶ�����
 *������Σ�int iYear �������
 *          int iMonth �������
 *          int iDay �������
 *�������Σ���
 *��������ֵ��int ����һ��ĵڶ�����
 *���ߣ�licaixia
 *����ʱ�䣺2017��11��11��
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
 *�������ƣ�farsight_dispTotalDay
 *�����������жϵ�ǰ�������������������һ��ĵڶ�����
 *������Σ�int iYear �������
 *          int iMonth �������
 *          int iDay �������
 *�������Σ���
 *��������ֵ��void
 *���ߣ�licaixia
 *����ʱ�䣺2017��11��11��
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
 *�������ƣ�farsight_calcWeek
 *���������� ����������������ж������ڼ�
 *������Σ�int iYear �������
 *          int iMonth �������
 *          int iDay �������
 *�������Σ���
 *��������ֵ��int �����ڼ� 0��6���ֱ��ʾ�����ա�������
 *���ߣ�licaixia
 *����ʱ�䣺2017��11��11��
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
 *�������ƣ�farsight_displayWeekInfo
 *������������������������ս����ڵ���Ϣ��ӡ����
 *������Σ�int iYear �������
 *          int iMonth �������
 *          int iDay �������
 *�������Σ���
 *��������ֵ��void
 *���ߣ�licaixia
 *����ʱ�䣺2017��11��11��
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
 *�������ƣ�farsight_dispMonthInfo
 *�����������жϵ�ǰ�������������������һ��ĵڶ�����
 *������Σ�int iYear �������
 *          int iMonth �������
 *          int iDay �������
 *�������Σ���
 *��������ֵ��void
 *���ߣ�licaixia
 *����ʱ�䣺2017��11��11��
 ***********************************************************/
void farsight_dispMonthInfo(int iYear, int iMonth, int iDay)
{
    int iWeekInfo;
    int i;
    int iDays = 0;

    /*���ÿ����1�������ڼ�*/
    iWeekInfo = farsight_calcWeek(iYear, iMonth, 1);
    
    printf("\t\t%d year %d month\n", iYear, iMonth);
    printf("SUN\tMON\tTUE\tWED\tTHU\tFRI\tSAT\n");
    
    /*��ӡ����ǰ��Ŀո�*/
    for (i = 0; i < iWeekInfo; i++)
    {
        printf("\t");   
    }

    /* ��ȡ���µ����� */
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

    /* ��������Ƿ������˳�*/
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
