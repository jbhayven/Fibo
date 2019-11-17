#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>
#include <algorithm>

using std::ostream;

class Fibo {
    using fibitset = std::vector<bool>;
    
private: 
    
    // otherwise, normalized, Fibonacci-based number
    // 0th bit = 1, 1st = 2, 2nd = 3, 3rd = 5, etc.
    fibitset _bits;
    
    // pushes right if and only if the byte [pos] and the byte [pos+1] are both true
    inline void push_pairs_right(size_t pos) {
        while(pos + 1 < _bits.size() && _bits[pos] && _bits[pos+1]) {
            _bits[pos] = 0;
            _bits[pos+1] = 0;
            
            pos += 2;
            
            if(pos < _bits.size()) 
                _bits[pos] = 1;
            else
                _bits.push_back(1);
        }
    }
    
    // NOTE: may require restructurization
    // maintains naturalization
    inline void add_bit(size_t pos) {
        if(pos >= _bits.size())
            _bits.resize(pos + 1, 0);
        
        while(_bits[pos]) {
            // the bit is already taken
            
            // in both cases F(pos + 1) is added
            if(pos + 1 < _bits.size()) {
                _bits[pos + 1] = true;
                push_pairs_right(pos + 1);
            }
            else {
                _bits.push_back(true);
            }
            
            if(pos == 0) {
                // F(2) + F(2) = F(1) + F(2) = F(3)
                _bits[pos] = 0;
            }
            else {
                // F(i) + F(i) = F(i-2) + F(i-1) + F(i) = F(i-2) + F(i+1)
                _bits[pos] = 0;
                if(pos == 1) 
                    pos = 0;
                else 
                    pos -= 2;
            }
        }
        
        _bits[pos] = 1;
        push_pairs_right(pos);
            
        if(pos > 0)
            push_pairs_right(pos-1);
    } 
    
public:
    Fibo() : _bits({0}) {}
    
    Fibo(const std::string& str) : _bits() {
        size_t size = str.size();
        
        _bits.resize(size);
        
        for(size_t i = 0; i < size; ++i)
            if(str[size - 1 - i] == '1') 
                add_bit(i);
    }
    
    Fibo(const Fibo& that) : _bits(that._bits) {}
    
    Fibo& operator+=(const Fibo& that) {
        size_t size = that._bits.size();
        
        for(size_t i = 0; i < size; ++i)
            if(that._bits[i])
                this->add_bit(i);

        return *this;
    }
    
    const Fibo operator+(const Fibo& that) const {
        return Fibo(*this) += that;
    }
    
    Fibo& operator=(const Fibo& that) {
        if (this != &that)
          _bits = that._bits;

        return *this;
    }
    
    bool operator==(const Fibo& that) const {
        if (this == &that) return true;

        if (this->_bits.size() != that._bits.size()) return false;
        
        return equal(this->_bits.begin(), this->_bits.end(), that._bits.begin());
    }
    
    bool operator<(const Fibo& that) const {
        if (this == &that) return false;

        if (this->_bits.size() < that._bits.size()) return true;
        
        auto diff = mismatch(this->_bits.rbegin(), // a pair of iterators
                             this->_bits.rend(), 
                             that._bits.rbegin());
            
        if(diff.first == this->_bits.rend()) return false;
        
        // this < that if and only if they differ 
        // where 'that' has a 1 and 'this' has a 0
        return(*(diff.second)); 
    }
    bool operator>(const Fibo& that) const {
        return that < *this;
    }
    bool operator<=(const Fibo& that) const {
        return !(that > *this);
    }
    bool operator>=(const Fibo& that) const {
        return !(that < *this);
    }
    bool operator!=(const Fibo& that) const {
        return !(that == *this);
    }
private:
    friend ostream& operator<<(ostream& os, const Fibo& fib) {
        for(auto i = fib._bits.rbegin(); i != fib._bits.rend(); ++i)
            os << (*i);
        
        return os;
    }
};

const Fibo& Zero() {
    static Fibo zero("0");
    
    return zero;
}

#include <cassert>

int main() {
    
    Fibo f1;
    Fibo f2("10000");
    f1 = f2;
    
    Fibo f3(f2);
    
    using std::cout, std::endl;
    
    cout << f1 << " " << f2 << " " << f3 << endl;
    
    f1 += f2;
    cout << f1 << endl;
    
    cout << f1 + f2 << endl;
    cout << f1 + (f2 + f3) << endl;
    
    cout << Fibo("1001") + Fibo("10") << endl;
    cout << Fibo("1011") << endl;
    
    assert(Fibo("11") == Fibo("100"));
    assert((Fibo("1001") + Fibo("10")) == Fibo("1011"));
    
    
    return 0;
}
