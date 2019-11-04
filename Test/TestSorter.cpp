#include "Test/TestSorter.h"
#include "Algorithm/Sorter.h"
#include <chrono>
#include <any>

static int NUM = 50000, times = 10;

static std::time_t get_time_stamp()
{
    typedef std::chrono::nanoseconds secondstype;
    std::chrono::time_point<std::chrono::system_clock, secondstype> tp = std::chrono::time_point_cast<secondstype>(std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<secondstype>(tp.time_since_epoch());
    return tmp.count();
}

static void print_time(std::time_t ns)
{
    std::cout << "uses time: ";
    std::cout << ns / 1000000 << " ms + ";
    ns %= 1000000;
    std::cout << ns / 1000 << " us + ";
    std::cout << ns % 1000 << " ns" << std::endl;
}

void rapid::test_Sorter_main()
{
    std::cout << "it will sort a array with " << NUM << " elements for " << times << " times\n";
    std::cout << "output time is the average\n";
    int *m = new int[static_cast<unsigned long long>(NUM)];
    int *n = new int[static_cast<unsigned long long>(NUM)];
    int *k = new int[static_cast<unsigned long long>(NUM)];
    int *o = new int[static_cast<unsigned long long>(NUM)];
    int *p = new int[static_cast<unsigned long long>(NUM)];
    int *q = new int[static_cast<unsigned long long>(NUM)];
    long long temp;
    long long insertion_sort_time = 0;
    long long quick_sort_time = 0;
    long long bubble_sort_time = 0;
    long long merge_sort_time = 0;
    long long shell_sort_time = 0;
    long long hash_sort_time = 0;
    for(int i = 0; i < times; ++i)
    {
        std::cout << "generate array element start" << std::endl;
        generate_random({m, n, k, o, p, q}, NUM);
        std::cout << "generate array element finish" << std::endl;

        std::cout << i + 1 << " times start" << std::endl;

        temp = get_time_stamp();
        isort(m, m + NUM);
        insertion_sort_time += get_time_stamp() - temp;
        std::cout << "insertion-sort finish" << std::endl;

        temp = get_time_stamp();
        qsort(n, n + NUM);
        quick_sort_time += get_time_stamp() - temp;
        std::cout << "quick-sort finish" << std::endl;

        temp = get_time_stamp();
        bsort(k, k + NUM);
        bubble_sort_time += get_time_stamp() - temp;
        std::cout << "bubble-sort finish" << std::endl;

        temp = get_time_stamp();
        msort(o, o + NUM);
        merge_sort_time += get_time_stamp() - temp;
        std::cout << "merge-sort finish" << std::endl;

        temp = get_time_stamp();
        ssort(p, p + NUM);
        shell_sort_time += get_time_stamp() - temp;
        std::cout << "shell-sort finish" << std::endl;

        temp = get_time_stamp();
        hsort(q, q + NUM, 0, NUM);
        hash_sort_time += get_time_stamp() - temp;
        std::cout << "hash-sort finish" << std::endl;

        std::cout << i + 1 << " times finish" << std::endl;
    }
    insertion_sort_time /= times;
    quick_sort_time /= times;
    bubble_sort_time /= times;
    merge_sort_time /= times;
    shell_sort_time /= times;
    hash_sort_time /= times;

    std::cout << "insertion-sort ";
    print_time(insertion_sort_time);
    std::cout << "quick-sort ";
    print_time(quick_sort_time);
    std::cout << "bubble-sort ";
    print_time(bubble_sort_time);
    std::cout << "merge-sort ";
    print_time(merge_sort_time);
    std::cout << "shell-sort ";
    print_time(shell_sort_time);
    std::cout << "hash-sort ";
    print_time(hash_sort_time);

    std::cout << "--------sort end-------" << std::endl;
    delete[] n;
    delete[] m;
    delete[] k;
    delete[] o;
    delete[] p;
    delete[] q;
}
