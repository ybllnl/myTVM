#!/bin/bash
# Create and activate conda environment
conda create -n mytvm python=3.9 -y
conda activate mytvm || source activate mytvm || exit 1

# Install PyTorch with CPU support
conda install -y pytorch torchvision torchaudio cpuonly -c pytorch # Replace 'cpuonly' with 'cudatoolkit=X.X' for GPU support

# Install ONNX ecosystem
pip install onnx onnxruntime numpy pytest

# Verify installations
python -c "import torch; print(f'PyTorch version: {torch.__version__}')"
python -c "import onnx; print(f'ONNX version: {onnx.__version__}')"
echo "Environment setup complete!"

