#include <iostream>
#include <string>
#include <array>
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
    double tmin;
    double tmax;
};

struct Grid
{
    int row;
    int column = 2 * row - 1;
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
Intervalle temporel_interval();
double ask_value();
Intervalle get_amplitude();
Grid tableau();



int main()
{
    Signal_type signal = get_signal();
    cout << "Signal " << signal << endl;
    int nbN = get_sine_term_count();
    cout << "nombre de termes " << nbN << endl;
    Intervalle inter = temporel_interval();
    cout << "temp interval" << inter.tmin << "       " << inter.tmax << endl;
    Intervalle amplitude = get_amplitude();
    cout << "amplitude " << amplitude.tmin << "       " << amplitude.tmax << endl;
    Grid grille = tableau();
    cout << "nb ligne et col " << grille.row << "         " << grille.column << endl;
    vector<vector<char>> grid;

}

Signal_type get_signal()
{
    string user_signal;
    cin >> user_signal;

    // La fonction compare de string revoie 0 si les deux string comparées 
    // sont similaires.
    constexpr int compare_success = 0;

    if (user_signal.compare("SAWTOOTH") == compare_success)
    {
        return SAWTOOTH;
    }
    else if (user_signal.compare("SQUARE") == compare_success)
    {
        return SQUARE;
    }
    else if (user_signal.compare("TRIANGLE") == compare_success)
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

double ask_value()
{
    double n;
    cin >> n;

    return n;
}




Intervalle temporel_interval() 
{
    double tmin = ask_value();
    double tmax = ask_value();

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
    Intervalle I = { tmin, tmax };
   
    return I;
}

Intervalle get_amplitude()
{
    double min = ask_value();
    double max = ask_value();
    
    if (min >= max) {
        print_error(SIGNAL_MIN_MAX);
    }

    Intervalle I = { min, max };

    return I;
}

Grid tableau()
{
    int n = ask_value();

    if (n <= 2) {
        print_error(NBN_TOO_SMALL);
    }
    else if (n % 2 == 0) 
    {
        print_error(NBL_NOT_ODD);
    }

    Grid I = { n };
    
    return I;
}

