#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AVAILABLE_POSITIONS 3

typedef struct
{
    char Name[80];
    float ExamGrade;
    float MathGrade;
    float BiggestGrade;
    float BaccalaureateAverage;
    int Mode;
    float CollegeAverage;
} StudentCharacteristics;

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

void GetPositionInStruct(StudentCharacteristics * Students, int Index, int Position, char * Word)
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

StudentCharacteristics * GetStudents(FILE * File, int NumberOfStudents)
{
    char Line[100];
    int Index = 0;

    StudentCharacteristics * EnrolledStudents =
            (StudentCharacteristics *)malloc(NumberOfStudents * sizeof(StudentCharacteristics));

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

float CalculateAverageFirstMode(StudentCharacteristics Student)
{
    return 0.8 * Student.ExamGrade + 0.2 * Student.BaccalaureateAverage;
}

float CalculateAverageSecondMode(StudentCharacteristics Student)
{
    return 0.6 * Student.ExamGrade + 0.2 * Student.BiggestGrade + 0.2 * Student.BaccalaureateAverage;
}

StudentCharacteristics * ValidateStudents(StudentCharacteristics * EnrolledStudents, int NumberOfStudents,
                                          int * NumberOfValidatedStudents)
{
    int Index = 0;
    StudentCharacteristics * ValidatedStudents =
            (StudentCharacteristics *)malloc(NumberOfStudents * sizeof(StudentCharacteristics));

    for (int i = 0; i < NumberOfStudents; ++i)
        if (EnrolledStudents[i].ExamGrade >= 5)
        {
            ValidatedStudents[Index] = EnrolledStudents[i];

            if (ValidatedStudents[Index].Mode == 1)
                ValidatedStudents[Index].CollegeAverage = CalculateAverageFirstMode(ValidatedStudents[Index]);
            else
                ValidatedStudents[Index].CollegeAverage = CalculateAverageSecondMode(ValidatedStudents[Index]);

            Index += 1;
        }

    *NumberOfValidatedStudents = Index;

    return ValidatedStudents;
}

void SwapStudents(StudentCharacteristics * FirstStudent, StudentCharacteristics * SecondStudent)
{
    StudentCharacteristics Auxiliary;

    Auxiliary = *FirstStudent;
    *FirstStudent = *SecondStudent;
    *SecondStudent = Auxiliary;
}

void SortStudents(StudentCharacteristics * Students, int NumberOfStudents)
{
    for (int i = 0; i < NumberOfStudents - 1; ++i)
        for (int j = i + 1; j < NumberOfStudents; ++j)
            if (Students[i].CollegeAverage < Students[j].CollegeAverage)
                SwapStudents(&Students[i], &Students[j]);
            else
            if (Students[i].CollegeAverage == Students[j].CollegeAverage)
            {
                if (Students[i].MathGrade < Students[j].MathGrade)
                    SwapStudents(&Students[i], &Students[j]);
            }
}

void WriteAdmittedStudentsInFile(FILE * File, StudentCharacteristics * Students, int NumberOfStudents)
{
    for (int i = 0; i < NumberOfStudents; ++i)
    {
        fprintf(File,"Name: %s || ", Students[i].Name);
        fprintf(File,"College Average: %0.2f\n", Students[i].CollegeAverage);
    }
}

void GetAdmittedStudents(StudentCharacteristics * ValidatedStudents, int NumberOfValidatedStudents)
{
    FILE * AdmittedStudentsFile = ValidateFile("AdmittedStudents.txt", "w");

    if (AVAILABLE_POSITIONS >= NumberOfValidatedStudents)
        WriteAdmittedStudentsInFile(AdmittedStudentsFile, ValidatedStudents, NumberOfValidatedStudents);
    else
        WriteAdmittedStudentsInFile(AdmittedStudentsFile, ValidatedStudents, AVAILABLE_POSITIONS);

    fclose(AdmittedStudentsFile);
}

int main()
{
    FILE * EnrolledStudentsFile = ValidateFile("EnrolledStudents.txt", "r");

    int NumberOfStudents = GetNumberOfStudents(EnrolledStudentsFile);
    fseek(EnrolledStudentsFile, 0, SEEK_SET);
    StudentCharacteristics * EnrolledStudents = GetStudents(EnrolledStudentsFile, NumberOfStudents);

    fclose(EnrolledStudentsFile);

    int NumberOfValidatedStudents = 0;
    StudentCharacteristics * ValidatedStudents = ValidateStudents(EnrolledStudents, NumberOfStudents, &NumberOfValidatedStudents);

    SortStudents(ValidatedStudents, NumberOfValidatedStudents);

    GetAdmittedStudents(ValidatedStudents, NumberOfValidatedStudents);

    free(EnrolledStudents);
    free(ValidatedStudents);

    return 0;
}
