#include "urn.hpp"
#include <cmath>
#include <stdexcept>
#include <cassert>

namespace urn
{   
    //Helpers and Math
    std::size_t falling_factorial(std::size_t n, std::size_t k)
    {
        std::size_t result {1};
        for (std::size_t i {0}; i < k; ++i)
        {
            result *= (n - i);
        }
        return result;
    }
    
    std::size_t binomial_coefficient(std::size_t n, std::size_t k) {
        if (k < 0 || k > n) return 0;
        if (k > n - k) k = n - k;

        std::size_t result {1};
        for (int i {1}; i <= k; ++i) {
            result = result * (n - k + i) / i;
        }
        return result;
    }

    //BaseUrn

    uint BaseUrn::n() const { return m_n; }
    uint BaseUrn::k() const { return m_k; }

    UrnIterator BaseUrn::begin() const { return UrnIterator(this, 0); }
    UrnIterator BaseUrn::end() const { return UrnIterator(this, static_cast<std::ptrdiff_t>(size())); }
    std::reverse_iterator<UrnIterator> BaseUrn::rbegin() const { return std::reverse_iterator{end()}; }
    std::reverse_iterator<UrnIterator> BaseUrn::rend() const { return std::reverse_iterator{begin()}; }

    //Iterator
    UrnIterator::UrnIterator(const BaseUrn* urn, UrnIterator::difference_type rank): m_urn(urn), m_rank(rank) {}

    //Utils
    std::size_t UrnIterator::size() const { return m_urn->size(); }
    std::size_t UrnIterator::n() const { return m_urn->n(); }
    std::size_t UrnIterator::k() const { return m_urn->k(); }
    std::size_t UrnIterator::rank() const { return m_rank; }

    //Dereference
    const UrnIterator::value_type UrnIterator::operator*() const 
    {   
        
        if(m_rank >= size() || m_rank < 0)
        {
            return std::vector<uint>((k()), 0);
        }
        
        return (*m_urn).draw(m_rank);
    }
    const UrnIterator::value_type UrnIterator::operator[](size_type index) const 
    {   

        if(index >= size() || index < 0)
        {
            return std::vector<uint>((k()), 0);
        }
        
        return (*m_urn).draw(index);
    }

    //Increment
    UrnIterator& UrnIterator::operator++()
    {
            ++m_rank;
            return *this;
    }

    UrnIterator UrnIterator::operator++(int)
    { 
        UrnIterator temp {*this};
        ++(*this);
        return temp;
    }

    //Decrement
    UrnIterator& UrnIterator::operator--()
    {
        --m_rank;
        return *this;
    }

    UrnIterator UrnIterator::operator--(int)
    { 
        UrnIterator temp {*this};
        --(*this);
        return temp;
    }

    //Aricmethic
    UrnIterator& UrnIterator::operator+=(UrnIterator::difference_type n)
    {
        m_rank += n;
        return *this;
    }

    UrnIterator& UrnIterator::operator-=(UrnIterator::difference_type n)
    {
        m_rank -= n;
        return *this;
    }

    UrnIterator operator+(UrnIterator other, UrnIterator::difference_type n)
    {
        other += n;
        return other;
    }

    UrnIterator operator+(UrnIterator::difference_type n, UrnIterator other)
    {
        other += n;
        return other;
    }

    UrnIterator operator-(UrnIterator other, UrnIterator::difference_type n)
    {
        other -= n;
        return other;
    }

    UrnIterator::difference_type operator-(const UrnIterator& a, const UrnIterator& b)
    {
        return a.m_rank - b.m_rank;
    }

    //Compersion
    bool operator==(const UrnIterator& a, const UrnIterator& b)
    {
        return a.m_urn == b.m_urn && a.m_rank == b.m_rank;
    }

    bool operator!=(const UrnIterator& a, const UrnIterator& b)
    {
        return !(a == b);
    }

    bool operator<(const UrnIterator& a, const UrnIterator& b)
    {
        return a.m_rank < b.m_rank;
    }

    bool operator>(const UrnIterator& a, const UrnIterator& b)
    {
        return a.m_rank > b.m_rank;
    }

    bool operator<=(const UrnIterator& a, const UrnIterator& b)
    {
        return a == b || a < b;
    }

    bool operator>=(const UrnIterator& a, const UrnIterator& b)
    {
        return a == b || a > b;
    }

    //UrnOR
    UrnOR::UrnOR(const uint& n, const uint& k): BaseUrn {n, k}
    {
        if (m_n == 0 && m_k > 0)
        {
            throw std::domain_error("UrnOR with n == 0 and k > 0 is not valid.");
        }
    }

    uint UrnOR::size() const
    {
        if(m_k == 0)    
        {
            return 1;
        }
        return static_cast<uint>(std::pow(m_n,m_k));
    }

    std::vector<uint> UrnOR::draw(uint rank) const
    {
        std::vector<uint> draw(m_k, 0);
        for (int i {static_cast<int>(m_k) - 1}; i >= 0; --i)
        {
            draw[i] = rank % m_n;
            rank /= m_n;
        }
        return draw;
    }

    //UrnO
    UrnO::UrnO(const uint& n, const uint& k): BaseUrn {n, k}
    {
        if (m_k > m_n)
        {
            throw std::domain_error("UrnO with n > k is not valid.");
        }
    }

    uint UrnO::size() const
    {
        if(m_k == 0)
        {
            return 1;
        }
        return falling_factorial(m_n, m_k);
    }
    
    std::vector<uint> UrnO::draw(uint rank) const
    {
        std::vector<uint> elements {};
        for (int i {1}; i <= m_n; ++i) {
            elements.push_back(i);
        }

        std::vector<uint> draw {};
        uint rest {rank};

        for (int i {1}; i <= m_k; ++i) {
            uint denominator = falling_factorial(m_n-i, m_k-i);
            uint inversion_number = rest / denominator;
            draw.push_back(elements[inversion_number] - 1);
            elements.erase(elements.begin() + inversion_number);

            rest = rest % denominator;
        }

        return draw;
    }

    /*
    "OPTIMIZED"
    std::vector<uint> UrnO::draw(uint rank) const
    {
        std::vector<uint> elements {};
        for (int i {1}; i <= m_n; ++i) {
            elements.push_back(i);
        }

        std::vector<uint> draw {};
        uint rest {rank};

        uint denominator = falling_factorial(m_n - 1, m_k - 1);
        for (int i {1}; i <= m_k; ++i) {
            uint inversion_number = rest / denominator;

            draw.push_back(elements[inversion_number] - 1);
            elements.erase(elements.begin() + inversion_number);

            rest = rest % denominator;
            
            if (i < m_k)
            {   
                denominator = denominator / (m_n - i);
            }
        }

        return draw;
    }
    */

    //UrnR
    UrnR::UrnR(const uint& n, const uint& k): BaseUrn {n, k}
    {
        if (m_n == 0 && m_k > 0)
        {
            throw std::domain_error("UrnR with n == 0 and k > 0 is not valid.");
        }
    }

    uint UrnR::size() const
    {
        if(m_k == 0)    
        {
            return 1;
        }
        return binomial_coefficient(m_n + m_k - 1, m_k);
    }
    
    std::vector<uint> UrnR::draw(uint rank) const
    {
        std::vector<uint> combination(m_k, 0);

        uint binomial = binomial_coefficient(m_n + m_k - 2, m_k - 1) * (m_n + m_k - 1);
        uint m {0};
        uint index {0};

        while (index < m_k - 1) {
            binomial = binomial / (m_n + m_k - 1 - m - index);

            if (binomial > rank) {
                combination[index] = m;
                binomial = binomial * (m_k - index - 1);
                ++index;
            } else {
                rank -= binomial;
                binomial = binomial * (m_n - 1 - m);
                ++m;
            }
        }

        if (m_n + m_k - 1 > 0) {
            combination[m_k - 1] = m_n + rank - binomial;
        }

        return combination;
    }

    //Urn
    Urn::Urn(const uint& n, const uint& k): BaseUrn {n, k}
    {
        if (m_n == 0 && m_k > 0)
        {
            throw std::domain_error("Urn with n == 0 and k > 0 is not valid.");
        }
    }

    uint Urn::size() const
    {
        if(m_k == 0)    
        {
            return 1;
        }
        return binomial_coefficient(m_n, m_k);
    }
    
    std::vector<uint> Urn::draw(uint rank) const
    {
        std::vector<uint> combination(m_k, 0);

        uint binomial = binomial_coefficient(m_n-1, m_k-1) * m_n;
        uint m {0};
        uint index {0};

        while (index < m_k - 1) {
            binomial = binomial / (m_n - m - index);

            if (binomial > rank) {
                combination[index] = m + index;
                binomial = binomial * (m_k - index - 1);
                ++index;
            } else {
                rank -= binomial;
                binomial = binomial * (m_n - m - m_k);
                ++m;
            }
        }

        if (m_k > 0) {
            combination[m_k - 1] = m_n + rank - binomial;
        }

        return combination;
    }

} //namespace urn