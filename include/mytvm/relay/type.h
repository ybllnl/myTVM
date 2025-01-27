#ifndef _MYTVM_RELAY_TYPE_H_
#define _MYTVM_RELAY_TYPE_H_

#include <vector>
#include <iostream>
#include <memory>


namespace mytvm {
namespace relay {

class Type {
public:
    virtual void print(std::ostream& os){
        os << "Not implemented print for Type";
        exit(1);
    }
} ;

class PrimType : public Type {
public:
    enum TypeKind {
        kInt,
        kFloat,
        kBool
    } ;

    TypeKind kind;
    PrimType(TypeKind kind) : kind(kind) {}
    virtual void print(std::ostream& os) {
        switch(kind) {
            case kInt:
                os << "int";
                break;
            case kFloat:
                os << "float";
                break;
            case kBool:
                os << "bool";
                break;
        }
    }
} ;

class TensorType : public Type {
public:
    std::vector<int> shape;
    PrimType dtype;
    TensorType(std::vector<int> shape, PrimType dtype) : shape(move(shape)), dtype(dtype) {}

    virtual void print(std::ostream& os){
        os << "Tensor[(";
        for(int i = 0; i < shape.size(); i++){
            os << shape[i];
            if(i < shape.size() - 1){
                os << ", ";
            }
        }
        os << "), ";
        dtype.print(os);
        os << "]";
    }
} ;

class FuncType : public Type {
public:
    std::vector<Type> arg_types;
    std::shared_ptr<Type> ret_type;
    FuncType(std::vector<Type> arg_types, std::shared_ptr<Type> ret_type) : arg_types(move(arg_types)), ret_type(ret_type) {}

    virtual void print(){
        std::cout << "Not implemented print for FuncType";
        exit(1);
    }
} ;

class TupleType : public Type {
public:
    std::vector<Type> fields;
    TupleType(std::vector<Type> fields) : fields(move(fields)) {}

    virtual void print(){
        std::cout << "Not implemented print for TupleType";
        exit(1);
    }
} ;

} // namespace relay
} // namespace mytvm
#endif // _MYTVM_RELAY_TYPE_H_
