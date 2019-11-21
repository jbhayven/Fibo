#ifndef __FIBO_H__
#define __FIBO_H__

#include <vector>
#include <typeinfo>
#include <string>

class Fibo {
    using fibitset = std::vector<bool>;
    
private: 
    
    fibitset bits_;
    
    inline void push_pairs_right(size_t pos);
  
    inline void add_bit(size_t pos);
        
    inline std::vector<unsigned long long>
            get_sequence(unsigned long long number);
       
    void normalize();

    void remove_leading_zeros();
    
public:
    Fibo();
    
    Fibo(const std::string& str);
    
    template<typename Number,
             typename = typename std::enable_if<
                std::is_integral<Number>::value
                && !std::is_same<char, Number>::value
                && !std::is_same<bool, Number>::value>::type>
    Fibo(Number number);

    Fibo(const Fibo& that);
    
    Fibo& operator+=(const Fibo& that);

    Fibo& operator&=(const Fibo& that);

    Fibo& operator|=(const Fibo& that);

    Fibo& operator^=(const Fibo& that);
    
    Fibo& operator<<=(const size_t& n);

    const Fibo operator<<(const size_t& n);

    Fibo& operator=(const Fibo& that);
    
    size_t length() const;

private:
    friend std::ostream& operator<<(std::ostream& os, const Fibo& fib);
    
    friend bool operator==(const Fibo& a, const Fibo& b);
    
    friend bool operator<(const Fibo& a, const Fibo& b);
};

bool operator>(const Fibo& a, const Fibo& b);
bool operator<=(const Fibo& a, const Fibo& b);
bool operator>=(const Fibo& a, const Fibo& b);
bool operator!=(const Fibo& a, const Fibo& b);
const Fibo operator+(const Fibo& a, const Fibo& b);
const Fibo operator|(const Fibo& a, const Fibo& b);
const Fibo operator&(const Fibo& a, const Fibo& b);
const Fibo operator^(const Fibo& a, const Fibo& b);


const Fibo& Zero();

const Fibo& One();

#endif /* __FIBO_H__ */
