#include "Test/TestGenetic.h"
#include "Algorithm/Genetic.h"
#include <ctime>
#include <cmath>
#include <iostream>

static constexpr int population_size = 10;//种群规模
static constexpr float cross_rate = 0.01f;//交叉概率
static constexpr float variation_rate = 0.005f;//变异概率
static constexpr int iterator_size = 1000;//迭代次数

static rapid::Genetic<double, double> g;

static constexpr int MAX = 300;
static constexpr int MIN = -300;

//适应度函数
double fitness(double x)
{
    if(x < MIN) return -9999999;
    if(x > MAX) return -9999999;
    return x * sin(10 * M_PI * x) + 2;
}

//交叉
void cross(double *population, unsigned long long size)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    for(unsigned long long i = 0; i < size; ++i)
    {
        int r = rand() % 100;
        if(r >= 95)
            population[i] += 0.1;
        else if(r >= 90)
            population[i] -= 0.1;
        else if(r >= 85)
            population[i] += 0.01;
        else if(r >= 80)
            population[i] -= 0.01;
        else if(r >= 75)
            population[i] += 0.001;
        else if(r >= 70)
            population[i] -= 0.001;
        else if(r >= 65)
            population[i] += 0.0001;
        else if(r >= 60)
            population[i] -= 0.0001;
        else if(r >= 55)
            population[i] += 0.00001;
        else if(r >= 50)
            population[i] -= 0.00001;
        else if(r >= 45)
            population[i] += 0.000001;
        else if(r >= 40)
            population[i] -= 0.000001;
        else if(r >= 35)
            population[i] += 0.0000001;
        else if(r >= 30)
            population[i] -= 0.0000001;
        else if(r >= 25)
            population[i] += 0.00000001;
        else if(r >= 20)
            population[i] -= 0.00000001;
        else if(r >= 15)
            population[i] += 0.000000001;
        else if(r >= 10)
            population[i] -= 0.000000001;
        else if(r >= 5)
            population[i] += 1;
        else
            population[i] -= 1;
    }
    srand(static_cast<unsigned int>(time(nullptr)));
    for(unsigned long long i = 0; i < size; ++i)
    {
        if(rand() % 100 < cross_rate * 100)
        {
            population[i] /= 10;
        }
    }
}

//变异
void variation(double *population, unsigned long long size)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    for(unsigned long long i = 0; i < size; ++i)
    {
        if(rand() % 100 < variation_rate * 100)
        {
            int r = rand() % 100;
            if(r >= 75)
                population[i] += 10;
            else if(r >= 50)
                population[i] -= 10;
            else if(r >= 25)
                population[i] += 100;
            else
                population[i] -= 100;
        }
    }
}

//初始种群
void generate()
{
    double population[population_size];
    srand(static_cast<unsigned int>(time(nullptr)));
    for(int i = 0; i < population_size; ++i)
    {
        int r = rand();
        population[i] = r % (MAX - MIN + 1) + MIN;
    }
    g.init(population, population_size);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    generate();
    std::function<double(double)> f = std::bind(&fitness, std::placeholders::_1);
    std::function<void(double*, unsigned long long)> c = std::bind(&cross, std::placeholders::_1, std::placeholders::_2);
    std::function<void(double*, unsigned long long)> v = std::bind(&variation, std::placeholders::_1, std::placeholders::_2);
    g.set_fitness_function(f);
    g.set_cross_function(c);
    g.set_variation_function(v);
    g.reproduce(iterator_size, 0);
    for(unsigned long long i = 0; i < population_size; ++i)
    {
        std::cout << g[i] << " ----> " << f(g[i]) << std::endl;
    }
    return 0;
}
