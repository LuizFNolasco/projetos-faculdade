sed -i 's/modelo = "abc"/modelo = "gemma3:4b"/g' benchmarks_miccai/miccai/config.py
python -m benchmarks_miccai.kfoldexperiment

sed -i 's/modelo = "gemma3:4b"/modelo = "gemma3:12b"/g' benchmarks_miccai/miccai/config.py
python -m benchmarks_miccai.kfoldexperiment

sed -i 's/modelo = "gemma3:12b"/modelo = "gemma3:27b"/g' benchmarks_miccai/miccai/config.py
python -m benchmarks_miccai.kfoldexperiment

sed -i 's/modelo = "gemma3:27b"/modelo = "llama4:16x17b"/g' benchmarks_miccai/miccai/config.py
python -m benchmarks_miccai.kfoldexperiment

sed -i 's/modelo = "llama4:16x17b"/modelo = "llava:7b"/g' benchmarks_miccai/miccai/config.py
python -m benchmarks_miccai.kfoldexperiment

sed -i 's/modelo = "llava:7b"/modelo = "llava:13b"/g' benchmarks_miccai/miccai/config.py
python -m benchmarks_miccai.kfoldexperiment

sed -i 's/modelo = "llava:13b"/modelo = "llava:34b"/g' benchmarks_miccai/miccai/config.py
python -m benchmarks_miccai.kfoldexperiment

sed -i 's/modelo = "llava:34b"/modelo = "qwen2.5vl:3b"/g' benchmarks_miccai/miccai/config.py
python -m benchmarks_miccai.kfoldexperiment

sed -i 's/modelo = "qwen2.5vl:3b"/modelo = "qwen2.5vl:7b"/g' benchmarks_miccai/miccai/config.py
python -m benchmarks_miccai.kfoldexperiment

sed -i 's/modelo = "qwen2.5vl:3b"/modelo = "qwen2.5vl:32b"/g' benchmarks_miccai/miccai/config.py
python -m benchmarks_miccai.kfoldexperiment