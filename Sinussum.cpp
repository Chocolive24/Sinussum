#include <iostream>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

using namespace std;

enum Signal_type
{
	SAWTOOTH,
	SQUARE,
	TRIANGLE,
	UNKNOWN
};

// Constants and piece of code to be used for the project SINUSSUM.  
const double EPSIL_DICHO(1e-9);
const double EPSIL_T(1e-13);

// Error messages.
const string BAD_SIGNAL("Error: signal type must be SAWTOOTH, SQUARE or TRIANGLE");
const string NBN_TOO_SMALL("Error: the number of sine terms must be greater than 0");
const string NBL_TOO_SMALL("Error: the number of lines must be greater than 2");
const string TIME_MIN_MAX("Error: time max is not bigger than min");
const string SIGNAL_MIN_MAX("Error: signal max is not bigger than min");
const string WRONG_TIME_VAL("Error: both time values must belong to [0., 1.]");
const string NBL_NOT_ODD("Error: the number of lines must be odd");

struct Interval
{
	double min;
	double max;
};

struct Grid_data 
{
	Interval t;
	Interval s;
	int row_count;
	int column_count;
	double delta_t;
	double delta_s;
	int s_zero_row_idx;
	const int first_row_idx = 0;
};

// Function prototypes.
void print_error(string message);

// Functions that retrieves data from user inputs.
Signal_type get_signal_type();
int get_sine_count();
Interval get_temporal_interval();
Interval get_amplitude_interval();
int get_grid_row_count();
Grid_data get_grid_data();

// Functions that handle theoretical signal values.
void add_theoritical_val(vector<vector<char>>& grid, Signal_type type, double t, 
						 const Grid_data& grid_data, int j);
bool is_value_discontinued(const vector<double>& discontinuous_values, double t);
double calculate_theoritcial_val(Signal_type signal_type, double t);

// Functions that handle approximate signal values.
double calculate_approx_val(Signal_type signal_type, int sine_count, double t);
void add_approximate_values(vector<vector<char>>& grid, Signal_type type,
							int sine_count, double t, const Grid_data& grid_data,
							int j);

// Functions that handle grid creation and display. 
vector<vector<char>> create_grid(Signal_type signal_type, int sine_count, 
								 const Grid_data& grid_data);
void draw_grid(const vector<vector<char>>& grid);

// Functions that manage the dichotomic search for the maximum of the 
// approximated signal over a period.
double find_approx_signal_max(Signal_type signal_type, int sine_count);
double dichotomic_max_search(Interval t, Signal_type signal_type, int sine_count);

int main()
{
	// Reading and checking parameters.
	Signal_type signal_type = get_signal_type();
	int sine_count = get_sine_count();
	const Grid_data grid_data = get_grid_data();

	// Calculation and display of the signal grid.
	vector<vector<char>> grid = create_grid(signal_type, sine_count, grid_data);
	draw_grid(grid);

	// Search for the maximum of the approximated signal over a period
	double max = find_approx_signal_max(signal_type, sine_count);
	cout << setprecision(8) << fixed << max << endl;
}

void print_error(string message)
{
	cout << message;
	cout << endl;
	exit(0);
}

Signal_type get_signal_type()
{
	string user_signal;
	cin >> user_signal;

	Signal_type signal_type = UNKNOWN;

	if (user_signal == "SAWTOOTH")
	{
		signal_type = SAWTOOTH;
	}
	else if (user_signal == "SQUARE")
	{
		signal_type = SQUARE;
	}
	else if (user_signal == "TRIANGLE")
	{
		signal_type = TRIANGLE;
	}
	else
	{
		print_error(BAD_SIGNAL);
	}
	
	return signal_type;
}

int get_sine_count()
{
	int n;
	cin >> n;

	if (n <= 0)
	{
		print_error(NBN_TOO_SMALL);
	}

	return n;
}

Interval get_temporal_interval()
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

	Interval i = { tmin, tmax };

	return i;
}

Interval get_amplitude_interval()
{
	double min;
	cin >> min;

	double max;
	cin >> max;

	if (min >= max)
	{
		print_error(SIGNAL_MIN_MAX);
	}

	Interval i = { min, max };

	return i;
}

int get_grid_row_count()
{
	int n;
	cin >> n;

	if (n <= 2)
	{
		print_error(NBL_TOO_SMALL);
	}
	else if (n % 2 == 0)
	{
		print_error(NBL_NOT_ODD);
	}

	return n;
}

Grid_data get_grid_data() 
{
	const Interval t = get_temporal_interval();
	const Interval s = get_amplitude_interval();
	const int row_count = get_grid_row_count();

	// Calculates the other grid values with the user input values.
	const int column_count = 2 * row_count - 1;
	const double delta_t = (t.max - t.min) / (column_count - 1);
	const double delta_s = (s.max - s.min) / (row_count - 1);
	const int s_zero_row_idx = (0 - s.min) / delta_s + 0.5;

	const Grid_data grid_data = { t, s, row_count, column_count, 
								  delta_t, delta_s, s_zero_row_idx };

	return grid_data;
}

vector<vector<char>> create_grid(Signal_type signal_type, int sine_count,
								 const Grid_data& grid_data)
{
	const int row_count = grid_data.row_count;
	const int column_count = grid_data.column_count;
	vector<vector<char>> grid(row_count, vector<char>(column_count, ' '));

	for (int j(0); j < column_count; j++)
	{
		// Add '.' symbol that represents the temporal axis when the amplitude is 0
		// only if the line index is indeed inside the grid.
		if (grid_data.s_zero_row_idx >= grid_data.first_row_idx)
		{
			grid[row_count - 1 - grid_data.s_zero_row_idx][j] = '.';
		}

		const double t = grid_data.t.min + (j * grid_data.delta_t);

		add_theoritical_val(grid, signal_type, t, grid_data, j);

		add_approximate_values(grid, signal_type, sine_count, t, grid_data, j);
	}

	return grid;
}


void add_theoritical_val(vector<vector<char>>& grid, Signal_type type, double t,
	const Grid_data& grid_data, int j)
{
	vector<double> discoutinuous_values = {};
	switch (type)
	{
	case SAWTOOTH:
		discoutinuous_values = { 0.0, 1.0 };
		break;
	case SQUARE:
		discoutinuous_values = { 0.0, 0.5, 1.0 };
		break;
	case TRIANGLE:
	default:
		break;
	}

	if (is_value_discontinued(discoutinuous_values, t))
	{
		if (grid_data.s_zero_row_idx >= grid_data.first_row_idx)
		{
			grid[grid_data.s_zero_row_idx][j] = '+';
		}
	}
	else
	{
		double v_th = calculate_theoritcial_val(type, t);
		double v = ((v_th - grid_data.s.min) / (grid_data.delta_s)) + 0.5;

		if (v >= grid_data.first_row_idx)
		{
			int i = grid_data.row_count - 1 - static_cast<int>(v);
			grid[i][j] = '+';
		}
	}
}

void add_approximate_values(vector<vector<char>>& grid, Signal_type type, 
							int sine_count, double t, const Grid_data& grid_data, 
							int j)
{
	// Add '*' symbol for approximate signal values.
	double s_t = calculate_approx_val(type, sine_count, t);
	double v = (s_t - grid_data.s.min) / grid_data.delta_s + 0.5;

	if ((v >= 0) and (v < grid_data.row_count))
	{
		int i = static_cast<int>(v);
		grid[grid_data.row_count - 1 - i][j] = '*';
	}
}

bool is_value_discontinued(const vector<double>& discontinuous_values, double t)
{
	for (const auto disc_value : discontinuous_values)
	{
		if ((t >= disc_value - EPSIL_T) and
			(t <= disc_value + EPSIL_T))
		{
			return true;
		}
	}

	return false;
}

void draw_grid(const vector<vector<char>>& grid)
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

double calculate_theoritcial_val(Signal_type signal_type, double t)
{
	switch (signal_type)
	{
		case SAWTOOTH:
			return -1. + (2. * t);
		case SQUARE:
			if ((t > 0.) and (t < 0.5))
			{
				return 1;
			}
			return -1;
		case TRIANGLE:
			if ((t >= 0) and (t < 0.5))
			{
				return -1 + 4 * t;
			}
			return -4 * t + 3;
		default:
			return 0;
	}
};

double calculate_approx_val(Signal_type signal_type, 
							int sine_count, double t)
{
	double sin_sum = 0.0;

	switch (signal_type)
	{
	case SAWTOOTH:
		for (int k(1); k <= sine_count; k++)
		{
			sin_sum += (pow(-1, k) / k) * sin(2 * M_PI * k * (t - 0.5));
		}
		return ((-2) / M_PI) * sin_sum;
	case SQUARE:
		for (int k(1); k <= sine_count; k++)
		{
			sin_sum += sin(2 * M_PI * (2 * k - 1) * t) / (2 * k - 1);
		}
		return (4 / M_PI) * sin_sum;
	case TRIANGLE:
		for (int k(1); k <= sine_count; k++)
		{
			sin_sum += (pow(-1, k) / pow(2 * k - 1, 2)) *
				sin(2 * M_PI * (2 * k - 1) * (t - 0.25));
		}
		return ((-8) / pow(M_PI, 2)) * sin_sum;
	default:
		return 0.0;
	}
}

double find_approx_signal_max(Signal_type signal_type, int sine_count)
{
	Interval t = { 0, 0 };

	// Get the correct period based on the signal type.
	switch (signal_type)
	{
	case SAWTOOTH:
		t.min = 1. - 1. / (2 * sine_count + 1);
		t.max = 1;
		break;
	case SQUARE:
		t.min = 0;
		t.max = 1. / (2 * sine_count + 1);
		break;
	case TRIANGLE:
		t.min = 0.5 - 1. / (2 * (2 * sine_count + 1));
		t.max = 0.5 + 1. / (2 * (2 * sine_count + 1));
		break;
	}

	return dichotomic_max_search(t, signal_type, sine_count);
}

double dichotomic_max_search(Interval t, Signal_type signal_type, int sine_count)
{
	double max = 0.0;
	double old_max = 0.0;
	while (true)
	{
		double t_mid = (t.max + t.min) / 2.0;
		double t_mid_left = (t_mid + t.min) / 2.0;
		double t_mid_right = (t.max + t_mid) / 2.0;

		double l_val = calculate_approx_val(signal_type, sine_count, t_mid_left);
		double r_val = calculate_approx_val(signal_type, sine_count, t_mid_right);

		if (l_val > r_val)
		{
			t.max = t_mid;
			max = l_val;
		}
		else
		{
			t.min = t_mid;
			max = r_val;
		}

		if (fabs(max - old_max) < EPSIL_DICHO)
		{
			return max;
		}

		old_max = max;
	}
}																					  