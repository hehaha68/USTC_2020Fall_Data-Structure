#include <cmath>
#include <iostream>

#include "util.h"
#include "../queue.h"

extern double simulate(const size_t queue_capacity, const size_t num_servers, Queue<Customer> &arrival_flow);

#define CONFIDENCE_95 1.96

std::random_device rd;
std::mt19937 generator(rd());

// arrive and service follows exponential distribution
auto new_customers(const double end_time, const double arrival_rate, const double service_rate)
{
    std::exponential_distribution<double> arrival(arrival_rate);
    std::exponential_distribution<double> service(service_rate);
    Queue<Customer> arrival_flow;
    Queue<Customer>::init(arrival_flow, static_cast<size_t>(arrival_rate * end_time));

    double arrival_time = 0;
    while (arrival_time < end_time)
    {
        arrival_time += arrival(generator);
        Customer customer;
        customer.arrive_time = arrival_time;
        customer.service_time = service(generator);
        Queue<Customer>::enqueue(arrival_flow, customer);
    }

    return arrival_flow;
}

double factorial(const int n)
{
    double result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

// M/M/c/N/inf/FCFS queuing theory model
double expect(const size_t queue_capacity, const size_t num_servers, const double arrival_rate, const double service_rate)
{
    double c = num_servers;
    double K = queue_capacity + c;

    double rho = arrival_rate / (c * service_rate); // traffic intensity

    if (rho >= 1 || arrival_rate <= 0 || service_rate <= 0)
    {
        return NAN;
    }

    // probability of no customer in the system (by Markov Stationary Distributions)
    double p0 = 0;
    for (size_t k = 0; k <= c; k++)
    {
        p0 += pow(rho * c, k) / factorial(k);
    }
    for (size_t k = c + 1; k <= K; k++)
    {
        p0 += pow(rho * c, k) * pow(c, c - k) / factorial(c);
    }
    p0 = 1 / p0;

    // probability of the system is full of customers
    double pK = p0 * pow(c, c) * pow(rho, K) / factorial(c);

    // expection of the length of the queue
    double queue_length = p0 * rho * pow(c * rho, c) * (1 - pow(rho, K - c) - (K - c) * pow(rho, K - c) * (1 - rho)) / factorial(c) / pow(1 - rho, 2);
    // expection of the time waiting in a queue
    double queue_time = queue_length / (arrival_rate * (1 - pK));
    return queue_time;
}

void statistics(const int N, const int T, double &mean, double &stddev, double &min, double &max, const size_t queue_capacity, const size_t num_servers, const double arrival_rate, const double service_rate)
{
    std::vector<double> result;
    for (int i = 0; i < N; i++)
    {
        auto arrival_flow = new_customers(T, arrival_rate, service_rate);
        result.push_back(simulate(queue_capacity, num_servers, arrival_flow));
    }

    mean = 0;
    for (auto it : result)
    {
        mean += it;
    }
    mean /= N;

    stddev = 0;
    for (auto it : result)
    {
        stddev += pow(it - mean, 2);
    }

    stddev = sqrt(stddev / (N - 1));

    // we have 99% confidence that it's in the interval [min, max]
    min = mean - CONFIDENCE_95 * stddev / sqrt(N);
    max = mean + CONFIDENCE_95 * stddev / sqrt(N);
}

bool test_simulation(const int N, const int T, const size_t queue_capacity, const size_t num_servers, const double arrival_rate, const double service_rate)
{
    double mean, stddev, min, max;
    double true_mean = expect(queue_capacity, num_servers, arrival_rate, service_rate);

    std::cout << "Expectation: " << true_mean << std::endl;
    statistics(N, T, mean, stddev, min, max, queue_capacity, num_servers, arrival_rate, service_rate);
    std::cout << "Simulation: [" << min << ", " << max << "]" << std::endl;

    return min <= true_mean && true_mean <= max;
}