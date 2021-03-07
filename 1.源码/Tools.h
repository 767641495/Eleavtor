#include "Structure.h"
const int timeinout = 5;  //进出时间
const int timeupdown = 5; //上下楼时间
const int AllLength = 60;
int AboveRequest; //[当前层，最高层]的请求人数
int UnderRequest; //[最底层，当前层)的请求人数
int Time;         //总时间
Floor floor[13];
int RuleCheck[20];
int tempCheck[20];
int mp[200][200];

//文件导入
void Fprint(Elevator *ele)
{
    FILE *fp;

    if ((fp = fopen("input.txt", "r")) == NULL)
    {
        system("cls");
        fflush(stdin);
        printf("输入文件不存在!\n请创建一个\"input.txt\"文件！\n");
        system("PAUSE");
        return;
    }
    int n;
    fscanf(fp, "%d", &n);
    for (int i = 0; i < n; i++)
    {
        Passenger person;
        fscanf(fp, "%d %d %d %d", &person.InitialFloor, &person.TargetFloor, &person.Weight, &person.Patient);
        person.InitialTime = Time;
        if (person.TargetFloor >= person.InitialFloor)
        {
            EnQueue(&(floor[person.InitialFloor].UpQueue), person);
            floor[person.InitialFloor].UpNum++;
        }
        else
        {
            EnQueue(&(floor[person.InitialFloor].DownQueue), person);
            floor[person.InitialFloor].DownNum++;
        }

        if ((*ele).Mode == 0)
        {
            (*ele).NowStayTime = 0;
            if (person.InitialFloor >= (*ele).NowFloor)
                (*ele).Mode = 1;
            else
                (*ele).Mode = 2;
        }

        if (person.InitialFloor > (*ele).NowFloor)
            AboveRequest++;
        if (person.InitialFloor < (*ele).NowFloor)
            UnderRequest++;
    }
    fclose(fp);
}

void PrintEle(Elevator *ele)
{
    if ((*ele).WeightWarn == 1)
    {
        printf("警报已被触发！！！");
    }
    for (int i = 0; i <= 7 * (*ele).MaxFloor + 1; i++)
    {
        for (int j = 0; j <= AllLength + 1; j++)
        {
            switch (mp[i][j])
            {
            case -11:
                printf("  ");
                break;
            case -10:
                printf("P ");
                break;
            case -9:
                printf("▼");
                break;
            case -8:
                printf("♀");
                break;
            case -7:
                printf("▲");
                break;
            case -6:
                printf("F ");
                break;
            case -5:
                printf("W ");
                break;
            case -4:
                printf("==");
                break;
            case -3:
                printf("| ");
                break;
            case -2:
                printf("■");
                break;
            case -1:
                printf("  ");
                break;
            default:
                printf("%02d", mp[i][j]);
                break;
            }
        }
        if (i <= 16)
        {
            if (tempCheck[i] == 1)
                printf(" √ ");
            else
                printf("    ");
        }

        switch (i)
        {
        case 0:
            printf("若楼上还有人有请求或者电梯里还有人，上升");
            break;
        case 1:
            printf("若楼下还有人有请求并且电梯里没人，下降");
            break;
        case 2:
            printf("若楼下还有人有请求或者电梯里还有人，继续下降");
            break;
        case 3:
            printf("若楼上还有人有请求并且电梯里没人，上升");
            break;
        case 4:
            printf("若没有楼层有请求并且电梯里没人，等待");
            break;
        case 5:
            printf("当没有用户请求300秒后，电梯将回到1层待命");
            break;
        case 6:
            printf("当一个人等候电梯时间超过耐心值，会离开");
            break;
        case 7:
            printf("当电梯超载时，部分乘客将无法进入");
            break;
        case 8:
            printf("每隔100秒，用户可选择生成乘客方式");
            break;
        case 9:
            printf("当有乘客要进出的时候，门打开");
            break;
        case 10:
            printf("当没乘客要进出的时候，门关闭");
            break;
        case 11:
            printf("动态显示人的进出");
            break;
        case 12:
            printf("动态显示电梯的警报");
            break;
        case 13:
            printf("动态显示电梯的升降");
            break;
        case 14:
            printf("实时显示时间");
            break;
        case 15:
            printf("电梯在超重后，将会报警");
            break;
        case 16:
            printf("电梯在超载或者停留过久后，将会报警");
            break;
        default:
            break;
        }
        if (i <= 16 && RuleCheck[i] == 1)
            printf(" √ ");
        puts("");
    }
}

//打印当前时间
void PrintTime(int Time)
{
    int h, min, sec;
    sec = Time % 60;
    min = Time / 60;
    h = Time / (60 * 60);
    printf("目前的运行时间为%02d:%02d:%02d\n", h, min, sec);
    tempCheck[14] = RuleCheck[14] = 1;
}

void InitMp(Elevator *ele)
{
    //空格对应-1" " 除了门全部清空
    for (int i = 0; i <= 7 * (*ele).MaxFloor + 1; i++)
        for (int j = 0; j <= AllLength + 1; j++)
            if (!(i % 7 != 0 && j == AllLength / 2 + 1))
                mp[i][j] = -1;

    //头尾封墙,墙对应-2 "■"
    for (int j = 0; j <= AllLength + 1; j++)
        mp[0][j] = mp[7 * (*ele).MaxFloor + 1][j] = -2;

    //左封墙
    for (int i = 0; i <= 7 * (*ele).MaxFloor + 1; i++)
        mp[i][0] = -2;

    //构建楼层
    for (int i = 7; i <= 7 * (*ele).MaxFloor; i += 7)
    {
        for (int j = AllLength / 2 + 1; j <= AllLength; j++)
            mp[i][j] = -2;
        mp[i][AllLength + 1] = (*ele).MaxFloor - i / 7 + 1;
    }
    for (int i = 1; i <= (*ele).MaxFloor; i++) //楼层数
    {
        //打印上下按钮
        mp[(ele->MaxFloor + 1 - i) * 7 - 5][AllLength / 2 + 2] = -7; //"▲"对应-7
        mp[(ele->MaxFloor + 1 - i) * 7 - 2][AllLength / 2 + 2] = -9;
    }
}

void test(Elevator *ele)
{
    printf("目前上升请求数：%d\n目前下降请求数：%d\n", AboveRequest, UnderRequest);
    for (int i = (*ele).MaxFloor; i >= 1; i--)
    {
        printf("第%d层的上升num为%d,下降num为%d.\n", i, floor[i].UpNum, floor[i].DownNum);
    }
}

void printIn(Elevator *ele, Passenger p)
{
    RuleCheck[11] = 1;
    for (int i = 1; i <= 4; i++)
    {
        //system("pause");
        Sleep(500);
        system("cls");
        //回溯

        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 3][AllLength / 2 - i - 1] = -11; //W对应-5"W"
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 3][AllLength / 2 - i + 2] = -11;

        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 2][AllLength / 2 - i + 1] = -11; //F -6
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 2][AllLength / 2 - i + 2] = -11;

        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 1][AllLength / 2 - i + 1] = -11;

        //动态移动
        //打印体重
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 3][AllLength / 2 - i] = -5; //W对应-5"W"
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 3][AllLength / 2 - i + 1] = p.Weight;
        //打印目标楼层
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 2][AllLength / 2 - i] = -6; //F -6
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 2][AllLength / 2 - i + 1] = p.TargetFloor;

        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 1][AllLength / 2 - i] = -8; //人
        PrintEle(ele);
        puts("");
        PrintTime(Time + i);
        printf("电梯人数：%d/8\n", (*ele).NowPassengerNumber);
        switch ((*ele).Mode)
        {
        case 0:
            printf("电梯目前为等待状态\n");
            break;
        case 1:
            printf("电梯目前为上升状态\n");
            break;
        case 2:
            printf("电梯目前为下降状态\n");
            break;
        default:
            printf("电梯状态出错!\n");
            return;
        }
        printf("目前有乘客正在进电梯\n");
    }
}

void printOut(Elevator *ele, Passenger p)
{
    tempCheck[11] = RuleCheck[11] = 1;
    for (int i = 4; i >= 1; i--)
    {
        //system("pause");
        Sleep(500);
        system("cls");
        //回溯

        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 3][AllLength / 2 - i - 1] = -11; //W对应-5"W"
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 3][AllLength / 2 - i] = -11;

        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 2][AllLength / 2 - i - 1] = -11; //F -6
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 2][AllLength / 2 - i] = -11;

        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 1][AllLength / 2 - i - 1] = -11;

        //动态移动
        //打印体重
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 3][AllLength / 2 - i] = -5; //W对应-5"W"
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 3][AllLength / 2 - i + 1] = p.Weight;
        //打印目标楼层
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 2][AllLength / 2 - i] = -6; //F -6
        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 2][AllLength / 2 - i + 1] = p.TargetFloor;

        mp[7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 1][AllLength / 2 - i] = -8; //人
        PrintEle(ele);
        puts("");
        PrintTime(Time + 5 - i);
        printf("电梯人数：%d/8\n", (*ele).NowPassengerNumber);
        switch ((*ele).Mode)
        {
        case 0:
            printf("电梯目前为等待状态\n");
            break;
        case 1:
            printf("电梯目前为上升状态\n");
            break;
        case 2:
            printf("电梯目前为下降状态\n");
            break;
        default:
            printf("电梯状态出错!\n");
            return;
        }
        printf("目前有乘客正在出电梯\n");
    }
}

void printUp(Elevator *ele)
{
    tempCheck[13] = RuleCheck[13] = 1;
    for (int i = 1; i <= 5; i++)
    {
        //system("pause");
        Sleep(500);
        system("cls");

        //动态移动
        for (int k = 6; k >= 0; k--)
        {
            for (int j = 1; j <= AllLength / 2; j++)
            {
                mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - k - i][j] = mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - k + 1 - i][j];
            }
        }
        //回溯
        for (int j = 1; j <= AllLength / 2; j++)
        {
            mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - (-1) - i][j] = -11;
        }
        PrintEle(ele);
        puts("");
        PrintTime(Time + i);
        printf("电梯人数：%d/8\n", (*ele).NowPassengerNumber);
        switch ((*ele).Mode)
        {
        case 0:
            printf("电梯目前为等待状态\n");
            break;
        case 1:
            printf("电梯目前为上升状态\n");
            break;
        case 2:
            printf("电梯目前为下降状态\n");
            break;
        default:
            printf("电梯状态出错!\n");
            return;
        }
        printf("电梯正在移动！\n");
    }
}

void printDown(Elevator *ele)
{
    tempCheck[13] = RuleCheck[13] = 1;
    for (int i = 1; i <= 5; i++)
    {
        //system("pause");
        Sleep(500);
        system("cls");

        //动态移动
        //mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7][j] = -4; //楼层对应-4 "="
        for (int k = 0; k <= 6; k++)
        {
            for (int j = 1; j <= AllLength / 2; j++)
            {
                mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 + i - k][j] = mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 + i - 1 - k][j];
            }
        }
        //回溯
        for (int j = 1; j <= AllLength / 2; j++)
            mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 + i - 7][j] = -11;
        PrintEle(ele);
        puts("");
        PrintTime(Time + i);
        printf("电梯人数：%d/8\n", (*ele).NowPassengerNumber);
        switch ((*ele).Mode)
        {
        case 0:
            printf("电梯目前为等待状态\n");
            break;
        case 1:
            printf("电梯目前为上升状态\n");
            break;
        case 2:
            printf("电梯目前为下降状态\n");
            break;
        default:
            printf("电梯状态出错!\n");
            return;
        }
        printf("电梯正在移动！\n");
    }
}

void PrintAll(Elevator *ele)
{
    //system("pause");
    Sleep(50);
    system("cls");
    InitMp(ele);
    int temp[13] = {0};
    //打印电梯
    //打印电梯底座
    for (int j = 1; j <= AllLength / 2; j++)
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7][j] = -4; //楼层对应-4 "="

    //打印电梯里的上升队列人
    QueuePtr p = (*ele).UpQueue.front;
    int stpos = 4;
    //上下按钮
    mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 5][stpos - 3] = -7; //"▲"对应-7
    mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 2][stpos - 3] = -9; //"▼"对应-9
    while (p && p->next)
    {
        //打印体重
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 6][stpos] = -5; //W对应-5"W"
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 6][stpos + 1] = p->next->people.Weight;
        //打印目标楼层
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 5][stpos] = -6; //F对应-6"F"
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 5][stpos + 1] = p->next->people.TargetFloor;

        //打印人
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 4][stpos + 1] = -8; //"♀"对应-8
        //改变stpos
        stpos += 2;
        p = p->next;
    }

    stpos = 4;
    //打印电梯里的下降队列人
    p = (*ele).DownQueue.front;
    while (p && p->next)
    {
        //打印体重
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 3][stpos] = -5; //W对应-5
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 3][stpos + 1] = p->next->people.Weight;
        //打印目标楼层
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 2][stpos] = -6; //F对应-6"F"
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 2][stpos + 1] = p->next->people.TargetFloor;
        //打印人
        mp[((*ele).MaxFloor - (*ele).NowFloor + 1) * 7 - 1][stpos + 1] = -8; //"#"对应-8
        //改变stpos
        stpos += 2;
        p = p->next;
    }

    //打印楼层里的人
    for (int i = 1; i <= (*ele).MaxFloor; i++) //楼层数
    {
        //打印楼层里的上升队列人
        p = floor[i].UpQueue.front;
        stpos = AllLength / 2 + 5;
        while (p->next)
        {
            //将耐心值超限的人移除
            if (p->next->people.Patient != -1 && p->next->people.Patient + p->next->people.InitialTime < Time)
            {
                QueuePtr q = p->next;
                if (q->people.InitialFloor > (*ele).NowFloor)
                    AboveRequest--;
                else if (q->people.InitialFloor < (*ele).NowFloor)
                    UnderRequest--;
                p->next = q->next;
                temp[q->people.InitialFloor]++;
                free(q);
                floor[i].UpNum--;
                continue;
            }

            //打印体重
            mp[(ele->MaxFloor + 1 - i) * 7 - 6][stpos] = -5; //W对应-5"W"
            mp[(ele->MaxFloor + 1 - i) * 7 - 6][stpos + 1] = p->next->people.Weight;
            mp[(ele->MaxFloor + 1 - i) * 7 - 6][stpos + 2] = -1;
            //打印目标楼层
            mp[(ele->MaxFloor + 1 - i) * 7 - 5][stpos] = -10; //P -10
            mp[(ele->MaxFloor + 1 - i) * 7 - 5][stpos + 1] = p->next->people.Patient;
            mp[(ele->MaxFloor + 1 - i) * 7 - 5][stpos + 2] = -1; // -1为空格
            //打印人
            mp[(ele->MaxFloor + 1 - i) * 7 - 4][stpos] = -8;     //"♀"对应-8
            mp[(ele->MaxFloor + 1 - i) * 7 - 4][stpos + 2] = -1; // -1为空格
            //改变stpos
            stpos += 3;
            p = p->next;
        }

        //打印楼层里的下降队列人
        p = floor[i].DownQueue.front;
        stpos = AllLength / 2 + 5;
        while (p->next)
        {
            //将耐心值超限的人移除
            if (p->next->people.Patient != -1 && p->next->people.Patient + p->next->people.InitialTime < Time)
            {
                QueuePtr q = p->next;
                if (q->people.InitialFloor > (*ele).NowFloor)
                    AboveRequest--;
                else if (q->people.InitialFloor < (*ele).NowFloor)
                    UnderRequest--;
                p->next = q->next;
                temp[q->people.InitialFloor]++;
                free(q);
                floor[i].DownNum--;
                continue;
            }

            //打印体重
            mp[(ele->MaxFloor + 1 - i) * 7 - 3][stpos] = -5;
            mp[(ele->MaxFloor + 1 - i) * 7 - 3][stpos + 1] = p->next->people.Weight;
            mp[(ele->MaxFloor + 1 - i) * 7 - 3][stpos + 2] = -1;
            //打印目标楼层
            mp[(ele->MaxFloor + 1 - i) * 7 - 2][stpos] = -10;
            mp[(ele->MaxFloor + 1 - i) * 7 - 2][stpos + 1] = p->next->people.Patient;
            mp[(ele->MaxFloor + 1 - i) * 7 - 2][stpos + 2] = -1;
            //打印人
            mp[(ele->MaxFloor + 1 - i) * 7 - 1][stpos] = -8;
            mp[(ele->MaxFloor + 1 - i) * 7 - 1][stpos + 2] = -1;
            //改变stpos
            stpos += 3;
            p = p->next;
        }
    }
    //输出
    puts("");
    PrintEle(ele);
    puts("");
    PrintTime(Time);
    for (int i = 1; i <= (*ele).MaxFloor; i++)
    {
        if (temp[i] != 0)
        {
            printf("第%d层有%d位乘客因为耐心值超限而离开!!!\n", i, temp[i]);
            RuleCheck[6] = 1;
        }
    }
    printf("电梯人数：%d/8\n", (*ele).NowPassengerNumber);
    switch ((*ele).Mode)
    {
    case 0:
        printf("电梯目前为等待状态\n");
        break;
    case 1:
        printf("电梯目前为上升状态\n");
        break;
    case 2:
        printf("电梯目前为下降状态\n");
        break;
    default:
        printf("电梯状态出错!\n");
        return;
    }

    //test(ele);
    for (int i = 0; i < 19; i++)
        tempCheck[i] = 0;
}

//电梯内的乘客出电梯
void GetOut(Elevator *ele, Floor *flo)
{
    //开始记录当层等待时间
    (*ele).NowWaitTime = 0;
    //删除门
    int st = 7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 1;
    for (int i = st; i >= st - 6; i--)
        mp[i][AllLength / 2 + 1] = -11;
    //1.处理电梯上升队列
    QueuePtr p = (*ele).UpQueue.front;
    //从电梯里的第一个乘客遍历到最后一个
    while (p && p->next)
    {
        QueuePtr q = p->next;
        //如果该乘客要在此楼下来，删除该节点
        if (q->people.TargetFloor == (*ele).NowFloor)
        {
            tempCheck[9] = RuleCheck[9] = 1;
            q->people.Patient = -1;
            //删除这个节点
            p->next = q->next;

            //打印
            PrintAll(ele);

            //动态显示
            printOut(ele, q->people);

            //加入该楼层上升队列的头
            EnFront(&((*flo).UpQueue), q->people);

            //电梯总人数--
            (*ele).NowPassengerNumber--;
            //减少总重
            (*ele).NowWeight -= q->people.Weight;
            //该楼层总人数++
            (*flo).UpNum++;
            //增加总时间
            Time += timeinout;
            //增加等待时间
            (*ele).NowWaitTime += timeinout;

            //开启时间警报
            if ((*ele).NowWaitTime > (*ele).MaxWaitTime)
                (*ele).TimeWarn = 1;

            //打印
            PrintAll(ele);

            //状态显示
            puts("乘客正在出电梯，门已经开启\n");

            //将该用户从该楼层上升队列出队
            DeQueue(&((*flo).UpQueue), &q->people);

            //该楼层总人数--
            (*flo).UpNum--;

            //打印
            PrintAll(ele);

            //释放这个节点
            free(q);
        }
        p = p->next;
    }

    //2.处理电梯下降队列
    p = (*ele).DownQueue.front;
    while (p && p->next)
    {
        QueuePtr q = p->next;
        //如果该乘客要在此楼下来，删除该节点
        if (q->people.TargetFloor == (*ele).NowFloor)
        {
            tempCheck[9] = RuleCheck[9] = 1;
            q->people.Patient = -1;
            //删除这个节点
            p->next = q->next;

            //打印
            PrintAll(ele);

            //动态显示
            printOut(ele, q->people);

            //加入该楼层上升队列的头
            EnFront(&((*flo).DownQueue), q->people);

            //电梯总人数--
            (*ele).NowPassengerNumber--;
            //减少总重
            (*ele).NowWeight -= q->people.Weight;
            //该楼层总人数++
            (*flo).DownNum++;
            //增加总时间
            Time += timeinout;
            //增加等待时间
            (*ele).NowWaitTime += timeinout;

            //开启时间警报
            if ((*ele).NowWaitTime > (*ele).MaxWaitTime)
                (*ele).TimeWarn = 1;

            RuleCheck[9] = 1;

            //打印
            PrintAll(ele);

            //状态显示
            puts("乘客正在出电梯，门已经开启\n");

            //将该用户从该楼层上升队列出队
            DeQueue(&((*flo).DownQueue), &q->people);

            //该楼层总人数--
            (*flo).DownNum--;

            //打印
            PrintAll(ele);

            //释放这个节点
            free(q);
        }
        p = p->next;
    }

    //回溯门
    for (int i = st; i >= st - 6; i--)
        mp[i][AllLength / 2 + 1] = -3;
    tempCheck[10] = RuleCheck[10] = 1;
}

//该层乘客进电梯
void GetIn(Elevator *ele, Floor *flo)
{
    //删除门
    int st = 7 * ((*ele).MaxFloor - (*ele).NowFloor + 1) - 1;
    for (int i = st; i >= st - 6; i--)
        mp[i][AllLength / 2 + 1] = -11;
    //电梯为上升状态
    if ((*ele).Mode == 1)
    {
        //处理楼层上升队列
        /* for (QueuePtr i = (*flo).UpQueue.front;(i!=NULL&&i->next != NULL); i = i->next)*/
        while ((*flo).UpNum > 0)
        {
            if ((*ele).NowPassengerNumber == (*ele).MaxPassengerNumber)
                break;
            QueuePtr i = (*flo).UpQueue.front;

            //删除这个节点
            QueuePtr p = i->next;
            i->next = p->next;

            tempCheck[9] = RuleCheck[9] = 1;

            //打印
            PrintAll(ele);
            //动态显示
            printIn(ele, p->people);

            //将这位乘客加入电梯队列队首
            EnFront(&((*ele).UpQueue), p->people);
            (*ele).NowWeight += p->people.Weight;
            free(p);

            //当层楼人数--
            (*flo).UpNum--;

            //增加乘客出电梯的时间
            Time += timeinout;
            //电梯内的乘客数加1
            (*ele).NowPassengerNumber++;
            //减少上升请求人数

            //特判一下有没有超重警告
            if ((*ele).NowWeight > (*ele).MaxWeight)
            {
                tempCheck[7] = RuleCheck[7] = 1;
                //启动重量警报
                (*ele).WeightWarn = 1;

                //从电梯里删除超重的那个人
                Passenger temp;
                DeQueue(&((*ele).UpQueue), &temp);
                (*ele).NowWeight -= temp.Weight;
                //重新把那个人放回该楼层原队列原位置
                QueuePtr q = (QueuePtr)malloc(sizeof(QNode));
                q->people = temp;
                q->next = i->next;
                i->next = q;
                //当层楼人数++
                (*flo).UpNum++;

                //增加总时间
                Time += timeinout;
                //增加等待时间
                (*ele).NowWaitTime += timeinout;
                //开启时间警报
                if ((*ele).NowWaitTime > (*ele).MaxWaitTime)
                    (*ele).TimeWarn = 1;
                //电梯内的乘客数减1
                (*ele).NowPassengerNumber--;
                //消除重量警报
                (*ele).WeightWarn = 0;
            }
            
            PrintAll(ele);
            //状态显示

            puts("乘客正在进电梯，门已经开启\n");
        }
    }
    else //下降状态
    {
        //处理楼层下降队列
        /*  for (QueuePtr i = (*flo).DownQueue.front; (i != NULL && i->next != NULL); i = i->next)*/
        while ((*flo).DownNum > 0)
        {
            if ((*ele).NowPassengerNumber == (*ele).MaxPassengerNumber)
                break;

            QueuePtr i = (*flo).DownQueue.front;

            //删除这个节点
            QueuePtr p = i->next;
            i->next = p->next;

            tempCheck[9] = RuleCheck[9] = 1;
            
            //打印
            PrintAll(ele);
            //动态显示
            printIn(ele, p->people);

            //将这位乘客加入电梯队列队首
            EnFront(&((*ele).DownQueue), p->people);
            (*ele).NowWeight += p->people.Weight;
            free(p);

            //当层楼人数--
            (*flo).DownNum--;
            //增加乘客出电梯的时间
            Time += timeinout;
            //电梯内的乘客数加1
            (*ele).NowPassengerNumber++;

            //特判一下有没有超重警告
            if ((*ele).NowWeight > (*ele).MaxWeight)
            {
                RuleCheck[7] = 1;
                //启动重量警报
                (*ele).WeightWarn = 1;

                //从电梯里删除超重的那个人
                Passenger temp;
                DeQueue(&((*ele).DownQueue), &temp);
                (*ele).NowWeight -= temp.Weight;

                //重新把那个人放回该楼层原队列原位置
                QueuePtr q = (QueuePtr)malloc(sizeof(QNode));
                q->people = temp;
                q->next = i->next;
                i->next = q;
                //当层楼人数++
                (*flo).DownNum++;

                //增加总时间
                Time += timeinout;
                //增加等待时间
                (*ele).NowWaitTime += timeinout;
                //开启时间警报
                if ((*ele).NowWaitTime > (*ele).MaxWaitTime)
                    (*ele).TimeWarn = 1;
                //电梯内的乘客数减1
                (*ele).NowPassengerNumber--;
                //消除重量警报
                (*ele).WeightWarn = 0;
            }
        
            PrintAll(ele);
            //状态显示
            puts("乘客正在进电梯，门已经开启\n");
        }
    }
    //等待状态不会调用这个函数，不用判断

    (*ele).TimeWarn = 0;    //取消时间警报
    (*ele).NowWaitTime = 0; //清空当前等待时间

    //回溯门
    for (int i = st; i >= st - 6; i--)
        mp[i][AllLength / 2 + 1] = -3;
    tempCheck[10] = RuleCheck[10] = 1;
}

//控制上下楼以及状态变更
void EleUpDown(Elevator *ele)
{
    //处理完(*ele).NowFloor楼层的进出后，调用此函数

    //上升
    if ((*ele).Mode == 1)
    {
        //如果全部楼层无人请求
        if (AboveRequest == 0 && UnderRequest == 0 && floor[(*ele).NowFloor].UpNum + floor[(*ele).NowFloor].DownNum == 0 && (*ele).NowPassengerNumber == 0)
        {
            //切换成等待状态
            (*ele).Mode = 0;
            tempCheck[4] = RuleCheck[4] = 1;
        }

        //上升切换下降
        else if ((*ele).NowFloor == (*ele).MaxFloor || AboveRequest == 0 && UnderRequest != 0 && floor[(*ele).NowFloor].UpNum == 0 && (*ele).NowPassengerNumber == 0)
        {
            //切换为下降状态
            (*ele).Mode = 2;
            tempCheck[1] = RuleCheck[1] = 1;
        }

        //一般情况
        else
        {
            tempCheck[0] = RuleCheck[0] = 1;
            UnderRequest = UnderRequest + floor[(*ele).NowFloor].DownNum + floor[(*ele).NowFloor].UpNum;
            printUp(ele);
            (*ele).NowFloor++;
            AboveRequest = AboveRequest - floor[(*ele).NowFloor].DownNum - floor[(*ele).NowFloor].UpNum;
            Time += timeupdown;
            PrintAll(ele);
        }
    }
    //下降
    else
    {
        //如果全部楼层无人请求
        if (AboveRequest == 0 && UnderRequest == 0 && floor[(*ele).NowFloor].UpNum + floor[(*ele).NowFloor].DownNum == 0 && (*ele).NowPassengerNumber == 0)
        {
            //切换成等待状态
            (*ele).Mode = 0;
            tempCheck[4] = RuleCheck[4] = 1;
        }

        //上升切换下降
        else if ((*ele).NowFloor == 1 || UnderRequest == 0 && AboveRequest != 0 && floor[(*ele).NowFloor].DownNum == 0 && (*ele).NowPassengerNumber == 0)
        {
            //切换为上升状态
            (*ele).Mode = 1;
            tempCheck[3] = RuleCheck[3] = 1;
        }

        //一般情况
        else
        {
            tempCheck[2] = RuleCheck[2] = 1;
            AboveRequest = AboveRequest + floor[(*ele).NowFloor].DownNum + floor[(*ele).NowFloor].UpNum;
            printDown(ele);
            (*ele).NowFloor--;
            UnderRequest = UnderRequest - floor[(*ele).NowFloor].DownNum - floor[(*ele).NowFloor].UpNum;
            Time += timeupdown;
            PrintAll(ele);
        }
    }
    //等待状态不调用此函数
}

//随机生成乘客
void RandomInput(Elevator *ele)
{
    srand(time(NULL));

    //控制生成人的概率为1/2
    if (rand() % 2 != 0)
        return;

    Passenger person;

    //生成起始楼层
    do
    {
        person.InitialFloor = rand() % (*ele).MaxFloor + 1;
    } while (person.InitialFloor == (*ele).NowFloor);

    //生成目标楼层
    do
    {
        person.TargetFloor = rand() % (*ele).MaxFloor + 1;
    } while (person.TargetFloor == person.InitialFloor);

    //生成乘客体重
    person.Weight = rand() % 60 + 40;

    //生成乘客耐心值
    person.Patient = rand() % 90 + 10;

    person.InitialTime = Time;

    if (person.TargetFloor > person.InitialFloor)
    {
        EnQueue(&(floor[person.InitialFloor].UpQueue), person);
        floor[person.InitialFloor].UpNum++;
    }
    else
    {
        EnQueue(&(floor[person.InitialFloor].DownQueue), person);
        floor[person.InitialFloor].DownNum++;
    }

    if ((*ele).Mode == 0)
    {
        (*ele).NowStayTime = 0;
        if (person.InitialFloor >= (*ele).NowFloor)
            (*ele).Mode = 1;
        else
            (*ele).Mode = 2;
    }

    if (person.InitialFloor > (*ele).NowFloor)
        AboveRequest++;
    else if (person.InitialFloor < (*ele).NowFloor)
        UnderRequest++;
}

//手动输入乘客
void ManualInput(Elevator *ele)
{
    char c;
    Passenger person;
    int Turn;
    while (1)
    {
        char c;
        fflush(stdin);
        printf("请输入这一秒生成的人数[0,10]:");
        if (scanf("%d", &Turn) == 0 || Turn < 0 || Turn > 10 || (scanf("%c", &c) && c != '\n'))
        {
            fflush(stdin);
            printf("输入非法\n");
            continue;
        }
        break;
    }

    for (int i = 1; i <= Turn; i++)
    {
        while (1)
        {
            printf("请输入第%d名乘客的起始楼层:", i);
            fflush(stdin);
            if (scanf("%d", &person.InitialFloor) == 0 || person.InitialFloor < 0 || person.InitialFloor > (*ele).MaxFloor || (scanf("%c", &c) && c != '\n'))
            {
                fflush(stdin);
                printf("输入非法\n");
                continue;
            }
            break;
        }
        while (1)
        {
            printf("请输入第%d名乘客的目标楼层:", i);
            fflush(stdin);
            if (scanf("%d", &person.TargetFloor) == 0 || person.TargetFloor < 1 || person.TargetFloor > (*ele).MaxFloor || (scanf("%c", &c) && c != '\n'))
            {
                fflush(stdin);
                printf("输入非法\n");
                continue;
            }
            break;
        }
        while (1)
        {
            printf("请输入第%d名乘客的体重[40,99]kg:", i);
            fflush(stdin);
            if (scanf("%d", &person.Weight) == 0 || person.Weight < 40 || person.Weight > 99 || (scanf("%c", &c) && c != '\n'))
            {
                fflush(stdin);
                printf("输入非法\n");
                continue;
            }
            break;
        }
        while (1)
        {
            printf("请输入第%d名乘客的耐心值[10,99]s:", i);
            fflush(stdin);
            if (scanf("%d", &person.Patient) == 0 || person.Patient < 10 || person.Patient > 99 || (scanf("%c", &c) && c != '\n'))
            {
                fflush(stdin);
                printf("输入非法\n");
                continue;
            }
            break;
        }
        person.InitialTime = Time;

        if (person.TargetFloor >= person.InitialFloor)
        {
            EnQueue(&(floor[person.InitialFloor].UpQueue), person);
            floor[person.InitialFloor].UpNum++;
        }
        else
        {
            EnQueue(&(floor[person.InitialFloor].DownQueue), person);
            floor[person.InitialFloor].DownNum++;
        }

        if ((*ele).Mode == 0)
        {
            (*ele).NowStayTime = 0;
            if (person.InitialFloor >= (*ele).NowFloor)
                (*ele).Mode = 1;
            else
                (*ele).Mode = 2;
        }

        if (person.InitialFloor > (*ele).NowFloor)
            AboveRequest++;
        else if (person.InitialFloor < (*ele).NowFloor)
            UnderRequest++;
    }
}

//打印时，将离开的用户出队列
void Init(Elevator *ele)
{
    InitQueue(&((*ele).UpQueue));
    InitQueue(&((*ele).DownQueue));
    (*ele).NowFloor = 1;
    (*ele).MaxFloor = 6;
    (*ele).NowWeight = 0;
    (*ele).MaxWeight = 500;
    (*ele).NowWaitTime = 0;
    (*ele).MaxWaitTime = 300;
    (*ele).NowStayTime = 0;
    (*ele).MaxStayTime = 0;
    (*ele).NowPassengerNumber = 0;
    (*ele).MaxPassengerNumber = 8;
    (*ele).Mode = 0;
    (*ele).WeightWarn = 0;
    (*ele).TimeWarn = 0;
    //楼层初始化
    for (int i = 1; i <= (*ele).MaxFloor; i++)
    {
        floor[i].UpNum = 0;
        floor[i].DownNum = 0;
        InitQueue(&(floor[i].UpQueue));
        InitQueue(&(floor[i].DownQueue));
    }
}