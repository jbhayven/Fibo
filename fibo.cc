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
    
    // pushes right (i.e. exchanges a pair of consecutive bits for one greater)
    // if and only if the byte [pos] and the byte [pos+1] are both true,
    // based on the fact that F(i) + F(i+1) = F(i+2)
    // Requires that all the bits to the right from such a pair already satisfy
    // the invariant of normalization
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
    
    //generate Fibonacci sequence
    inline std::vector<unsigned long long>
            get_sequence(const unsigned long long& number) {
        std::vector<unsigned long long> fibo_sequence;
    	fibo_sequence.push_back(1);
        fibo_sequence.push_back(2);

        while(fibo_sequence.end()[-1] <= number && fibo_sequence.end()[-1] > 0) {
           	fibo_sequence
  			.push_back(fibo_sequence.end()[-1]+fibo_sequence.end()[-2]);
        }

        return fibo_sequence;
    }

    void remove_leading_zeros() {
    	size_t i = _bits.size()-1;

    	while(_bits[i] == 0 && i > 0)
    		--i;

    	_bits.resize(i+1);
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
    
    template<typename Number,
             typename = typename std::enable_if<
                std::is_integral<Number>::value
                && !std::is_same<char, Number>::value
                && !std::is_same<bool, Number>::value>::type>

    Fibo(Number number) : _bits() {
    	if(number < 0)
    		throw std::invalid_argument("Negative number");

        std::vector<unsigned long long> fibo_sequence
		    = get_sequence(number);

        size_t size = fibo_sequence.size();
        _bits.resize(size-1);

        for(size_t i = 0; i < size; i++) {
        	if(fibo_sequence[size-i-1] <= number) {
                number -= fibo_sequence[size-i-1];
                _bits[size-i-1] = true;
        	}
        	else {
        		_bits[size-i-1] = false;
        	}
        }
    }

    Fibo(const Fibo& that) : _bits(that._bits) {}
    
    size_t length() {
     	return _bits.size();
     }

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

    Fibo& operator&=(const Fibo& that) {
    	int size = that._bits.size();
    	if(size > _bits.size())
    	    _bits.resize(size);

    	for(size_t i = 0; i < size; ++i)
    		if(!that._bits[i] || !_bits[i])
    			_bits[i] = 0;

    	remove_leading_zeros();

    	return *this;
    }

    const Fibo operator&(const Fibo& that) const {
    	return Fibo(*this) &= that;
    }

    Fibo& operator|=(const Fibo& that) {
        size_t size = that._bits.size();

        for(size_t i = 0; i < size; ++i)
            if(that._bits[i] && !_bits[i])
                this->add_bit(i);

        return *this;
    }

    const Fibo operator|(const Fibo& that) const {
    	return Fibo(*this) |= that;
    }

    Fibo& operator^=(const Fibo& that) {
    	int size = that._bits.size();
    	if(size > _bits.size())
    	    _bits.resize(size);

    	for(size_t i = 0; i < size; ++i)
    		if(that._bits[i] || !_bits[i])
    			this->add_bit(i);
    		else if(that._bits[i] && !_bits[i])
    			_bits[i] = 0;

    	remove_leading_zeros();

    	return *this;
    }

    const Fibo operator^(const Fibo& that) const {
    	return Fibo(*this) ^= that;
    }

    Fibo& operator<<=(const size_t& n) {
    	if(n == 0)
    		return *this;

    	_bits.resize(_bits.size() + n);
    	std::rotate(_bits.begin(), _bits.end() - n, _bits.end());

    	return *this;
    }

    const Fibo operator<<(const size_t& n) {
    	return Fibo(*this) <<= n;
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

const Fibo& One() {
	static Fibo one("1");

	return one;
}

#include <cassert>


int main() {
    
    Fibo f1;
    Fibo f2("10000");
    f1 = f2;
    
    Fibo f3(f2);
    
    
    std::cout << f1 << " " << f2 << " " << f3 << std::endl;
    
    f1 += f2;
    std::cout << f1 << std::endl;
    
    std::cout << f1 + f2 << std::endl;
    std::cout << f1 + (f2 + f3) << std::endl;

    std::cout << Fibo("1001") + Fibo("10") << std::endl;
    std::cout << Fibo("1011") << std::endl;

    assert((Fibo("11") == Fibo("100")));
    assert(((Fibo("1001") + Fibo("10")) == Fibo("1011")));

    long long a = 1000000000000000;
    Fibo b(3);
    std::cout<<a;


    return 0;
}
