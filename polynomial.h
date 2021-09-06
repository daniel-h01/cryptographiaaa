#include <iostream>
#include <vector>

template<typename T>
class Polynomial {
private:
    std::vector<T> data;

    void del_zeros() {
        int i = size() - 1;
        for (; i != 0; --i)
            if (data[i] != T(0)) break;
        data.resize(i + 1);
    }

public:
    // =================CONSTRUCTOR==================

    Polynomial(const std::vector<T> v) : data(v) {
        del_zeros();
    }

    Polynomial(const T& v = T()) : data({v}) {}

    template<typename It>
    Polynomial(const It first, const It last) : data(first, last) {
        del_zeros();
    }

    // ===================EQUAL======================

    bool operator == (const Polynomial& other) const {
        return data == other.data;
    }

    bool operator != (const Polynomial& other) const {
        return data != other.data;
    }

    // ===================SIZE=======================

    size_t size() const {
        return data.size();
    }

    // ================OPERATOR(+=, -=)==============

    Polynomial& operator += (const Polynomial& other) {
        if (size() < other.size()) {
            data.resize(other.size());
        }
        for (size_t i = 0; i != other.size(); ++i) {
            data[i] += other.data[i];
        }
        del_zeros();
        return *this;
    }

    Polynomial& operator -= (const Polynomial& other) {
        if (size() < other.size()) {
            data.resize(other.size());
        }
        for (size_t i = 0; i != other.size(); ++i) {
            data[i] -= other.data[i];
        }
        del_zeros();
        return *this;
    }
    // ================DEGREE, []====================

    T operator[] (const size_t i) const {
        if (i >= size()) {
            return T(0);
        }
        return data[i];
    }

    int Degree() const {
        if (size() == 1 && data[0] == T(0)) {
            return -1;
        }
        return size() - 1;
    }

    // =================ITERATORS====================

    auto begin() const {
        return data.cbegin();
    }

    auto end() const {
        return data.cend();
    }

    // ==================PRODUCT=====================

    Polynomial& operator *= (const Polynomial& other) {
        std::vector<T> temp(data);
        data.clear();
        data.resize(temp.size() + other.size());
        for (size_t i = 0; i != temp.size(); ++i) {
            for (size_t j = 0; j != other.size(); ++j) {
                data[i + j] += temp[i] * other[j];
            }
        }
        del_zeros();
        return *this;
    }

    // =================OPERATOR()===================

    T operator() (const T& arg) const {
        T ans = T(0);
        for (int i = size() - 1; i != -1; --i) {
            ans = ans * arg + data[i];
        }
        return ans;
    }

    // =================COMPOSITION==================

    Polynomial operator & (const Polynomial& other) const {
        Polynomial ans(T(0));
        for (int i = size() - 1; i != -1; --i) {
            ans = ans * other + data[i];
        }
        return ans;
    }

    // =================DIVMOD=======================

    Polynomial& operator /= (const Polynomial& other) {
        Polynomial ans(T(0));
        while (Degree() >= other.Degree()) {
            std::vector<T> v(size() - other.size() + 1, T(0));
            v.back() = data.back() / other.data.back();
            Polynomial temp(v);
            ans += temp;
            *this -= temp * other;
            del_zeros();
        }
        data = ans.data;
        return *this;
    }

    Polynomial operator / (const Polynomial& other) const {
        Polynomial cur(*this);
        cur /= other;
        return cur;
    }

    Polynomial operator % (const Polynomial& other) const {
        Polynomial cur(*this);
        cur /= other;
        return *this - cur * other;
    }
};

template<class T>
Polynomial<T> operator , (const Polynomial<T>& a, const Polynomial<T>& b) {
        return (b != Polynomial<T>(T(0))) ? operator , (b, a % b) : a / a[a.Degree()];
    }

// ======================SUM=========================

template<class T>
Polynomial<T> operator + (const Polynomial<T>& lhs_, const T& rhs_) {
    Polynomial<T> lhs(lhs_);
    Polynomial<T> rhs(rhs_);
    return lhs += rhs;
}

template<class T>
Polynomial<T> operator + (const T& lhs_, const Polynomial<T>& rhs_) {
    Polynomial<T> lhs(lhs_);
    Polynomial<T> rhs(rhs_);
    return rhs += lhs;
}

template<class T>
Polynomial<T> operator + (const Polynomial<T>& lhs_, const Polynomial<T>& rhs) {
    Polynomial<T> lhs(lhs_);
    return lhs += rhs;
}

// ====================DIFFERENCE====================

template<class T>
Polynomial<T> operator - (const Polynomial<T>& lhs_, const T& rhs_) {
    Polynomial<T> lhs(lhs_);
    Polynomial<T> rhs(rhs_);
    return lhs -= rhs;
}

template<class T>
Polynomial<T> operator - (const T& lhs_, const Polynomial<T>& rhs) {
    Polynomial<T> lhs(lhs_);
    return lhs -= rhs;
}

template<class T>
Polynomial<T> operator - (const Polynomial<T>& lhs_, const Polynomial<T>& rhs) {
    Polynomial<T> lhs(lhs_);
    return lhs -= rhs;
}

// ======================PRODUCT=====================

template<class T>
Polynomial<T> operator * (const Polynomial<T>& lhs_, const T& rhs_) {
    Polynomial<T> lhs(lhs_);
    Polynomial<T> rhs(rhs_);
    return lhs *= rhs;
}

template<class T>
Polynomial<T> operator * (const T& lhs_, const Polynomial<T>& rhs_) {
    Polynomial<T> lhs(lhs_);
    Polynomial<T> rhs(rhs_);
    return rhs *= lhs;
}

template<class T>
Polynomial<T> operator * (const Polynomial<T>& lhs_, const Polynomial<T>& rhs) {
    Polynomial<T> lhs(lhs_);
    return lhs *= rhs;
}
// =======================PRINT======================

template<class T>
std::ostream& operator<< (std::ostream& out, const Polynomial<T>& pol) {
    if (pol[pol.size() - 1] == T(0)) {
        out << T(0);
        return out;
    }
    for (int ii = pol.size() - 1; ii != -1; --ii) {
        size_t i = ii;
        if (pol[i] == T(0)) {
            continue;
        } else if (pol[i] < T(0)) {
            out << '-';
            if (pol[i] != T(-1) || i == 0) out << -pol[i];
        } else {
            out << ((i == pol.size() - 1) ? "" : "+");
            if (pol[i] != T(1) || i == 0) out << pol[i];
        }
        if (i == 0) continue;
        else if (i == 1) out << ((pol[i] == T(1) || pol[i] == T(-1)) ? "" : "*") << "x";
        else if (i > 1) out << ((pol[i] == T(1) || pol[i] == T(-1)) ? "" : "*") << "x^" << i;
    }
    return out;
}
