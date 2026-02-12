#include <catch2/catch_all.hpp>
#include "urn.hpp"
#include <vector>
#include <cstdint>
#include <string>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <discreture.hpp>
#include "benchmark_config.hpp"

using uint = unsigned int;

/**************************************************************************************************
*** NAIVE HELPERS
***************************************************************************************************/
namespace na {
    bool unsorted(const std::vector<uint>& d, uint k) {
        for (uint i=0;i+1<k;++i) if (d[i]>d[i+1]) return true;
        return false;
    }

    bool repetitions(const std::vector<uint>& d, uint k) {
        for (uint i=0;i+1<k;++i)
            for (uint j=i+1;j<k;++j)
                if (d[i]==d[j]) return true;
        return false;
    }

    bool increment_draw(std::vector<uint>& d, uint n, uint k) {
        for (uint p=k; p>0; --p) {
            if (d[p-1] < n-1) { ++d[p-1]; return true; }
            d[p-1]=0;
        }
        return false;
    }
}

/**************************************************************************************************
*** SPECIFIC DRAW – UrnOR (order relevant, repetition)
***************************************************************************************************/
#ifdef URN_OR_SPECIFIC_DRAW_ENABLED

static std::vector<uint> urnor_naive(uint n,uint k,uint idx){
    std::vector<uint> d(k,0);
    for(uint i=0;i<idx;++i) na::increment_draw(d,n,k);
    return d;
}

static std::vector<int> urnor_discreture(uint n,uint k,uint idx){
    return discreture::multisets(k,n-1)[idx];
}

TEST_CASE("Specific draw UrnOR")
{
    struct NK{uint n,k;};
    std::vector<NK> cases{{6,2},{10,3},{15,4},{20,5}};

    std::mt19937 gen{std::random_device{}()};

    for(auto [n,k]:cases)
    {
        uint64_t maxDraw = static_cast<uint64_t>(std::pow(n,k)) - 1;
        std::uniform_int_distribution<uint64_t> dist(0,maxDraw);

        BENCHMARK_ADVANCED("UrnOR|urn::UrnOR|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){
            urn::UrnOR urn{n,k};
            m.measure([&]{ return urn.draw(dist(gen)); });
        };

        BENCHMARK_ADVANCED("UrnOR|na::naive_approach|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){
            m.measure([&]{ return urnor_naive(n,k,dist(gen)); });
        };

        BENCHMARK_ADVANCED("UrnOR|discreture::multisets|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){
            m.measure([&]{ return urnor_discreture(n,k,dist(gen)); });
        };
    }
}
#endif

/**************************************************************************************************
*** SPECIFIC DRAW – UrnO (permutations)
***************************************************************************************************/
#ifdef URN_O_SPECIFIC_DRAW_ENABLED

static std::vector<uint> urno_naive(uint n,uint idx){
    std::vector<uint> d(n,0);
    uint c=0;
    do{
        if(!na::repetitions(d,n)){
            if(c++==idx) return d;
        }
    }while(na::increment_draw(d,n,n));
    return d;
}

static std::vector<int> urno_discreture(int n,int idx){
    return discreture::permutations(n)[idx];
}

static std::vector<uint> urno_std(uint n,uint idx){
    std::vector<uint> d(n);
    std::iota(d.begin(),d.end(),0);
    for(uint i=0;i<idx;++i)
        std::next_permutation(d.begin(),d.end());
    return d;
}

TEST_CASE("Specific draw UrnO")
{
    for(uint n : {3,5,7,8})
    {
        uint64_t maxDraw = discreture::factorial(n)-1;
        std::mt19937 gen{std::random_device{}()};
        std::uniform_int_distribution<uint64_t> dist(0,maxDraw);

        BENCHMARK_ADVANCED("UrnO|urn::UrnO|n="+std::to_string(n))
        (Catch::Benchmark::Chronometer m){
            urn::UrnO urn{n,n};
            m.measure([&]{ return urn.draw(dist(gen)); });
        };

        BENCHMARK_ADVANCED("UrnO|na::naive_approach|n="+std::to_string(n))
        (Catch::Benchmark::Chronometer m){
            m.measure([&]{ return urno_naive(n,dist(gen)); });
        };

        BENCHMARK_ADVANCED("UrnO|discreture::permutations|n="+std::to_string(n))
        (Catch::Benchmark::Chronometer m){
            m.measure([&]{ return urno_discreture(n,dist(gen)); });
        };

        BENCHMARK_ADVANCED("UrnO|std::next_permutation|n="+std::to_string(n))
        (Catch::Benchmark::Chronometer m){
            m.measure([&]{ return urno_std(n,dist(gen)); });
        };
    }
}
#endif

/**************************************************************************************************
*** SPECIFIC DRAW – UrnR (combinations with repetition)
***************************************************************************************************/
#ifdef URN_R_SPECIFIC_DRAW_ENABLED

static std::vector<uint> urnr_naive(uint n,uint k,uint idx){
    std::vector<uint> d(k,0);
    uint c=0;
    do{
        if(!na::unsorted(d,k)){
            if(c++==idx) return d;
        }
    }while(na::increment_draw(d,n,k));
    return d;
}

static std::vector<int> urnr_discreture(int n,int k,int idx){
    auto it = discreture::combinations(n+k-1,k)[idx];
    for(int i=0;i<it.size();++i) it[i]-=i;
    return it;
}

TEST_CASE("Specific draw UrnR")
{
    struct NK{uint n,k;};
    std::vector<NK> cases{{5,2},{10,3},{20,5},{30,6}};

    std::mt19937 gen{std::random_device{}()};

    for(auto [n,k]:cases)
    {
        uint64_t maxDraw = discreture::combinations(n+k-1,k).size()-1;
        std::uniform_int_distribution<uint64_t> dist(0,maxDraw);

        BENCHMARK_ADVANCED("UrnR|urn::UrnR|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){
            urn::UrnR urn{n,k};
            m.measure([&]{ return urn.draw(dist(gen)); });
        };

        BENCHMARK_ADVANCED("UrnR|na::naive_approach|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){
            m.measure([&]{ return urnr_naive(n,k,dist(gen)); });
        };

        BENCHMARK_ADVANCED("UrnR|discreture::combinations|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){
            m.measure([&]{ return urnr_discreture(n,k,dist(gen)); });
        };
    }
}
#endif

/**************************************************************************************************
*** SPECIFIC DRAW – Urn (combinations without repetition)
***************************************************************************************************/
#ifdef URN_SPECIFIC_DRAW_ENABLED

static std::vector<uint> urn_naive(uint n,uint k,uint idx){
    std::vector<uint> d(k,0);
    uint c=0;
    do{
        if(!na::unsorted(d,k)&&!na::repetitions(d,k)){
            if(c++==idx) return d;
        }
    }while(na::increment_draw(d,n,k));
    return d;
}

static std::vector<int> urn_discreture(int n,int k,int idx){
    return discreture::combinations(n,k)[idx];
}

TEST_CASE("Specific draw Urn")
{
    struct NK{uint n,k;};
    std::vector<NK> cases{{5,2},{10,3},{20,5},{30,6}};

    std::mt19937 gen{std::random_device{}()};

    for(auto [n,k]:cases)
    {
        uint64_t maxDraw = discreture::combinations(n,k).size()-1;
        std::uniform_int_distribution<uint64_t> dist(0,maxDraw);

        BENCHMARK_ADVANCED("Urn|urn::Urn|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){
            urn::Urn urn{n,k};
            m.measure([&]{ return urn.draw(dist(gen)); });
        };

        BENCHMARK_ADVANCED("Urn|na::naive_approac|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){
            m.measure([&]{ return urn_naive(n,k,dist(gen)); });
        };

        BENCHMARK_ADVANCED("Urn|discreture::combinations|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){
            m.measure([&]{ return urn_discreture(n,k,dist(gen)); });
        };
    }
}
#endif
