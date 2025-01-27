#include <gtest/gtest.h>
#include "mytvm/relay/operator_registry.h"

using namespace mytvm::relay;

TEST(OperatorRegistry, SingletonBehavior) {
    OperatorRegistry* instance1 = OperatorRegistry::get_instance();
    OperatorRegistry* instance2 = OperatorRegistry::get_instance();
    ASSERT_EQ(instance1, instance2);
}

TEST(OperatorRegistry, OperatorRegistration) {
    OperatorRegistry* registry = OperatorRegistry::get_instance();
    
    // Test registered operator
    ASSERT_NO_THROW(registry->getConversionFunc("Gemm"));
    
    // Test unregistered operator
    ASSERT_THROW(registry->getConversionFunc("UnknownOp"), std::runtime_error);
}

TEST(OperatorRegistry, NodeConversion) {
    OperatorRegistry* registry = OperatorRegistry::get_instance();
    onnx::NodeProto node;
    std::vector<std::shared_ptr<RelayExpr>> inputs(3);
    
    // Test valid conversion
    node.set_op_type("Gemm");
    ASSERT_NO_THROW(auto result = registry->convertNode(node, inputs));
    
    // Test invalid operator
    node.set_op_type("InvalidOp");
    ASSERT_THROW(registry->convertNode(node, inputs), std::runtime_error);
}

TEST(RelayTypes, TensorTypePrint) {
    TensorType tensor({1, 3}, PrimType::kFloat);
    std::stringstream ss;
    tensor.print(ss);
    ASSERT_EQ(ss.str(), "Tensor[(1, 3), float]");
}

TEST(RelayExpr, RelayVarCreation) {
    auto tensor_type = std::make_shared<TensorType>(std::vector<int>{2}, PrimType::kInt);
    auto var = RelayVar::create("x", tensor_type);
    std::stringstream ss;
    var->print(ss);
    ASSERT_EQ(ss.str(), "%x");
}

// TODO: check these tests
TEST(RelayExpr, RelayFunctionPrint) {
    std::vector<std::shared_ptr<RelayExpr>> params = {
        RelayVar::create("input", std::make_shared<TensorType>(std::vector<int>{1,3}, PrimType::kFloat)),
        RelayVar::create("fc_weight", std::make_shared<TensorType>(std::vector<int>{2,3}, PrimType::kFloat))
    };
    auto body = RelayVar::create("output", std::make_shared<TensorType>(std::vector<int>{2}, PrimType::kFloat));
    auto func = RelayFunction::create(params, body, "main");
    
    std::stringstream ss;
    func->print(ss);
    std::string expected = 
        "def @main(%input: Tensor[(1, 3), float], %fc_weight: Tensor[(2, 3), float]) {\n"
        "  %output\n"
        "}\n";
    ASSERT_EQ(ss.str(), expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
