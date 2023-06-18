#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <cmath>
#define EPSILON 1.77635683940025e-15
using namespace std;
string guards("ABCDEFGH");
double radius(5.0);
void checkArray(string points[], int guardIndex[], double x[], double y[], int size, double r);
bool SeeEachOther(string points[], double x[], double y[], int size, double r, int AIndex, int BIndex);
bool ASeesB(string points[], double x[], double y[], int size, double r, int AIndex, int BIndex);
bool doesIntersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
double doesIntersectInRay(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
double distance(double Ax, double Ay, double Bx, double By);
bool lessThan(double a, double b);
bool greaterThan(double a, double b);
bool equalsTo(double a, double b);
void printInput(string points[], double x[], double y[], int size);

int main()
{
    fstream f;
	string value;
	string filename = "./coord.txt";
    
	ifstream myFile;
	string line;
	int lines;
	myFile.open(filename);
	for(lines = 0; getline(myFile,line); lines++);
	myFile.close();

	int size = lines;
	string* points = new string[size]; // point or guard name
	double* x = new double[size]; // x-coords
	double* y = new double[size]; // y-coords
    int *gIndex = new int[guards.length()];
	f.open(filename.c_str());
	int i = 0; 
	int j = 0;
	
	while (f >> value)
	{
		if (i % 3 == 0)
		{
			points[j] = value;
            if (points[j].length() == 1) // save guard index to gIndex
            {
                int index = (int)(points[j].at(0));
                gIndex[(index - 65)] = j;
            }
		}
		if (i % 3 == 1)
		{
			x[j] = atof(value.c_str());
		}
		if (i % 3 == 2)
		{
			y[j] = atof(value.c_str());
			j++;
		}
		i++;
	}
    f.close();

    cout << setprecision(15);
	printInput(points, x, y, size);
    checkArray(points, gIndex, x, y, size, radius);
	return 0;
}

void checkArray(string points[], int guardIndex[], double x[], double y[], int size, double r)
{
    for (int i = 0; i < size; ++i)
    {
        if (points[i].length() > 1) // viewpoint
		{
            for (int j = 0; j < guards.length(); ++j)
			{
                if (strchr(points[i].c_str(), guards.at(j)))
                {
                    if (!SeeEachOther(points, x, y, size, r, i, guardIndex[j]) )
                    {
                        cout << "Error: " << points[i] << " does not see " << points[guardIndex[j]]<< endl;
                        cout << "(" << x[i] << ", " << y[i]  << ")" << endl;
                        cout << "(" << x[guardIndex[j]] << ", " << y[guardIndex[j]]  << ")" << endl;
                        cout << "x_midpoint = " << (x[i] + x[guardIndex[j]]) / 2.0 << endl;
                        cout << "y_midpoint = " << (y[i] + y[guardIndex[j]]) / 2.0 << endl;
                    }
                }
                else
                {
                    if (SeeEachOther(points, x, y, size, r, i, guardIndex[j]))
                    {
                        cout << "Error: " << points[i] << " sees " << points[guardIndex[j]] << endl;
                        cout << "(" << x[i] << ", " << y[i] << ")" << endl;
                        cout << "x_midpoint = " << (x[i] + x[guardIndex[j]]) / 2.0 << endl;
                        cout << "y_midpoint = " << (y[i] + y[guardIndex[j]]) / 2.0 << endl;
                    }
                }
            }
        }
    }
}

bool SeeEachOther(string points[], double x[], double y[], int size, double r, int AIndex, int BIndex)
{
    if (distance(x[AIndex], y[AIndex], x[BIndex], y[BIndex]) > r)
    {
        return false; 
    }
    if (!ASeesB(points, x, y, size, r, AIndex, BIndex))
    {
        return false;
    }
    return true;   
}

bool ASeesB(string points[], double x[], double y[], int size, double r, int AIndex, int BIndex)
{
    if (((AIndex + 1)%size == BIndex) || ((BIndex + 1)%size == AIndex))
    {
        return true;
    }
    for (int i = 0; i < size; i++)
    {
        if (!(((i%size) == AIndex) || (((i%size) == BIndex) || (((i+1)%size) == AIndex) || (((i+1)%size) == BIndex))))
        {
            if (doesIntersect(x[i%size], y[i%size], x[(i+1)%size], y[(i+1)%size], x[AIndex], y[AIndex], x[BIndex], y[BIndex]))
            {
                return false; 
            }
        }
    }
    double x_midpoint = (x[AIndex] + x[BIndex]) / 2.0;
    double y_midpoint = (y[AIndex] + y[BIndex]) / 2.0;
    double intersects = 0.0;
    for (int i = 0; i < size; i++)
    {
        intersects += doesIntersectInRay(x[i%size], y[i%size], x[(i+1)%size], y[(i+1)%size], x_midpoint, y_midpoint, x_midpoint, y_midpoint-1);
    }
    if (ceil(intersects) != floor(intersects))
    {
        cout << "Error: ceil(intersects) != floor(intersects)" << endl;
    }
    return ((int)intersects % 2 == 1);
}


bool doesIntersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    double a1 = y2 - y1; 
    double b1 = x1 - x2; 
    double c1 = a1*(x1) + b1*(y1); 
  
    double a2 = y4 - y3; 
    double b2 = x3 - x4; 
    double c2 = a2*(x3)+ b2*(y3); 
  
    double determinant = a1*b2 - a2*b1; 
  
    if (determinant != 0)
    {   

        double x = (b2*c1 - b1*c2)/determinant; 
        double y = (a1*c2 - a2*c1)/determinant; 
        if (equalsTo(x3, x4) || equalsTo(x3, x4))
        {
            if ((greaterThan(y, y3) && lessThan(y, y4)) || (greaterThan(y, y4) && lessThan(y, y3)))
            {
                if ((greaterThan(y, y1) && lessThan(y, y2)) || (greaterThan(y, y2) && lessThan(y, y1)))
                {
                    return true;
                }
            } 
        }
        else
        {
            if ((greaterThan(x, x3) && lessThan(x, x4)) || (greaterThan(x, x4) && lessThan(x, x3)))
            { 
                if ((greaterThan(x, x1) && lessThan(x, x2)) || (greaterThan(x, x2) && lessThan(x, x1)))
                {
                    return true;
                }
            } 
        }
    } 
    return false;
}

double doesIntersectInRay(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    // Line AB represented as a1x + b1y = c1 
    double a1 = y2 - y1; 
    double b1 = x1 - x2; 
    double c1 = a1*(x1) + b1*(y1); 
  
    // Line CD represented as a2x + b2y = c2 
    double a2 = y4 - y3; 
    double b2 = x3 - x4; 
    double c2 = a2*(x3)+ b2*(y3); 
  
    double determinant = a1*b2 - a2*b1; 
  

    if (determinant != 0)
    { 
        double x = (b2*c1 - b1*c2)/determinant; 
        double y = (a1*c2 - a2*c1)/determinant; 

        if (equalsTo(x, x1) && lessThan(y, y3))
        {
            return 0.5;
        }

        if (equalsTo(x, x2) && lessThan(y, y3))
        {
            return 0.5;
        }

        if ((greaterThan(x, x1) && lessThan(x, x2)) || (greaterThan(x, x2) && lessThan(x, x1)))
        {
            if ((greaterThan(y, y1) && lessThan(y, y2)) || (greaterThan(y, y2) && lessThan(y, y1)))
            {
                if (lessThan(y, y3))
                {
                    return 1.0;
                }
            }
        }
    } 
    return 0.0;
}

double distance(double Ax, double Ay, double Bx, double By)
{
	return sqrt(pow((Bx - Ax), 2) + pow((By - Ay), 2));
}


void printInput(string points[], double x[], double y[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        cout << points[i] << "=(" << x[i] << ", " << y[i] << ")" << endl;
    }
    fstream fin;
    fin.open("allviewpoints.txt");
    string* viewpoints = new string[1024];
    int k = 0;
    while (fin >> viewpoints[k])
    {
        k++;
    }
    bool found;
    int miss = 0;
    if (miss == 0)
    {
        cout << "Every viewpoints were placed!" << endl;
    }
    else
    {
        cout << "Missing viewpoints: ";
        for (int j = 0; j < k; ++j)
        {
            found = false;
            for (int r = 0; r < size; ++r)
            {
                if (strcmp(points[r].c_str(), viewpoints[j].c_str()) == 0)
                {
                    found = true;
                }
            }
            if (!found)
            {
                cout << viewpoints[j] << ", ";
                miss++;
            }
        }
        cout << endl;
    }
    
    fin.close();
	
}

bool lessThan(double a, double b)
{
    return (a + 2 * EPSILON <= b);
}
bool greaterThan(double a, double b)
{
    return (a >= b + 2 * EPSILON);
}
bool equalsTo(double a, double b)
{
    return (abs(a-b) < 2 * EPSILON);
}
