#include <catch2/catch_all.hpp>

#include "urn.hpp"
#include <vector>
#include <cstdint>
#include <string>
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
*** UrnOR – order relevant, with repetition (n^k)
***************************************************************************************************/
#ifdef URN_OR_FULL_ITERATION_ENABLED

static uint64_t iter_urnor_urn(uint n,uint k) {
    urn::UrnOR urn{n,k};
    uint64_t c=0;
    for (auto&& _ : urn) ++c;
    return c;
}

static uint64_t iter_urnor_naive(uint n,uint k) {
    std::vector<uint> d(k,0);
    uint64_t c=0;
    do { ++c; } while (na::increment_draw(d,n,k));
    return c;
}

static uint64_t iter_urnor_discreture(uint n,uint k) {
    uint64_t c=0;
    for (auto&& _ : discreture::multisets(k,n-1)) ++c;
    return c;
}

TEST_CASE("UrnOR full iteration")
{
    struct NK{uint n,k;};
    std::vector<NK> cases{{6,2},{10,3},{15,4},{20,5}};

    for (auto [n,k]:cases)
    {
        BENCHMARK_ADVANCED("UrnOR|urn::UrnOR|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urnor_urn(n,k); }); };

        BENCHMARK_ADVANCED("UrnOR|na::naive_approach|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urnor_naive(n,k); }); };

        BENCHMARK_ADVANCED("UrnOR|discreture::multisets|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urnor_discreture(n,k); }); };
    }
}
#endif

/**************************************************************************************************
*** UrnO – permutations (n!)
***************************************************************************************************/
#ifdef URN_O_FULL_ITERATION_ENABLED

static uint64_t iter_urno_urn(uint n){
    urn::UrnO urn{n,n};
    uint64_t c=0;
    for (auto&& _ : urn) ++c;
    return c;
}

static uint64_t iter_urno_naive(uint n){
    std::vector<uint> d(n,0);
    uint64_t c=0;
    do { if(!na::repetitions(d,n)) ++c; }
    while (na::increment_draw(d,n,n));
    return c;
}

static uint64_t iter_urno_discreture(int n){
    uint64_t c=0;
    for (auto&& _ : discreture::permutations(n)) ++c;
    return c;
}

static uint64_t iter_urno_std(uint n){
    std::vector<uint> d(n);
    for (uint i=0;i<n;++i) d[i]=i;

    uint64_t c=0;
    do { ++c; }
    while (std::next_permutation(d.begin(), d.end()));

    return c;
}

TEST_CASE("UrnO full iteration")
{
    for (uint n : {3,5,7,8})
    {
        BENCHMARK_ADVANCED("UrnO|urn::UrnO|n="+std::to_string(n)+"|k="+std::to_string(n))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urno_urn(n); }); };

        BENCHMARK_ADVANCED("UrnO|na::naive_approach|n="+std::to_string(n)+"|k="+std::to_string(n))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urno_naive(n); }); };

        BENCHMARK_ADVANCED("UrnO|discreture::permutations|n="+std::to_string(n)+"|k="+std::to_string(n))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urno_discreture(n); }); };

        BENCHMARK_ADVANCED("UrnO|std::next_permutation|n="+std::to_string(n)+"|k="+std::to_string(n))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urno_std(n); }); };
    }
}
#endif

/**************************************************************************************************
*** UrnR – combinations with repetition
***************************************************************************************************/
#ifdef URN_R_FULL_ITERATION_ENABLED

static uint64_t iter_urnr_urn(uint n,uint k){
    urn::UrnR urn{n,k};
    uint64_t c=0;
    for(auto&& _ : urn) ++c;
    return c;
}

static uint64_t iter_urnr_naive(uint n,uint k){
    std::vector<uint> d(k,0);
    uint64_t c=0;
    do { if(!na::unsorted(d,k)) ++c; }
    while (na::increment_draw(d,n,k));
    return c;
}

static uint64_t iter_urnr_discreture(uint n,uint k){
    uint64_t c=0;
    for (auto it : discreture::combinations(n+k-1,k))
    {
        for (int i {0}; i < it.size(); ++i) { it[i] -= i; }
       ++c;
    }
    return c;
}

TEST_CASE("UrnR full iteration")
{
    struct NK{uint n,k;};
    std::vector<NK> cases{{5,2},{10,3},{20,5},{30,6}};

    for (auto [n,k]:cases)
    {
        BENCHMARK_ADVANCED("UrnR|urn::UrnR|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urnr_urn(n,k); }); };

        BENCHMARK_ADVANCED("UrnR|na::naive_approach|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urnr_naive(n,k); }); };

        BENCHMARK_ADVANCED("UrnR|discreture::combinations|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urnr_discreture(n,k); }); };
    }
}
#endif

/**************************************************************************************************
*** Urn – combinations without repetition
***************************************************************************************************/
#ifdef URN_FULL_ITERATION_ENABLED

static uint64_t iter_urn_urn(uint n,uint k){
    urn::Urn urn{n,k};
    uint64_t c=0;
    for(auto&& _ : urn) ++c;
    return c;
}

static uint64_t iter_urn_naive(uint n,uint k){
    std::vector<uint> d(k,0);
    uint64_t c=0;
    do { if(!na::unsorted(d,k)&&!na::repetitions(d,k)) ++c; }
    while (na::increment_draw(d,n,k));
    return c;
}

static uint64_t iter_urn_discreture(uint n,uint k){
    uint64_t c=0;
    for(auto&& _ : discreture::combinations(n,k)) ++c;
    return c;
}

TEST_CASE("Urn full iteration")
{
    struct NK{uint n,k;};
    std::vector<NK> cases{{5,2},{10,3},{20,5},{30,6}};

    for (auto [n,k]:cases)
    {
        BENCHMARK_ADVANCED("Urn|urn::Urn|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urn_urn(n,k); }); };

        BENCHMARK_ADVANCED("Urn|na::naive_approach|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urn_naive(n,k); }); };

        BENCHMARK_ADVANCED("Urn|discreture::combinations|n="+std::to_string(n)+"|k="+std::to_string(k))
        (Catch::Benchmark::Chronometer m){ m.measure([&]{ return iter_urn_discreture(n,k); }); };
    }
}
#endif
