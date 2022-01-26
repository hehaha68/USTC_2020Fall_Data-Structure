#include <random>

#include <stddef.h>

struct Customer
{
    double arrive_time;
    double service_time;
    double leave_time;
};

typedef struct Customer Customer;

bool test_simulation(const int N, const int T, const size_t queue_capacity, const size_t num_servers, const double arrival_rate, const double service_rate);

