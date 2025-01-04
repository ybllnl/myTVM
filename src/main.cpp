#include <iostream>
#include "mytvm/onnx_parser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <onnx_model_path>" << std::endl;
        return 1;
    }

    mytvm::parse_onnx_model(argv[1]);
    return 0;
}
