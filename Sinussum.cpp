#include <iostream>
#include <vector>

using namespace std;

// constants and piece of code to be used for the project SINUSSUM
const double EPSIL_DICHO(1e-9);
const double EPSIL_T(1e-13);

// Error messages
const string BAD_SIGNAL("Error: signal type must be SAWTOOTH, SQUARE or TRIANGLE");
const string NBN_TOO_SMALL("Error: the number of sine terms must be greater than 0");
const string NBL_TOO_SMALL("Error: the number of lines must be greater than 2");
const string TIME_MIN_MAX("Error: time max is not bigger than min");
const string SIGNAL_MIN_MAX("Error: signal max is not bigger than min");
const string WRONG_TIME_VAL("Error: both time values must belong to [0., 1.]");
const string NBL_NOT_ODD("Error: the number of lines must be odd");

enum Signal_type
{
    SAWTOOTH,
    SQUARE,
    TRIANGLE,
    UNKNOWN
};

struct Intervalle
{
    double min;
    double max;
};

void print_error(string message) 
{
    cout << message;
    cout << endl;
    exit(0);
}

// Prototypes de fonctions.
Signal_type get_signal();
int get_sine_term_count();
Intervalle get_temporal_interval();
Intervalle get_amplitude();
int get_row_number();
vector<vector<char>> create_grid(int row, int column, Intervalle time, Intervalle s);
void draw_grid(vector<vector<char> > grid);





int main()
{
    Signal_type signal = get_signal();
    cout << "Signal " << signal << endl;

    int nbN = get_sine_term_count();
    cout << "nombre de termes " << nbN << endl;

    Intervalle inter = get_temporal_interval();
    cout << "temp interval" << inter.min << "       " << inter.max << endl;

    Intervalle amplitude = get_amplitude();
    cout << "amplitude " << amplitude.min << "       " << amplitude.max << endl;

    int row = get_row_number();
    int column = 2 * row - 1;
    cout << "nb ligne et col " << row << "         " << column << endl;

    vector<vector<char>> grid = create_grid(row, column, inter, amplitude);
    draw_grid(grid);




   /* vector<vector<char> > grid(row, vector<char>(column, '*'));

    for (int j(0); j < column; j++)
    {
        cout << '-';
    }
    cout << endl;

    for (int i(0); i < row; i++)
    {
        if (i == row / 2 )
        {
            for (int j(0); j < column; j++)
            {
                grid[i][j] = '.';
                cout << grid[i][j];
            }
            cout << endl;
            continue;
        }
        for (int j(0); j < column; j++)
        {
            cout << grid[i][j];
        }

        cout << endl;
    }

    for (int j(0); j < column; j++)
    {
        cout << '-';
    }
    cout << endl;*/



}

Signal_type get_signal()
{
    string user_signal;
    cin >> user_signal;

    if (user_signal == "SAWTOOTH")
    {
        return SAWTOOTH;
    }
    else if (user_signal == "SQUARE")
    {
        return SQUARE;
    }
    else if (user_signal == "TRIANGLE")
    {
        return TRIANGLE;
    }

    print_error(BAD_SIGNAL);

    return UNKNOWN;
}

int get_sine_term_count()
{
    int n;
    cin >> n;
    
    if (n <= 0) 
    {
        print_error(NBN_TOO_SMALL);
    }

    return n;
}



Intervalle get_temporal_interval() 
{
    double tmin;
    cin >> tmin;
    
    double tmax;
    cin >> tmax;

    if (tmin >= tmax) 
    {
        print_error(TIME_MIN_MAX);
    }
    
    if ((tmin < 0) or (tmin > 1))
    {
        print_error(WRONG_TIME_VAL);
    }
    else if ((tmax > 1) or (tmax < 0)) 
    {
        print_error(WRONG_TIME_VAL);
    }

    Intervalle i = { tmin, tmax };
   
    return i;
}

Intervalle get_amplitude()
{
    double min;
    cin >> min;

    double max;
    cin >> max;
    
    if (min >= max) 
    {
        print_error(SIGNAL_MIN_MAX);
    }

    Intervalle i = { min, max };

    return i;
}

int get_row_number()
{
    int n;
    cin >> n;

    if (n <= 2) 
    {
        print_error(NBN_TOO_SMALL);
    }
    else if (n % 2 == 0) 
    {
        print_error(NBL_NOT_ODD);
    }
    
    return n;
}

vector<vector<char>> create_grid(int row, int column, Intervalle time, Intervalle s)
{
    vector<vector<char>> grid(row, vector<char>(column, '@'));

    double delta_t = (time.max - time.min) / (column - 1);
    double delta_s = (s.max - s.min) / (row - 1);
    
    int i = (0 - s.min) / delta_s + 0.5;
    
    if (i >= 0)
    {
        for (size_t j(0); j < column; j++)
        {
            grid[row - 1 - i][j] = '.';
        }
    }
   

    return grid;
}

void draw_grid(vector<vector<char> > grid)
{ 
    const size_t column = grid[0].size();
    const size_t row = grid.size();

    for (size_t i(0); i < row; i++)
    {
        for (size_t j(0); j < column; j++)
        {
          cout << grid[i][j];
        }
        cout << endl;
    }
}

