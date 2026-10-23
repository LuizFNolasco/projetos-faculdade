from benchmarks.augsburg import config
import pandas as pd

from benchmarks.augsburg.dataset import AUGSBURG
from raug.raug.utils.loader import split_k_folder_csv, label_categorical_to_number
from utils.simulatemissingdata import simulate_missing_data
from sklearn.model_selection import StratifiedGroupKFold

def generate_sentence(df:pd.DataFrame) -> pd.DataFrame: # Aqui é só pra criar a sentença, mas já temos o arquivo
    df = df.reset_index(drop=True)

    return df[[AUGSBURG.PATIENT_ID, AUGSBURG.IMAGE_COLUMN, AUGSBURG.TARGET_COLUMN, AUGSBURG.SENTENCE, 'folder', AUGSBURG.TARGET_NUMBER_COLUMN]]

if __name__ == '__main__':
    print("- Loading the csv")
    df = pd.read_csv(config.AUGSBURG_RAW_METADATA)

    df = df.astype(str)
    
    print("- Splitting the dataset")

    print(f"- Using stratified k-fold grouped by {AUGSBURG.PATIENT_ID}")
    kfold = StratifiedGroupKFold(n_splits=5, shuffle=True, random_state=42)

    df['folder'] = None
    for i, (_, test_indexes) in enumerate(kfold.split(df, df[AUGSBURG.TARGET_COLUMN], groups=df[AUGSBURG.PATIENT_ID])):
        df.loc[test_indexes, 'folder'] = i + 1
    
    # Validate patient id separation across folders
    patient_ids = df.groupby('folder')[AUGSBURG.PATIENT_ID].unique()
    for i, ids in enumerate(patient_ids):
        for j, other_ids in enumerate(patient_ids):
            if i !=j and set(ids).intersection(other_ids):
                raise ValueError(f"{ids} and {other_ids} are present in the same folder {i+1} and {j+1}.")

    print("- Converting the labels to numbers")
    df = label_categorical_to_number (df, AUGSBURG.TARGET_COLUMN, col_target_number=AUGSBURG.TARGET_NUMBER_COLUMN)
    
    config.DATA_PATH.parent.mkdir(exist_ok=True)
    config.DATA_PATH.mkdir(exist_ok=True)

    print("- Simulating missing data")
    simulate_missing_data(df, save_folder=config.DATA_PATH,
                            encoder_function=generate_sentence,
                            encoder_name= AUGSBURG.SENTENCE,
                            dataset='augsburg', model=f'{config.modelo}')
    print("- Done!")