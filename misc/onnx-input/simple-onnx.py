import torch
import torch.nn as nn
import onnx

# Define a simple model
class SimpleModel(nn.Module):
    def __init__(self):
        super(SimpleModel, self).__init__()
        self.fc = nn.Linear(3, 2)  # A single fully connected layer

    def forward(self, x):
        return self.fc(x)

# Initialize the model and dummy input
model = SimpleModel()
dummy_input = torch.randn(1, 3)  # Batch size of 1, input dimension of 3

# Export the model to ONNX
torch.onnx.export(
    model,                   # Model to export
    dummy_input,             # Sample input
    "simple_model.onnx",     # Output ONNX file name
    input_names=["input"],   # Optional input name
    output_names=["output"], # Optional output name
    opset_version=11         # ONNX opset version
)

print("Model has been exported to simple_model.onnx")

# Load and check the ONNX model
onnx_model = onnx.load("simple_model.onnx")
onnx.checker.check_model(onnx_model)
print("ONNX model is valid.")
print(onnx.helper.printable_graph(onnx_model.graph))
