#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include "fibo.h"

// Pushes right (i.e. exchanges a pair of consecutive bits for one greater)
// if and only if the byte [pos] and the byte [pos+1] are both true,
// based on the fact that F(i) + F(i+1) = F(i+2).
// Requires that all the bits to the right from such a pair already satisfy
// the invariant of normalization.
void Fibo::push_pairs_right(size_t pos) {
    while(pos + 1 < bits_.size() && bits_[pos] && bits_[pos+1]) {
        bits_[pos] = false;
        bits_[pos+1] = false;
        
        pos += 2; // Now [pos] correcponds to the position 
                  // where the bit is to be added.
        
        if(pos < bits_.size()) 
            bits_[pos] = true;
        else
            bits_.push_back(true);
    }
}

void Fibo::normalize() {
    for(size_t i = bits_.size(); i > 0; --i)
        push_pairs_right(i-1);
}

// Maintains naturalization.
void Fibo::add_bit(size_t pos) {
    if(pos >= bits_.size())
        bits_.resize(pos + 1, 0);
    
    while(bits_[pos]) {
        // The bit is already taken.
        
        // The bit at [pos] removed...
        bits_[pos] = false;
        
        // ... F(pos + 1) is added...
        if(pos + 1 < bits_.size()) {
            bits_[pos + 1] = true;
            push_pairs_right(pos + 1);
        }
        else {
            bits_.push_back(true);
        }
        
        // ... and an additional bit is added, with respect to the three possible cases.
        if(pos == 0)
            return; // 1 + 1 = 2, that's it
        if(pos == 1)
            pos = 0; // 2 + 2 = (1 + 3), so we need to add 1
        else  
            pos -= 2; // F(i) + F(i) = F(i-2) + F(i+1), so we need to add F(i-2) 
    }
    
    // The bit at [pos] is free so we can set it to 1.
    bits_[pos] = true;
    push_pairs_right(pos);
        
    if(pos > 0)
        push_pairs_right(pos - 1);
} 

// Generates Fibonacci sequence.
std::vector<unsigned long long> Fibo::get_sequence(unsigned long long number) {
    std::vector<unsigned long long> fibo_sequence;
    fibo_sequence.push_back(1);
    fibo_sequence.push_back(2);

    while(number - fibo_sequence.end()[-1] >= fibo_sequence.end()[-2]) {
        fibo_sequence
            .push_back(fibo_sequence.end()[-1] + fibo_sequence.end()[-2]);
    }

    return fibo_sequence;
}

void Fibo::remove_leading_zeros() {
    size_t i = bits_.size() - 1;

    while(bits_[i] == 0 && i > 0)
        --i;

    bits_.resize(i + 1);
}

Fibo::Fibo() : bits_({0}) {}

Fibo::Fibo(const std::string& str) : bits_() {
    size_t size = str.size();
    
    for(size_t i = 0; i < size; ++i) 
        if(str[i] != '0' && str[i] != '1')
            throw std::invalid_argument("Negative number");
    
    bits_.resize(size);
    
     // Filling the bits in the reverse order.
    for(size_t i = 0; i < size; ++i)
        bits_[i] = (str[i] == '1');
        
    std::reverse(bits_.begin(), bits_.end());
        
    normalize();
}

template<typename Number,
         typename = typename std::enable_if<
            std::is_integral<Number>::value
            && !std::is_same<char, Number>::value
            && !std::is_same<bool, Number>::value>::type>
Fibo::Fibo(Number n) : bits_() {
    if(n < 0) throw std::invalid_argument("Negative number");
    unsigned long long number = n;
    
    std::vector<unsigned long long> fibo_sequence = get_sequence(number);

    size_t size = fibo_sequence.size();
    bits_.resize(size);

    for(size_t i = 0; i < size; ++i) {
        if(fibo_sequence[size - i - 1] <= number) {
            number -= fibo_sequence[size - i - 1];
            bits_[size - i - 1] = true;
        }
        else {
            bits_[size - i - 1] = false;
        }
    }
}

Fibo::Fibo(const Fibo& that) : bits_(that.bits_) {}

size_t Fibo::length() const {
    return bits_.size();
 }

Fibo& Fibo::operator+=(const Fibo& that) {
    size_t size = that.bits_.size();
    
    for(size_t i = 0; i < size; ++i)
        if(that.bits_[i])
            this->add_bit(i);

    return *this;
}

Fibo& Fibo::operator&=(const Fibo& that) {
    size_t size = that.bits_.size();
    if(size < bits_.size())
        bits_.resize(size);

    for(size_t i = 0; i < size; ++i)
        bits_[i] = bits_[i] && that.bits_[i];

    remove_leading_zeros();

    return *this;
}

Fibo& Fibo::operator|=(const Fibo& that) {
    size_t size = that.bits_.size();
    if(size > bits_.size())
        bits_.resize(size);

    for(size_t i = 0; i < size; ++i)
            bits_[i] = bits_[i] || that.bits_[i];

    normalize();

    return *this;
}

Fibo& Fibo::operator^=(const Fibo& that) {
    size_t size = that.bits_.size();
    if(size > bits_.size())
        bits_.resize(size);

    for(size_t i = 0; i < size; ++i)
        bits_[i] = bits_[i] != that.bits_[i];

    normalize();
    remove_leading_zeros();

    return *this;
}

Fibo& Fibo::operator<<=(const size_t& n) {
    if(n == 0)
        return *this;

    bits_.resize(bits_.size() + n);
    std::rotate(bits_.begin(), bits_.end() - n, bits_.end());

    return *this;
}

const Fibo Fibo::operator<<(const size_t& n) {
    return Fibo(*this) <<= n;
}

Fibo& Fibo::operator=(const Fibo& that) {
    if (this != &that)
      bits_ = that.bits_;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Fibo& fib) {
    for(auto i = fib.bits_.rbegin(); i != fib.bits_.rend(); ++i)
        os << (*i);
    
    return os;
}

bool operator==(const Fibo& a, const Fibo& b) {
    if (&a == &b) return true;
    
    if (a.bits_.size() != b.bits_.size()) return false;
        
    return equal(a.bits_.begin(), a.bits_.end(), b.bits_.begin());
}
    
bool operator<(const Fibo& a, const Fibo& b) {
    if (&a == &b) return false;

    if (a.bits_.size() < b.bits_.size()) return true;
        
    auto diff = mismatch(a.bits_.rbegin(), // A pair of iterators.
                            a.bits_.rend(), 
                            b.bits_.rbegin());
            
    if(diff.first == a.bits_.rend()) return false;
        
    // a < b if and only if they differ 
    // where 'b' has a 1 and 'a' has a 0
    return(*(diff.second)); 
}

const Fibo& Zero() {
    static Fibo zero("0");
    
    return zero;
}

const Fibo& One() {
    static Fibo one("1");

    return one;
}
   
bool operator>(const Fibo& a, const Fibo& b) { return b < a; }
bool operator<=(const Fibo& a, const Fibo& b) { return !(a > b); }
bool operator>=(const Fibo& a, const Fibo& b) { return !(b < a); }
bool operator!=(const Fibo& a, const Fibo& b) { return !(b == a); }
const Fibo operator+(const Fibo& a, const Fibo& b) { return Fibo(a) += b; }
const Fibo operator|(const Fibo& a, const Fibo& b) { return Fibo(a) |= b; }
const Fibo operator&(const Fibo& a, const Fibo& b) { return Fibo(a) &= b; }
const Fibo operator^(const Fibo& a, const Fibo& b) { return Fibo(a) ^= b; }
