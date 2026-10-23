
import benchmarks.augsburg.config as config
import pandas as pd

from benchmarks.augsburg.dataset import AUGSBURG
from raug.raug.utils.loader import split_k_folder_csv, label_categorical_to_number
from utils.simulatemissingdata import simulate_missing_data
from sklearn.model_selection import StratifiedGroupKFold

def generate_sentence(df:pd.DataFrame) -> pd.DataFrame:
    df = df.reset_index(drop=True)

    field_to_label = {
        'age': 'Age',
        'gender': 'Gender',
        'background_mother': "Maternal ancestry",
        'background_father': "Paternal ancestry",
        'has_piped_water': 'Access to piped water',
        'has_sewage_system': 'Access to sewer system',
        'drink': 'Drink',
        'smoke': 'Smoke',
        'pesticide': 'Pesticide exposure',
        'fitspatrick': 'Fitzpatrick scale',
        'skin_cancer_history': 'Family skin cancer history',
        'cancer_history': 'Family any cancer history',
        'region': 'Lesion region',
        'grew': 'Lesion grew',
        'itch': 'Lesion itch',
        'bleed': 'Lesion bled',
        'hurt': 'Lesion hurt',
        'changed': 'Lesion changed',
        'elevation': 'Lesion elevation',
        'diameter_1': 'First diameter (mm)',
        'diameter_2': 'Second diameter (mm)',
    }

    features = ['age', 'gender', 'background_mother', 'background_father',
                'has_piped_water', 'has_sewage_system', 'smoke', 'drink',
                'pesticide', 'fitspatrick', 'skin_cancer_history',
                'cancer_history', 'region', 'grew', 'itch', 'bleed',
                'hurt', 'changed', 'elevation', 'diameter_1', 'diameter_2']
    
    sentences = []
    for _, row in df.iterrows():
        anamnese = "Patient History: "
        for col in features:
            # only add to sentence if the value is not NaN or UNK
            if not pd.isna(row[col]) and row[col] != 'UNK':
                anamnese += f"{field_to_label[col]}: {str(row[col]).lower()}, "  
        anamnese = anamnese[:-2] + '.'
        sentences.append(anamnese)
    df['sentence'] = pd.Series(sentences)

    return df[[AUGSBURG.PATIENT_ID, AUGSBURG.TARGET_COLUMN, AUGSBURG.TARGET_NUMBER_COLUMN, AUGSBURG.IMAGE_COLUMN, 'folder', 'sentence']]

if __name__ == '__main__':
    print("- Loading the csv")
    df = pd.read_csv(config.AUGSBURG_RAW_METADATA)
    
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
                            encoder_name= f'sentence',
                            features=AUGSBURG.RAW_CATEGORICAL_FEATURES + AUGSBURG.NUMERICAL_FEATURES,
                            dataset='pad-ufes-20')
    print("- Done!")