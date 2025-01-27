#ifndef _MYTVM_RELAY_RELAY_EXPR_H_
#define _MYTVM_RELAY_RELAY_EXPR_H_

#include "./relay_include.h"
#include "./type.h"

#include <vector>
#include <memory>
#include <ostream>

namespace mytvm {
namespace relay {

class RelayExpr : public RelayNode {
public:
    RelayExpr() {}
    virtual ~RelayExpr() {}
    virtual void print(std::ostream& os) = 0;
};

class RelayOp : public RelayExpr {
public:
    /*! \brief The operator name
     */
    std::string name;
    RelayOp(std::string n) {
        name = n;
    }

    void print(std::ostream& os) override {
        os << name;
    }
};

class RelayCall : public RelayExpr {
public:
    /*! \brief The operator called
     */
    std::shared_ptr<RelayExpr> op;

    /*! \brief The arguments to the call
     */
    std::vector<std::shared_ptr<RelayExpr>> args;

    RelayCall(std::shared_ptr<RelayExpr> o, std::vector<std::shared_ptr<RelayExpr>>& a) {
        op = o;
        args = std::move(a);
    }
    //virtual ~RelayCall() {}
    void print(std::ostream& os) override {
        op->print(os);
        os << "(";
        for(size_t i = 0; i < args.size(); ++i) {
            args[i]->print(os);
            if(i < args.size() - 1) {
                os << ", ";
            }
        }
        os << ")";
    }
};

class RelayTuple : public RelayExpr {
private:
    RelayTuple(std::vector<std::shared_ptr<RelayExpr>>& f) {
        fields = std::move(f);
    }
public:
    /*! \brief The fields of the tuple
     */
    std::vector<std::shared_ptr<RelayExpr>> fields;


    static std::shared_ptr<RelayTuple> create(std::vector<std::shared_ptr<RelayExpr>>& f) {
        return std::shared_ptr<RelayTuple>(new RelayTuple(f));
    }
    //virtual ~RelayTuple() {}
    void print(std::ostream& os) override {
        os << "Not implemented print for RelayTuple" << std::endl;
    }
};

static int var_id_counter = 0;

class RelayVar : public RelayExpr {
private:
    RelayVar(int var_id, std::string name, std::shared_ptr<Type> type) 
        : var_id(var_id), name(name), type(type) {}
public:
    int var_id; 
    std::string name;
    std::shared_ptr<Type> type;
    // Declaration in header
    static int temp_counter;

    static std::shared_ptr<RelayVar> create(std::string name, std::shared_ptr<Type> type) {
        return std::shared_ptr<RelayVar>(new RelayVar(var_id_counter++, name, type));
    }

    static std::shared_ptr<RelayVar> createTemp(std::shared_ptr<Type> type) {
        return create("%" + std::to_string(temp_counter++), type);
    }

    void print(std::ostream& os) override {
        os << "%" << name;
    }
};

class RelayFunction : public RelayExpr {
private:
    RelayFunction(std::vector<std::shared_ptr<RelayExpr>>& p, std::shared_ptr<RelayExpr> b, std::string n) {
        func_name = n;
        params = std::move(p);
        body = b;
    }
public:
    /*! \brief The parameters of the function
     */
    std::vector<std::shared_ptr<RelayExpr>> params;

    /*! \brief The body of the function
     */
    std::shared_ptr<RelayExpr> body;

    /*! \brief The name of the function
     */
    std::string func_name;

    /*! \brief Create a RelayFunction
     * \param p The parameters of the function
     * \param b The body of the function
     * \return The created RelayFunction
     */
    static std::shared_ptr<RelayFunction> create(std::vector<std::shared_ptr<RelayExpr>>& p, std::shared_ptr<RelayExpr> b, std::string n) {
        return std::shared_ptr<RelayFunction>(new RelayFunction(p, b, n));
    }
    //virtual ~RelayFunction() {}
    void print(std::ostream& os) override {
        os << "def @" << func_name << "(";
        for(int i = 0; i < params.size(); i++) {
            if(RelayVar* v = dynamic_cast<RelayVar*>(params[i].get())) {
                os << "%" << v->name << ": ";
                v->type->print(os);
            } else {
                throw std::runtime_error("Function parameter is not a RelayVar");
            }

            if(i < params.size() - 1) {
                os << ", ";
            }
        }
        os << ") {\n  ";
        body->print(os);
        os << "\n}\n";
    }
};

} // namespace relay
} // namespace mytvm


#endif
