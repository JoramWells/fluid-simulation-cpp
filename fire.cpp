#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

#define TREE 0
#define SMOKING 1
#define BURNING 2
#define BURNT 3

int main();
int seed_by_time(int offset);
void timestamp(void);

// ***************************************************************************class -->Time
class Time
{
public:
    int seed_by_time(int offset)

    //******************************************************************************
    //    SEED_BY_TIME seeds the random number generator using the time as a seed.
    //

    {
        int seed;
        time_t the_time;

        time(&the_time);

        seed = (int)the_time + offset;

        return seed;
    }

    void timestamp()
    {
#define TIME_SIZE 40

        static char time_buffer[TIME_SIZE];
        const struct std::tm *tm_ptr;
        std::time_t now;

        now = std::time(NULL);
        tm_ptr = std::localtime(&now);

        std::strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr);

        std::cout << time_buffer << "\n";

        return;
#undef TIME_SIZE
    }
};

// ***************************************************************************class -->Forest
class Forest
{
private:
    //****************************************************************************80
    // determines whether the fire should spread
    bool fire_spreads(double probability_spread)
    {
        double u;
        bool value;

        u = (double)rand() / (double)RAND_MAX;

        if (u < probability_spread)
        {
            value = true;
        }
        else
        {
            value = false;
        }
        return value;
    }

    //****************************************************************************80
public:
    int forest_size, x_point, y_point;
    int **forest;

    // Forest constructor
    Forest(int size, int **d2, int x, int y)
    {
        forest_size = size;
        forest = d2;
        x_point = x;
        y_point = y;
    }

    // 21 X 21 grid cell of fire
    // Entire forest with only origin o of fire starting point
    void forest_2D()
    {
        int i, j;
        for (i = 0; i < forest_size; i++)
        {
            cout << " ";
            for (j = 0; j < forest_size; j++)
            {
                // Print origin o of a cell, starting fire
                if (i == x_point && j == y_point)
                {
                    cout << " o ";
                }
                else

                // Print T representing a tree in a forest.
                {
                    cout << " T ";
                }
            }
            cout << "\n";
        }
    }

    // Display grid 21 X 21 of a forest on fire
    void forest_2D_on_fire()
    {
        int i;
        int j;

        cout << "\n";
        cout << "  Map of fire damage.\n";
        cout << "  Fire starts at cell with 'o'.\n";
        cout << "  Burned trees are indicated by '.'.\n";
        cout << "  Standing trees 'T'.\n";
        cout << "\n";

        for (i = 0; i < forest_size; i++)
        {
            cout << "  ";
            for (j = 0; j < forest_size; j++)
            {
                if (i == x_point && j == y_point)
                {
                    cout << " o ";
                }

                // print burnt trees
                else if (forest[i][j] == BURNT)
                {
                    cout << " . ";
                }
                else

                // Unburt trees
                {
                    cout << " T ";
                }
            }
            cout << "\n";
        }
        return;
    }

    //****************************************************************************
    //
    //    allocates space for a forest of the given size.
    int **forest_space_allocate()

    {
        int i;

        forest = new int *[forest_size];
        for (i = 0; i < forest_size; i++)
        {
            forest[i] = new int[forest_size];
        }

        return forest;
    }

    //****************************************************************************
    //    sets a given tree to the SMOKING state.
    //****************************************************************************
    void start_fire()

    {
        // point of fire origin
        forest[x_point][y_point] = SMOKING;

        return;
    }

    //****************************************************************************
    //   shows if any trees in the forest is on fire.
    //****************************************************************************
    bool is_burning()
    {
        int i;
        int j;
        bool value;

        value = false;

        for (i = 0; i < forest_size; i++)
        {
            for (j = 0; j < forest_size; j++)
            {
                if (forest[i][j] == SMOKING || forest[i][j] == BURNING)
                {
                    value = true;
                    return value;
                }
            }
        }

        return value;
    }

    // an instance of the burning forest.
    // 50% probability of a tree to catch
    void apply_spread(double probability_spread)
    {
        int i;
        int j;
        //
        //  Burning trees burn down;
        //  Smoldering trees ignite;
        //
        for (i = 0; i < forest_size; i++)
        {
            for (j = 0; j < forest_size; j++)
            {
                if (forest[i][j] == BURNING)
                {
                    forest[i][j] = BURNT;
                }
                else if (forest[i][j] == SMOKING)
                {
                    forest[i][j] = BURNING;
                }
            }
        }
        //
        //  Unburnt trees might catch fire.
        //
        for (i = 0; i < forest_size; i++)
        {
            for (j = 0; j < forest_size; j++)
            {
                if (forest[i][j] == BURNING)
                {
                    //
                    //  North.
                    //
                    if (i != 0)
                    {
                        if (fire_spreads(probability_spread) && forest[i - 1][j] == TREE)
                        {
                            forest[i - 1][j] = SMOKING;
                        }
                    }
                    //
                    //  South.
                    //
                    if (i != forest_size - 1)
                    {
                        if (fire_spreads(probability_spread) && forest[i + 1][j] == TREE)
                        {
                            forest[i + 1][j] = SMOKING;
                        }
                    }
                    //
                    //  West.
                    //
                    if (j != 0)
                    {
                        if (fire_spreads(probability_spread) && forest[i][j - 1] == TREE)
                        {
                            forest[i][j - 1] = SMOKING;
                        }
                    }
                    //
                    //  East.
                    //
                    if (j != forest_size - 1)
                    {
                        if (fire_spreads(probability_spread) && forest[i][j + 1] == TREE)
                        {
                            forest[i][j + 1] = SMOKING;
                        }
                    }
                }
            }
        }
        return;
    }

    //****************************************************************************80

    double get_percent_burned()

    {
        int i;
        int j;
        int sum;
        double value;

        sum = 0;
        for (i = 0; i < forest_size; i++)
        {
            for (j = 0; j < forest_size; j++)
            {
                if (forest[i][j] == BURNT)
                {
                    sum = sum + 1;
                }
            }
        }

        value = (double)(sum) / (double)(forest_size * forest_size);

        return value;
    }
    //****************************************************************************80

    void del_forest()

    //****************************************************************************80
    //    FOREST_DELETE deletes the memory previously allocated for a forest.
    {
        int i;

        for (i = 0; i < forest_size; i++)
        {
            delete[] forest[i];
        }
        delete[] forest;

        return;
    }
};

//****************************************************************************80

int main()

//****************************************************************************80
//
//  Purpose:
//
//    MAIN is the main program for FIRE_SERIAL.
//
{
    int **forest;
    int forest_size = 20;
    int i_ignite;
    int j_ignite;
    int offset;
    double percent_burned = 0.0;
    double probability_spread = 0.5;
    int seed;
    double u;

    // Instantiating time class
    Time t;

    t.timestamp();
    cout << "\n";
    cout << "FIRE_SERIAL\n";
    cout << "  C++ version\n";
    cout << "  A probabilistic simulation of a forest fire.\n";
    cout << "  The probability of tree-to-tree spread is " << probability_spread << "\n";
    //
    //  Initialize the random number generator.
    //
    offset = 0;
    seed = t.seed_by_time(offset);
    cout << "  The random number generator is seeded by " << seed << ".\n";
    srand(seed);

    u = (double)rand() / (double)RAND_MAX;
    i_ignite = (int)((double)forest_size * u);
    u = (double)rand() / (double)RAND_MAX;
    j_ignite = (int)((double)forest_size * u);

    Forest fores(forest_size, forest, i_ignite, j_ignite);

    //
    //  Create the forest.
    //
    forest = fores.forest_space_allocate();

    //  Choose a tree at random where the fire will start.
    //

    fores.start_fire();
    cout << "\n";
    cout << "  Fire starts at tree[" << i_ignite << "][" << j_ignite << "].\n";
    //
    //  Let time run until nothing is burning any more.
    //
    while (fores.is_burning())
    {
        fores.apply_spread(probability_spread);
    }

    //
    //  Display the final forest state. Using class
    //
    // forest_2D_on_fire(forest_size, forest, i_ignite, j_ignite);
    int in;

    do
    {
        cout << "Select 1 to print area map." << endl;
        cout << "Select 2 to print area of burnt forest." << endl;
        cout << "Select 3 to restart." << endl;

        cin >> in;

        switch (in)
        {
        case 1:
            fores.forest_2D();
            break;
        case 2:
            fores.forest_2D_on_fire();
            break;
        case 3:
            exit(0);
            break;
        default:
            cout << "Invalid input";
        }
    } while (true);

    // fores.forest_2D_on_fire();
    //
    //  Report the percentage of forest burned.
    //
    percent_burned = fores.get_percent_burned();

    cout << "\n";
    cout << "  Percentage of forest burned = " << percent_burned << "\n";
    //
    //  Free memory.
    //
    fores.del_forest();
    //
    //  Terminate.
    //
    cout << "\n";
    cout << "FIRE_SERIAL:\n";
    cout << "  Normal end of execution.\n";
    cout << "\n";
    t.timestamp();

    return 0;
}