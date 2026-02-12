#include "urn.hpp"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <discreture.hpp>
#include "benchmark_config.hpp"

/*
Fuer das Ranges und Views Beispiel wird C++23 benoetigt.
Kompelierung und Ausfuehrund der Beispiele:
g++ -std=c++23 urn.cpp ranges_and_views.cpp -o ranges_and_views
./ranges_and_views
*/

constexpr int ITERATIONS = 1000;

//namespace naive approach
namespace na {

    class NaiveUrn{
        private:
            std::vector<std::vector<uint>> m_draws {};

            bool increment_draw(std::vector<uint>& d, uint n, uint k) 
            {
                for (uint p=k; p>0; --p) {
                    if (d[p-1] < n-1) { ++d[p-1]; return true; }
                    d[p-1]=0;
                }
                return false;
            }

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

            void generate_draws(const uint& n, const uint& k)
            {   
                std::vector<uint> draw(k,0);
                do{
                    if(!unsorted(draw,k)&&!repetitions(draw,k))
                    {
                        m_draws.push_back(draw);
                    }
                }while(increment_draw(draw,n,k));
            }

        public:
            NaiveUrn(const uint&n, const uint&k){
                generate_draws(n,k);
            }

            std::vector<uint> operator[](int index)
            {
                return m_draws[index];
            }
        
    };

}

#ifdef URN_MEMORY_ENABLED
int main()
{   
    std::cout << "urn::Urn" << std::endl;
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dist(0,15503);
    
    for(int i {}; i < ITERATIONS; ++i)
    {   
        urn::Urn u {20,5};
        std::cout << urn::to_string(u.draw(dist(gen))) << std::endl;
    }

    return 0;
}
#endif

#ifdef NAIVE_APPROACH_MEMORY_ENABLED
int main()
{  
    std::cout << "na::NaiveUrn" << std::endl;
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dist(0,15503);
    
    for(int i {}; i < ITERATIONS; ++i)
    {   
        na::NaiveUrn u {20,5};
        std::cout << urn::to_string(u[dist(gen)]) << std::endl;;
    }

    return 0;
}
#endif


#ifdef DISCRETURE_MEMORY_ENABLED
int main()
{  
    std::cout << "discreture::combinations" << std::endl;
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dist(0,15503);
    
    for(int i {}; i < ITERATIONS; ++i)
    {   
        auto combs {discreture::combinations(20,5)};
        std::cout << urn::to_string(combs[dist(gen)]) << std::endl;
    }

    return 0;
}
#endif
