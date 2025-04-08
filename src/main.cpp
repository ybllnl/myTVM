#include <iostream>
#include "mytvm/onnx_parser.h"
#include "mytvm/relay/module.h"
#include "mytvm/relay/transform.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <onnx_model_path>" << std::endl;
        return 1;
    }

    mytvm::IRModule module = mytvm::parse_onnx_model(argv[1]);
    mytvm::transform::PassContext pass_ctx = mytvm::transform::PassContext::Create();
    return 0;
}
