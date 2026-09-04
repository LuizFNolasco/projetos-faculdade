from pathlib import Path

modelo = "abc"
DATA_PATH = Path('data') /'MICCAI'
MICCAI_PATH = Path("/home/luiznolasco/modelos/data")
MICCAI_IMAGES_FOLDER = MICCAI_PATH / "MICCAI/imgs"
MICCAI_RAW_METADATA = MICCAI_PATH / f"results/MICCAI_metadata_with_sentences_of_image-description_{modelo}.csv"
MICCAI_SENTENCE_ENCODED = DATA_PATH / f"results/MICCAI_metadata_with_sentences_of_image-description_{modelo}.csv"
MICCAI_BAYESIAN_DATA = DATA_PATH

# vision baselines results path (for bayesian network)
IMAGE_ONLY_EXPERIMENT = MICCAI_IMAGES_FOLDER#174692956751647 # change this to match the folder of the results of your image-only model
