#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char Name[80];
    float ExamGrade;
    float MathGrade;
    float BiggestGrade;
    float BaccalaureateAverage;
    int Mode;
} Student;

/*
 * FIRST_MODE = 0,8 * ExamGrade + 0,2 * BaccalaureateAverage;
 * SECOND_MODE = 0,6 * ExamGrade + 0,2 * BiggestGrade + 0,2 * BaccalaureateAverage
 */
enum {FIRST_MODE, SECOND_MODE};

FILE * OpenFile(const char * FileName, const char * FileMode)
{
    FILE * File = fopen(FileName, FileMode);

    return File;
}

FILE * ValidateFile(const char * FileName, const char * FileMode)
{
    FILE * File = OpenFile(FileName, FileMode);

    if (!File)
    {
        printf("File not found.");
        exit(5);
    }

    return File;
}

int GetNumberOfStudents(FILE * File)
{
    int NumberOfStudents = 0;

    char Line[50];

    while (fgets(Line, 50, File))
        NumberOfStudents += 1;

    return NumberOfStudents;
}

void GetPositionInStruct(Student * Students, int Index, int Position, char * Word)
{
    if (Position == 1)
        strcpy(Students[Index].Name, Word);

    if (Position == 2)
        Students[Index].ExamGrade = (float)strtod(Word, NULL);

    if (Position == 3)
        Students[Index].MathGrade = (float)strtod(Word, NULL);

    if (Position == 4)
        Students[Index].BiggestGrade = (float)strtod(Word, NULL);

    if (Position == 5)
        Students[Index].BaccalaureateAverage = (float)strtod(Word, NULL);

    if (Position == 6)
    {
        int ChosenMode = atoi(Word);

        if (ChosenMode == 1)
            Students[Index].Mode = FIRST_MODE;
        else
            Students[Index].Mode = SECOND_MODE;
    }
}

Student * GetStudents(FILE * File, int NumberOfStudents)
{
    char Line[100];
    int Index = 0;

    Student * EnrolledStudents = (Student *)malloc(NumberOfStudents * sizeof(Student));

    for (int i = 0; i < NumberOfStudents; ++i)
    {
        fgets(Line, 100, File);

        int Position = 1;
        char * Word = strtok(Line, ",");

        while (Word != NULL)
        {
            GetPositionInStruct(EnrolledStudents, Index, Position, Word);

            Position += 1;
            Word = strtok(NULL, ",");
        }

        Index += 1;
    }

    return EnrolledStudents;
}

void ShowStudents(Student * Students, int NumberOfStudents)
{
    for (int i = 0; i < NumberOfStudents; ++i)
    {
        printf("Name: %s||", Students[i].Name);
        printf("Exam grade: %0.2f||", Students[i].ExamGrade);
        printf("Math grade: %0.2f||", Students[i].MathGrade);
        printf("Biggest grade: %0.2f||", Students[i].BiggestGrade);
        printf("Baccalaureate Average: %0.2f||", Students[i].BaccalaureateAverage);

        if (Students[i].Mode == FIRST_MODE)
            printf("Mode: 1\n");
        else
            printf("Mode: 2\n");
    }
}

int main()
{
    return 0;
}
