#include "mytvm/onnx_parser.h"
#include "onnx/onnx.pb.h"
#include "mytvm/relay/operator_registry.h"
#include "mytvm/relay/relay_include.h"
#include "mytvm/relay/type.h"

#include <iostream>
#include <fstream>



namespace mytvm {
    void print_onnx_model(onnx::ModelProto& model) {
        const auto& graph = model.graph();
        std::cout << "Graph name: " << model.graph().name() << std::endl;

        std::cout << "Inputs: " << std::endl;
        for(const auto& input : graph.input()) {
            std::cout << "  " << input.name() << " : (";
            for(const auto& dim : input.type().tensor_type().shape().dim()) {
                std::cout << dim.dim_value() << ", ";
            }
            std::cout << ")" << std::endl;
            std::cout << std::endl;
        }

        std::cout << "Outputs: " << std::endl;
        for(const auto& output : graph.output()) {
            std::cout << "  " << output.name() << " : (";
            for(const auto& dim : output.type().tensor_type().shape().dim()) {
                std::cout << dim.dim_value() << ", ";
            }
            std::cout << ")" << std::endl;
            std::cout << std::endl;
        }

        std::cout << "Nodes: " << std::endl;
        for(const auto& node : graph.node()) {
            std::cout << "  " << node.name() << " : " << node.op_type() << std::endl;
            for(const auto& input : node.input()) {
                std::cout << "    input: " << input << std::endl;
            }
            for(const auto& output : node.output()) {
                std::cout << "    output: " << output << std::endl;
            }
            std::cout << std::endl;
        }
        return;
    }
    void parse_onnx_model(const std::string& model_path) {
        onnx::ModelProto model;
        std::cout << "Parsing ONNX model: " << model_path << std::endl;

        std::ifstream model_file(model_path, std::ios::binary);
        if(!model_file) {
            std::cerr << "Failed to open model file: " << model_path << std::endl;
            return;
        }

        if(!model.ParseFromIstream(&model_file)) {
            std::cerr << "Failed to parse ONNX model" << std::endl;
            return;
        }

        print_onnx_model(model);

        std::unordered_map<std::string, std::shared_ptr<relay::RelayVar>> input2relayVars;
        std::unordered_map<std::string, std::shared_ptr<relay::RelayExpr>> output2relayExprs;
        // iterate over input nodes
        for(const auto& input : model.graph().input()) {
            std::string input_name = input.name();
            // get the shape of the input
            std::vector<int> input_shape;
            for(const auto& dim : input.type().tensor_type().shape().dim()) {
                input_shape.push_back(dim.dim_value());
            }

            std::shared_ptr<relay::Type> inputVarType = 
                std::make_shared<relay::TensorType>(input_shape, relay::PrimType::kFloat);
            std::shared_ptr<relay::RelayVar> inputVar = relay::RelayVar::create(input_name, inputVarType);
            input2relayVars[input_name] = inputVar;
        }

        // iterate over initializers
        for(const auto& initializer : model.graph().initializer()) {
            std::string initializer_name = initializer.name();
            std::vector<int> initializer_shape;
            for(const auto& dim : initializer.dims()) {
                initializer_shape.push_back(dim);
            }

            std::shared_ptr<relay::Type> initializerVarType;
            initializerVarType = std::make_shared<relay::TensorType>(initializer_shape, relay::PrimType::kFloat);
            std::shared_ptr<relay::RelayVar> initializerVar = relay::RelayVar::create(initializer_name, initializerVarType);
            input2relayVars[initializer_name] = initializerVar;
        }


        // iterate over nodes
        for(auto node : model.graph().node()) {
            std::vector<std::shared_ptr<relay::RelayExpr>> args;
            for(const std::string& input : node.input()) {
                std::shared_ptr<relay::RelayVar> inputVar = input2relayVars[input];
                args.push_back(inputVar);
            }

            relay::OperatorRegistry* converter = relay::OperatorRegistry::get_instance();
            std::vector<std::shared_ptr<relay::RelayExpr>> output = converter->convertNode(node, args);


            // assert that the number of outputs is the same as the number of outputs in the node
            #ifdef NDEBUG
            assert(output.size() == node.output_size());
            #endif

            for(int i = 0; i < output.size(); i++) {
                std::string output_name = node.output(i);
                output2relayExprs[output_name] = output[i];
            }

        }

        std::vector<std::shared_ptr<relay::RelayExpr>> outputs;
        // iterate over outputs
        for(const auto& output : model.graph().output()) {
            std::string output_name = output.name();
            outputs.push_back(output2relayExprs[output_name]);
        }

        std::vector<std::shared_ptr<relay::RelayExpr>> params;
        for(const auto& input : model.graph().input()) {
            std::string input_name = input.name();
            std::shared_ptr<relay::RelayVar> inputVar = input2relayVars[input_name];
            params.push_back(inputVar);
        }

        std::shared_ptr<relay::RelayExpr> body;
        if(outputs.size() == 1) {
            body = outputs[0];
        } else {
            body = relay::RelayTuple::create(outputs);
        }
        std::shared_ptr<relay::RelayFunction> relayFunction = relay::RelayFunction::create(params, body, "main");

        relayFunction->print(std::cout);

        return;

        
    }
}
