from pathlib import Path

modelo = "gemma3:27b"
DATA_PATH = Path('data') /'AUGSBURG'
AUGSBURG_PATH = Path("/home/luiznolasco/modelos/data")
AUGSBURG_IMAGES_FOLDER = AUGSBURG_PATH / "AUGSBURG/imgs"
AUGSBURG_RAW_METADATA = AUGSBURG_PATH / f"aux/results_novos/AUGSBURG_metadata_with_sentences_of_image-description_{modelo}.csv"
AUGSBURG_SENTENCE_ENCODED = DATA_PATH / f"aux/results_novos/AUGSBURG_metadata_with_sentences_of_image-description_{modelo}.csv"
AUGSBURG_BAYESIAN_DATA = DATA_PATH

# vision baselines results path (for bayesian network)
IMAGE_ONLY_EXPERIMENT = AUGSBURG_IMAGES_FOLDER#174692956751647 # change this to match the folder of the results of your image-only model
