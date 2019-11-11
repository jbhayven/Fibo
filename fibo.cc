#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

using std::ostream;

class Fibo {
    using fibitset = std::vector<bool>;
    
private: 
    
    // otherwise, normalized, Fibonacci-based number
    // 0th bit = 1, 1st = 2, 2nd = 3, 3rd = 5, etc.
    fibitset _bits;

    void normalize(fibitset& bits) {
        // couples fibits with their neighbors, 
        // starting with the most significant ones
        
        // maintaining: no bit, starting with the i-th, has a neighbor
        for(size_t j = bits.size(), i = bits.size(); i > 1; --i, j = i) {
            // maintaining: j > 1, propagating the couplings
            while(j < bits.size() && bits[j-1] && bits[j-2]) {
                bits[j] = 1;
                bits[j-1] = 0;
                bits[j-2] = 0;
                j += 2;
            }
            if(j == bits.size() && bits[j-1] && bits[j-2]) {
                bits.push_back(1);
                bits[j-1] = 0;
                bits[j-2] = 0;
            }
        }
    }

public:
    Fibo() : _bits() {
        _bits.push_back(0);
    }
    
    Fibo(const std::string& str) : _bits() {
        size_t size = str.size();
        
        _bits.resize(size);
        
        for(size_t i = 0; i < size; ++i)
            _bits[i] = str[size - 1 - i] - '0';
        
        normalize(_bits);
    }
    
    Fibo& operator=(const Fibo& that) {
        if (this != &that)
          _bits = that._bits;

        return *this;
    }

private:
    friend ostream& operator<<(ostream& os, const Fibo& fib) {
        for(auto i = fib._bits.rbegin(); i != fib._bits.rend(); ++i)
            os << (*i);
        
        return os;
    }
};

int main() {
    
    Fibo f1;
    Fibo f2("10000");
    f1 = f2;
    
    std::cout << f1 << std::endl;
    
    return 0;
}
