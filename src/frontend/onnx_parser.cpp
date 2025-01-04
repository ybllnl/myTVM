#include "mytvm/onnx_parser.h"
#include "onnx/onnx.pb.h"
#include <iostream>

namespace mytvm {
    void parse_onnx_model(const std::string& model_path) {
        onnx::ModelProto model;
        std::cout << "Parsing ONNX model: " << model_path << std::endl;
        // place holder for parsing onnx model
        return;
    }
}
