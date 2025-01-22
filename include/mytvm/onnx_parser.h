#ifndef _MYTVM_ONNX_PARSER_H_
#define _MYTVM_ONNX_PARSER_H_

#include <string>
#include "mytvm/relay/relay_include.h"
#include "onnx/onnx.pb.h"

namespace mytvm {
    void parse_onnx_model(const std::string& model_path);
    void print_onnx_model(onnx::ModelProto& model);
}


#endif // _MYTVM_ONNX_PARSER_H_
