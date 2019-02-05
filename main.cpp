#include <iostream>
using namespace std;

class SparseRow {
protected:
    int row; //Row
    int col; //Column
    int value; //Value only integer

public:
    SparseRow();
    void display();
    void setSparseRow(int r, int c, int val);
    void setRow(int r); //
    void setCol(int c);
    void setVal(int val);
    int getRow();
    int getCol();
    int getValue();
};

class SparseMatrix {
protected:
    int noRows; // the number of rows
    int noCols; // the number of columns
    int commonValue; // read from input
    int noNonSparseValues; //the number of elements which is not zero
    SparseRow* myMatrix; //Array consisted of SparseRow : [SparseRow, SparseRow, ....] the size is by the number of noSparseValues

public:
    SparseMatrix();
    SparseMatrix(int n, int m, int cv, int noNSV);
    SparseMatrix* Transpose();
    SparseMatrix* Multiply(SparseMatrix& M);
    SparseMatrix* Add(SparseMatrix& M);
    void display();
    void displayMatrix();

    int getRow();
    int getCol();
    int getCV();
    int getNoNSV();
    SparseRow* getMyMatrix();
    void setSparseRow(int r, int c, int val, int count);
    void sorted();
    int detectValue(int row, int col);
};

SparseRow::SparseRow()
{
    row = -1;
    col = -1;
    value = 0;
}

void SparseRow::display()
{
    cout << getRow() << ", " << getCol() << ", " << getValue() << endl;
}

void SparseRow::setSparseRow(int r, int c, int val)
{
    row = r;
    col = c;
    value = val;
}

void SparseRow::setRow(int r)
{
    row = r;
}

void SparseRow::setCol(int c)
{
    col = c;
}

void SparseRow::setVal(int val)
{
    value = val;
}

int SparseRow::getRow()
{
    return row;
}

int SparseRow::getCol()
{
    return col;
}

int SparseRow::getValue()
{
    return value;
}

SparseMatrix::SparseMatrix()
{
    noRows = 2;
    noCols = 2;
    commonValue = 0;
    noNonSparseValues = 0;
    myMatrix = new SparseRow[noNonSparseValues]; //array
}

SparseMatrix::SparseMatrix (int n, int m, int cv, int noNSV)
{
    noRows = n;
    noCols = m;
    commonValue = cv;
    noNonSparseValues = noNSV;
    myMatrix = new SparseRow[noNSV];
}

int SparseMatrix::getRow()
{
    return noRows;
}

int SparseMatrix::getCol()
{
    return noCols;
}

int SparseMatrix::getCV()
{
    return commonValue;
}

int SparseMatrix::getNoNSV()
{
    return noNonSparseValues;
}

SparseRow* SparseMatrix::getMyMatrix()
{
    return myMatrix;
}

void SparseMatrix::setSparseRow(int row , int col , int value, int count)
{
    myMatrix[count].setSparseRow(row, col, value); // insert the SparseRow into the myMatrix by count
}

void SparseMatrix::display()
{
    for (int i = 0; i < noNonSparseValues; i++)
    {
        if(myMatrix[i].getValue() != commonValue)//need to update method and sort method
        {
            myMatrix[i].display();
        }
    }
}

void SparseMatrix::displayMatrix()
{
    int count = 0;
    while(myMatrix[count].getRow() < 0)
    {
        count++;
    }
    for(int i = 0; i < noRows; i++)
    {
        for(int j = 0; j < noCols; j++)
        {
            if(myMatrix[count].getRow() == i && myMatrix[count].getCol() == j) //this works if it is ordered
            {
                cout << myMatrix[count].getValue() << "\t";
                count++;
            }
            else
            {
                cout << commonValue << "\t";
            }
        }
        cout << endl;
    }
}

SparseMatrix* SparseMatrix::Transpose()
{
    SparseMatrix* transposed = new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues);

    for(int i = 0; i < noNonSparseValues; i++)
    {
        (*transposed).myMatrix[i].setSparseRow(myMatrix[i].getCol(), myMatrix[i].getRow(), myMatrix[i].getValue());
    }

    (*transposed).sorted();

    return transposed;
}

SparseMatrix* SparseMatrix::Add(SparseMatrix &M)
{
    SparseMatrix* added = new SparseMatrix(noRows, noCols, commonValue, noNonSparseValues + M.noNonSparseValues);

    int currentSize = 0;
    int i = 0;
    int j;
    bool found;
    bool* isBox = new bool[M.noNonSparseValues];

    for(j = 0; j < M.noNonSparseValues; j++)
    {
        isBox[j] = false;
    }

    while (i < noNonSparseValues)
    {
        found = false;
        (*added).myMatrix[currentSize].setRow(myMatrix[i].getRow());
        (*added).myMatrix[currentSize].setCol(myMatrix[i].getCol());

        for(j = 0; j < M.noNonSparseValues; j++)
        {
            if((myMatrix[i].getRow() == M.myMatrix[j].getRow()) &&
                    (myMatrix[i].getCol() == M.myMatrix[j].getCol()))
            {
                (*added).myMatrix[currentSize++].setVal(myMatrix[i].getValue() + M.myMatrix[j].getValue());
                found = true;
                isBox[j] = found;
                break;
            }
        }
        if(!found)
        {
            (*added).myMatrix[currentSize++].setVal((myMatrix[i].getValue()));
        }
        i++;
    }

    i = 0;
    while(i < M.noNonSparseValues)
    {
        if(!isBox[i])
        {
            (*added).myMatrix[currentSize++].setSparseRow(M.myMatrix[i].getRow(), M.myMatrix[i].getCol(), M.myMatrix[i].getValue());
        }
        i++;
    }

    (*added).sorted();

    return added;

}

int SparseMatrix::detectValue(int row, int col)
{
    for(int i = 0; i < noNonSparseValues; i++)
    {
        if(myMatrix[i].getRow() == row && myMatrix[i].getCol() == col)
        {
            return myMatrix[i].getValue();
        }
    }
    return -1;
}

SparseMatrix* SparseMatrix::Multiply(SparseMatrix &M) {

    SparseMatrix* Multiplied = new SparseMatrix(noRows, noCols, commonValue, noRows * noCols);

    int count = 0;
    int sum;
    int a;
    int b;

    for(int i = 0; i < noRows; i++)
    {
        for(int j = 0; j < M.noCols; j++)
        {
            sum = 0;
            for(int k = 0; k < noRows; k++)
            {
                a = detectValue(i, k);
                b = M.detectValue(k, j);

                if(a != -1 && b != -1)
                {
                   sum += a * b;
                }
            }
            if(sum != commonValue)
            {
                (*Multiplied).setSparseRow(i, j, sum, count++);
            }
        }
    }
    (*Multiplied).sorted();

    return Multiplied;
}

void SparseMatrix::sorted()
{

    SparseRow tmp;

    for(int i = 0; i < noNonSparseValues - 1; i++)
    {
        for(int j = 0; j < noNonSparseValues - i - 1; j++)
        {
            if(myMatrix[j].getRow() > myMatrix[j + 1].getRow())
            {
                tmp = myMatrix[j];
                myMatrix[j] = myMatrix[j + 1];
                myMatrix[j + 1] = tmp;
            }
        }
    }

    for(int i = 0; i < noNonSparseValues - 1; i++)
    {
        for(int j = 0; j < noNonSparseValues- i - 1; j++)
        {
            if(myMatrix[j].getCol() > myMatrix[j + 1].getCol() && myMatrix[j].getRow() == myMatrix[j + 1].getRow())
            {
                tmp = myMatrix[j];
                myMatrix[j] = myMatrix[j + 1];
                myMatrix[j + 1] = tmp;
            }
        }
    }
}

int main()
{
    int row, col, cv, noNSV;
    SparseMatrix* temp;

    cin >> row >> col >> cv >> noNSV;

   SparseMatrix* firstOne = new SparseMatrix(row, col, cv, noNSV);

   int value;
   int count = 0;
   for(int i = 0; i < row; i++)
   {
       for(int j = 0; j < col; j++)
       {
           cin  >> value;
           if(value != cv)
           {
               (*firstOne).setSparseRow(i, j, value, count);
               count++;
           }
       }
   }

    cin >> row >> col >> cv >> noNSV;

    SparseMatrix* secondOne = new SparseMatrix(row, col, cv, noNSV);

    count = 0;
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            cin  >> value;
            if(value != cv) {
                (*secondOne).setSparseRow(i, j, value, count);
                count++;
            }
        }
    }

    cout << endl;

    cout << "First one in sparse matrix format " << endl;
    (*firstOne).display();

    cout << "First one in normal matrix format " << endl;
    (*firstOne).displayMatrix();

    cout << "Second one in sparse matrix format " << endl;
    (*secondOne).display();

    cout << "Second one in normal matrix format " << endl;
    (*secondOne).displayMatrix();

    cout << "After Transpose first one in normal format " << endl;
    temp = (*firstOne).Transpose();
    (*temp).displayMatrix();

    cout << "After Transpose second one in normal format " << endl;
    temp = (*secondOne).Transpose();
    (*temp).displayMatrix();

    cout << "multiplication of matrices in sparse matrix form: " << endl;
    temp = (*secondOne).Multiply(*firstOne);
    (*temp).display();

    cout << "addition of matrices in sparse matrix form: " << endl;
    temp = (*secondOne).Add(*firstOne);
    (*temp).display();

    return 0;
}