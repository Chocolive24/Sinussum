#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

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
vector<vector<char>> create_grid(Signal_type type, int row, int column, Intervalle time, Intervalle s, int nbN);
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

    vector<vector<char>> grid = create_grid(signal, row, column, inter, amplitude, nbN);
    draw_grid(grid);
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

vector<vector<char>> create_grid(Signal_type type, int row, int column, Intervalle time, Intervalle s, int nbN)
{
    vector<vector<char>> grid(row, vector<char>(column, ' '));

    double delta_t = (time.max - time.min) / (column - 1);
    double delta_s = (s.max - s.min) / (row - 1);
    
    const int s_zero = (0 - s.min) / delta_s + 0.5;

    for (size_t j(0); j < column; j++) 
    {
        double t = time.min + (j * delta_t);

        // Add '.' symbol to represent temporal axis when equal to zero.
        if (s_zero >= 0)
        {
            grid[row - 1 - s_zero][j] = '.';
        }

        bool is_value_discountinued = false;

        switch (type)
        { 
            case SAWTOOTH:
            {
              // Add '+' symbol to represent theorical signal value.

              // For discontinue values
              if ((t >= 0 - EPSIL_T) and (t <= 0 + EPSIL_T)) 
              {
                if (s_zero >= 0) 
                {
                    grid[s_zero][j] = '+';
                    is_value_discountinued = true;
                }
              }

              if ((t >= 1 - EPSIL_T) and (t <= 1 + EPSIL_T)) 
              {
                if (s_zero >= 0) 
                {
                    grid[s_zero][j] = '+';
                    is_value_discountinued = true;
                }
              }

              if (!is_value_discountinued)
              {
                // For "Normal" values
                double saw = -1. + (2. * t);
                double v_th = ((saw - s.min) / (delta_s)) + 0.5;

                if (v_th >= 0) 
                {
                    int i = row - 1 - static_cast<int>(v_th);
                    grid[i][j] = '+';
                }
              }

              // Add '*' symbol for approximate signal values.

              double sin_sum = 0.;

              for (int k(1); k <= nbN; k++) 
              {
                sin_sum += (pow(-1, k) / k) * sin(2 * M_PI * k * (t - 0.5));
              }

              double v = ((((-2) / M_PI) * sin_sum) - s.min) / delta_s + 0.5;

              if ((v >= 0) and (v < row)) 
              {
                int i = static_cast<int>(v);
                grid[row - 1 - i][j] = '*';
              }

              break;
            }
              
            case SQUARE:

            {
                // Add '+' symbol to represent theorical signal value.

                // For discontinue values
                if ((t >= 0 - EPSIL_T) and (t <= 0 + EPSIL_T)) 
                {
                    if (s_zero >= 0)
                    {
                      grid[s_zero][j] = '+'; 
                      is_value_discountinued = true;
                    }

                }

                if ((t >= 0.5 - EPSIL_T) and (t <= 0.5 + EPSIL_T)) 
                {
                    if (s_zero >= 0) 
                    {
                      grid[s_zero][j] = '+';
                      is_value_discountinued = true;
                    }

                }

                if ((t >= 1 - EPSIL_T) and (t <= 1 + EPSIL_T)) 
                {
                    if (s_zero >= 0) 
                    {
                      grid[s_zero][j] = '+';
                      is_value_discountinued = true;
                    }

                }

                if (!is_value_discountinued)
                {
                    // '+'
                    int squa = 0;

                    if ((t > 0.) and (t < 0.5)) 
                    {
                      squa = 1;
                    }
                    if ((t > 0.5) and (t < 1.)) 
                    {
                      squa = -1;
                    }

                    double v_th = (squa - s.min) / delta_s + 0.5;
                    if (v_th >= 0) 
                    {
                      int i = row - 1 - static_cast<int>(v_th);
                      grid[i][j] = '+';
                    }
                }

                // '*'
                double sin_sum = 0;

                for (int k(1); k <= nbN; k++)
                {
                    sin_sum += sin(2 * M_PI * (2 * k - 1) * t) / (2 * k - 1);
                }

                double v = (4 / M_PI * sin_sum - s.min) / delta_s + 0.5;

                if ((v >= 0) and (v < row))
                {
                    int i = row - 1 - static_cast<int>(v);
                    grid[i][j] = '*';
                }

                break;
            }

            case TRIANGLE:
            {
                // '+'
                double trig;
                if ((t >= 0) and (t < 0.5))
                {
                    trig = -1 + 4 * t;


                }
                else
                {
                    trig = -4 * t + 3;

                }
                double v_th = (trig - s.min) / delta_s + 0.5;
                if (v_th >= 0)
                {
                    int i = row - 1 - static_cast<int>(v_th);
                    grid[i][j] = '+';
                }
                
                double sin_sum(0);
                
                for (int k(1); k <= row; k++)
                {
                    sin_sum += (pow(-1, k) / pow(2 * k - 1, 2))*sin(2 * M_PI * (2 * k - 1) * (t - 0.25));
                   
                }

                double v = ((-8) / pow(M_PI, 2) * sin_sum - s.min) / delta_s + 0.5;

                 if (v >= 0) 
                 {
                    int i = row - 1 - static_cast<int>(v);
                    grid[i][j] = '*';
                }


                break;
            }
        }
       
    }

   

    return grid;
}

void draw_grid(vector<vector<char> > grid)
{ 
    const size_t column = grid[0].size();
    const size_t row = grid.size();

    for (size_t j(0); j < column; j++)
    {
        cout << '-';
    }
    cout << endl;
        

    for (size_t i(0); i < row; i++)
    {
        for (size_t j(0); j < column; j++)
        {
          cout << grid[i][j];
        }
        cout << endl;
    }

    for (size_t j(0); j < column; j++)
    {
        cout << '-';
    }
    cout << endl;
}
