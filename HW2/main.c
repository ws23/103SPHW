#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _labelTableStruct
{
    char name[10];
    char addr[10];

} labelTableStruct;

typedef struct _obLabelStruct
{
    char id[10];
    char name[10];
    char *addr;

} obLabelStruct;

typedef struct _mdataStruct
{
    int position;
    char lengthType[10];
    int op; // 1+ 2-
    char useLabel[10];

} mdataStruct;


typedef struct _obcodeData
{
    char addr[10];
    obLabelStruct obLabel[10];
    int obLabelNum;
    char length[10];
    int memoryLength;
    char memory[1000];
    mdataStruct mdata[50];
    int mdataNum;
    int endMemoryStart;
    int mathcount;

} obcodeData;


char userCommend[100];
char memoryADDR[10]= "4000";
int progAmount = 3;
obcodeData obcode[10];
labelTableStruct labelTable[20];
int labelTableNum = 0;

void getData(char *input, int order)
{
    FILE *fin= fopen(input, "r");
    char readingData[100];
    int i, j, k;

    obcode[order].obLabelNum = 0;
    memset(obcode[order].memory, '*', sizeof(obcode[order].memory));
    obcode[order].mdataNum = 0;
    obcode[order].mathcount = 0;

    while(fgets(readingData, 100, fin))
    {
        for(i=0; readingData[i]; i++)
            if(readingData[i]=='\n' || readingData[i]=='\r')
                readingData[i] = ' ';

        if(readingData[0] == 'H')
        {
            char *str = strtok(readingData, " ");
            strcpy(obcode[order].obLabel[obcode[order].obLabelNum].id, "01");
            strcpy(obcode[order].obLabel[obcode[order].obLabelNum].name, str+1);
            obcode[order].obLabelNum++;
            strcpy(labelTable[labelTableNum].name, str+1);
            strcpy(labelTable[labelTableNum].addr, memoryADDR);
            labelTableNum++;
            str = strtok(NULL, " ");
            strcpy(obcode[order].length, str+6);
            obcode[order].memoryLength = strtol(obcode[order].length, NULL, 16) * 2;
        }
        else if(readingData[0] == 'D')
        {
            char *str = strtok(readingData, " ");
            char *temp;
            char str2[25];

            strcpy(labelTable[labelTableNum].name, str+1);
            str = strtok(NULL, " ");

            while(1)
            {
                strncpy(str2, str, 6);
                str2[6] = '\0';
                sprintf(labelTable[labelTableNum].addr, "%x", strtol(memoryADDR, NULL, 16) + strtol(str2, NULL, 16));
                //printf("%s\n",labelTable[labelTableNum].addr);
                labelTableNum++;

                temp = strtok(NULL, " ");
                if(temp != NULL)
                {
                    strcpy(labelTable[labelTableNum].name, str+6);
                    str = temp;
                }
                else
                {
                    break;
                }

            }

        }
        else if(readingData[0] == 'R')
        {
            char *str = strtok(readingData, " ");
            char str2[25];

            strncpy(str2, str+1, 2);
            str2[2] = '\0';

            strcpy(obcode[order].obLabel[obcode[order].obLabelNum].id, str2);
            strcpy(obcode[order].obLabel[obcode[order].obLabelNum].name, str+3);
            //printf("%d %s %s\n",obcode[order].obLabelNum, obcode[order].obLabel[obcode[order].obLabelNum].id, obcode[order].obLabel[obcode[order].obLabelNum].name);
            obcode[order].obLabelNum++;

            while(str = strtok(NULL, " "))
            {
                strncpy(str2, str, 2);
                str2[2] = '\0';

                strcpy(obcode[order].obLabel[obcode[order].obLabelNum].id, str2);
                strcpy(obcode[order].obLabel[obcode[order].obLabelNum].name, str+2);
                //printf("%d %s %s\n",obcode[order].obLabelNum, obcode[order].obLabel[obcode[order].obLabelNum].id, obcode[order].obLabel[obcode[order].obLabelNum].name);
                obcode[order].obLabelNum++;
            }

        }
        else if(readingData[0] == 'T')
        {
            char *str;
            char str2[25];

            strncpy(str2, readingData+1, 6);
            str2[6] = '\0';
            int start = strtol(str2, NULL, 16) * 2;

            strncpy(str2, readingData+7, 2);
            str2[2] = '\0';
            int length = strtol(str2, NULL, 16) * 2;

            strncpy(obcode[order].memory + start, readingData+9, length);

        }
        else if(readingData[0] == 'M')
        {
            char *str;
            char str2[25];

            strncpy(str2, readingData+1, 6);
            str2[6] = '\0';
            obcode[order].mdata[obcode[order].mdataNum].position = strtol(str2, NULL, 16) * 2;

            strncpy(str2, readingData+7, 2);
            str2[2] = '\0';
            strcpy(obcode[order].mdata[obcode[order].mdataNum].lengthType, str2);

            if(readingData[9] == '+')
                obcode[order].mdata[obcode[order].mdataNum].op = 1;
            else if(readingData[9] == '-')
                obcode[order].mdata[obcode[order].mdataNum].op = 2;
            else
                obcode[order].mdata[obcode[order].mdataNum].op = 0;

            strncpy(str2, readingData+10, 2);
            str2[2] = '\0';
            strcpy(obcode[order].mdata[obcode[order].mdataNum].useLabel, str2);

            //printf("%d %s %d %s\n",obcode[order].mdata[obcode[order].mdataNum].position, obcode[order].mdata[obcode[order].mdataNum].lengthType, obcode[order].mdata[obcode[order].mdataNum].op, obcode[order].mdata[obcode[order].mdataNum].useLabel);

            obcode[order].mdataNum++;

        }
        else if(readingData[0] == 'E')
        {
            char *str;
            char str2[25];

            if(order == 0)
            {
                strncpy(str2, readingData+1, 6);
                str2[6] = '\0';
                obcode[order].endMemoryStart = strtol(str2, NULL, 16) * 2;
            }

            sprintf(memoryADDR, "%x", strtol(memoryADDR, NULL, 16) + strtol(obcode[order].length, NULL, 16));
            //printf("%s\n",memoryADDR);

        }
        else
        {
        }

    }

    /*
    for(i = 0; i < obcode[order].memoryLength; i++)
    {
        if(i%32 == 0)
            printf("\n");

        printf("%c",obcode[order].memory[i]);
    }
    */

}


void matchTable()
{
    int i, j, k;

    for(i = 0; i < progAmount; i++)
    {
        for(j = 0; j < obcode[i].obLabelNum; j++)
        {
            for(k = 0; k < labelTableNum; k++)
            {
                if(strcmp(obcode[i].obLabel[j].name, labelTable[k].name) == 0)
                {
                    obcode[i].obLabel[j].addr = labelTable[k].addr;
                    break;
                }
            }
        }
    }

    /*
    for(i = 0; i < progAmount; i++)
    {
        for(j = 0; j < obcode[i].obLabelNum; j++)
        {
            printf("%s %s %s\n",obcode[i].obLabel[j].id ,obcode[i].obLabel[j].name, obcode[i].obLabel[j].addr);
        }

    }
    */
}


void modifyMemory()
{
    char *str;
    char str2[25];
    int i, j, k, l;

    for(i = 0; i < progAmount; i++)
    {
        for(j = 0; j < obcode[i].mdataNum; j++)
        {
            int position = obcode[i].mdata[j].position;
            int result;

            if(strcmp(obcode[i].mdata[j].lengthType, "05") == 0)
            {
                strncpy(str2, obcode[i].memory+position+1, 5);
                str2[5]= '\0';

                for(k = 0; k < obcode[i].obLabelNum; k++)
                {
                    if(strcmp(obcode[i].mdata[j].useLabel, obcode[i].obLabel[k].id) == 0)
                        break;
                }

                if(obcode[i].mathcount >= 0)
                {
                    if(obcode[i].mdata[j].op == 1)
                        result = strtol(str2, NULL, 16) + strtol(obcode[i].obLabel[k].addr, NULL, 16);
                    else if(obcode[i].mdata[j].op == 2)
                        result = strtol(str2, NULL, 16) - strtol(obcode[i].obLabel[k].addr, NULL, 16);

                    if(result < 0)
                        obcode[i].mathcount = result;
                    else
                        obcode[i].mathcount = 0;
                }
                else
                {
                    if(obcode[i].mdata[j].op == 1)
                        result = obcode[i].mathcount + strtol(obcode[i].obLabel[k].addr, NULL, 16);
                    else if(obcode[i].mdata[j].op == 2)
                        result = obcode[i].mathcount - strtol(obcode[i].obLabel[k].addr, NULL, 16);

                    if(result < 0)
                        obcode[i].mathcount = result;
                    else
                        obcode[i].mathcount = 0;
                }

                if(result > 1048575)
                    result -= 1048576;

                if(result >= 0)
                {
                    sprintf(str2, "%x", result);

                    int cnt = 5 - strlen(str2);
                    for(l = 0; l < cnt; l++)
                    {
                        obcode[i].memory[position+1+l] = '0';
                    }

                    strncpy(obcode[i].memory+position+1+cnt, str2, strlen(str2));
                }

            }
            else if(strcmp(obcode[i].mdata[j].lengthType, "06") == 0)
            {
                strncpy(str2, obcode[i].memory+position, 6);
                str2[6]= '\0';

                for(k = 0; k < obcode[i].obLabelNum; k++)
                {
                    if(strcmp(obcode[i].mdata[j].useLabel, obcode[i].obLabel[k].id) == 0)
                        break;
                }

                if(obcode[i].mathcount >= 0)
                {
                    if(obcode[i].mdata[j].op == 1)
                        result = strtol(str2, NULL, 16) + strtol(obcode[i].obLabel[k].addr, NULL, 16);
                    else if(obcode[i].mdata[j].op == 2)
                        result = strtol(str2, NULL, 16) - strtol(obcode[i].obLabel[k].addr, NULL, 16);

                    if(result < 0)
                        obcode[i].mathcount = result;
                    else
                        obcode[i].mathcount = 0;
                }
                else
                {
                    if(obcode[i].mdata[j].op == 1)
                        result = obcode[i].mathcount + strtol(obcode[i].obLabel[k].addr, NULL, 16);
                    else if(obcode[i].mdata[j].op == 2)
                        result = obcode[i].mathcount - strtol(obcode[i].obLabel[k].addr, NULL, 16);

                    if(result < 0)
                        obcode[i].mathcount = result;
                    else
                        obcode[i].mathcount = 0;
                }

                if(result > 16777215)
                    result -= 16777216;

                if(result >= 0)
                {
                    sprintf(str2, "%x", result);

                    int cnt = 6 - strlen(str2);
                    for(l = 0; l < cnt; l++)
                    {
                        obcode[i].memory[position+l] = '0';
                    }

                    strncpy(obcode[i].memory+position+cnt, str2, strlen(str2));
                }

            }

        }

    }

}


void show()
{
    int i, j, k, target = 0;
    char memory[5000];

    for(i = 0; i < progAmount; i++)
    {
        for(j = 0; j < obcode[i].memoryLength; j++)
        {
            memory[target] = obcode[i].memory[j];
            target++;
        }
    }

    char index[5] = {obcode[0].obLabel[0].addr[0],
                     obcode[0].obLabel[0].addr[1],
                     obcode[0].obLabel[0].addr[2],
                     '0',
                     '\0'
                    };

    int spaceNum = strtol(obcode[0].obLabel[0].addr+3, NULL, 16);
    int dir = 0;
    i = 0;
    int mode = 0;

    while(1)
    {
        if(dir == 0)
            printf("%s  ",index);

        if(spaceNum > 0)
        {
            printf("x");
            spaceNum--;
            dir++;
        }
        else if(i < target)
        {
            printf("%c",memory[i]);
            i++;
            dir++;
        }
        else
        {
            printf("x");
            dir++;
        }

        if(i >= target)
            mode = 1;

        if(dir %8 == 0)
            printf("  ");

        if(dir %32 == 0)
        {
            printf("\n");
            if(mode == 1)
                break;

            sprintf(index, "%x", strtol(index,NULL,16) + 16);
            printf("%s  ",index);
        }

    }

}


int main()
{
    char word[100], *str, str2[10][10];
    int order = 0;
    int i;

    fgets(word, 100, stdin);
    for(i=0; word[i]; i++)
        if(word[i]=='\n' || word[i]=='\r')
            word[i] = ' ';

    str = strtok(word, " ");
    strcpy(memoryADDR, strtok(NULL, " "));

    while(str = strtok(NULL, " "))
    {
        strcpy(str2[order], str);
        order++;
    }

    for(i=0; i< order; i++)
    {
        strcat(str2[i],".txt");
        getData(str2[i], i);
    }

    //load 4000 PROGA PROGB PROGC
    matchTable();
    modifyMemory();
    show();

    return 0;
}



