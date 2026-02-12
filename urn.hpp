#ifndef URN_HPP
#define URN_HPP

#include <vector>
#include <string>
#include <iterator>

namespace urn
{
    using uint = unsigned int;
    
    //Helpers and Math
    template<class T>
    std::string to_string(const std::vector<T>& draw)
    {
        std::string stdraw {};
        for(uint posCount{}; posCount < draw.size(); ++posCount)
        {
            stdraw += std::to_string(draw.at(posCount));
            if(posCount != (draw.size()-1))
            {
                stdraw += " ";
            }
        }
        return stdraw;
    }

    std::size_t falling_factorial(std::size_t n, std::size_t k);

    std::size_t binomial_coefficient(std::size_t n, std::size_t k);

    class UrnIterator; // forward declaration

    //BaseUrn
    class BaseUrn
    {
        protected:
            uint m_n;
            uint m_k;

        public:
            explicit BaseUrn(const uint& n, const uint& k): m_n(n), m_k(k) {}

            uint n() const;
            uint k() const;

            UrnIterator begin() const;
            UrnIterator end() const;
            std::reverse_iterator<UrnIterator> rbegin() const;
            std::reverse_iterator<UrnIterator> rend() const;

            virtual uint size() const = 0;
            virtual std::vector<uint> draw(uint r) const = 0;
            virtual ~BaseUrn() = default;
    };

    //Iterator
    class UrnIterator
    {   
        public:
            using iterator_category = std::random_access_iterator_tag;
            using iterator_concept = std::random_access_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = std::vector<uint>;
            using pointer           = void;
            using reference         = value_type;
            using size_type         = std::size_t;

            //Constructors
            UrnIterator() = default;
            UrnIterator(const BaseUrn* urn, difference_type rank);

            //Utils
            std::size_t size() const;
            std::size_t n() const;
            std::size_t k() const;
            std::size_t rank() const;

            //Dereference
            const value_type operator*() const;
            const value_type operator[](size_type index) const;

            //Increment
            UrnIterator& operator++();
            UrnIterator operator++(int);

            //Decrement
            UrnIterator& operator--();
            UrnIterator operator--(int);

            //Aricmethic
            UrnIterator& operator+=(difference_type n);
            UrnIterator& operator-=(difference_type n);
            friend UrnIterator operator+(UrnIterator other, difference_type n);
            friend UrnIterator operator+(difference_type n, UrnIterator other);
            friend UrnIterator operator-(UrnIterator other, difference_type n);
            friend difference_type operator-(const UrnIterator& a, const UrnIterator& b);

            //Compersion
            friend bool operator==(const UrnIterator& a, const UrnIterator& b);
            friend bool operator!=(const UrnIterator& a, const UrnIterator& b);
            friend bool operator<(const UrnIterator& a, const UrnIterator& b);
            friend bool operator>(const UrnIterator& a, const UrnIterator& b);
            friend bool operator<=(const UrnIterator& a, const UrnIterator& b);
            friend bool operator>=(const UrnIterator& a, const UrnIterator& b);

        private:
            const BaseUrn* m_urn {nullptr};
            difference_type m_rank {0};
    };

    //UnrOR
    class UrnOR: public BaseUrn
    {
        public:
            explicit UrnOR(const uint& n, const uint& k);
            uint size() const override;
            std::vector<uint> draw(uint rank) const override;
        
    };

    //UnrO
    class UrnO: public BaseUrn
    {
        public:
            explicit UrnO(const uint& n, const uint& k);
            uint size() const override;
            std::vector<uint> draw(uint rank) const override;
        
    };

    //Unr
    class Urn: public BaseUrn
    {
        public:
            explicit Urn(const uint& n, const uint& k);
            uint size() const override;
            std::vector<uint> draw(uint rank) const override;
        
    };

    //UnrR
    class UrnR: public BaseUrn
    {
        public:
            explicit UrnR(const uint& n, const uint& k);
            uint size() const override;
            std::vector<uint> draw(uint rank) const override;
        
    };

} //namespace urn


#endif // URN_HPP