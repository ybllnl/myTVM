# Install PyTorch (choose the correct CUDA version if you need GPU support)
conda install pytorch torchvision torchaudio cpuonly -c pytorch  # Replace 'cpuonly' with 'cudatoolkit=X.X' for GPU support

# Install ONNX
pip install onnx

