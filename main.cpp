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
    void setSparseRow(int, int, int); //(row, col, value)
    void setRow(int);
    void setCol(int);
    void setVal(int);
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
    SparseMatrix(); //I do not know
    SparseMatrix(int n, int m, int cv, int noNSV);
    SparseMatrix(SparseMatrix& P);
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
    void setSparseRow(int, int, int, int);
    void sorted();
    bool existMymatrix(SparseRow* M, int row, int col);
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

void SparseRow::setSparseRow(int n, int m, int cv)
{
    row = n;
    col = m;
    value = cv;
}

void SparseRow::setRow(int r) {
    row = r;
}
void SparseRow::setCol(int c){
    col = c;
}
void SparseRow::setVal(int v) {
    value = v;
}

int SparseRow::getRow(){
    return row;
}
int SparseRow::getCol(){
    return col;
}
int SparseRow::getValue(){
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

SparseMatrix::SparseMatrix(SparseMatrix &P) {
    noRows = P.getRow();
    noCols = P.getCol();
    commonValue = P.getCV();
    noNonSparseValues = P.getNoNSV();
    myMatrix = new SparseRow (*P.getMyMatrix());
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
    getMyMatrix()[count].setSparseRow(row, col, value);
}

void SparseMatrix::display()
{
    for (int i = 0; i < getNoNSV(); i++) {
        if(getMyMatrix()[i].getValue() != getCV()) { //need to update method and sort method
            getMyMatrix()[i].display();
        }
    }
}

void SparseMatrix::displayMatrix()
{
    int count = 0;
    while(getMyMatrix()[count].getRow() < 0)
    {
        count++;
    }
    for(int i = 0; i < getRow(); i++)
    {
        for(int j = 0; j < getCol(); j++)
        {

            if(getMyMatrix()[count].getRow() == i && getMyMatrix()[count].getCol() == j) //this works if it is ordered
            {
                cout << getMyMatrix()[count].getValue() << "\t";
                count++;
            }
            else
            {
                cout << getCV() << "\t";
            }
        }
        cout << endl;
    }
}

SparseMatrix* SparseMatrix::Transpose()
{
    SparseMatrix* transposed = new SparseMatrix(getCol(), getRow(), getCV(), getNoNSV());

    for(int i = 0; i < getNoNSV(); i++)
    {
        (*transposed).myMatrix[i].setSparseRow(myMatrix[i].getCol(), myMatrix[i].getRow(), myMatrix[i].getValue());
    }

    (*transposed).sorted();

    return transposed;
}

SparseMatrix* SparseMatrix::Add(SparseMatrix &M)
{
    SparseMatrix* added = new SparseMatrix(getRow(), getCol(), getCV(), getNoNSV() + M.getNoNSV());

    int currentSize = 0;
    int i = 0;
    int j;
    bool found;
    bool* isBox = new bool[M.getNoNSV()];

    for(j = 0; j < M.getNoNSV(); j++)
    {
        isBox[j] = false;
    }

    while (i < getNoNSV())
    {
        found = false;
        (*added).getMyMatrix()[currentSize].setRow(getMyMatrix()[i].getRow());
        (*added).getMyMatrix()[currentSize].setCol(getMyMatrix()[i].getCol());

        for(j = 0; j < M.getNoNSV(); j++)
        {
            if((getMyMatrix()[i].getRow() == M.getMyMatrix()[j].getRow()) &&
                    (getMyMatrix()[i].getCol() == M.getMyMatrix()[j].getCol()))
            {
                (*added).getMyMatrix()[currentSize++].setVal(getMyMatrix()[i].getValue() + M.getMyMatrix()[j].getValue());
                found = true;
                isBox[j] = found;
                break;
            }
        }
        if(!found)
        {
            (*added).getMyMatrix()[currentSize++].setVal((getMyMatrix()[i].getValue()));
        }
        i++;
    }

    i = 0;
    while(i < M.getNoNSV())
    {
        if(!isBox[i])
        {
            (*added).getMyMatrix()[currentSize++].setSparseRow(M.getMyMatrix()[i].getRow(),M.getMyMatrix()[i].getCol(),M.getMyMatrix()[i].getValue());
        }
        i++;
    }

    (*added).sorted();

    return added;

}

bool SparseMatrix::existMymatrix(SparseRow* M, int row, int col)
{
    
}

SparseMatrix* SparseMatrix::Multiply(SparseMatrix &M) {

    int sum = 0;
    int aval;
    int bval;

    for(int i = 0; i < getRow(); i++)
    {
        for(int j = 0; j < M.getCol(); j++)
        {
            for(int k = 0; k < getRow(); k++)
            {
                if(existMymatrix(getMyMatrix(), i, k) && existMymatrix(M.getMyMatrix(), k, j))
                {
                    aval = getMyMatrix()[]
                }
            }
        }
    }

}

void SparseMatrix::sorted()
{

    SparseRow amatrix;
    SparseRow tmp;

    for(int i = 0; i < getNoNSV() - 1; i++)
    {
        for(int j = 0; j < getNoNSV() - i - 1; j++)
        {
            if(getMyMatrix()[j].getRow() > getMyMatrix()[j + 1].getRow())
            {
                tmp = getMyMatrix()[j];
                getMyMatrix()[j] = getMyMatrix()[j + 1];
                getMyMatrix()[j + 1] = tmp;
            }
        }
    }

    for(int i = 0; i < getNoNSV() - 1; i++)
    {
        for(int j = 0; j < getNoNSV() - i - 1; j++)
        {
            if(getMyMatrix()[j].getCol() > getMyMatrix()[j + 1].getCol() && getMyMatrix()[j].getRow() == getMyMatrix()[j + 1].getRow())
            {
                tmp = getMyMatrix()[j];
                getMyMatrix()[j] = getMyMatrix()[j + 1];
                getMyMatrix()[j + 1] = tmp;
            }
        }
    }
}
int main()
{

    int n, m, cv, noNSV;
    SparseMatrix* temp;

    cin >> n >> m >> cv >> noNSV;

   SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);


   int value;
   int count = 0;
   for(int i = 0; i < n; i++)
   {
       for(int j = 0; j < m; j++)
       {
           cin  >> value;
           if(value != cv)
           {
               (*firstOne).setSparseRow(i, j, value, count);
               count++;
           }

       }
   }

    cin >> n >> m >> cv >> noNSV;

    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);

    count = 0;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
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

//    cout << "multiplication of matrices in sparse matrix form: " << endl;
//    temp = (*secondOne).Multiply(*firstOne);
//    (*temp).display();

    cout << "addition of matrices in sparse matrix form: " << endl;
    temp = (*secondOne).Add(*firstOne);

    (*temp).displayMatrix();

    return 0;
}